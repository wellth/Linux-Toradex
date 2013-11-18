/*
 * Copyright (c) 2011 The Chromium OS Authors.
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef _TEGRA_USB_H_
#define _TEGRA_USB_H_


/* USB Controller (USBx_CONTROLLER_) regs */
struct usb_ctlr {
	/* 0x000 */
	uint id;
	uint reserved0;
	uint host;
	uint device;

	/* 0x010 */
	uint txbuf;
	uint rxbuf;
	uint reserved1[2];

	/* 0x020 */
	uint reserved2[56];

	/* 0x100 */
	u16 cap_length;
	u16 hci_version;
	uint hcs_params;
	uint hcc_params;
	uint reserved3[5];

	/* 0x120 */
	uint dci_version;
	uint dcc_params;
	uint reserved4[2];

	/* 0x130 */
	uint usb_cmd;
	uint usb_sts;
	uint usb_intr;
	uint frindex;

	/* 0x140 */
	uint reserved5;
	uint periodic_list_base;
	uint async_list_addr;
	uint reserved5_1;

	/* 0x150 */
	uint burst_size;
	uint tx_fill_tuning;
	uint reserved6;   /* is this port_sc1 on some controllers? */
	uint icusb_ctrl;

	/* 0x160 */
	uint ulpi_viewport;
	uint reserved7;
	uint endpt_nak;
	uint endpt_nak_enable;

	/* 0x170 */
	uint reserved;
	uint port_sc1;
	uint reserved8[6];

	/* 0x190 */
	uint reserved9[8];

	/* 0x1b0 */
	uint reserved10;
	uint hostpc1_devlc;
	uint reserved10_1[2];

	/* 0x1c0 */
	uint reserved10_2[4];

	/* 0x1d0 */
	uint reserved10_3[4];

	/* 0x1e0 */
	uint reserved10_4[4];

	/* 0x1f0 */
	uint reserved10_5;
	uint otgsc;
	uint usb_mode;
	uint reserved10_6;

	/* 0x200 */
	uint reserved11[2];
	uint endpt_setup_stat;
	uint reserved11_1[0x7D];

	/* 0x400 */
	uint susp_ctrl;
	uint phy_vbus_sensors;
	uint phy_vbus_wakeup_id;
	uint phy_alt_vbus_sys;

	/* 0x410 */
	uint reserved12[4];

	/* 0x420 */
	uint reserved13[56];

	/* 0x500 */
	uint reserved14[64 * 3];

	/* 0x800 */
	uint reserved15[2];
	uint utmip_xcvr_cfg0;
	uint utmip_bias_cfg0;

	/* 0x810 */
	uint utmip_hsrx_cfg0;
	uint utmip_hsrx_cfg1;
	uint utmip_fslsrx_cfg0;
	uint utmip_fslsrx_cfg1;

	/* 0x820 */
	uint utmip_tx_cfg0;
	uint utmip_misc_cfg0;
	uint utmip_misc_cfg1;
	uint utmip_debounce_cfg0;

	/* 0x830 */
	uint utmip_bat_chrg_cfg0;
	uint utmip_spare_cfg0;
	uint utmip_xcvr_cfg1;
	uint utmip_bias_cfg1;
};


/* USB1_LEGACY_CTRL */
#define USB1_NO_LEGACY_MODE_RANGE		0:0
#define NO_LEGACY_MODE				1

#define VBUS_SENSE_CTL_RANGE			2:1
#define VBUS_SENSE_CTL_VBUS_WAKEUP		0
#define VBUS_SENSE_CTL_AB_SESS_VLD_OR_VBUS_WAKEUP	1
#define VBUS_SENSE_CTL_AB_SESS_VLD		2
#define VBUS_SENSE_CTL_A_SESS_VLD		3

/* USB2D_HOSTPC1_DEVLC_0 */
#define PTS_RANGE				31:29
#define PTS_UTMI	0
#define PTS_RESERVED	1
#define PTS_ULP		2
#define PTS_ICUSB_SER	3
#define PTS_HSIC	4

#define STS_RANGE				28:28
#define STS_PARALLEL_IF	0
#define STS_SERIAL_IF	1

/* USB2D_USBMODE_0 */
#define CM_RANGE				1:0
#define CM_DEVICE_MODE				2
#define CM_HOST_MODE				3

/* USBx_IF_USB_SUSP_CTRL_0 */
#define UTMIP_PHY_ENB_RANGE			12:12
#define UTMIP_RESET_RANGE			11:11
#define USB_PHY_CLK_VALID_RANGE			7:7

/* USBx_UTMIP_MISC_CFG0 */
#define UTMIP_SUSPEND_EXIT_ON_EDGE_RANGE	22:22

/* USBx_UTMIP_MISC_CFG1 */
#define UTMIP_PHY_XTAL_CLOCKEN_RANGE		30:30

/* USBx_UTMIP_BIAS_CFG0_0 */
#define UTMIP_HSDISCON_LEVEL_MSB_RANGE		24:24
#define UTMIP_OTGPD_RANGE			11:11
#define UTMIP_BIASPD_RANGE			10:10
#define UTMIP_HSDISCON_LEVEL_RANGE		3:2
#define UTMIP_HSSQUELCH_LEVEL_RANGE		1:0

/* USBx_UTMIP_BIAS_CFG1_0 */
#define UTMIP_BIAS_PDTRK_COUNT_RANGE		7:3
#define UTMIP_FORCE_PDTRK_POWERUP_RANGE		1:1
#define UTMIP_FORCE_PDTRK_POWERDOWN_RANGE	0:0

/* USBx_UTMIP_DEBOUNCE_CFG0_0 */
#define UTMIP_DEBOUNCE_CFG0_RANGE		15:0

/* USBx_UTMIP_TX_CFG0_0 */
#define UTMIP_FS_PREAMBLE_J_RANGE		19:19

/* USBx_UTMIP_BAT_CHRG_CFG0_0 */
#define UTMIP_PD_CHRG_RANGE			0:0

/* USBx_UTMIP_SPARE_CFG0_0 */
#define FUSE_SETUP_SEL_RANGE			3:3

/* USBx_UTMIP_HSRX_CFG0_0 */
#define UTMIP_IDLE_WAIT_RANGE			19:15
#define UTMIP_ELASTIC_LIMIT_RANGE		14:10

/* USBx_UTMIP_HSRX_CFG0_1 */
#define UTMIP_HS_SYNC_START_DLY_RANGE		4:1

/* USBx_CONTROLLER_2_USB2D_ICUSB_CTRL_0 */
#define IC_ENB1_RANGE				3:3

/* USBx_UTMIP_XCVR_CFG0_0 */
#define UTMIP_XCVR_HSSLEW_MSB_RANGE		31:25
#define UTMIP_XCVR_SETUP_MSB_RANGE		24:22
#define UTMIP_XCVR_LSBIAS_SE_RANGE		21:21
#define UTMIP_FORCE_PD_POWERDOWN_RANGE		14:14
#define UTMIP_FORCE_PD2_POWERDOWN_RANGE		16:16
#define UTMIP_FORCE_PDZI_POWERDOWN_RANGE	18:18
#define UTMIP_XCVR_SETUP_RANGE			3:0

/* USBx_UTMIP_XCVR_CFG1_0 */
#define UTMIP_XCVR_TERM_RANGE_ADJ_RANGE		21:18
#define UTMIP_FORCE_PDDR_POWERDOWN_RANGE	4:4
#define UTMIP_FORCE_PDCHRP_POWERDOWN_RANGE	2:2
#define UTMIP_FORCE_PDDISC_POWERDOWN_RANGE	0:0

/* USB3_IF_USB_PHY_VBUS_SENSORS_0 */
#define VBUS_VLD_STS_RANGE			26:26

struct ehci_hccr;
struct ehci_hcor;

/* Change the USB host port into host mode */
void usb_set_host_mode(void);

/* Setup USB on the board */
int board_usb_init(const void *blob);

/**
 * Start up the given port number (ports are numbered from 0 on each board).
 * This returns values for the appropriate hccr and hcor addresses to use for
 * USB EHCI operations.
 *
 * @param portnum	port number to start
 * @param hccr		returns start address of EHCI HCCR registers
 * @param hcor		returns start address of EHCI HCOR registers
 * @return 0 if ok, -1 on error (generally invalid port number)
 */
int tegrausb_start_port(unsigned portnum, struct ehci_hccr **hccr,
	struct ehci_hcor **hcor);

/**
 * Stop the selected port
 *
 * @param portnum	port number to stop
 * @return 0 if ok, -1 if no port was active
 */
int tegrausb_stop_port(unsigned portnum);

#endif	/* _TEGRA_USB_H_ */
