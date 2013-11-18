/*
 *  (C) Copyright 2010,2011
 *  NVIDIA Corporation <www.nvidia.com>
 *
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef _SDMMC_H_
#define _SDMMC_H_

/* SD/MMC Controller regs */
struct sdmmc_ctlr {
	uint reserved0[9];	/* 0x0 ~ 0x20 */
	uint present_state;	/* 0x24 */
	uint reserved2[2];	/* 0x28, 0x2c */
	uint int_status;		/* 0x30 */
	uint int_status_enb;	/* 0x34 */
	uint int_sig_enb;		/* 0x38 */
	uint reserved3[5];	/* 0x3c ~ 0x4c */
	uint force_event;		/* 0x50 */
	uint reserved5[43];	/* 0x54 ~ 0xfc */
	uint vendor_clk_cntrl;	/* _VENDOR_CLOCK_CNTRL_0,	0x100 */
	uint vendor_spi_cntrl;	/* _VENDOR_SPI_CNTRL_0,		0x104 */
	uint vendor_spi_intr_status;/* _VENDOR_SPI_INTR_STATUS_0,	0x108 */
	uint reserved10;		/* 0x10c */
	uint vendor_boot_cntrl;	/* _VENDOR_BOOT_CNTRL_0,	0x110 */
	uint vendor_boot_ack_timeout;/* _VENDOR_BOOT_ACK_TIMEOUT,	0x114 */
	uint vendor_boot_dat_timeout;/* _VENDOR_BOOT_DAT_TIMEOUT,	0x118 */
	uint vendor_debounce_cnt;	/* _VENDOR_DEBOUNCE_COUNT_0,	0x11c */
	uint vendor_misc_cntrl;	/* _VENDOR_MISC_CNTRL_0,	0x120 */
	uint reserved11[47];	/* 0x124 ~ 0x1dc */
	uint sdmemcomp_pad_ctrl;	/* _SDMEMCOMPPADCTRL_0,		0x1e0 */
	uint auto_cal_config;	/* _AUTO_CAL_CONFIG_0,		0x1e4 */
	uint auto_cal_interval;	/* _AUTO_CAL_INTERVAL_0,	0x1e8 */
	uint auto_cal_status;	/* _AUTO_CAL_STATUS_0,		0x1ec */
};

#endif
