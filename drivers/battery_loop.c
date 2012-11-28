/*
 * battery_loop.c
 *
 * Copyright (C) Amazon Technologies Inc. All rights reserved.
 * Donald Chan (hoiho@lab126.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <config.h>
#include <common.h>
#include <twl6030.h>


/* display function */
int Init_LCD(void);
void Power_on_LCD(void);
void Power_off_LCD(void);
void show_lowbattery(void);
void show_charging(void);
void show_devicetoohot(void);

/* battery function */
#if defined(CONFIG_BQ27541)
int bq27541_capacity(u16 *capacity);
int bq27541_voltage(u16 *voltage);
int bq27541_current(s16 *current);
int bq27541_temperature(s16 *temperature);
#endif /* CONFIG_BQ27541 */

#ifdef CONFIG_SMB347
/* charger function */
int smb347_init(int *wall_charger);
int smb347_redo_apsd(void);
int smb347_apsd_process(int apsd, int *wall_charger);
#define DOUBLE_CHECK_APSD_TIME 4 /* 4 secs */
#endif

#define TEMPERATURE_COLD_LIMIT -19 /* -19 C */
#define TEMPERATURE_HOT_LIMIT 59 /*  59 C */
#define LOW_BATTERY_CAPACITY_LIMIT 2 /*  2% */
#define LOW_BATTERY_VOLTAGE_LIMIT 3430 /* 3.43 V */
#define LOW_BATTERY_SPLASH_VOLTAGE_LIMIT 3100	/* 3.1 V */
#define LOW_CURRENT_LIMIT 1400 /* 1400 mA (Wall Charger) */

#define BATTERY_CHRG_ON 0x01
#define BATTERY_CHRG_OFF 0x00

#define TMP103_I2C_BUS (2) /* I2C Bus # of TMP103 */
#define TMP103_ADDR (0x70) /* I2C Addr of TMP103 */

#define DEVICE_TEMP_BOOT_LIMIT 64 /* in degres C */
#define MAX_CHARGING_TIME_TOO_LONG (30*60*5*2) /* in seconds */
#define DISCHARGING_COUNT 10
#define POWER_BUTTON_SCAN_LOOP_COUNT 20   /* # of for loop for 1 milliscecond */
#define BATTERY_READY_TO_BOOT_COUNT 5
#define READ_TEMP_ERR_COUNT_LIMIT 10

int gnUSBAttached = 0;

struct battery_charging_info {
	u16 voltage;
	u16 capacity;
	s16 current;
	s16 temperature;
	u8 tmp103_temp;
};

enum pic_type {
	PIC_LOWBATTERY = 0,
	PIC_CHARGING,
	PIC_TOOHOT,
	NUM_OF_PIC
};

static int tmp103_temperature_read(void *buffer, size_t size)
{
	int status = -1;

	/* Switch to the I2C bus the gas gauge is connected to */
	if (select_bus(TMP103_I2C_BUS, 100)) {
		printf("TMP103: Failed to switch to bus %d\n", TMP103_I2C_BUS);
		goto done;
	}

	if ((status = i2c_read(TMP103_ADDR, 0x0, 1, buffer, size)))
		printf("TMP103: I2C read failed: %d\n", status);

done:
	/* Switch back to the original I2C bus */
	select_bus(0, 100);

	return status;
}

static void delay_ms(int ms)
{
	int timeout;
	for (timeout = 0; timeout < ms; timeout++) {
		udelay(1000);
	}
}

static void shut_down(void)
{
	printf("Shutting down device...\n");

#if defined(CONFIG_TWL6030)
	twl6030_shutdown();
#endif

	while (1);
}

static int check_usb_vbus_connection(void)
{
	if (twl6030_get_vbus_status() == 0) {
		printf("No USB/Charger connection detected\n");
		return 0;
	}

	return 1;
}

static void show_error_logo(enum pic_type type, int pre_ms, int max_ms)
{
#define CHECK_VBUS_MS 500
	int ms, times, remain_ms;
	int check_vbus_flag = 0;

	if(!Init_LCD())
		Power_on_LCD();

	switch(type) {
		case PIC_LOWBATTERY:
			show_lowbattery();
			printf("Display low battery picture...\n");
		break;
		case PIC_CHARGING:
			show_charging();
			printf("Display low battery charging picture...\n");
		break;
		case PIC_TOOHOT:
			show_devicetoohot();
			printf("Display device too hot picture...\n");
		break;
		default:
			printf("Error to display picture with type %d...\n", type);
		break;
	}

	if ((pre_ms > 0) || ((pre_ms == 0) && (type == PIC_CHARGING)))
		check_vbus_flag = 1;

	//keep display for ms delay
	ms = max_ms - (pre_ms * 50);
	times = ms / CHECK_VBUS_MS;
	remain_ms = ms - (CHECK_VBUS_MS * times);
	for (; times-- > 0;) {
		delay_ms(CHECK_VBUS_MS);
		if (check_vbus_flag && !check_usb_vbus_connection()) {
			shut_down();
		}
	}
	if (remain_ms)
		delay_ms(remain_ms);

	Power_off_LCD();
}

static int check_pwr_key_press(int *ms)
{
	int pressed = 0;
	int local_ms = 0;
	u8 button_state = 0x01;

	//printf("check_pwr_key_press start...\n");

	/* Delay for ~5 seconds, note: hand-tuned loop, be careful! */
	for (local_ms = 0; local_ms < 20; local_ms++) {

		/* Read whether a button was pressed/released */

		u8 data = 0;
		twl6030_i2c_read_u8(TWL6030_CHIP_PM, &data,	PHOENIX_STS_HW_CONDITION);
		data &= 0x01;

		/* If it's different, then it's been pressed */
		if (button_state ^ data) {
			printf("power key press...\n");
			pressed = 1;
			break;
		}

		button_state = data;

		delay_ms(50); //50ms
	}

	*ms = local_ms;
	//printf("check_pwr_key_press end...\n");

	return pressed;
}

int get_battery_charging_info(struct battery_charging_info *data)
{
	int i = 0;

	data->voltage = 0;
	data->capacity = 0;
	data->current = 0;
	data->temperature = 0;
	data->tmp103_temp = 0;

#if defined(CONFIG_BQ27541)
	if (bq27541_voltage(&data->voltage)) {
		printf("Unable to determine battery voltage\n");
	}

	if (bq27541_capacity(&data->capacity)) {
		printf("Unable to determine battery capacity\n");
	}

	if (bq27541_current(&data->current)) {
		printf("Unable to determine battery current\n");
	}

/*
 * NOTICE: In factory, some station will use Main board without real battery to power on,
 *         We can't shut down the device for this case.
 */
#if 0
	i = 0;
	do {
		if (!bq27541_temperature(&data->temperature))
			break;
		else {
			printf("Unable to determine battery temperature (%d)\n", i);
			delay_ms(100);
		}
	} while (++i < READ_TEMP_ERR_COUNT_LIMIT);

	if(i >= READ_TEMP_ERR_COUNT_LIMIT) {
		printf("Too many error to read the temperature, shut down the device..\n");
		shut_down();
	}
#else
	if (bq27541_temperature(&data->temperature)) {
		printf("Unable to determine battery temperature\n");
		return -1;
	}
#endif

#endif /* CONFIG_BQ27541 */

	i = 0;
	do {
		if (!tmp103_temperature_read(&data->tmp103_temp, sizeof(u8)))
			break;
		else {
			printf("Unable to read temperature from TMP103 (%d)\n", i);
			delay_ms(100);
		}
	} while (++i < READ_TEMP_ERR_COUNT_LIMIT);

	if(i >= READ_TEMP_ERR_COUNT_LIMIT) {
		printf("Too many error to read the temperature, shut down the device..\n");
		shut_down();
	}

	return 0;
}

void check_battery_condition(void)
{
	struct battery_charging_info info;
	int wall_charger = -1;
	int charge_screen_displayed = 0;
	int pwr_key_pressed = 0;
	int discharge_count = 0, charge_enough = 0;
	int sec = 0, powerkey_ms = 0;
	int cable_status = 0;
	int ret = 0;
#ifdef CONFIG_SMB347
	int redo_apsd_flag = 1;
#endif

#ifdef CONFIG_SMB347
	if (smb347_init(&wall_charger) == -1)
		printf("SMB347 initial failed\n");
#endif

	cable_status = check_usb_vbus_connection();
	if(cable_status  == 1) {
	  if(wall_charger == 1) {
	    printf("Detect wall charger.\n");
	  }else{
	    gnUSBAttached = 1;
	    printf("Detect usb.\n");
	  }
	}

	ret = get_battery_charging_info(&info);

	/* Display battery information */
	printf("Battery: voltage = %4d mV, current = %4d mA,\n", info.voltage, info.current);
	printf("         capacity = %4d %%,  temperature = %3d C, tmp103 = %3d C\n", info.capacity, info.temperature, info.tmp103_temp);

	if ((info.temperature > TEMPERATURE_HOT_LIMIT) || (info.tmp103_temp >= DEVICE_TEMP_BOOT_LIMIT)) {
		printf("Device is too hot (battery:%d C, temp103:%d C). It is not safe to boot\n", info.temperature, info.tmp103_temp);

		//if (voltage > LOW_BATTERY_SPLASH_VOLTAGE_LIMIT)
		if (1)
			show_error_logo(PIC_TOOHOT, 0, 4000);

		shut_down();
	}

#ifdef UBOOT_FTM
	if (ret) {
		printf("For factory build, skip gas-gauge issue and keep power-on\n");
		return;
	}
#endif

	if ((info.voltage >= LOW_BATTERY_VOLTAGE_LIMIT) && (info.capacity >= LOW_BATTERY_CAPACITY_LIMIT)) {
		printf("cond: voltage = %d, capacity = %d can start to boot..\n", info.voltage, info.capacity);
	}
	else {
		if ((info.voltage > LOW_BATTERY_SPLASH_VOLTAGE_LIMIT) || (wall_charger == 1) ) {
			if (info.current > 0)
				show_error_logo(PIC_CHARGING, 0, 5000);
			else {
				if (cable_status == 1) {
					enum pic_type pic = PIC_LOWBATTERY;
					int i = 0;
					while (++i <= 30) {
						get_battery_charging_info(&info);
						printf("cable_status = 1, current = %d, retry = %d\n", info.current, i);
						if (info.current > 0) {
							pic = PIC_CHARGING;
							break;
						}
						delay_ms(50);
					}

					show_error_logo(pic, 0, 5000);
				}
				else {
					show_error_logo(PIC_LOWBATTERY, 0, 5000);
				}
			}

			charge_screen_displayed = 1;
		}
		else
			printf("Unable to dispay low battery (charging) notification\n");
		//FIXME: prcm_reinit_battery_chrg() would cost 5 secs, it's too long.
		prcm_reinit_battery_chrg(BATTERY_CHRG_ON);
		scale_reinit_vcores(BATTERY_CHRG_ON);

		printf("Entering charge loop...\n");

		while(1) {

			/* USB/Charger connection check */
			if(!check_usb_vbus_connection()) {
			  gnUSBAttached = 0;
				shut_down();
			}
#ifdef CONFIG_SMB347
			else if (redo_apsd_flag) {
				int ret = smb347_init(&wall_charger);
				if (ret == 0) {
					if(wall_charger == 1)
						printf("Detect wall charger.\n");
					else
						printf("Detect usb.\n");
				}
				else if ((ret == 1) && (sec >= DOUBLE_CHECK_APSD_TIME)) {
					int apsd = smb347_redo_apsd();
					smb347_apsd_process(apsd, &wall_charger);
					if(wall_charger == 1)
						printf("Double detect wall charger.\n");
					else
						printf("Double detect usb.\n");

					redo_apsd_flag = 0;
				}
			}
#endif

			get_battery_charging_info(&info);

			/* Display status */
			printf("t = %5d, voltage = %4d mV, current = %4d mA, capacity = %2d %%, temperature = %3d C, temp103 = %3d C\n",
						sec, info.voltage, info.current, info.capacity, info.temperature, info.tmp103_temp );

			/* Temperature check */
			if ((info.temperature > TEMPERATURE_HOT_LIMIT) ||
				(info.temperature < TEMPERATURE_COLD_LIMIT)) {
				printf("Battery temperature threshold reached: %d C", info.temperature);

				/* FIXME: do we need to show something in the dispay here!! */
				shut_down();
			}

			/* Charging current check */
			if (info.current <= 0 && ++discharge_count > DISCHARGING_COUNT) {
				printf("Battery is being discharged for too long\n");
				shut_down();
			}
			else if (info.current > 0) {
				/* Reset discharge count */
				discharge_count = 0;
			}

			/* Software charging time check */
			if (sec > MAX_CHARGING_TIME_TOO_LONG) {
				printf("Battery has been charged for too long: %d s\n", sec);
				shut_down();
			}

			/* Check if battery is good enough to boot into system */
			if ((info.voltage >= LOW_BATTERY_VOLTAGE_LIMIT) &&
				(info.capacity >= LOW_BATTERY_CAPACITY_LIMIT)) {
				if (++charge_enough > BATTERY_READY_TO_BOOT_COUNT) {
					printf("Battery is enough to boot into system, breaking out of loop\n");
					break;
				}
			}
			else
				charge_enough = 0;

#if 0
			/* Check for Wall Charger and exit if present. */
			if (info.current > LOW_CURRENT_LIMIT) {
				printf("Charging current is high enough to boot into system.");
				break;
			}
#endif

			/* Delay for ~5 seconds and check the power key status */
			pwr_key_pressed = check_pwr_key_press(&powerkey_ms);

			if ((pwr_key_pressed || !charge_screen_displayed) &&
				((info.voltage > LOW_BATTERY_SPLASH_VOLTAGE_LIMIT) || (wall_charger == 1) ) ) {
				if(info.current > 0)
					show_error_logo(PIC_CHARGING, powerkey_ms, 5000);
				else
					show_error_logo(PIC_LOWBATTERY, powerkey_ms, 5000);

				charge_screen_displayed = 1;
				sec += 5;
			}
			else {
				sec += 1;
			}
		}

		printf("Leaving charge loop...\n");

		scale_reinit_vcores(BATTERY_CHRG_OFF);
		prcm_reinit_battery_chrg(BATTERY_CHRG_OFF);
	}
}

