/*
 * Copyright (c) 2011 The Chromium OS Authors.
 * (C) Copyright 2010,2011 NVIDIA Corporation <www.nvidia.com>
 * (C) Copyright 2012 Toradex, Inc.
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <asm/io.h>
#include <asm/arch-tegra/bitfield.h>
#include <asm/arch/tegra.h>
#include <asm/arch/sys_proto.h>

#include <asm/arch-tegra/clk_rst.h>
#include <asm/arch/clock.h>
#include <asm/arch/pinmux.h>
#include <asm/arch-tegra/uart.h>
#include <asm/arch/gpio.h>
#include <asm/arch/usb.h>
#include <fdt_decode.h>

#define CLK_RST_CONTROLLER_CLK_OUT_ENB_U_0	0x18
#define CLK_RST_CONTROLLER_PLLP_OUTB_0		0xA8

/* ASIX AX88772B Ethernet LAN GPIOs */
#if !defined(CONFIG_TEGRA3)
#define LAN_V_BUS GPIO_PBB1
#define LAN_RESET GPIO_PV4
#define LAN_V_BUS_PINGRP PINGRP_DTE
#define LAN_RESET_PINGRP PINGRP_GPV
#else
#define LAN_V_BUS GPIO_PDD2
#define LAN_RESET GPIO_PDD0
#define LAN_V_BUS_PINGRP PINGRP_PEX_L0_CLKREQ_N
#define LAN_RESET_PINGRP PINGRP_PEX_L0_PRSNT_N
#endif

enum {
	USB_PORTS_MAX	= 4,			/* Maximum ports we allow */
};

struct usb_port {
	struct usb_ctlr *reg;
};

static struct usb_port port[USB_PORTS_MAX];	/* List of valid USB ports */
static unsigned port_count;			/* Number of available ports */

/* Record which controller can switch from host to device mode */
static struct usb_ctlr *host_dev_ctlr;

/*
 * This table has USB timing parameters for each Oscillator frequency we
 * support. There are four sets of values:
 *
 * 1. PLLU configuration information (reference clock is osc/clk_m and
 * PLLU-FOs are fixed at 12MHz/60MHz/480MHz).
 * (T2x)
 *  Reference frequency     13.0MHz      19.2MHz      12.0MHz      26.0MHz
 *  ----------------------------------------------------------------------
 *      DIVN                960 (0x3c0)  200 (0c8)    960 (3c0h)   960 (3c0)
 *      DIVM                13 (0d)      4 (04)       12 (0c)      26 (1a)
 * Filter frequency (MHz)   1            4.8          6            2
 * CPCON                    1100b        0011b        1100b        1100b
 * LFCON0                   0            0            0            0
 *
 * (T3x)
 * Reference frequency MHZ 12.0  13.0  16.8  19.2  26.0  38.4  48.0
 * ----------------------------------------------------------------------
 *      DIVN              960   960   400   200   960   200   960
 *      DIVM               12    13     7     4    26     4    12
 *
 * 2. PLL CONFIGURATION & PARAMETERS for different clock generators:
 * (T2x)
 * Reference frequency     13.0MHz         19.2MHz         12.0MHz     26.0MHz
 * ---------------------------------------------------------------------------
 * Index                    0               1               2           3
 * PLLU_ENABLE_DLY_COUNT   02 (0x02)       03 (03)         02 (02)     04 (04)
 * PLLU_STABLE_COUNT       51 (33)         75 (4B)         47 (2F)    102 (66)
 * PLL_ACTIVE_DLY_COUNT    05 (05)         06 (06)         04 (04)     09 (09)
 * XTAL_FREQ_COUNT        127 (7F)        187 (BB)        118 (76)    254 (FE)
 *
 * (T3x)
 * Reference frequency MHZ 12.0  13.0  16.8  19.2  26.0  38.4  48.0
 * ---------------------------------------------------------------------------
 * Index                    8     0     1     4    12     5     9
 * PLLU_ENABLE_DLY_COUNT   02     2     3     3     4     5     6
 * PLLU_STABLE_COUNT       47    51    66    75   102   150   188
 * PLL_ACTIVE_DLY_COUNT    08     9    11    12    17    24    31
 * XTAL_FREQ_COUNT        118   127   165   188   254   375   469
 *
 * 3. Debounce values IdDig, Avalid, Bvalid, VbusValid, VbusWakeUp, and
 * SessEnd. Each of these signals have their own debouncer and for each of
 * those one out of two debouncing times can be chosen (BIAS_DEBOUNCE_A or
 * BIAS_DEBOUNCE_B).
 *
 * The values of DEBOUNCE_A and DEBOUNCE_B are calculated as follows:
 *    0xffff -> No debouncing at all
 *    <n> ms = <n> *1000 / (1/19.2MHz) / 4
 *
 * So to program a 1 ms debounce for BIAS_DEBOUNCE_A, we have:
 * BIAS_DEBOUNCE_A[15:0] = 1000 * 19.2 / 4  = 4800 = 0x12c0
 *
 * We need to use only DebounceA for BOOTROM. We dont need the DebounceB
 * values, so we can keep those to default.
 *
 * 4. The 20 microsecond delay after bias cell operation.
 */
#if !defined(CONFIG_TEGRA3)
static const int usb_pll[CLOCK_OSC_FREQ_COUNT][PARAM_COUNT] = {
	/* DivN, DivM, DivP, CPCON, LFCON,EN_DLY,STB,ACT, XTAL,Debounce,Bias */
	{ 0x3C0, 0x0D, 0x00, 0xC,   0,  0x02, 0x33, 0x05, 0x7F, 0x7EF4, 5 },
	{ 0x0C8, 0x04, 0x00, 0x3,   0,  0x03, 0x4B, 0x06, 0xBB, 0xBB80, 7 },
	{ 0x3C0, 0x0C, 0x00, 0xC,   0,  0x02, 0x2F, 0x04, 0x76, 0x7530, 5 },
	{ 0x3C0, 0x1A, 0x00, 0xC,   0,  0x04, 0x66, 0x09, 0xFE, 0xFDE8, 9 }
};
#endif
/* UTMIP Idle Wait Delay */
static const u8 utmip_idle_wait_delay = 17;

/* UTMIP Elastic limit */
static const u8 utmip_elastic_limit = 16;

/* UTMIP High Speed Sync Start Delay */
static const u8 utmip_hs_sync_start_delay = 9;

extern void ulpi_phy_power_on(void);

/* Put the port into host mode (this only works for USB1) */
static void set_host_mode(struct usb_ctlr *usbctlr)
{
//check USB3
	/* Check whether remote host from USB1 is driving VBus */
	if (bf_readl(VBUS_VLD_STS, &usbctlr->phy_vbus_sensors))
		return;

#if !defined(CONFIG_TEGRA3)
//only required for USB3 at least on Iris
	/*
	 * If not driving, we set GPIO USB1_VBus_En. Colibri T20 uses
	 * PAD SPIG (GPIO W.02) as USB1_VBus_En Config as GPIO
	 */
	gpio_direction_output(GPIO_PW2, 0);

	/* Z_SPIG = 0, normal, not tristate */
	pinmux_tristate_disable(PINGRP_SPIG);
#else
	/* T30 pinmuxes are set globally; GPIOs from fdt */
#endif
}

/* Put our ports into host mode */
void usb_set_host_mode(void)
{
	if (host_dev_ctlr)
		set_host_mode(host_dev_ctlr);
}

void usbf_reset_controller(enum periph_id id, struct usb_ctlr *usbctlr)
{
#if !defined(CONFIG_TEGRA3)
	uint reg;

	/* Configure ULPI pin mux */
//ToDo: find better place to do this
	pinmux_set_func(PINGRP_UAA, PMUX_FUNC_ULPI);
	pinmux_set_func(PINGRP_UAB, PMUX_FUNC_ULPI);
	pinmux_set_func(PINGRP_UDA, PMUX_FUNC_ULPI);
	pinmux_tristate_disable(PINGRP_UAA);
	pinmux_tristate_disable(PINGRP_UAB);
	pinmux_tristate_disable(PINGRP_UDA);
#endif

	/* Reset the USB controller with 2us delay */
	reset_periph(id, 2);

#if !defined(CONFIG_TEGRA3)
	if (id == PERIPH_ID_USB2) {

		/* Reset ULPI PHY */
		gpio_direction_output(GPIO_PV1, 0);
		pinmux_tristate_disable(PINGRP_UAC);
		udelay(5000);
		gpio_set_value(GPIO_PV1, 1);

		/* Configure CDEV2 as PLLP_OUT4 */
		pinmux_set_func(PINGRP_CDEV2, PMUX_FUNC_PLLP_OUT4);
		pinmux_tristate_disable(PINGRP_CDEV2);

		/* Configure 24 MHz clock for ULPI PHY */
//enable
		reg = readl(NV_PA_CLK_RST_BASE + CLK_RST_CONTROLLER_CLK_OUT_ENB_U_0);
		reg |= (1 << 29);
//		writel(NV_PA_CLK_RST_BASE+CLK_RST_CONTROLLER_CLK_OUT_ENB_U_0, reg);
//above not working!
*((uint *) (NV_PA_CLK_RST_BASE + CLK_RST_CONTROLLER_CLK_OUT_ENB_U_0)) = reg;
//rate
		reg = readl(NV_PA_CLK_RST_BASE + CLK_RST_CONTROLLER_PLLP_OUTB_0);
		reg |= (8 << 25) | (0 << 24) | (1 << 18);
//		writel(NV_PA_CLK_RST_BASE+CLK_RST_CONTROLLER_PLLP_OUTB_0, reg);
*((uint *) (NV_PA_CLK_RST_BASE + CLK_RST_CONTROLLER_PLLP_OUTB_0)) = reg;

		ulpi_phy_power_on();
	}

	/*
	 * Set USB1_NO_LEGACY_MODE to 1, Registers are accessible under
	 * base address
	 */
//what about tegra 3?
	if (id == PERIPH_ID_USBD)
		bf_writel(USB1_NO_LEGACY_MODE, NO_LEGACY_MODE,
			  &usbctlr->usb1_legacy_ctrl);

	/* Put UTMIP1/3 in reset */
	if ((id == PERIPH_ID_USBD) || (id == PERIPH_ID_USB3))
		bf_writel(UTMIP_RESET, 1, &usbctlr->susp_ctrl);

	/* Set USB3 to use UTMIP PHY */
	if (id == PERIPH_ID_USB3)
		bf_writel(UTMIP_PHY_ENB, 1, &usbctlr->susp_ctrl);
#else /* !CONFIG_TEGRA3 */
	/* Put UTMIP1/2/3 in reset */
	bf_writel(UTMIP_RESET, 1, &usbctlr->susp_ctrl);

	/* Set USB1/2/3 to use UTMIP PHY */
	bf_writel(UTMIP_PHY_ENB, 1, &usbctlr->susp_ctrl);
#endif

	/*
	 * TODO: where do we take the USB1 out of reset? The old code would
	 * take USB3 out of reset, but not USB1. This code doesn't do either.
	 */

	if (id == PERIPH_ID_USB2) {
		/* Fix Ethernet detection faults */
		udelay(100 * 1000);

		/* Enable ASIX AX88772B V_BUS */
		gpio_direction_output(LAN_V_BUS, 1);
		pinmux_tristate_disable(LAN_V_BUS_PINGRP);

		/* Reset */
		gpio_direction_output(LAN_RESET, 0);
		pinmux_tristate_disable(LAN_RESET_PINGRP);

		udelay(5);

		/* Unreset */
		gpio_set_value(LAN_RESET, 1);
	}
}

/* set up the USB controller with the parameters provided */
static void init_usb_controller(enum periph_id id, struct usb_ctlr *usbctlr,
				const int *params)
{
	u32 val;
	int loop_count;
#if defined(CONFIG_TEGRA3)
	struct clk_rst_ctlr *clkrst = (struct clk_rst_ctlr *)NV_PA_CLK_RST_BASE;
#endif
	clock_enable(id);

	/* Reset the usb controller */
	usbf_reset_controller(id, usbctlr);

	/* Stop crystal clock by setting UTMIP_PHY_XTAL_CLOCKEN low */
	bf_clearl(UTMIP_PHY_XTAL_CLOCKEN, &usbctlr->utmip_misc_cfg1);

	/* Follow the crystal clock disable by >100ns delay */
	udelay(1);
#if !defined(CONFIG_TEGRA3)
	/*
	 * To Use the A Session Valid for cable detection logic, VBUS_WAKEUP
	 * mux must be switched to actually use a_sess_vld threshold.
	 */
	if (id == PERIPH_ID_USBD)
		bf_enum_writel(VBUS_SENSE_CTL, A_SESS_VLD,
			       &usbctlr->usb1_legacy_ctrl);

	/*
	 * PLL Delay CONFIGURATION settings. The following parameters control
	 * the bring up of the plls.
	 */
	val = readl(&usbctlr->utmip_misc_cfg1);
	bf_update(UTMIP_PLLU_STABLE_COUNT, val, params[PARAM_STABLE_COUNT]);
	bf_update(UTMIP_PLL_ACTIVE_DLY_COUNT, val,
		  params[PARAM_ACTIVE_DELAY_COUNT]);
	writel(val, &usbctlr->utmip_misc_cfg1);

	/* Set PLL enable delay count and crystal frequency count */
	val = readl(&usbctlr->utmip_pll_cfg1);
	bf_update(UTMIP_PLLU_ENABLE_DLY_COUNT, val,
		  params[PARAM_ENABLE_DELAY_COUNT]);
	bf_update(UTMIP_XTAL_FREQ_COUNT, val, params[PARAM_XTAL_FREQ_COUNT]);
	writel(val, &usbctlr->utmip_pll_cfg1);
#else /* !CONFIG_TEGRA3 */
	/*
	 * PLL Delay CONFIGURATION settings. The following parameters control
	 * the bring up of the plls.
	 */
	val = readl(&clkrst->crc_pll_cfg2);
	bf_update(UTMIP_PLLU_STABLE_COUNT, val, params[PARAM_STABLE_COUNT]);
	bf_update(UTMIP_PLL_ACTIVE_DLY_COUNT, val,
		  params[PARAM_ACTIVE_DELAY_COUNT]);
	writel(val, &clkrst->crc_pll_cfg2);

	/* Set PLL enable delay count and crystal frequency count */
	val = readl(&clkrst->crc_pll_cfg1);
	bf_update(UTMIP_PLLU_ENABLE_DLY_COUNT, val,
		  params[PARAM_ENABLE_DELAY_COUNT]);
	bf_update(UTMIP_XTAL_FREQ_COUNT, val, params[PARAM_XTAL_FREQ_COUNT]);
	writel(val, &clkrst->crc_pll_cfg1);

	/* Disable Power Down state for PLL */
	bf_writel(UTMIP_FORCE_PLLU_POWERDOWN, 0, &clkrst->crc_pll_cfg1);
	bf_writel(UTMIP_FORCE_PLL_ENABLE_POWERDOWN, 0, &clkrst->crc_pll_cfg1);
	bf_writel(UTMIP_FORCE_PLL_ACTIVE_POWERDOWN, 0, &clkrst->crc_pll_cfg1);

	/* Recommended PHY settings for EYE diagram */
	bf_writel(UTMIP_XCVR_SETUP, 0x4, &usbctlr->utmip_xcvr_cfg0);
	bf_writel(UTMIP_XCVR_SETUP_MSB, 0x3, &usbctlr->utmip_xcvr_cfg0);
	bf_writel(UTMIP_XCVR_HSSLEW_MSB, 0x8, &usbctlr->utmip_xcvr_cfg0);
	bf_writel(UTMIP_XCVR_TERM_RANGE_ADJ, 0x7, &usbctlr->utmip_xcvr_cfg1);
	bf_writel(UTMIP_HSDISCON_LEVEL_MSB, 0x1, &usbctlr->utmip_bias_cfg0);
	bf_writel(UTMIP_HSDISCON_LEVEL, 0x1, &usbctlr->utmip_bias_cfg0);
	bf_writel(UTMIP_HSSQUELCH_LEVEL, 0x2, &usbctlr->utmip_bias_cfg0);

	/* Miscellaneous setting mentioned in Programming Guide */
	bf_writel(UTMIP_SUSPEND_EXIT_ON_EDGE, 0, &usbctlr->utmip_misc_cfg0);
#endif /* !CONFIG_TEGRA3 */
	/* Setting the tracking length time */
	bf_writel(UTMIP_BIAS_PDTRK_COUNT, params[PARAM_BIAS_TIME],
		  &usbctlr->utmip_bias_cfg1);

	/* Program debounce time for VBUS to become valid */
	bf_writel(UTMIP_DEBOUNCE_CFG0, params[PARAM_DEBOUNCE_A_TIME],
		  &usbctlr->utmip_debounce_cfg0);

	/* Set UTMIP_FS_PREAMBLE_J to 1 */
	bf_writel(UTMIP_FS_PREAMBLE_J, 1, &usbctlr->utmip_tx_cfg0);

	/* Disable battery charge enabling bit */
	bf_writel(UTMIP_PD_CHRG, 1, &usbctlr->utmip_bat_chrg_cfg0);

	/* Set UTMIP_XCVR_LSBIAS_SEL to 0 */
	bf_writel(UTMIP_XCVR_LSBIAS_SE, 0, &usbctlr->utmip_xcvr_cfg0);

	/* Set bit 3 of UTMIP_SPARE_CFG0 to 1 */
	bf_writel(FUSE_SETUP_SEL, 1, &usbctlr->utmip_spare_cfg0);

	/*
	 * Configure the UTMIP_IDLE_WAIT and UTMIP_ELASTIC_LIMIT
	 * Setting these fields, together with default values of the
	 * other fields, results in programming the registers below as
	 * follows:
	 *         UTMIP_HSRX_CFG0 = 0x9168c000
	 *         UTMIP_HSRX_CFG1 = 0x13
	 */

	/* Set PLL enable delay count and Crystal frequency count */
	val = readl(&usbctlr->utmip_hsrx_cfg0);
	bf_update(UTMIP_IDLE_WAIT, val, utmip_idle_wait_delay);
	bf_update(UTMIP_ELASTIC_LIMIT, val, utmip_elastic_limit);
	writel(val, &usbctlr->utmip_hsrx_cfg0);

	/* Configure the UTMIP_HS_SYNC_START_DLY */
	bf_writel(UTMIP_HS_SYNC_START_DLY, utmip_hs_sync_start_delay,
		  &usbctlr->utmip_hsrx_cfg1);

	/* Precede the crystal clock enable by >100ns delay. */
	udelay(1);

	/* Resuscitate crystal clock by setting UTMIP_PHY_XTAL_CLOCKEN */
	bf_writel(UTMIP_PHY_XTAL_CLOCKEN, 1, &usbctlr->utmip_misc_cfg1);
#if defined(CONFIG_TEGRA3)
	if (id == PERIPH_ID_USBD)
		bf_writel(UTMIP_FORCE_PD_SAMP_A_POWERDOWN, 0,
			  &clkrst->crc_pll_cfg2);
	if (id == PERIPH_ID_USB2)
		bf_writel(UTMIP_FORCE_PD_SAMP_B_POWERDOWN, 0,
			  &clkrst->crc_pll_cfg2);
	if (id == PERIPH_ID_USB3)
		bf_writel(UTMIP_FORCE_PD_SAMP_C_POWERDOWN, 0,
			  &clkrst->crc_pll_cfg2);
#endif
	/* Finished the per-controller init. */

	/* De-assert UTMIP_RESET to bring out of reset. */
	bf_clearl(UTMIP_RESET, &usbctlr->susp_ctrl);

	/* Wait for the phy clock to become valid in 100 ms */
	for (loop_count = 100000; loop_count != 0; loop_count--) {
		if (bf_readl(USB_PHY_CLK_VALID, &usbctlr->susp_ctrl))
			break;
		udelay(1);
	}
}

static void power_up_port(struct usb_ctlr *usbctlr)
{
	u32 val;

	/* Deassert power down state */
	val = readl(&usbctlr->utmip_xcvr_cfg0);
	bf_update(UTMIP_FORCE_PD_POWERDOWN, val, 0);
	bf_update(UTMIP_FORCE_PD2_POWERDOWN, val, 0);
	bf_update(UTMIP_FORCE_PDZI_POWERDOWN, val, 0);
	writel(val, &usbctlr->utmip_xcvr_cfg0);

	val = readl(&usbctlr->utmip_xcvr_cfg1);
	bf_update(UTMIP_FORCE_PDDISC_POWERDOWN, val, 0);
	bf_update(UTMIP_FORCE_PDCHRP_POWERDOWN, val, 0);
	bf_update(UTMIP_FORCE_PDDR_POWERDOWN, val, 0);
	writel(val, &usbctlr->utmip_xcvr_cfg1);
}

static void config_clock(const int params[])
{
	clock_start_pll(CLOCK_ID_USB,
			params[PARAM_DIVM], params[PARAM_DIVN],
			params[PARAM_DIVP], params[PARAM_CPCON],
			params[PARAM_LFCON]);
}

/**
 * Add a new USB port to the list of available ports
 *
 * @param id		peripheral id of port (PERIPH_ID_USB3, for example)
 * @param usbctlr	register address of controller
 * @param params	timing parameters
 * @param utmi		1 if using internal UTMI transceiver
 * @return 0 if ok, -1 if error (too many ports)
 */
static int add_port(enum periph_id id, struct usb_ctlr *usbctlr,
		    const int params[], int utmi)
{
	volatile u32 *ahb_prefetch_reg;

	if (port_count == USB_PORTS_MAX) {
		debug("tegrausb: Cannot register more than %d ports\n",
		      USB_PORTS_MAX);
		return -1;
	}
	init_usb_controller(id, usbctlr, params);
#if defined(CONFIG_TEGRA3)
	/*
	 * BIAS Pad Power Down is common among all 3 USB
	 * controllers and can be controlled from USB1 only.
	 */
	if (id == PERIPH_ID_USBD)
		bf_writel(UTMIP_BIASPD, 0, &usbctlr->utmip_bias_cfg0);
#endif
	if (utmi) {
		/* Disable ICUSB FS/LS transceiver */
//should actually default to disabled on Tegra 3 according to TRM
		bf_writel(IC_ENB1, 0, &usbctlr->icusb_ctrl);

#if !defined(CONFIG_TEGRA3)
		/* Select UTMI parallel interface */
		bf_writel(PTS, PTS_UTMI, &usbctlr->port_sc1);
		bf_writel(STS, 0, &usbctlr->port_sc1);
#endif
//probably just following required
		power_up_port(usbctlr);
	}
	port[port_count++].reg = usbctlr;

	/* Setting AHB-Prefetch register to avoid TX FIFO underrun
	   Colibri T20 has Ethernet on USB2 */
	if (id == PERIPH_ID_USB2) {
//		printf("setting AHB-prefetch registers for USB2\n");
		ahb_prefetch_reg = (u32 *)0x6000c0f4;
		/* AHB_AHB_MEM_PREFETCH_CFG2_0
		   0b1: ENABLE
		   0b10010: 18 = USB2
		   0b01100: ADDR_BNDRY 2^(12+4) = 65536
		   0x800: INACTIVITY_TIMEOUT */
		*ahb_prefetch_reg = 0xc9800800;
	}

	return 0;
}

int tegrausb_start_port(unsigned portnum, struct ehci_hccr **hccr,
			struct ehci_hcor **hcor)
{
	struct usb_ctlr *usbctlr;

	if (portnum >= port_count)
		return -1;
	tegrausb_stop_port(portnum);

	usbctlr = port[portnum].reg;
#if defined(CONFIG_TEGRA3)
	/* Set Controller Mode as Host mode after Controller Reset was done */
	bf_writel(CM, CM_HOST_MODE, &usbctlr->usb_mode);

	/* Select UTMI parallel interface after setting host mode */
	bf_writel(PTS, PTS_UTMI, &usbctlr->hostpc1_devlc);
	bf_writel(STS, STS_PARALLEL_IF, &usbctlr->hostpc1_devlc);
#endif

	*hccr = (struct ehci_hccr *)&usbctlr->cap_length;
	*hcor = (struct ehci_hcor *)&usbctlr->usb_cmd;
	return 0;
}

int tegrausb_stop_port(unsigned portnum)
{
	struct usb_ctlr *usbctlr;

	if (portnum >= port_count)
		return -1;

	usbctlr = port[portnum].reg;

	/* Stop controller */
	writel(0, &usbctlr->usb_cmd);
	udelay(1000);

	/* Initiate controller reset */
	writel(2, &usbctlr->usb_cmd);
	udelay(1000);
	return 0;
}

int board_usb_init(const void *blob)
{
	struct fdt_usb config;
	int clk_done = 0;
	int node, upto = 0;
	unsigned osc_freq = clock_get_rate(CLOCK_ID_OSC);

	do {
		node = fdt_decode_next_alias(blob, "usb",
					     COMPAT_NVIDIA_TEGRA250_USB, &upto);
		if (node < 0)
			break;
		if (fdt_decode_usb(blob, node, osc_freq, &config))
			return -1;
		if (!config.enabled)
			continue;

		/* The first port we find gets to set the clocks */
		if (!clk_done) {
			config_clock(config.params);
			clk_done = 1;
		}
		if (config.host_mode) {
			/* Only one host-dev port is supported */
			if (host_dev_ctlr)
				return -1;
			host_dev_ctlr = config.reg;
		}
		if (add_port(config.periph_id, config.reg, config.params,
			     config.utmi))
			return -1;

		fdt_setup_gpio(&config.vbus_gpio);
		fdt_setup_gpio(&config.vbus_pullup_gpio);
	} while (node);

	usb_set_host_mode();

	return 0;
}
