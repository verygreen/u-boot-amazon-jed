/*
 * (C) Copyright 2006-2009
 * Texas Instruments.
 * Richard Woodruff <r-woodruff2@ti.com>
 * Syed Mohammed Khasim <x0khasim@ti.com>
 *
 * Configuration settings for the 4430 TI SDP4430 board.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef __CONFIG_H
#define __CONFIG_H

/*
 * High Level Configuration Options
 */
#define CONFIG_ARMCORTEXA9	1    /* This is an ARM V7 CPU core */
#define CONFIG_OMAP		1    /* in a TI OMAP core */
#define CONFIG_OMAP44XX		1    /* which is a 44XX */
#define CONFIG_OMAP4430		1    /* which is in a 4430 */
#define CONFIG_4430SDP		1    /* working with SDP */
#define CONFIG_FASTBOOT		1    /* Using fastboot interface */
#define CONFIG_BOARD_REVISION	1    /* board revision id */

#define BOARD_LATE_INIT 1


/* Bowser revisions */
#define BOARD_REVISION_INVALID	0x0
#define BOARD_REVISION_BOWSER_1	0x1
#define BOARD_REVISION_BOWSER_2	0x2


/* disabling I Cache for ZeBu */
/* #define CONFIG_ICACHE_OFF 1 */


//#define CONFIG_OFF_PADCONF	0    /* Enable OFFMODE pad configuration */

#include <asm/arch/cpu.h>        /* get chip and board defs */

/* Clock Defines */
#define V_OSCK                   38400000  /* Clock output from T2 */

#define V_SCLK                   V_OSCK

/* #define PRCM_CLK_CFG2_266MHZ   1    VDD2=1.15v - 133MHz DDR */
#define PRCM_CLK_CFG2_332MHZ     1    /* VDD2=1.15v - 166MHz DDR */
#define PRCM_PCLK_OPP2           1    /* ARM=500MHz - VDD1=1.20v */

#undef CONFIG_USE_IRQ                 /* no support for IRQs */
#define CONFIG_MISC_INIT_R

#define CONFIG_CMDLINE_TAG       1    /* enable passing of ATAGs */
#define CONFIG_SETUP_MEMORY_TAGS 1
#define CONFIG_INITRD_TAG        1
#define CONFIG_REVISION_TAG      1

#define CONFIG_MACADDR_TAG       1 /* enable MAC address and sec number */
#define CONFIG_SERIAL16_TAG      1 /* enable board serial */
#define CONFIG_BOOTMODE_TAG      1 /* enable bootmode */
#define CONFIG_REVISION16_TAG    1 /* enable revision */
#define CONFIG_PRODUCTID_TAG     1 /* enable productid */

/*
 * Size of malloc() pool
 */
#define CFG_ENV_SIZE             SZ_128K    /* Total Size Environment Sector */
#define CFG_MALLOC_LEN           (CFG_ENV_SIZE + SZ_128K)
#define CFG_GBL_DATA_SIZE        128  /* bytes reserved for initial data */

/*
 * Bootmessage (/misc partition)
 */
#define CFG_BOOTMSG             1      /* Enable bootmessage support (used for entering into recovery mode) */
#define CFG_BOOTMSG_IN_EMMC     1      /* bootmsg stored into the emmc */
#define CFG_BOOTMSG_PARTITION   "misc" /* name of the RAW data-partition which holds the bootmessage */

/*
 * Hardware drivers
 */

/*
 * NS16550 Configuration
 */
#define V_NS16550_CLK            (48000000)  /* 48MHz (APLL96/2) */

#define CFG_NS16550
#define CFG_NS16550_SERIAL
#define CFG_NS16550_REG_SIZE     (-4)
#define CFG_NS16550_CLK          V_NS16550_CLK
#define CFG_NS16550_COM3         OMAP44XX_UART3

/*
 * select serial console configuration
 */
#define CONFIG_SERIAL1           1    /* UART1 on SDP */
/* allow to overwrite serial and ethaddr */
#define CONFIG_ENV_OVERWRITE
#define CONFIG_CONS_INDEX        3
#define CONFIG_BAUDRATE          115200
#define CFG_BAUDRATE_TABLE       {4800, 9600, 19200, 38400, 57600, 115200}

#define CONFIG_MMC              1
#define CONFIG_DOS_PARTITION    1

#define NET_CMDS                 (CFG_CMD_DHCP|CFG_CMD_NFS|CFG_CMD_NET)

/* Config CMD*/
#define CONFIG_COMMANDS          ((CFG_CMD_I2C | CONFIG_CMD_DFL | \
	CFG_CMD_JFFS2 | CFG_CMD_MMC | CFG_CMD_FAT) &			\
	~(CFG_CMD_AUTOSCRIPT | CFG_CMD_NAND | CFG_CMD_ONENAND | NET_CMDS))

#define CFG_I2C_SPEED            100
#define CFG_I2C_SLAVE            1
#define CFG_I2C_BUS              0
#define CFG_I2C_BUS_SELECT       1
#define CONFIG_DRIVER_OMAP44XX_I2C 1
#define CONFIG_TWL6030		 1

#define CONFIG_CMD_PMI
#define PMIC_I2C_BUS	0
#define PMIC_I2C_SPEED  10

#undef CONFIG_BOOTP_MASK

/* this must be included AFTER the definition of CONFIG_COMMANDS (if any) */
#include <cmd_confdefs.h>

#define NAND_MAX_CHIPS           1

/* Environment information */
#if defined(CONFIG_4430ZEBU) && !defined(CONFIG_4430ES2)
/* Give the standard Kernel jump command as boot cmd and without any delay */
#define CONFIG_BOOTDELAY         0
#define CONFIG_EXTRA_ENV_SETTINGS \
	"bootcmd=go 0x80008000\0"
#else
 #if !defined(CONFIG_BOOTDELAY)
  #define CONFIG_BOOTDELAY         0
  #define CONFIG_ZERO_BOOTDELAY_CHECK     /* check for keypress on bootdelay==0 */
 #endif
#endif /*CONFIG_4430ZEBU */


#define CONFIG_BOOTARGS "console=ttyO2,115200n8 mem=456M@0x80000000 mem=512M@0xA0000000" \
			" init=/init vram=10M omapfb.vram=0:4M androidboot.console=ttyO2"

#define CONFIG_BOOTCOMMAND "booti mmc1"
#define CONFIG_RECOVERYCMD "booti mmc1 recovery"

#define CONFIG_BOOTFILE          "uImage"
#define CONFIG_AUTO_COMPLETE     1
/*
 * Miscellaneous configurable options
 */
#define V_PROMPT                 "OMAP44XX SDP # "

#define CFG_LONGHELP             /* undef to save memory */
#define CFG_PROMPT               V_PROMPT
#define CFG_CBSIZE               256  /* Console I/O Buffer Size */
/* Print Buffer Size */
#define CFG_PBSIZE               (CFG_CBSIZE+sizeof(CFG_PROMPT)+16)
#define CFG_MAXARGS              16          /* max number of command args */
#define CFG_BARGSIZE             CFG_CBSIZE  /* Boot Argument Buffer Size */

#define CFG_MEMTEST_START        (OMAP44XX_SDRC_CS0)  /* memtest works on */
#define CFG_MEMTEST_END          (OMAP44XX_SDRC_CS0+SZ_31M)

#undef	CFG_CLKS_IN_HZ           /* everything, incl board info, in Hz */

#define CFG_LOAD_ADDR            (OMAP44XX_SDRC_CS0) /* default load address */

/* 2430 has 12 GP timers, they can be driven by the SysClk (12/13/19.2) or by
 * 32KHz clk, or from external sig. This rate is divided by a local divisor.
 */
#define V_PVT                    7

#define CFG_TIMERBASE            OMAP44XX_GPT2
#define CFG_PVT                  V_PVT  /* 2^(pvt+1) */
#define CFG_HZ                   ((V_SCLK)/(2 << CFG_PVT))

/*-----------------------------------------------------------------------
 * Stack sizes
 *
 * The stack sizes are set up in start.S using the settings below
 */
#define CONFIG_STACKSIZE	SZ_128K /* regular stack */
#ifdef CONFIG_USE_IRQ
#define CONFIG_STACKSIZE_IRQ	SZ_4K   /* IRQ stack */
#define CONFIG_STACKSIZE_FIQ	SZ_4K   /* FIQ stack */
#endif

/*-----------------------------------------------------------------------
 * Physical Memory Map : Two memory parts used in in interleaved mode
 */
#define CONFIG_NR_DRAM_BANKS	1
#define PHYS_SDRAM_1		0x80000000
#define PHYS_SDRAM_1_SIZE	SZ_512M

/* SDRAM Bank Allocation method */
/*#define SDRC_B_R_C		1 */
/*#define SDRC_B1_R_B0_C	1 */
#define SDRC_R_B_C		1

/*-----------------------------------------------------------------------
 * FLASH and environment organization
 */

/* **** PISMO SUPPORT *** */

/* Configure the PISMO */
#define PISMO1_NOR_SIZE_SDPV2	GPMC_SIZE_128M
#define PISMO1_NOR_SIZE		GPMC_SIZE_64M

#define PISMO1_NAND_SIZE	GPMC_SIZE_128M
#define PISMO1_ONEN_SIZE	GPMC_SIZE_128M
#define DBG_MPDB_SIZE		GPMC_SIZE_16M
#define PISMO2_SIZE		0

#define CFG_MAX_FLASH_SECT	(520)	/* max number of sectors on one chip */
#define CFG_MAX_FLASH_BANKS      2		/* max number of flash banks */
#define CFG_MONITOR_LEN		SZ_256K 	/* Reserve 2 sectors */

#define PHYS_FLASH_SIZE_SDPV2	SZ_128M
#define PHYS_FLASH_SIZE		SZ_32M


#define CFG_FLASH_BASE		0x0

#define CFG_MONITOR_BASE	CFG_FLASH_BASE /* Monitor at start of flash */
#define CFG_ONENAND_BASE	ONENAND_MAP

#define CFG_ENV_IS_NOWHERE	1
/* Fastboot variables */
#define CFG_FASTBOOT_TRANSFER_BUFFER (PHYS_SDRAM_1 + SZ_64M)
#define CFG_FASTBOOT_TRANSFER_BUFFER_SIZE (SZ_1G - SZ_64M)


/*-----------------------------------------------------------------------
 * CFI FLASH driver setup
 */
#ifndef CONFIG_4430ZEBU
#define CFG_FLASH_CFI		1    /* Flash memory is CFI compliant */
#define CFG_FLASH_CFI_DRIVER	1    /* Use drivers/cfi_flash.c */
#define CFG_FLASH_USE_BUFFER_WRITE 1    /* Use buffered writes (~10x faster) */
#define CFG_FLASH_PROTECTION	1    /* Use hardware sector protection */
#define CFG_FLASH_QUIET_TEST	1    /* Dont crib abt missing chips */
#define CFG_FLASH_CFI_WIDTH	0x02
/* timeout values are in ticks */
#define CFG_FLASH_ERASE_TOUT	(100*CFG_HZ) /* Timeout for Flash Erase */
#define CFG_FLASH_WRITE_TOUT	(100*CFG_HZ) /* Timeout for Flash Write */

/* Flash banks JFFS2 should use */
#define CFG_MAX_MTD_BANKS	(CFG_MAX_FLASH_BANKS+CFG_MAX_NAND_DEVICE)
#define CFG_JFFS2_MEM_NAND
#define CFG_JFFS2_FIRST_BANK	CFG_MAX_FLASH_BANKS /* use flash_info[2] */
#define CFG_JFFS2_NUM_BANKS	1

#endif

#ifndef __ASSEMBLY__
extern unsigned int nand_cs_base;
extern unsigned int boot_flash_base;
extern volatile unsigned int boot_flash_env_addr;
extern unsigned int boot_flash_off;
extern unsigned int boot_flash_sec;
extern unsigned int boot_flash_type;
#endif

#define WRITE_NAND_COMMAND(d, adr) \
	__raw_writew(d, (nand_cs_base + GPMC_NAND_CMD))
#define WRITE_NAND_ADDRESS(d, adr) \
	__raw_writew(d, (nand_cs_base + GPMC_NAND_ADR))
#define WRITE_NAND(d, adr) __raw_writew(d, (nand_cs_base + GPMC_NAND_DAT))
#define READ_NAND(adr) __raw_readw((nand_cs_base + GPMC_NAND_DAT))

/* Other NAND Access APIs */
#define NAND_WP_OFF()  do {*(volatile u32 *)(GPMC_CONFIG) \
	|= 0x00000010; } while (0)
#define NAND_WP_ON()  do {*(volatile u32 *)(GPMC_CONFIG) \
	&= ~0x00000010; } while (0)
#ifdef CFG_NAND_LEGACY
#define NAND_CTL_CLRALE(nandptr)
#define NAND_CTL_SETALE(nandptr)
#define NAND_CTL_CLRCLE(nandptr)
#define NAND_CTL_SETCLE(nandptr)
#endif
#define NAND_DISABLE_CE(nand)
#define NAND_ENABLE_CE(nand)
#define NAND_WAIT_READY(nand)	udelay(10)


/* ported for idme.c */
#define CONFIG_MMC_BOOTFLASH    1  // slot 1 is used for internal emmc
#define CONFIG_MMC_USERDATA_ADDR 0
#define CONFIG_MMC_USERDATA_SIZE (5*1024)
#define CONFIG_MMC_BLOCK_SIZE 512
#define CONFIG_IDME_PARTITION_NUM 1

/* enable to support IDME, disabled by default */
#define CONFIG_ENABLE_IDME

/* enable to support : POWER_KEY + VOL_UP to enter FTM mode */
#define ENABLE_DIAG_KEY
#if 0
/* Bowser uses the BQ27200 gas gauge */
#define CONFIG_BQ27200 1
#endif

/* FIXME: We need a separate build config for Jem */
/* Jem uses the BQ27541 gas gauge */
#define CONFIG_BQ27541 1

/*
 * comment in the following flag to enable the command for checking the image's crc on mmc
 * disabled by default
 */
//#define CONFIG_CMD_CHECKCRC

/* crc32 Buffer*/
#define CRC32_BUFFER        0x02000000 + PHYS_SDRAM_1
#define CRC32_BUFFER_SIZE   0x00400000
#define CRC32_CHECK_UBOOT   0x00000010   /* check crc32 on u-boot.bin */
#define CRC32_CHECK_UIMAGE  0x00000020  /* check crc32 on uImage */
#define CRC32_CHECK_ROOTFS  0x00000040  /* check crc32 on rootfs.img */
#define CRC32_CHECK_MBR     0x00000080     /* check crc32 on mbr-xxx-bin */

/* SMB347 driver */
#define CONFIG_SMB347 1

/* Jem uses the PWM backlight */
#define CONFIG_BOWSER_BACKLIGHT_PWM 1
#define BRIGHTNESS_OFF 0
#define BRIGHTNESS_LOWBATTERY_ON 64 /* 25% */
#define BRIGHTNESS_NORMAL_ON 102 /* 40% */

#define CONFIG_ENABLE_USER_ACCESS_IDME 1
#define CONFIG_MACH_BOWSER_SUBTYPE_JEM

#define CONFIG_FASTBOOT_COUNTDOWN    1
#define CFG_FASTBOOT_COUNTDOWN   700000

/* Fastboot download speed enhance */
#define CONFIG_FASTBOOT_ENHANCE_SPEED

#endif                           /* __CONFIG_H */
