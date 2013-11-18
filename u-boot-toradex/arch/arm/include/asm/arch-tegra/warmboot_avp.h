/*
 * (C) Copyright 2010-2012
 * NVIDIA Corporation <www.nvidia.com>
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

#ifndef _WARMBOOT_AVP_H_
#define _WARMBOOT_AVP_H_

#define TEGRA_DEV_L			0
#define TEGRA_DEV_H			1
#define TEGRA_DEV_U			2
#define TEGRA_DEV_V			0

#define SIMPLE_PLLX			(CLOCK_ID_XCPU - CLOCK_ID_FIRST_SIMPLE)
#define SIMPLE_PLLE			(CLOCK_ID_EPCI - CLOCK_ID_FIRST_SIMPLE)

#define TIMER_USEC_CNTR			(NV_PA_TMRUS_BASE + 0)
#define TIMER_USEC_CFG			(NV_PA_TMRUS_BASE + 4)

#define USEC_CFG_DIVISOR_MASK		0xffff

#define CONFIG_CTL_TBE			(1 << 7)
#define CONFIG_CTL_JTAG			(1 << 6)

#define CPU_RST				(1 << 0)
#define CLK_ENB_CPU			(1 << 0)
#define SWR_TRIG_SYS_RST		(1 << 2)
#define SWR_CSITE_RST			(1 << 9)
#define CLK_ENB_CSITE			(1 << 9)

#define PWRGATE_STATUS_CPU		(1 << 0)
#define PWRGATE_TOGGLE_PARTID_CPU	(0 << 0)
#define PWRGATE_TOGGLE_START		(1 << 8)

#define CPU_CMPLX_CPU_BRIDGE_CLKDIV_4	(3 << 0)
#define CPU_CMPLX_CPU0_CLK_STP_STOP	(1 << 8)
#define CPU_CMPLX_CPU0_CLK_STP_RUN	(0 << 8)
#define CPU_CMPLX_CPU1_CLK_STP_STOP	(1 << 9)
#define CPU_CMPLX_CPU1_CLK_STP_RUN	(0 << 9)

#define CPU_CMPLX_CPURESET0		(1 << 0)
#define CPU_CMPLX_CPURESET1		(1 << 1)
#define CPU_CMPLX_DERESET0		(1 << 4)
#define CPU_CMPLX_DERESET1		(1 << 5)
#define CPU_CMPLX_DBGRESET0		(1 << 12)
#define CPU_CMPLX_DBGRESET1		(1 << 13)

#if defined(CONFIG_TEGRA3)
#define CPU_CMPLX_CPURESET2		(1 << 2)
#define CPU_CMPLX_CPURESET3		(1 << 3)
#define CPU_CMPLX_DERESET2		(1 << 6)
#define CPU_CMPLX_DERESET3		(1 << 7)
#define CPU_CMPLX_DBGRESET2		(1 << 14)
#define CPU_CMPLX_DBGRESET3		(1 << 15)
#endif

#define PLLM_OUT1_RSTN_RESET_DISABLE	(1 << 0)
#define PLLM_OUT1_CLKEN_ENABLE		(1 << 1)
#define PLLM_OUT1_RATIO_VAL_8		(8 << 8)

#define SCLK_SYS_STATE_IDLE		(1 << 28)
#define SCLK_SWAKE_FIQ_SRC_PLLM_OUT1	(7 << 12)
#define SCLK_SWAKE_IRQ_SRC_PLLM_OUT1	(7 << 8)
#define SCLK_SWAKE_RUN_SRC_PLLM_OUT1	(7 << 4)
#define SCLK_SWAKE_IDLE_SRC_PLLM_OUT1	(7 << 0)

#define EVENT_ZERO_VAL_20		(20 << 0)
#define EVENT_ZERO_VAL_250		(250 << 0)
#define EVENT_MSEC			(1 << 24)
#define EVENT_USEC			(1 << 25)
#define EVENT_JTAG			(1 << 28)
#define EVENT_MODE_STOP			(2 << 29)

#define CCLK_PLLP_BURST_POLICY		0x20004444

#if defined(CONFIG_TEGRA3)
/* FLOW_CTLR_CLUSTER_CONTROL_0 2ch */
#define ACTIVE_LP			(1 << 0)

/* AHB_ARBITRATION_XBAR_CTRL_0 e0h */
#define ARBITRATION_XBAR_CTRL_PPSB_ENABLE_ENABLE (1 << 2)

/* SDMMC_VENDOR_CLOCK_CNTRL_0 100h */
#define HW_RSTN_OVERRIDE_OVERRIDE	(1 << 4)

/* APBDEV_PMC_PWRGATE_TOGGLE_0 30h */
#define PWRGATE_TOGGLE_PARTID_CP	(0 << 0)
#define PWRGATE_TOGGLE_PARTID_A9LP	(12 << 0)
#define PWRGATE_TOGGLE_START		(1 << 8)

/* APBDEV_PMC_REMOVE_CLAMPING_CMD_0 34h */
#define REMOVE_CLAMPING_CMD_CPU_ENABLE	(1 << 0)
#define REMOVE_CLAMPING_CMD_A9LP_ENABLE	(1 << 12)

/* APBDEV_PMC_PWRGATE_STATUS_0 38 */
#define PWRGATE_STATUS_CPU_ENABLE	(1 << 0)
#define PWRGATE_STATUS_A9LP_ENABLE	(1 << 12)

#define CPU_WAKEUP_CLUSTER		(1 << 31)

#endif

#endif

