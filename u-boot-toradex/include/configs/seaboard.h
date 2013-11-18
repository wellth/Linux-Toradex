/*
 *  (C) Copyright 2010,2011
 *  NVIDIA Corporation <www.nvidia.com>
 * Portions Copyright (c) 2011 The Chromium OS Authors
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

#ifndef __CONFIG_H
#define __CONFIG_H

#include <asm/sizes.h>

#define CONFIG_SPI_UART_SWITCH

#define CONFIG_TEGRA_LP0
#define CONFIG_TEGRA2_WARMBOOT

/* High-level configuration options */
#define TEGRA2_SYSMEM		"mem=1024M@0M"
#define V_PROMPT		"Tegra2 # "

#include "tegra2-common.h"

/* So our flasher can verify that all is well */
#define CONFIG_CRC32_VERIFY

#ifndef CONFIG_OF_CONTROL
/* Things in here are defined by the device tree now. Let it grow! */

#define CONFIG_TEGRA2_BOARD_STRING	"NVIDIA Seaboard"

#define CONFIG_TEGRA2_ENABLE_UARTD
#define CONFIG_SYS_NS16550_COM1		NV_PA_APB_UARTD_BASE

/* Seaboard SPI activity corrupts the first UART */
#define CONFIG_SPI_CORRUPTS_UART	NV_PA_APB_UARTD_BASE
#define CONFIG_SPI_CORRUPTS_UART_NR	3

/* On Seaboard: GPIO_PI3 = Port I = 8, bit = 3 */
#define UART_DISABLE_GPIO	GPIO_PI3

/* To select the order in which U-Boot sees USB ports */
#define CONFIG_TEGRA2_USB0      NV_PA_USB3_BASE
#define CONFIG_TEGRA2_USB1      NV_PA_USB1_BASE
#define CONFIG_TEGRA2_USB2      0
#define CONFIG_TEGRA2_USB3      0

/* Put USB1 in host mode */
#define CONFIG_TEGRA2_USB1_HOST
#define CONFIG_MACH_TYPE	MACH_TYPE_SEABOARD

/* Keyboard scan matrix configuration */
#define CONFIG_TEGRA2_KBC_PLAIN_KEYCODES {			\
	  0,    0,  'w',  's',  'a',  'z',    0,    KEY_FN,	\
	  0,    0,    0,    0,    0,    0,    0,    0,		\
	  0,    0,    0,    0,    0,    0,    0,    0,		\
	'5',  '4',  'r',  'e',  'f',  'd',  'x',    0,		\
	'7',  '6',  't',  'h',  'g',  'v',  'c',  ' ',		\
	'9',  '8',  'u',  'y',  'j',  'n',  'b', '\\',		\
	'-',  '0',  'o',  'i',  'l',  'k',  ',',  'm',		\
	  0,  '=',  ']', '\r',    0,    0,    0,    0,		\
	  0,    0,    0,    0, KEY_SHIFT, KEY_SHIFT,    0,    0,	\
	  0,    0,    0,    0,    0,    0,    0,    0,		\
	  0,    0,    0,    0,    0,    0,    0,    0,		\
	'[',  'p', '\'',  ';',  '/',  '.',    0,    0,		\
	  0,    0, 0x08,  '3',  '2',    0,    0,    0,		\
	  0, 0x7F,    0,    0,    0,    0,    0,    0,		\
	  0,    0,    0,  'q',    0,    0,  '1',    0,		\
	0x1B,  '`',   0, 0x09,    0,    0,    0,    0		\
}

#define CONFIG_TEGRA2_KBC_SHIFT_KEYCODES {			\
	  0,    0,  'W',  'S',  'A',  'Z',    0,    0,		\
	  0,    0,    0,    0,    0,    0,    0,    0,		\
	  0,    0,    0,    0,    0,    0,    0,    0,		\
	'%',  '$',  'R',  'E',  'F',  'D',  'X',    0,		\
	'&',  '^',  'T',  'H',  'G',  'V',  'C',  ' ',		\
	'(',  '*',  'U',  'Y',  'J',  'N',  'B',  '|',		\
	'_',  ')',  'O',  'I',  'L',  'K',  ',',  'M',		\
	  0,  '+',  '}', '\r',    0,    0,    0,    0,		\
	  0,    0,    0,    0,    0,    0,    0,    0,		\
	  0,    0,    0,    0,    0,    0,    0,    0,		\
	  0,    0,    0,    0,    0,    0,    0,    0,		\
	'{',  'P',  '"',  ':',  '?',  '>',    0,    0,		\
	  0,    0, 0x08,  '#',  '@',    0,    0,    0,		\
	  0, 0x7F,    0,    0,    0,    0,    0,    0,		\
	  0,    0,    0,  'Q',    0,    0,  '!',    0,		\
	0x1B, '~',    0, 0x09,    0,    0,    0,    0		\
}

#define CONFIG_TEGRA2_KBC_FUNCTION_KEYCODES {			\
	  0,     0,    0,    0,    0,    0,    0,    0,		\
	  0,     0,    0,    0,    0,    0,    0,    0,		\
	  0,     0,    0,    0,    0,    0,    0,    0,		\
	  0,     0,    0,    0,    0,    0,    0,    0,		\
	'7',     0,    0,    0,    0,    0,    0,    0,		\
	'9',   '8',  '4',    0,  '1',    0,    0,    0,		\
	  0,   '/',  '6',  '5',  '3',  '2',    0,  '0',		\
	  0,     0,    0,    0,    0,    0,    0,    0,		\
	  0,     0,    0,    0,    0,    0,    0,    0,		\
	  0,     0,    0,    0,    0,    0,    0,    0,		\
	  0,     0,    0,    0,    0,    0,    0,    0,		\
	  0,  '\'',    0,  '-',  '+',  '.',    0,    0,		\
	  0,     0,    0,    0,    0,    0,    0,    0,		\
	  0,     0,    0,    0,    0,    0,    0,    0,		\
	  0,     0,    0,    0,    0,    0,    0,    0,		\
	  0,     0,    0,    0,  '?',    0,    0,    0		\
}

/* pin-mux settings for seaboard */
#define CONFIG_I2CP_PIN_MUX		1
#define CONFIG_I2C1_PIN_MUX		1
#define CONFIG_I2C2_PIN_MUX		2
#define CONFIG_I2C3_PIN_MUX		1

/*
 * For HYNIX HY27UF4G2B NAND device.
 * Get the following timing values from data sheet.
 */

/*
 * non-EDO mode: value (in ns) = Max(tRP, tREA) + 6ns
 * EDO mode: value (in ns) = tRP timing
 */
#define CONFIG_NAND_MAX_TRP_TREA	26

#define CONFIG_NAND_TWB		100

/* Value = Max(tCR, tAR, tRR) */
#define CONFIG_NAND_MAX_TCR_TAR_TRR	20
#define CONFIG_NAND_TWHR		80

/* Value = Max(tCS, tCH, tALS, tALH) */
#define CONFIG_NAND_MAX_TCS_TCH_TALS_TALH	20
#define CONFIG_NAND_TWH			10
#define CONFIG_NAND_TWP			12
#define CONFIG_NAND_TRH			10
#define CONFIG_NAND_TADL		70

/* How many bytes for data area */
#define CONFIG_NAND_PAGE_DATA_BYTES	2048

/*
 * How many bytes in spare area
 * spare area = skipped bytes + ECC bytes of data area
 * + tag bytes + ECC bytes of tag bytes
 */
#define CONFIG_NAND_PAGE_SPARE_BYTES	64

#define CONFIG_NAND_SKIPPED_SPARE_BYTES	4

/* How many ECC bytes for data area */
#define CONFIG_NAND_RS_DATA_ECC_BYTES	36

/* How many tag bytes in spare area */
#define CONFIG_NAND_TAG_BYTES	20

/* How many ECC bytes to be generated for tag bytes */
#define CONFIG_NAND_TAG_ECC_BYTES	4

/* n bits */
#define CONFIG_NAND_BUS_WIDTH	8

/* GPIO port H bit 3, H.03, GMI_AD11->MFG_MODE_R */
#define CONFIG_NAND_WP_GPIO	GPIO_PH3

/* physical address to access nand at CS0 */
#define CONFIG_SYS_NAND_BASE	TEGRA2_NAND_BASE
#else
#define CONFIG_SYS_NAND_BASE_LIST {}
#endif /* CONFIG_OF_CONTROL not defined ^^^^^^^ */

#define CONFIG_TEGRA_KEYBOARD
#define CONFIG_KEYBOARD

#define CONFIG_CONSOLE_MUX
#define CONFIG_SYS_CONSOLE_IS_IN_ENV
#define CONFIG_STD_DEVICES_SETTINGS	"stdin=serial,tegra-kbc\0" \
					"stdout=serial,lcd\0" \
					"stderr=serial,lcd\0"

#define CONFIG_SYS_BOARD_ODMDATA	0x300d8011 /* lp1, 1GB */

/* GPIO */
#define CONFIG_TEGRA_GPIO
#define CONFIG_CMD_TEGRA_GPIO_INFO

/* SPI */
#define CONFIG_TEGRA_SPI
#define CONFIG_USE_SFLASH		/* T20 boards use SFLASH */
#define CONFIG_SPI_FLASH
#define CONFIG_SPI_FLASH_WINBOND
#define CONFIG_NEW_SPI_XFER
#define CONFIG_SF_DEFAULT_MODE		SPI_MODE_0
#define CONFIG_CMD_SPI
#define CONFIG_CMD_SF

/* I2C */
#define CONFIG_TEGRA_I2C
#define CONFIG_SYS_I2C_INIT_BOARD
#define CONFIG_I2C_MULTI_BUS
#define CONFIG_SYS_MAX_I2C_BUS		4
#define CONFIG_SYS_I2C_SPEED		100000
#define CONFIG_CMD_I2C

/* PMU and EMC support, requires i2c */
#define CONFIG_TEGRA_PMU
#define CONFIG_TEGRA_CLOCK_SCALING

/* TPM */
#define CONFIG_INFINEON_TPM_I2C
#define CONFIG_INFINEON_TPM_I2C_BUS	2
#define CONFIG_TPM_SLB9635_I2C
#define CONFIG_TPM_I2C_BURST_LIMITATION	3

/* SD/MMC */
#define CONFIG_MMC
#define CONFIG_GENERIC_MMC
#define CONFIG_TEGRA_MMC
#define CONFIG_CMD_MMC

#define CONFIG_DOS_PARTITION
#define CONFIG_EFI_PARTITION
#define CONFIG_CMD_EXT2
#define CONFIG_CMD_FAT

/* Environment in SPI */
#define CONFIG_ENV_IS_IN_SPI_FLASH

#define CONFIG_ENV_SECT_SIZE    CONFIG_ENV_SIZE
#define CONFIG_ENV_OFFSET       (SZ_4M - CONFIG_ENV_SECT_SIZE)

/*
 *  LCDC configuration
 */
#define CONFIG_LCD
#define CONFIG_VIDEO_TEGRA

/* TODO: This needs to be configurable at run-time */
#define LCD_BPP             LCD_COLOR16
#define CONFIG_SYS_WHITE_ON_BLACK       /*Console colors*/

#define CONFIG_EXTRA_ENV_SETTINGS \
	CONFIG_EXTRA_ENV_SETTINGS_COMMON \
	"board=seaboard\0" \

#define CONFIG_DEFAULT_DEVICE_TREE "tegra2-seaboard"

/* NAND support */
#define CONFIG_CMD_NAND
#define CONFIG_TEGRA2_NAND

/* Max number of NAND devices */
#define CONFIG_SYS_MAX_NAND_DEVICE	1
#endif /* __CONFIG_H */
