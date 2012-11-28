/*
 * (C) Copyright 2004-2009
 * Texas Instruments, <www.ti.com>
 * Richard Woodruff <r-woodruff2@ti.com>
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
#include <common.h>
#include <asm/arch/mux.h>
#include <asm/io.h>
#include <asm/arch/sys_proto.h>

#define		OMAP44XX_WKUP_CTRL_BASE		0x4A31E000
#if 1
#define M0_SAFE M0
#define M1_SAFE M1
#define M2_SAFE M2
#define M4_SAFE M4
#define M7_SAFE M7
#define M3_SAFE M3
#define M5_SAFE M5
#define M6_SAFE M6
#else
#define M0_SAFE M7
#define M1_SAFE M7
#define M2_SAFE M7
#define M4_SAFE M7
#define M7_SAFE M7
#define M3_SAFE M7
#define M5_SAFE M7
#define M6_SAFE M7
#endif
#define		MV(OFFSET, VALUE)\
			__raw_writew((VALUE), OMAP44XX_CTRL_BASE + (OFFSET));
#define		MV1(OFFSET, VALUE)\
			__raw_writew((VALUE), OMAP44XX_WKUP_CTRL_BASE + (OFFSET));

#define		CP(x)	(CONTROL_PADCONF_##x)
#define		WK(x)	(CONTROL_WKUP_##x)

#if defined(CONFIG_MACH_BOWSER_SUBTYPE_JEM)
/*Platform JEM pinmux setting*/
#include "jem_pinmux.c"
#endif

/*
 * IEN  - Input Enable
 * IDIS - Input Disable
 * PTD  - Pull type Down
 * PTU  - Pull type Up
 * DIS  - Pull type selection is inactive
 * EN   - Pull type selection is active
 * M0   - Mode 0
 * The commented string gives the final mux configuration for that pin
 */
#define MUX_DEFAULT_OMAP4() \
	MV(CP(GPMC_AD0) , ( PTU | IEN | OFF_EN | OFF_PD | OFF_IN | M1))  /* sdmmc2_dat0 */ \
	MV(CP(GPMC_AD1) , ( PTU | IEN | OFF_EN | OFF_PD | OFF_IN | M1))  /* sdmmc2_dat1 */ \
	MV(CP(GPMC_AD2) , ( PTU | IEN | OFF_EN | OFF_PD | OFF_IN | M1))  /* sdmmc2_dat2 */ \
	MV(CP(GPMC_AD3) , ( PTU | IEN | OFF_EN | OFF_PD | OFF_IN | M1))  /* sdmmc2_dat3 */ \
	MV(CP(GPMC_AD4) , ( PTU | IEN | OFF_EN | OFF_PD | OFF_IN | M1))  /* sdmmc2_dat4 */ \
	MV(CP(GPMC_AD5) , ( PTU | IEN | OFF_EN | OFF_PD | OFF_IN | M1))  /* sdmmc2_dat5 */ \
	MV(CP(GPMC_AD6) , ( PTU | IEN | OFF_EN | OFF_PD | OFF_IN | M1))  /* sdmmc2_dat6 */ \
	MV(CP(GPMC_AD7) , ( PTU | IEN | OFF_EN | OFF_PD | OFF_IN | M1))  /* sdmmc2_dat7 */ \
	MV(CP(GPMC_AD8) , ( PTU | IEN | M3))  /* gpio_32 => emmc_reset_b_1v8 */ \
	MV(CP(GPMC_AD9) , ( PTU | IEN | M3))  /* gpio_33 => emmc_wp_sw */ \
	MV(CP(GPMC_AD10) , ( PTU | IEN | M3))  /* gpio_34 => emmc_cd_sw */ \
	MV(CP(GPMC_AD11) , ( PTD | IEN | M3))  /* gpio_35 */ \
	MV(CP(GPMC_AD12) , ( PTD | IEN | M3))  /* gpio_36 */ \
	MV(CP(GPMC_AD13) , ( PTD | IEN | M3))  /* gpio_37 */ \
	MV(CP(GPMC_AD14) , ( PTD | IEN | M3))  /* gpio_38 */ \
	MV(CP(GPMC_AD15) , ( PTD | IEN | M3))  /* gpio_39 */ \
	MV(CP(GPMC_A16) , ( PTD | IEN | OFF_EN | OFF_PD | OFF_IN | M3))  /* gpio_40 => bridge_resx */ \
	MV(CP(GPMC_A17) , ( PTD | M3))  /* gpio_41 ==> HDMI_LS_OE*/ \
	MV(CP(GPMC_A18) , ( PTD | IEN | M3))  /* gpio_42 => FM_IRQ */ \
	MV(CP(GPMC_A19) , ( PTD | IEN | OFF_EN | OFF_PD | OFF_IN | M3))  /* gpio_43 => WLAN_EN */ \
	MV(CP(GPMC_A20) , ( PTD | IEN | OFF_EN | OFF_PD | OFF_IN | M3))  /* gpio_44 => FM_EN */ \
	MV(CP(GPMC_A21) , ( PTU | IEN | M3))  /* gpio_45 => CHRG_SUSP */ \
	MV(CP(GPMC_A22) , ( PTD | IEN | OFF_EN | OFF_PD | OFF_IN | M3))  /* gpio_46 => BT_EN */ \
	MV(CP(GPMC_A23) , ( PTU | IEN | M3))  /* gpio_47 => CHRG_USB_HCS */ \
	MV(CP(GPMC_A24) , ( PTD | IEN | OFF_EN | OFF_PD | OFF_IN |M3))  /* gpio_48 => chrgled_en */ \
	MV(CP(GPMC_A25) , ( PTD | M3))  /* gpio_49 => BT_WAKEUP */ \
	MV(CP(GPMC_NCS0) , ( PTU | IEN | M3))  /* gpio_50 => volume_down */ \
	MV(CP(GPMC_NCS1) , ( PTU | M7))  /* Safe_mode */ \
	MV(CP(GPMC_NCS2) , ( PTU | M7))  /* Safe_mode */ \
	MV(CP(GPMC_NCS3) , ( IEN | M3))  /* gpio_53 => WLAN_NIRQ */ \
	MV(CP(GPMC_NWP) , ( PTD | IEN | M3))  /* gpio_54 => GPADC_START */ \
	MV(CP(GPMC_CLK) , ( PTD | IEN | M3))  /* gpio_55 */ \
	MV(CP(GPMC_NADV_ALE) , ( PTD | IEN | M3))  /* gpio_56 => GPS_IRQ */ \
	MV(CP(GPMC_NOE) , ( PTU | IEN | OFF_EN | OFF_OUT_PTD | M1))  /* sdmmc2_clk */ \
	MV(CP(GPMC_NWE) , ( PTU | IEN | OFF_EN | OFF_PD | OFF_IN | M1))  /* sdmmc2_cmd */ \
	MV(CP(GPMC_NBE0_CLE) , ( PTD | IEN | OFF_EN | OFF_PD | OFF_IN | M3))  /* gpio_59 => GPS_EN */ \
	MV(CP(GPMC_NBE1) , ( PTD | M3))  /* gpio_60 => HDMI_CT_CP_HPD */ \
	MV(CP(GPMC_WAIT0) , ( PTD | IEN | OFF_EN | OFF_PD | OFF_IN | M3))  /* gpio_61 => USB_EXT_PWR_EN */ \
	MV(CP(GPMC_WAIT1) , ( PTU | IEN | M3))  /* gpio_62 => USB_NRESET */ \
	MV(CP(C2C_DATA11) , ( PTU | IEN | M3))  /* board_id1 */ \
	MV(CP(C2C_DATA12) , ( PTU | IEN | M3))  /* board_id2 */ \
	MV(CP(C2C_DATA13) , ( PTU | IEN | M3))  /* board_id3 */ \
	MV(CP(C2C_DATA14) , ( PTU | IEN | M3))  /* board_id4 */ \
	MV(CP(C2C_DATA15) , ( PTD | M7))  /* Safe_mode */ \
	MV(CP(HDMI_HPD) , ( M0))  /* hdmi_hpd */ \
	MV(CP(HDMI_CEC) , ( M0))  /* hdmi_cec */ \
	MV(CP(HDMI_DDC_SCL) , ( PTU | M0))  /* hdmi_ddc_scl */ \
	MV(CP(HDMI_DDC_SDA) , ( PTU | IEN | M0))  /* hdmi_ddc_sda */ \
	MV(CP(CSI21_DX0) , ( IEN | M0))  /* csi21_dx0 */ \
	MV(CP(CSI21_DY0) , ( IEN | M0))  /* csi21_dy0 */ \
	MV(CP(CSI21_DX1) , ( IEN | M0))  /* csi21_dx1 */ \
	MV(CP(CSI21_DY1) , ( IEN | M0))  /* csi21_dy1 */ \
	MV(CP(CSI21_DX2) , ( IEN | M0))  /* csi21_dx2 */ \
	MV(CP(CSI21_DY2) , ( IEN | M0))  /* csi21_dy2 */ \
	MV(CP(CSI21_DX3) , ( IEN | M0))  /* csi21_dx3 */ \
	MV(CP(CSI21_DY3) , ( IEN | M0))  /* csi21_dy3 */ \
	MV(CP(CSI21_DX4) , ( IEN | M0))  /* csi21_dx4 */ \
	MV(CP(CSI21_DY4) , ( IEN | M0))  /* csi21_dy4 */ \
	MV(CP(CSI22_DX0) , ( IEN | M0))  /* csi22_dx0 */ \
	MV(CP(CSI22_DY0) , ( IEN | M0))  /* csi22_dy0 */ \
	MV(CP(CSI22_DX1) , ( IEN | M0))  /* csi22_dx1 */ \
	MV(CP(CSI22_DY1) , ( IEN | M0))  /* csi22_dy1 */ \
	MV(CP(CAM_SHUTTER) , ( OFF_EN | OFF_PD | OFF_OUT_PTD | M0))  /* cam_shutter */ \
	MV(CP(CAM_STROBE) , ( OFF_EN | OFF_PD | OFF_OUT_PTD | M0))  /* cam_strobe */ \
	MV(CP(CAM_GLOBALRESET) , ( OFF_EN | OFF_PD | OFF_OUT_PTD | M0))  /* cam_globalreset */ \
	MV(CP(USBB1_ULPITLL_CLK) , ( PTD | M7))  /* Safe_mode */ \
	MV(CP(USBB1_ULPITLL_STP) , ( PTU | M7))  /* Safe_mode */ \
	MV(CP(USBB1_ULPITLL_DIR) , ( PTD | M7))  /* Safe_mode */ \
	MV(CP(USBB1_ULPITLL_NXT) , ( PTD | IEN | M2))  /* mcbsp4_fsx */ \
	MV(CP(USBB1_ULPITLL_DAT0) , ( PTD | IEN | M2))  /* mcbsp4_clk */ \
	MV(CP(USBB1_ULPITLL_DAT1) , ( M2))  /* mcbsp4_dx */ \
	MV(CP(USBB1_ULPITLL_DAT2) , ( PTD | IEN | M2))  /* mcbsp4_dr */ \
	MV(CP(USBB1_ULPITLL_DAT3) , ( PTD | M7))  /* Safe_mode */ \
	MV(CP(USBB1_ULPITLL_DAT4) , ( PTD | M7))  /* Safe_mode */ \
	MV(CP(USBB1_ULPITLL_DAT5) , ( PTD | M7))  /* Safe_mode */ \
	MV(CP(USBB1_ULPITLL_DAT6) , ( PTD | M7))  /* Safe_mode */ \
	MV(CP(USBB1_ULPITLL_DAT7) , ( PTD | M7))  /* Safe_mode */ \
	MV(CP(USBB1_HSIC_DATA) , ( IEN | OFF_EN | OFF_PD | OFF_IN | M0))  /* h_usbb1_hsic_data */ \
	MV(CP(USBB1_HSIC_STROBE) , ( IEN | OFF_EN | OFF_PD | OFF_IN | M0))  /* h_usbb1_hsic_strobe */ \
	MV(CP(USBC1_ICUSB_DP) , (PTU | IEN | M3))  /* ACC_EN_B*/ \
	MV(CP(USBC1_ICUSB_DM) , ( PTD | IEN | M3))  /* TSRQ */ \
	MV(CP(SDMMC1_CLK) , ( PTU | OFF_EN | OFF_OUT_PTD | M0))  /* sdmmc1_clk */ \
	MV(CP(SDMMC1_CMD) , ( PTU | IEN | OFF_EN | OFF_PD | OFF_IN | M0))  /* sdmmc1_cmd */ \
	MV(CP(SDMMC1_DAT0) , ( PTU | IEN | OFF_EN | OFF_PD | OFF_IN | M0))  /* sdmmc1_dat0 */ \
	MV(CP(SDMMC1_DAT1) , ( PTU | IEN | OFF_EN | OFF_PD | OFF_IN | M0))  /* sdmmc1_dat1 */ \
	MV(CP(SDMMC1_DAT2) , ( PTU | IEN | OFF_EN | OFF_PD | OFF_IN | M0))  /* sdmmc1_dat2 */ \
	MV(CP(SDMMC1_DAT3) , ( PTU | IEN | OFF_EN | OFF_PD | OFF_IN | M0))  /* sdmmc1_dat3 */ \
	MV(CP(SDMMC1_DAT4) , ( PTU | IEN | OFF_EN | OFF_PD | OFF_IN | M0))  /* sdmmc1_dat4 */ \
	MV(CP(SDMMC1_DAT5) , ( PTU | IEN | OFF_EN | OFF_PD | OFF_IN | M0))  /* sdmmc1_dat5 */ \
	MV(CP(SDMMC1_DAT6) , ( PTU | IEN | OFF_EN | OFF_PD | OFF_IN | M0))  /* sdmmc1_dat6 */ \
	MV(CP(SDMMC1_DAT7) , ( PTU | IEN | OFF_EN | OFF_PD | OFF_IN | M0))  /* sdmmc1_dat7 */ \
	MV(CP(ABE_MCBSP2_CLKX) , ( IEN | OFF_EN | OFF_PD | OFF_IN | M0))  /* h_mcbsp2_clkx */ \
	MV(CP(ABE_MCBSP2_DR) , ( IEN | OFF_EN | OFF_OUT_PTD | M0))  /* h_mcbsp2_dr */ \
	MV(CP(ABE_MCBSP2_DX) , ( OFF_EN | OFF_OUT_PTD | M0))  /* h_mcbsp2_dx */ \
	MV(CP(ABE_MCBSP2_FSX) , ( IEN | OFF_EN | OFF_PD | OFF_IN | M0))  /* h_mcbsp2_fsx */ \
	MV(CP(ABE_MCBSP1_CLKX) , ( IEN | OFF_EN | OFF_PD | OFF_IN | M0))  /* abe_mcbsp1_clkx */ \
	MV(CP(ABE_MCBSP1_DR) , ( IEN | OFF_EN | OFF_OUT_PTD | M0))  /* abe_mcbsp1_dr */ \
	MV(CP(ABE_MCBSP1_DX) , ( OFF_EN | OFF_OUT_PTD | M0))  /* abe_mcbsp1_dx */ \
	MV(CP(ABE_MCBSP1_FSX) , ( IEN | OFF_EN | OFF_PD | OFF_IN | M0))  /* abe_mcbsp1_fsx */ \
	MV(CP(ABE_PDM_UL_DATA) , ( PTD | IEN | OFF_EN | OFF_PD | OFF_IN | M1))  /* abe_mcbsp3_dr */ \
	MV(CP(ABE_PDM_DL_DATA) , ( PTD | IEN | OFF_EN | OFF_PD | OFF_IN | M1))  /* abe_mcbsp3_dx */ \
	MV(CP(ABE_PDM_FRAME) , ( PTU | IEN | OFF_EN | OFF_PD | OFF_IN | M1))  /* abe_mcbsp3_fsx */ \
	MV(CP(ABE_PDM_LB_CLK) , ( PTD | IEN | OFF_EN | OFF_PD | OFF_IN | M1))  /* abe_mcbsp3_clk */ \
	MV(CP(ABE_CLKS) , ( PTD | IEN | OFF_EN | OFF_PD | OFF_IN | M0))  /* abe_clks */ \
	MV(CP(ABE_DMIC_CLK1) , ( M0))  /* abe_dmic_clk1 */ \
	MV(CP(ABE_DMIC_DIN1) , ( IEN | M0))  /* abe_dmic_din1 */ \
	MV(CP(ABE_DMIC_DIN2) , ( PTD | IEN | M3))  /* gpio_121 */ \
	MV(CP(ABE_DMIC_DIN3) , ( PTU | IEN | M3))  /* gpio_122 */ \
	MV(CP(UART2_CTS) , ( PTU | IEN | M0))  /* uart2_cts */ \
	MV(CP(UART2_RTS) , ( M0))  /* uart2_rts */ \
	MV(CP(UART2_RX) , ( PTU | IEN | M0))  /* uart2_rx */ \
	MV(CP(UART2_TX) , ( M0))  /* uart2_tx */ \
	MV(CP(HDQ_SIO) , ( M3))  /* gpio_127 */ \
	MV(CP(I2C1_SCL) , ( PTU | IEN | M0))  /* i2c1_scl */ \
	MV(CP(I2C1_SDA) , ( PTU | IEN | M0))  /* i2c1_sda */ \
	MV(CP(I2C2_SCL) , ( PTU | IEN | M0))  /* i2c2_scl */ \
	MV(CP(I2C2_SDA) , ( PTU | IEN | M0))  /* i2c2_sda */ \
	MV(CP(I2C3_SCL) , ( PTU | IEN | M0))  /* i2c3_scl */ \
	MV(CP(I2C3_SDA) , ( PTU | IEN | M0))  /* i2c3_sda */ \
	MV(CP(I2C4_SCL) , ( PTU | IEN | M0))  /* i2c4_scl */ \
	MV(CP(I2C4_SDA) , ( PTU | IEN | M0))  /* i2c4_sda */ \
	MV(CP(MCSPI1_CLK) , ( IEN | OFF_EN | OFF_PD | OFF_IN | M0))  /* mcspi1_clk */ \
	MV(CP(MCSPI1_SOMI) , ( IEN | OFF_EN | OFF_PD | OFF_IN | M0))  /* mcspi1_somi */ \
	MV(CP(MCSPI1_SIMO) , ( IEN | OFF_EN | OFF_PD | OFF_IN | M0))  /* mcspi1_simo */ \
	MV(CP(MCSPI1_CS0) , ( PTD | IEN | OFF_EN | OFF_PD | OFF_IN | M0))  /* mcspi1_cs0 */ \
	MV(CP(MCSPI1_CS1) , ( PTU | IEN | M1))  /* uart1_rx */ \
	MV(CP(MCSPI1_CS2) , ( PTU | IEN | M1))  /* uart1_cts */ \
	MV(CP(MCSPI1_CS3) , ( M1))  /* uart1_rts */ \
	MV(CP(UART3_CTS_RCTX) , ( M1))  /* uart1_tx */ \
	MV(CP(UART3_RTS_SD) , ( PTU | M7))  /* safe_mode */ \
	MV(CP(UART3_RX_IRRX) , ( IEN | M0))  /* uart3_rx */ \
	MV(CP(UART3_TX_IRTX) , ( M0))  /* uart3_tx */ \
	MV(CP(SDMMC5_CLK) , ( PTU | IEN | OFF_EN | OFF_OUT_PTD | M0))  /* sdmmc5_clk */ \
	MV(CP(SDMMC5_CMD) , ( PTU | IEN | OFF_EN | OFF_PD | OFF_IN | M0))  /* sdmmc5_cmd */ \
	MV(CP(SDMMC5_DAT0) , ( PTU | IEN | OFF_EN | OFF_PD | OFF_IN | M0))  /* sdmmc5_dat0 */ \
	MV(CP(SDMMC5_DAT1) , ( PTU | IEN | OFF_EN | OFF_PD | OFF_IN | M0))  /* sdmmc5_dat1 */ \
	MV(CP(SDMMC5_DAT2) , ( PTU | IEN | OFF_EN | OFF_PD | OFF_IN | M0))  /* sdmmc5_dat2 */ \
	MV(CP(SDMMC5_DAT3) , ( PTU | IEN | OFF_EN | OFF_PD | OFF_IN | M0))  /* sdmmc5_dat3 */ \
	MV(CP(MCSPI4_CLK) , ( IEN | OFF_EN | OFF_PD | OFF_IN | M0))  /* mcspi4_clk */ \
	MV(CP(MCSPI4_SIMO) , ( IEN | OFF_EN | OFF_PD | OFF_IN | M0))  /* mcspi4_simo */ \
	MV(CP(MCSPI4_SOMI) , ( IEN | OFF_EN | OFF_PD | OFF_IN | M0))  /* mcspi4_somi */ \
	MV(CP(MCSPI4_CS0) , ( PTD | IEN | OFF_EN | OFF_PD | OFF_IN | M0))  /* mcspi4_cs0 */ \
	MV(CP(UART4_RX) , ( IEN | M0))  /* uart4_rx */ \
	MV(CP(UART4_TX) , ( M0))  /* uart4_tx */ \
	MV(CP(USBB2_ULPITLL_CLK) , ( PTD | IEN | OFF_EN | OFF_PD | OFF_IN | M1))  /* usbb2_ulpiphy_clk */ \
	MV(CP(USBB2_ULPITLL_STP) , ( OFF_EN | OFF_OUT_PTD | M1))  /* usbb2_ulpiphy_stp */ \
	MV(CP(USBB2_ULPITLL_DIR) , ( IEN | OFF_EN | OFF_PD | OFF_IN | M1))  /* usbb2_ulpiphy_dir */ \
	MV(CP(USBB2_ULPITLL_NXT) , ( IEN | OFF_EN | OFF_PD | OFF_IN | M1))  /* usbb2_ulpiphy_nxt */ \
	MV(CP(USBB2_ULPITLL_DAT0) , ( IEN | OFF_EN | OFF_PD | OFF_IN | M1))  /* usbb2_ulpiphy_dat0 */ \
	MV(CP(USBB2_ULPITLL_DAT1) , ( IEN | OFF_EN | OFF_PD | OFF_IN | M1))  /* usbb2_ulpiphy_dat1 */ \
	MV(CP(USBB2_ULPITLL_DAT2) , ( IEN | OFF_EN | OFF_PD | OFF_IN | M1))  /* usbb2_ulpiphy_dat2 */ \
	MV(CP(USBB2_ULPITLL_DAT3) , ( IEN | OFF_EN | OFF_PD | OFF_IN | M1))  /* usbb2_ulpiphy_dat3 */ \
	MV(CP(USBB2_ULPITLL_DAT4) , ( IEN | OFF_EN | OFF_PD | OFF_IN | M1))  /* usbb2_ulpiphy_dat4 */ \
	MV(CP(USBB2_ULPITLL_DAT5) , ( IEN | OFF_EN | OFF_PD | OFF_IN | M1))  /* usbb2_ulpiphy_dat5 */ \
	MV(CP(USBB2_ULPITLL_DAT6) , ( IEN | OFF_EN | OFF_PD | OFF_IN | M1))  /* usbb2_ulpiphy_dat6 */ \
	MV(CP(USBB2_ULPITLL_DAT7) , ( IEN | OFF_EN | OFF_PD | OFF_IN | M1))  /* usbb2_ulpiphy_dat7 */ \
	MV(CP(USBB2_HSIC_DATA) , ( PTD | IEN | M0))  /* usbb2_hsic_data */ \
	MV(CP(USBB2_HSIC_STROBE) , ( PTD | IEN | M0))  /* usbb2_hsic_strobe */ \
	MV(CP(UNIPRO_TY0) , ( PTD | M7))  /* safe_mode */ \
	MV(CP(UNIPRO_TX0) , ( PTD | IEN | M3))  /* ON_BUTTON */ \
	MV(CP(UNIPRO_TX1) , ( PTD | M7))  /* safe_mode */ \
	MV(CP(UNIPRO_TY1) , ( PTU | IEN | M3))  /* gpio_174 */ \
	MV(CP(UNIPRO_TX2) , ( PTU | IEN | M3))  /* gpio_0 */ \
	MV(CP(UNIPRO_TY2) , ( PTD | M7))  /* safe_mode */ \
	MV(CP(UNIPRO_RX0) , ( PTD | M7))  /* safe_mode */ \
	MV(CP(UNIPRO_RY0) , ( PTD | M7))  /* safe_mode */ \
	MV(CP(UNIPRO_RX1) , ( PTD | M7))  /* safe_mode */ \
	MV(CP(UNIPRO_RY1) , ( PTD | M7))  /* safe_mode */ \
	MV(CP(UNIPRO_RX2) , ( PTD | M7))  /* safe_mode */ \
	MV(CP(UNIPRO_RY2) , ( PTD | M7))  /* safe_mode */ \
	MV(CP(USBA0_OTG_CE) , ( PTD | OFF_EN | OFF_PD | OFF_OUT_PTD | M0))  /* usba0_otg_ce */ \
	MV(CP(USBA0_OTG_DP) , ( IEN | OFF_EN | OFF_PD | OFF_IN | M0))  /* usba0_otg_dp */ \
	MV(CP(USBA0_OTG_DM) , ( IEN | OFF_EN | OFF_PD | OFF_IN | M0))  /* usba0_otg_dm */ \
	MV(CP(FREF_CLK1_OUT) , ( M0))  /* fref_clk1_out */ \
	MV(CP(FREF_CLK2_OUT) , ( M0))  /* fref_clk2_out */ \
	MV(CP(SYS_NIRQ1) , ( PTU | IEN | M0))  /* sys_nirq1 */ \
	MV(CP(SYS_NIRQ2) , ( PTU | IEN | M0))  /* sys_nirq2 */ \
	MV(CP(SYS_BOOT0) , ( PTD | IEN | M0))  /* sys_boot0 */ \
	MV(CP(SYS_BOOT1) , ( PTD | IEN | M0))  /* sys_boot1 */ \
	MV(CP(SYS_BOOT2) , ( PTD | IEN | M0))  /* sys_boot2 */ \
	MV(CP(SYS_BOOT3) , ( PTD | IEN | M0))  /* sys_boot3 */ \
	MV(CP(SYS_BOOT4) , ( PTD | IEN | M0))  /* sys_boot4 */ \
	MV(CP(SYS_BOOT5) , ( PTD | IEN | M0))  /* sys_boot5 */ \
	MV(CP(DPM_EMU0) , ( IEN | M0))  /* dpm_emu0 */ \
	MV(CP(DPM_EMU1) , ( IEN | M0))  /* dpm_emu1 */ \
	MV(CP(DPM_EMU2) , ( PTD | IEN | M3))  /* gpio_13 */ \
	MV(CP(DPM_EMU3) , ( PTD | IEN | M3))  /* gpio_14 */ \
	MV(CP(DPM_EMU4) , ( PTD | IEN | M3))  /* gpio_15 */ \
	MV(CP(DPM_EMU5) , ( PTD | IEN | M3))  /* gpio_16 */ \
	MV(CP(DPM_EMU6) , ( PTU | IEN | M3))  /* gpio_17 */ \
	MV(CP(DPM_EMU7) , ( M3))  /* gpio_18 */ \
	MV(CP(DPM_EMU8) , ( M3))  /* gpio_19 */ \
	MV(CP(DPM_EMU9) , ( M3))  /* gpio_20 */ \
	MV(CP(DPM_EMU10) , ( PTU | IEN | M3))  /* gpio_21 */ \
	MV(CP(DPM_EMU11) , ( PTD | IEN | M3))  /* gpio_22 */ \
	MV(CP(DPM_EMU12) , ( PTU | IEN | M3))  /* gpio_23 */ \
	MV(CP(DPM_EMU13) , ( PTD | IEN | M3))  /* gpio_24 */ \
	MV(CP(DPM_EMU14) , ( M3))  /* gpio_25 */ \
	MV(CP(DPM_EMU15) , ( PTD | IEN | M3))  /* gpio_26 */ \
	MV(CP(DPM_EMU16) , ( PTD | IEN | M3))  /* gpio_27 */ \
	MV(CP(DPM_EMU17) , ( M7))  /* safe_mode */ \
	MV(CP(DPM_EMU18) , ( M1))  /* dmtimer10_pwm_evt */ \
	MV(CP(DPM_EMU19) , ( PTD | IEN | M3))  /* gpio_191 */ \
	MV1(WK(PAD0_SIM_IO) , ( PTU | IEN | M3))  /* gpio_home_b */ \
	MV1(WK(PAD1_SIM_CLK) , ( PTU | IEN | M3))  /* volume_up */ \
	MV1(WK(PAD0_SIM_RESET) , ( PTD | IEN | M3))  /* chrg_stat */ \
	MV1(WK(PAD1_SIM_CD) , ( PTD | IEN | M3))  /* wan_host_wake */ \
	MV1(WK(PAD0_SIM_PWRCTRL) , ( PTD | IEN | M3))  /* airq_1 */ \
	MV1(WK(PAD1_SR_SCL) , ( PTU | IEN | M0))  /* sr_scl */ \
	MV1(WK(PAD0_SR_SDA) , ( PTU | IEN | M0))  /* sr_sda */ \
	MV1(WK(PAD1_FREF_XTAL_IN) , ( M0))  /* # */ \
	MV1(WK(PAD0_FREF_SLICER_IN) , ( M0))  /* fref_slicer_in */ \
	MV1(WK(PAD1_FREF_CLK_IOREQ) , ( PTD | M0))  /* fref_clk_ioreq */ \
	MV1(WK(PAD0_FREF_CLK0_OUT) , ( M0))  /* fref_clk0_out */ \
	MV1(WK(PAD1_FREF_CLK3_REQ) , ( PTD | IEN | M2))  /* sys_drm_msecure */ \
	MV1(WK(PAD0_FREF_CLK3_OUT) , ( M0))  /* fref_clk3_out */ \
	MV1(WK(PAD1_FREF_CLK4_REQ)  , (IEN | M3))  /* GPIO_HOME_B */\
	MV1(WK(PAD0_FREF_CLK4_OUT) , ( M0))  /* # */ \
	MV1(WK(PAD1_SYS_32K) , ( IEN | M0))  /* sys_32k */ \
	MV1(WK(PAD0_SYS_NRESPWRON) , ( M0))  /* sys_nrespwron */ \
	MV1(WK(PAD1_SYS_NRESWARM) , ( M0))  /* sys_nreswarm */ \
	MV1(WK(PAD0_SYS_PWR_REQ) , ( PTU | M0))  /* sys_pwr_req */ \
	MV1(WK(PAD1_SYS_PWRON_RESET) , ( M0))  /* sys_pwron_reset_out */ \
	MV1(WK(PAD0_SYS_BOOT6) , ( IEN | M0))  /* sys_boot6 */ \
	MV1(WK(PAD1_SYS_BOOT7) , ( IEN | M0))  /* sys_boot7 */
/*	MV1(WK(PAD1_FREF_CLK3_REQ),     (M3)) */ /* gpio_wk30 */
/*	MV1(WK(PAD1_FREF_CLK4_REQ),     (M3)) */ /* gpio_wk7 */
/*	MV1(WK(PAD0_FREF_CLK4_OUT),     (M3)) */ /* gpio_wk8 */


/**********************************************************
 * Routine: set_muxconf_regs
 * Description: Setting up the configuration Mux registers
 *              specific to the hardware. Many pins need
 *              to be moved from protect to primary mode.
 *********************************************************/
void set_muxconf_regs(void)
{
	/* Move pinmux config from x-loader to u-boot */
	MUX_DEFAULT_OMAP4(); //Config Bowser 5 default setting.
}

/******************************************************************************
 * Routine: update_mux()
 * Description:Update balls which are different between boards.  All should be
 *             updated to match functionality.  However, I'm only updating ones
 *             which I'll be using for now.  When power comes into play they
 *             all need updating.
 *****************************************************************************/
void update_mux(u32 btype, u32 mtype)
{
	/* REVISIT  */
	return;

}


/*****************************************
 * Routine: get_bowser_board_revision 
 * Description: Uses 4 reserved board id pins as GPIO's
 *   to determine the bowser board id
 *****************************************/
u32 get_bowser_board_revision()
{
#if !defined(CONFIG_MACH_BOWSER_SUBTYPE_TATE) /* Bowser has board ID pins */
	u32 val;
	u32 rev = BOARD_REVISION_INVALID;
	u16 data11_backup, data12_backup, data13_backup, data14_backup;

	const u32 gpio4_base_address = 0x48059000;
	const u32 gpio_oe_offset = 0x0134;
	const u32 gpio_datain_offset = 0x0138;

	// backup pad configuration values
	data11_backup = __raw_readw(OMAP44XX_CTRL_BASE + CONTROL_PADCONF_C2C_DATA11);
	data12_backup = __raw_readw(OMAP44XX_CTRL_BASE + CONTROL_PADCONF_C2C_DATA12);
	data13_backup = __raw_readw(OMAP44XX_CTRL_BASE + CONTROL_PADCONF_C2C_DATA13);
	data14_backup = __raw_readw(OMAP44XX_CTRL_BASE + CONTROL_PADCONF_C2C_DATA14);

	// set pad configuration values for ID detection
	MV(CONTROL_PADCONF_C2C_DATA11, (IEN | PTU | M3));
	MV(CONTROL_PADCONF_C2C_DATA12, (IEN | PTU | M3));
	MV(CONTROL_PADCONF_C2C_DATA13, (IEN | PTU | M3));
	MV(CONTROL_PADCONF_C2C_DATA14, (IEN | PTU | M3));

	// configure board id pin's GPIO settings as inputs
	val = __raw_readl(gpio4_base_address + gpio_oe_offset);
	val |= 0x000000F0;
	__raw_writel(val, gpio4_base_address + gpio_oe_offset);

	// read values of board id pins
	val = __raw_readl(gpio4_base_address + gpio_datain_offset);
	val = ((val & 0x000000F0) >> 4);

	// translate board id pin values into board revisions
	switch (val)
	{
		case 0xF: rev = BOARD_REVISION_BOWSER_1; break;
		case 0x8: rev = BOARD_REVISION_BOWSER_2; break;
		default:
			  printf("ERROR: invalid board revision detected: 0x%x!\n", val);
			  rev = BOARD_REVISION_INVALID;  
			  break;
	}
	
	// restore original pad configuration values
	MV(CONTROL_PADCONF_C2C_DATA11, data11_backup);
	MV(CONTROL_PADCONF_C2C_DATA12, data12_backup);
	MV(CONTROL_PADCONF_C2C_DATA13, data13_backup);
	MV(CONTROL_PADCONF_C2C_DATA14, data14_backup);
#else /* Tate does not have board ID pins */
        u32 rev = BOARD_REVISION_INVALID;
#endif
	return rev;
}

/*****************************************
 * Routine: board_init
 * Description: Early hardware init.
 *****************************************/
int board_init(void)
{
	DECLARE_GLOBAL_DATA_PTR;

	gpmc_init();

#if 0 /* No eMMC env partition for now */
	/* Intializing env functional pointers with eMMC */
	boot_env_get_char_spec = mmc_env_get_char_spec;
	boot_env_init = mmc_env_init;
	boot_saveenv = mmc_saveenv;
	boot_env_relocate_spec = mmc_env_relocate_spec;
	env_ptr = (env_t *) (CFG_FLASH_BASE + CFG_ENV_OFFSET);
	env_name_spec = mmc_env_name_spec;
#endif

	/* board id for Linux */
	gd->bd->bi_arch_number = MACH_TYPE_OMAP4_BOWSER;
	gd->bd->bi_boot_params = (0x80000000 + 0x100); /* boot param addr */

	// get bowser board revision
	gd->bd->bi_board_revision = get_bowser_board_revision();

	return 0;
}

/****************************************************************************
 * check_fpga_revision number: the rev number should be a or b
 ***************************************************************************/
inline u16 check_fpga_rev(void)
{
	return __raw_readw(FPGA_REV_REGISTER);
}

/****************************************************************************
 * check_uieeprom_avail: Check FPGA Availability
 * OnBoard DEBUG FPGA registers need to be ready for us to proceed
 * Required to retrieve the bootmode also.
 ***************************************************************************/
int check_uieeprom_avail(void)
{
	volatile unsigned short *ui_brd_name =
	    (volatile unsigned short *)EEPROM_UI_BRD + 8;
	int count = 1000;

	/* Check if UI revision Name is already updated.
	 * if this is not done, we wait a bit to give a chance
	 * to update. This is nice to do as the Main board FPGA
	 * gets a chance to know off all it's components and we can continue
	 * to work normally
	 * Currently taking 269* udelay(1000) to update this on poweron
	 * from flash!
	 */
	while ((*ui_brd_name == 0x00) && count) {
		udelay(200);
		count--;
	}
	/* Timed out count will be 0? */
	return count;
}

/***********************************************************************
 * get_board_type() - get board revision if it wasn't already set in the
 * global data board information structure
 ************************************************************************/
u32 get_board_type(void)
{
	DECLARE_GLOBAL_DATA_PTR;

	if (gd->bd->bi_board_revision == BOARD_REVISION_INVALID)
	{
		gd->bd->bi_board_revision = get_bowser_board_revision();
	}

	return gd->bd->bi_board_revision;
}
