//#include <asm/arch/mux.h>
//#include <asm/io.h>
//#include <asm/arch/sys_proto.h>
#include <mmc.h>
#include <idme.h>

extern const u8 *idme_get_board_id(void); /* Read Board id for reconfig Pinmux*/

/*Pinmux difference table between Bowser5 and EVT2*/
#define JEM_EVT2_PINMUX_CHANGES() \
	MV(CP(GPMC_AD8) , ( PTU | IDIS | M3))  /* gpio_32 => emmc_reset_b_1v8 */ \
	MV(CP(GPMC_AD9) , ( M0))  /* gpio_33 => NC */ \
	MV(CP(GPMC_AD10) , ( M0))  /* gpio_34 => NC */ \
	MV(CP(GPMC_AD11) , ( PTD | IDIS | M3))  /* gpio_35 => LCDPANEL_EN */ \
	MV(CP(GPMC_AD12) , ( PTU | IEN | M3))  /* gpio_36 => ALT_INT_N */ \
	MV(CP(GPMC_AD13) , ( PTD | IDIS | M3))  /* gpio_37 => BKL_CABC_APS_EN */ \
	MV(CP(GPMC_AD14) , ( M0))  /* gpio_38 => NC */ \
	MV(CP(GPMC_AD15) , ( PTD | IDIS | M3))  /* gpio_39 => CHRG_EXTCHRG_ENZ */ \
	MV(CP(GPMC_A16) , ( PTD | IEN | M7))  /* gpio_40 => NC */ \
	MV(CP(GPMC_A18) , ( PTD | IEN | M7))  /* gpio_42 => NC */ \
	MV(CP(GPMC_A19) , ( PTD | IDIS | M3))  /* gpio_43 => WLAN_EN */ \
	MV(CP(GPMC_A20) , ( PTU | IEN | M3))  /* gpio_44 => WLAN_HOSTWAKE */ \
	MV(CP(GPMC_A21) , ( PTU | IDIS | M3))  /* gpio_45 => CHRG_SUSP */ \
	MV(CP(GPMC_A22) , ( PTD | IDIS | M3))  /* gpio_46 => BTGPS_EN */ \
	MV(CP(GPMC_A23) , ( PTU | IDIS | M3))  /* gpio_47 => CHRG_USB_HCS */ \
	MV(CP(GPMC_A24) , ( PTD | IEN | M7))  /* gpio_48 => NC */ \
	MV(CP(GPMC_NCS1) , ( PTD | IEN | M7))  /* gpio_51 => NC */ \
	MV(CP(GPMC_NCS2) , ( PTD | IEN | M7))  /* gpio_52 => NC */ \
	MV(CP(GPMC_NCS3) , ( PTD | IEN | M7))  /* gpio_53 => NC */ \
	MV(CP(GPMC_NWP) , ( PTD | IDIS | M3))  /* gpio_54 => GPADC_START */ \
	MV(CP(GPMC_CLK) , ( M0))  /* gpio_55 => NC */ \
	MV(CP(GPMC_NADV_ALE) , ( M0))  /* gpio_56 => NC */ \
	MV(CP(GPMC_NBE0_CLE) , ( M0))  /* gpio_59 => NC */ \
	MV(CP(GPMC_WAIT0) , ( M0))  /* gpio_61 => NC */ \
	MV(CP(GPMC_WAIT1) , ( PTU | IDIS | M3))  /* gpio_62 => USB_NRESET */ \
	MV(CP(C2C_DATA11) , ( M0))  /*  NC */ \
	MV(CP(C2C_DATA12) , ( M0))  /*  NC */ \
	MV(CP(C2C_DATA13) , ( M0))  /*  NC */ \
	MV(CP(C2C_DATA14) , ( PTD | IEN | M7))  /*  GPIO_103 => ENG_PROD */ \
	MV(CP(C2C_DATA15) , ( PTD | IEN | M7))  /*  NC */ \
        MV(CP(CAM_SHUTTER) , ( OFF_EN | OFF_PD | OFF_OUT_PTD | M0))  /* NC */ \
        MV(CP(CAM_STROBE) , ( PTU | IEN | M3))  /* HOST_WAKE */ \
        MV(CP(CAM_GLOBALRESET) , ( PTD | M3))  /* H_CAM_2M_RESET */ \
	MV(CP(USBB1_ULPITLL_CLK) , ( PTD | IEN | M7))  /* gpio_84 => NC */ \
	MV(CP(USBB1_ULPITLL_STP) , ( PTU | IEN | M3))  /* gpio_85 => PROX_INT_N */ \
	MV(CP(USBB1_ULPITLL_DIR) , ( PTD | IEN | M7))  /* gpio_86 => NC */ \
	MV(CP(USBB1_ULPITLL_NXT) , ( PTD | IEN | M7))  /* gpio_87 => NC */ \
	MV(CP(USBB1_ULPITLL_DAT0) , ( PTD | IEN | M7))  /* gpio_88 => NC */ \
	MV(CP(USBB1_ULPITLL_DAT1) , ( PTD | IEN | M7))  /* gpio_89 => NC */ \
	MV(CP(USBB1_ULPITLL_DAT2) , ( PTD | IEN | M7))  /* gpio_90 => NC */ \
	MV(CP(USBB1_ULPITLL_DAT3) , ( PTD | IEN | M7))  /* gpio_91 => NC */ \
	MV(CP(USBB1_ULPITLL_DAT4) , ( PTD | IEN | M7))  /* gpio_92 => NC */ \
	MV(CP(USBB1_ULPITLL_DAT5) , ( PTD | IEN | M7))  /* gpio_93 => NC */ \
	MV(CP(USBB1_ULPITLL_DAT6) , ( PTD | IEN | M7))  /* gpio_94 => NC */ \
	MV(CP(USBB1_ULPITLL_DAT7) , ( PTD | IEN | M7))  /* gpio_95 => NC */ \
	MV(CP(USBB1_HSIC_DATA) , ( PTD | IEN | M7))  /* gpio_96 => NC */ \
	MV(CP(USBB1_HSIC_STROBE) , ( PTD | IEN | M7))  /* gpio_97 => NC */ \
	MV(CP(USBC1_ICUSB_DP) , ( PTD | IEN | M7))  /* gpio_98 => NC */ \
	MV(CP(USBC1_ICUSB_DM) , ( PTD | IEN | M7))  /* gpio_99 => NC */ \
	MV(CP(SDMMC1_CLK) , ( PTD | IEN | M7))  /* gpio_100 => NC */ \
	MV(CP(SDMMC1_CMD) , ( PTD | IEN | M7))  /* gpio_101 => NC */ \
	MV(CP(SDMMC1_DAT0) , ( PTD | IEN | M7))  /* gpio_102 => NC */ \
	MV(CP(SDMMC1_DAT1) , ( PTD | IEN | M7))  /* gpio_103 => NC */ \
	MV(CP(SDMMC1_DAT2) , ( PTD | IEN | M7))  /* gpio_104 => NC */ \
	MV(CP(SDMMC1_DAT3) , ( PTD | IEN | M7))  /* gpio_105 => NC */ \
	MV(CP(SDMMC1_DAT4) , ( PTD | IEN | M7))  /* gpio_106 => NC */ \
	MV(CP(SDMMC1_DAT5) , ( PTD | IEN | M7))  /* gpio_107 => NC */ \
	MV(CP(SDMMC1_DAT6) , ( PTD | IEN | M7))  /* gpio_108 => NC */ \
	MV(CP(SDMMC1_DAT7) , ( PTD | IEN | M7))  /* gpio_109 => NC */ \
	MV(CP(HDQ_SIO) , ( PTD | IEN | M7))  /* gpio_127 => NC */ \
	MV(CP(MCSPI1_CLK) , ( PTD | IEN | M7))  /* gpio_134 => NC */ \
	MV(CP(MCSPI1_SOMI) , ( PTD | IEN | M7))  /* gpio_135 => NC */ \
	MV(CP(MCSPI1_SIMO) , ( PTD | IEN | M7))  /* gpio_136 => NC */ \
	MV(CP(MCSPI1_CS0) , ( PTD | IEN | M7))  /* gpio_137 => NC */ \
	MV(CP(MCSPI1_CS1) , ( PTD | IEN | M7))  /* gpio_138 => NC */ \
	MV(CP(MCSPI1_CS2) , ( PTD | IEN | M7))  /* gpio_139 => NC */ \
	MV(CP(MCSPI1_CS3) , ( PTD | IEN | M7))  /* gpio_140 => NC */ \
	MV(CP(UART3_CTS_RCTX) , ( PTD | IEN | M7))  /* gpio_141 => NC */ \
	MV(CP(UART3_RTS_SD) , ( PTD | IEN | M7))  /* gpio_142 => NC */ \
	MV(CP(UART3_RX_IRRX) , ( PTU | IEN | M0))  /* H_UART3_RX_IRRX */ \
	MV(CP(UART3_TX_IRTX) , ( M0))  /* uart3_tx */ \
	MV(CP(SDMMC5_CLK) , ( PTD | M0))  /* WLAN_SDIO_CLK */ \
	MV(CP(SDMMC5_CMD) , ( PTU | M0))  /* WLAN_SDIO_CMD */ \
	MV(CP(SDMMC5_DAT0) , ( PTU | M0))  /* WLAN_SDIO_D0 */ \
	MV(CP(SDMMC5_DAT1) , ( PTU | M0))  /* WLAN_SDIO_D1 */ \
	MV(CP(SDMMC5_DAT2) , ( PTU | M0))  /* WLAN_SDIO_D2 */ \
	MV(CP(SDMMC5_DAT3) , ( PTU | M0))  /* WLAN_SDIO_D3 */ \
	MV(CP(MCSPI4_CLK) , ( PTD | IEN | M7))  /* gpio_151 => NC */ \
	MV(CP(MCSPI4_SIMO) , ( PTD | IEN | M7))  /* gpio_152 => NC */ \
	MV(CP(MCSPI4_SOMI) , ( PTD | IEN | M7))  /* gpio_153 => NC */ \
	MV(CP(MCSPI4_CS0) , ( PTD | IEN | M7))  /* gpio_154 => NC */ \
	MV(CP(UART4_RX) , ( PTD | IEN | M7))  /* gpio_155 => NC */ \
	MV(CP(UART4_TX) , ( PTD | IEN | M7))  /* gpio_156 => NC */ \
	MV(CP(USBB2_HSIC_DATA) , ( PTD | IEN | M7))  /* gpio_169 => NC */ \
	MV(CP(USBB2_HSIC_STROBE) , ( PTD | IEN | M7))  /* gpio_170 => NC */ \
	MV(CP(UNIPRO_TY0) , ( PTD | IEN | M7))  /* gpio_172 => NC */ \
	MV(CP(UNIPRO_TX0) , ( PTU | IEN | M3))  /* gpio171 => HDSET_DETECT */ \
	MV(CP(UNIPRO_TX1) , ( PTU | IEN | M3))  /* gpio173 => CHRG_SYS_OK */ \
	MV(CP(UNIPRO_TY1) , ( PTD | IEN | M7))  /* gpio174 => NC */ \
	MV(CP(UNIPRO_TX2) , ( PTD | IEN | M7))  /* gpio_0 => NC */ \
	MV(CP(UNIPRO_TY2) , ( PTD | IEN | M7))  /* gpio_1 => NC */ \
	MV(CP(UNIPRO_RX0) , ( PTD | IEN | M7))  /* gpio_175 => NC */ \
	MV(CP(UNIPRO_RY0) , ( PTD | IEN | M7))  /* gpio_176 => NC */ \
	MV(CP(UNIPRO_RX1) , ( PTD | IEN | M7))  /* gpio_177 => NC */ \
	MV(CP(UNIPRO_RY1) , ( PTD | IEN | M7))  /* gpio_178 => NC */ \
	MV(CP(UNIPRO_RX2) , ( PTD | IEN | M7))  /* gpio_2 => NC */ \
	MV(CP(UNIPRO_RY2) , ( PTD | IEN | M7))  /* gpio_3 => NC */ \
	MV(CP(USBA0_OTG_CE) , ( M0))  /* NC */ \
	MV(CP(FREF_CLK2_OUT) , ( PTD | IEN | M7))  /* gpio_182 => NC */ \
	MV(CP(SYS_BOOT0) , ( PTD | IEN | M7))  /* gpio_184 => NC */ \
	MV(CP(SYS_BOOT1) , ( PTD | IEN | M7))  /* gpio_185 => NC */ \
	MV(CP(SYS_BOOT2) , ( PTD | IEN | M7))  /* gpio_186 => NC */ \
	MV(CP(SYS_BOOT3) , ( PTD | IEN | M7))  /* gpio_187 => NC */ \
	MV(CP(SYS_BOOT4) , ( PTD | IEN | M7))  /* gpio_188 => NC */ \
	MV(CP(DPM_EMU0) , ( PTD | IEN | M7))  /* gpio_11 => NC */ \
	MV(CP(DPM_EMU1) , ( PTD | IEN | M7))  /* gpio_12 => NC */ \
	MV(CP(DPM_EMU2) , ( PTD | IEN | M7))  /* gpio_13 => NC */ \
	MV(CP(DPM_EMU3) , ( PTD | IEN | M7))  /* gpio_14 => NC */ \
	MV(CP(DPM_EMU4) , ( PTD | IDIS | M3))  /* gpio_15 => WAN_HMI */ \
	MV(CP(DPM_EMU5) , ( PTD | IEN | M3))  /* gpio_16 => WAN_FW_RDY */ \
	MV(CP(DPM_EMU6) , ( IEN | M3))  /* gpio_17 => SIM_PRESENT_B */ \
	MV(CP(DPM_EMU7) , ( PTU | IDIS | M3))  /* gpio_18 => WAN_ON_OFF */ \
	MV(CP(DPM_EMU8) , ( PTU | IDIS | M3))  /* gpio_19 => WAN_RST_N */ \
	MV(CP(DPM_EMU9) , ( PTD | IDIS | M3))  /* gpio_20 => WAN_USB_EN */ \
	MV(CP(DPM_EMU10) , ( PTD | IEN | M7))  /* gpio_21 => NC */ \
	MV(CP(DPM_EMU11) , ( PTD | IEN | M7))  /* gpio_22 => NC */ \
	MV(CP(DPM_EMU12) , ( PTU | IDIS | M3))  /* gpio_23 => TOUCH_RST_N */ \
	MV(CP(DPM_EMU13) , ( PTU | IEN | M3))  /* gpio_24 => TOUCH_INT_N */ \
	MV(CP(DPM_EMU14) , ( PTD | IDIS | M3))  /* gpio_25 => LCDBKL_EN */ \
	MV(CP(DPM_EMU15) , ( PTD | IEN | M3))  /* gpio_26 => AUDIO_GPIO1_A */ \
	MV(CP(DPM_EMU16) , ( PTD | IEN | M7))  /* gpio_27 => NC */ \
	MV(CP(DPM_EMU17) , ( PTD | IEN | M7))  /* gpio_28 => NC */ \
	MV(CP(DPM_EMU19) , ( PTD | IEN | M7))  /* NC */ \
	MV1(WK(PAD0_SIM_IO) , ( PTU | IEN | M3))  /* HALL_EFFECT */ \
	MV1(WK(PAD0_SIM_RESET) , ( IEN | M3))  /* CHGR_STAT */ \
	MV1(WK(PAD0_SIM_PWRCTRL) , ( PTU | IEN | M3))  /* airq_1 */ \
	MV1(WK(PAD1_FREF_CLK4_REQ) , ( PTU | IEN | M3))  /* ON_BOTTON */\
	MV1(WK(PAD0_FREF_CLK4_OUT) , ( M0))  /* NC */

/**
*	Board ID Table
*
*	Board ID Format
*	Type Code(3bit) + Rev(2bit) + Reserve(11bit)
*
*	Type code: 001
*	0010200000000000 For EVT1_2 Wifi
*	0010300000000000 For EVT1_2 WAN
*
*	Type code: 801 (WiFi Tpye)
*	8010100000000000 For EVT2
*	8010200000000000 For EVT3
*
*	Type code: 802 (WAN Type)
*	8020100000000000 For EVT2
*	8020200000000000 For EVT3
*
*/

void JEM_MUX_INIT(void)
{
	const u8 *board_id = 0;
	char board_id_string[16];

	board_id=idme_get_board_id();

	if (!board_id){
		printf("Error, failed to get_board_id\n");
	}else{
		sprintf(board_id_string,"%s", (unsigned)board_id);

		if ((strncmp(board_id_string, "801", 3) == 0) || (strncmp(board_id_string, "802", 3) == 0)){
			printf("Set JEM EVT2 pin mux changes\n");
			JEM_EVT2_PINMUX_CHANGES();
		}else{
			printf("WARN: Unknow board id, assume JEM EVT1.2 board.\n");
		}
	}
}

