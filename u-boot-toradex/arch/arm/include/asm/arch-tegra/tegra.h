/*
 * (C) Copyright 2010,2011
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

#ifndef _TEGRA_H_
#define _TEGRA_H_

#if !(defined(TEGRA_SLINK4_BASE) ^ defined(TEGRA_SPIFLASH_BASE))
#error "define either TEGRA_SLINK4_BASE or TEGRA_SPIFLASH_BASE"
#endif

#ifdef TEGRA_SLINK4_BASE
#define TEGRA_SPI_BASE		TEGRA_SLINK4_BASE
#else
#define TEGRA_SPI_BASE		TEGRA_SPIFLASH_BASE
#endif

#define NV_PA_ARM_PERIPHBASE	0x50040000
#define NV_PA_PG_UP_BASE	0x60000000
#define NV_PA_TMRUS_BASE	0x60005010
#define NV_PA_CLK_RST_BASE	0x60006000
#define NV_PA_FLOW_BASE		0x60007000
#define NV_PA_GPIO_BASE		0x6000D000
#define NV_PA_EVP_BASE		0x6000F000
#define NV_PA_APB_MISC_BASE	0x70000000
#define NV_PA_APB_MISC_GP_BASE	(NV_PA_APB_MISC_BASE + 0x0800)
#define NV_PA_APB_UARTA_BASE	(NV_PA_APB_MISC_BASE + 0x6000)
#define NV_PA_APB_UARTB_BASE	(NV_PA_APB_MISC_BASE + 0x6040)
#define NV_PA_APB_UARTC_BASE	(NV_PA_APB_MISC_BASE + 0x6200)
#define NV_PA_APB_UARTD_BASE	(NV_PA_APB_MISC_BASE + 0x6300)
#define NV_PA_APB_UARTE_BASE	(NV_PA_APB_MISC_BASE + 0x6400)
#define TEGRA_NAND_BASE		0x70008000
#define TEGRA_I2C1_BASE		0x7000C000
#define TEGRA_I2C2_BASE		0x7000C400
#define TEGRA_I2C3_BASE		0x7000C500
#define TEGRA_DVC_BASE		0x7000D000
#define TEGRA_KBC_BASE		0x7000E200
#define NV_PA_PMC_BASE		0x7000E400
#define NV_PA_MC_BASE		0x7000F000
#define NV_PA_MC_EMEM_CFG_0		(NV_PA_MC_BASE + 0x50)
#define NV_PA_EMC_BASE		0x7000F400
#define NV_PA_EMC_ADR_CFG_BASE	(NV_PA_EMC_BASE + 0x10)
#define EMEM_DEVSIZE_MASK	(0xf << EMEM_DEVSIZE_SHIFT)
#define EMEM_DEVSIZE_SHIFT	(16)
#define NV_PA_FUSE_BASE		0x7000F800
#define NV_PA_CSITE_BASE	0x70040000

#define TEGRA_SDRC_CS0		NV_PA_SDRAM_BASE
#define LOW_LEVEL_SRAM_STACK	0x4000FFFC
#define EARLY_AVP_STACK		(NV_PA_SDRAM_BASE + 0x20000)
#define EARLY_CPU_STACK		(EARLY_AVP_STACK - 4096)
#define PG_UP_TAG_AVP		0xAAAAAAAA

#define PRM_RSTCTRL		NV_PA_PMC_BASE

/* Address at which WB code runs, it must not overlap Bootrom's IRAM usage */
#define AP20_WB_RUN_ADDRESS	0x40020000

#ifndef __ASSEMBLY__
struct timerus {
	unsigned int cntr_1us;
};

#define NVBOOTINFOTABLE_BCTSIZE	0x38	/* BCT size in BIT in IRAM */
#define NVBOOTINFOTABLE_BCTPTR	0x3C	/* BCT pointer in BIT in IRAM */

/* These are the available SKUs (product types) for Tegra */
enum {
	SKU_ID_T20		= 0x8,
	SKU_ID_T25SE		= 0x14,
	SKU_ID_AP25		= 0x17,
	SKU_ID_T25		= 0x18,
	SKU_ID_AP25E		= 0x1b,
	SKU_ID_T25E		= 0x1c,

	SKU_ID_T30		= 0x81,	/* TBD use real Cardhu value */
};

/*
 * These are used to distinguish SOC types for setting up clocks. Mostly
 * we can tell the clocking required by looking at the SOC sku_id, but
 * for T30 it is a user option as to whether to run PLLP in fast or slow
 * mode, so we have two options there.
 */
enum {
	TEGRA_SOC_T20,
	TEGRA_SOC_T25,
	TEGRA_SOC_T30,
	TEGRA_SOC_T30_408MHZ,	/* A T30 with faster PLLP */
	TEGRA_SOC2_SLOW,	/* T2x needs to run at slow clock initially */

	TEGRA_SOC_COUNT,
	TEGRA_SOC_UNKNOWN	= -1,
};
#endif

#endif	/* TEGRA_H */
