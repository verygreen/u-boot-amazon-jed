/*
 * Backlight driver for bowser board, based on TI & O2Micro chips
 *
 * Copyright (C) Amazon Technologies Inc. All rights reserved.
 * Feng Ye (fengy@lab126.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <common.h>
#include <command.h>
#include <asm/io.h>
#include <asm/errno.h>

#include <dmtimer.h>
#include <bowser_bl.h>

#define TIMERVALUE_OVERFLOW    0xFFFFFFFF
#define TIMERVALUE_RELOAD      (TIMERVALUE_OVERFLOW - ctx->timer_range) /* the reload value when timer overflows, the bigger the shorter the whole duty cycle */

#define BL_LOG(fmt, args...) printf("bowser_bl: " fmt, ## args)

struct bowser_backlight_ctx {
	struct omap_dm_timer *timer;
	int gpio_en_o2m;
	int gpio_vol_o2m;
	int gpio_cabc_en;
	unsigned long timer_range;
	unsigned int brightness_max;
	unsigned int brightness_set; /* brightness will be set to */
	unsigned int brightness;     /* current brightness */
};

static struct bowser_backlight_ctx *g_ctx;
static int bl_initialed = 0;

static void bowser_backlight_set_pwm(struct bowser_backlight_ctx *ctx)
{
	unsigned long cmp_value;

	BL_LOG("brightness_set 0x%02X\n", ctx->brightness_set);

	if (ctx->brightness_set == ctx->brightness)
		return;

	/* Set the new brightness */
	if (ctx->brightness_set == ctx->brightness_max) {
		cmp_value = TIMERVALUE_OVERFLOW+1;
	} else {
		cmp_value = (ctx->timer_range * ctx->brightness_set)/ctx->brightness_max + TIMERVALUE_RELOAD;
	}
	BL_LOG("Bowser backlight cmp val=0x%lX\n", cmp_value);

	if (ctx->brightness_set) {
		omap_dm_timer_enable(ctx->timer);
		omap_dm_timer_set_source(ctx->timer,  OMAP_TIMER_SRC_SYS_CLK);
		omap_dm_timer_set_prescaler(ctx->timer, 0);

		omap_dm_timer_start(ctx->timer);
		omap_dm_timer_set_match(ctx->timer, 1 /* compare enable */, cmp_value);
		omap_dm_timer_set_load(ctx->timer, 1, TIMERVALUE_RELOAD);
		omap_dm_timer_set_pwm(ctx->timer, 0 /* positive pulse */, 1 /* toggle */, OMAP_TIMER_TRIGGER_OVERFLOW_AND_COMPARE /* trigger */);
		/* Enabling it as it's disabled after setting pwm */
		omap_dm_timer_enable(ctx->timer);
	} else {
		omap_dm_timer_stop(ctx->timer);
		omap_dm_timer_disable(ctx->timer);
	}

	ctx->brightness = ctx->brightness_set;
}

static void bowser_backlight_brightness_work(struct bowser_backlight_ctx *ctx, int power_up, int power_down)
{
	if (power_up) {
		BL_LOG("backlight powering up\n");
		/* Enable VIN */
		if (ctx->gpio_vol_o2m >= 0) {
			set_gpio_output(ctx->gpio_vol_o2m, 1);
		}
		/* Enable PWM */
		bowser_backlight_set_pwm(ctx);
		/* Enable the O2M */
		if (ctx->gpio_en_o2m >= 0) {
			set_gpio_output(ctx->gpio_en_o2m, 1);
		}
	} else if (power_down) {
		BL_LOG("backlight powering down\n");
		/* Disable the O2M */
		if (ctx->gpio_en_o2m >= 0) {
			set_gpio_output(ctx->gpio_en_o2m, 0);
		}
		/* Disable PWM */
		bowser_backlight_set_pwm(ctx);
		/* Disable VIN */
		if (ctx->gpio_vol_o2m >= 0) {
			set_gpio_output(ctx->gpio_vol_o2m, 0);
		}
	} else {
		bowser_backlight_set_pwm(ctx);
	}
}

static int bowser_backlight_check_brightness(int brightness, int max_brightness)
{
	int new_brightness = brightness;

	if (brightness < 0)
		new_brightness = 0;
	else if (brightness > max_brightness)
		new_brightness = max_brightness;

	if (new_brightness != brightness)
		BL_LOG("Re-assign brightness as 0x%02X\n", new_brightness);

	return new_brightness;
}

static void bowser_backlight_config_gpio(struct bowser_backlight_ctx *ctx, struct bowser_backlight_platform_data *pdata)
{
	/* gpio request */
	ctx->gpio_en_o2m = pdata->gpio_en_o2m;
	if (pdata->gpio_en_o2m >= 0) {
		//gpio_direction_output(ctx->gpio_en_o2m, 1);
		configure_pad_mode(ctx->gpio_en_o2m);
		configure_gpio_output(ctx->gpio_en_o2m);
		set_gpio_output(ctx->gpio_en_o2m, 0);
	}

	ctx->gpio_vol_o2m = pdata->gpio_vol_o2m;
	if (pdata->gpio_vol_o2m >= 0) {
		//gpio_direction_output(ctx->gpio_vol_o2m, 1);
		configure_pad_mode(ctx->gpio_vol_o2m);
		configure_gpio_output(ctx->gpio_vol_o2m);
		set_gpio_output(ctx->gpio_vol_o2m, 0);
	}

	ctx->gpio_cabc_en = pdata->gpio_cabc_en;
	if (pdata->gpio_cabc_en >= 0) {
		//gpio_direction_output(ctx->gpio_cabc_en, 1);
		configure_pad_mode(ctx->gpio_cabc_en);
		configure_gpio_output(ctx->gpio_cabc_en);
		set_gpio_output(ctx->gpio_cabc_en, 0);
	}

}

void bowser_backlight_set_brightness(int brightness)
{
	struct bowser_backlight_ctx *ctx = g_ctx;
	int power_up = 0, power_down = 0;

	if (!bl_initialed) {
		BL_LOG("backlight not initial\n");
		return;
	}

	brightness = bowser_backlight_check_brightness(brightness, ctx->brightness_max);
	power_up   = !ctx->brightness && brightness;
	power_down = ctx->brightness && !brightness;
	ctx->brightness_set = brightness;

	if (ctx->brightness_set == ctx->brightness) {
		BL_LOG("Do nothing with the same brightness: 0x%02X\n", ctx->brightness);
		return;
	}

	bowser_backlight_brightness_work(ctx, power_up, power_down);
}

int bowser_backlight_get_brightness(void)
{
	struct bowser_backlight_ctx *ctx = g_ctx;
	if (!bl_initialed)
		return 0;

	return ctx->brightness;
}

int bowser_backlight_init(struct bowser_backlight_platform_data *pdata)
{
	struct bowser_backlight_ctx *ctx;

	if (!pdata) {
		BL_LOG("No platform data.\n");
		return -ENODEV;
	}

	ctx = malloc(sizeof(struct bowser_backlight_ctx));
	if (!ctx) {
		BL_LOG("No memory for backlight\n");
		return -1;
	}

	/* timer request */
	ctx->timer = omap_dm_timer_request_specific(pdata->timer, pdata->sysclk);
	if (ctx->timer == NULL) {
		BL_LOG("Failed to request pwm timer\n");
		free(ctx);
		return -1;
	}

	bowser_backlight_config_gpio(ctx, pdata);

	/* timer count range value calculate, the bigger the longer the whole duty cycle */
	/* *2 because PWM high and low count as one cycle */
	ctx->timer_range = (pdata->sysclk / (pdata->pwmfreq * 2));

	/* backlight device register */
	ctx->brightness_max = pdata->totalsteps;
	ctx->brightness = pdata->initialstep;

	g_ctx = ctx;
	bl_initialed = 1;

	bowser_backlight_set_brightness(BRIGHTNESS_OFF);

	return 0;
}

static int do_brightness_commands(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	int retval = 0, brightness = 0;

	if (argc == 2 && !strcmp("get", argv[1])) {
		brightness = bowser_backlight_get_brightness();
		BL_LOG("brightness:0x%02X\n", brightness);
	} else if (argc == 3 && !strcmp("set", argv[1])) {
		brightness = (int)simple_strtol(argv[2], NULL, 10);
		bowser_backlight_set_brightness(brightness);
	} else {
		BL_LOG("Invalid command\n");
		retval = -1;
	}

	return retval;
}

U_BOOT_CMD(
	brightness, 3, 0, do_brightness_commands,
	"brightness - Commands for backlight brightness\n",
        "brightness get\n    Reads current brightness\n"
        "brightness set <value>\n    Writes <value> to change brightness\n"
);

