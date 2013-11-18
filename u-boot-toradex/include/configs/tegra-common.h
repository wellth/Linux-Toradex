/*
 * Copyright (c) 2011 The Chromium OS Authors.
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

#ifndef __TEGRA_COMMON_H
#define __TEGRA_COMMON_H

/*
 * QUOTE(m) will evaluate to a string version of the value of the macro m
 * passed in.  The extra level of indirection here is to first evaluate the
 * macro m before applying the quoting operator.
 */
#define QUOTE_(m)	#m
#define QUOTE(m)	QUOTE_(m)

/* FDT support */
#define CONFIG_OF_LIBFDT	/* Device tree support */
#define CONFIG_OF_CONTROL	/* Use the device tree to set up U-Boot */

/* Embed the device tree in U-Boot, if not otherwise handled */
#ifndef CONFIG_OF_SEPARATE
#define CONFIG_OF_EMBED
#endif

/*
 * High Level Configuration Options
 */
#define CONFIG_ARMCORTEXA9		/* This is an ARM V7 CPU core */
#define CONFIG_TEGRA			/* in the NVidia Tegra family */
#define CONFIG_MACH_TEGRA_GENERIC	/* which is a Tegra generic machine */
#define CONFIG_SYS_NO_L2CACHE		/* No L2 cache */
#define CONFIG_BOOTSTAGE		/* Record boot time */
#define CONFIG_BOOTSTAGE_REPORT		/* Print a boot time report */
#define CONFIG_ARCH_CPU_INIT		/* Fire up the A9 core */
#define CONFIG_ALIGN_LCD_TO_SECTION	/* Align LCD to 1MB boundary */
#define CONFIG_BOARD_EARLY_INIT_F
#define BOARD_LATE_INIT			/* Make sure LCD init is complete */

#include <asm/arch/tegra.h>		/* get chip and board defs */

#define CACHE_LINE_SIZE			32

/*
 * Display CPU and Board information
 */
#define CONFIG_DISPLAY_CPUINFO
#define CONFIG_DISPLAY_BOARDINFO_LATE
#define CONFIG_SYS_CONSOLE_INFO_QUIET

#define CONFIG_SKIP_LOWLEVEL_INIT

#define CONFIG_CMDLINE_TAG		/* enable passing of ATAGs */
#define CONFIG_OF_LIBFDT		/* enable passing of devicetree */

/*
 * Size of malloc() pool
 */
#define CONFIG_SYS_MALLOC_LEN		(4 << 20)	/* 4MB  */

/*
 * PllX Configuration
 */
#define CONFIG_SYS_CPU_OSC_FREQUENCY	1000000	/* Set CPU clock to 1GHz */

/*
 * NS16550 Configuration
 */
#define CONFIG_SERIAL_MULTI
#define CONFIG_NS16550_BUFFER_READS
#define CONFIG_SYS_NS16550
#define CONFIG_SYS_NS16550_REG_SIZE	(-4)

/* Default serial multiplier */
#define CONFIG_DEFAULT_NS16550_MULT	16

#ifdef CONFIG_OF_CONTROL
#define CONFIG_COMPAT_STRING		"nvidia,tegra250"
#else

#define CONFIG_SYS_NS16550_SERIAL
#define CONFIG_SYS_NS16550_CLK		216000000

/*
 * select serial console configuration
 */
#define CONFIG_CONS_INDEX	1
#endif /* CONFIG_OF_CONTROL ^^^^^ not defined */

/* Environment information */
#define CONFIG_ENV_SIZE         SZ_4K

/* allow to overwrite serial and ethaddr */
#define CONFIG_ENV_OVERWRITE
/* environment baudrate unfortunately does not seem to work properly */
#define CONFIG_BAUDRATE			115200
#define CONFIG_SYS_BAUDRATE_TABLE	{4800, 9600, 19200, 38400, 57600,\
					115200}

#define CONFIG_SYS_HZ			1000

/*
 * Miscellaneous configurable options
 */
#define CONFIG_SYS_LONGHELP		/* undef to save memory */
#define CONFIG_SYS_HUSH_PARSER		/* use "hush" command parser */
#define CONFIG_SYS_PROMPT_HUSH_PS2	"> "
#define CONFIG_SYS_PROMPT		V_PROMPT
#define CONFIG_SILENT_CONSOLE
/*
 * Increasing the size of the IO buffer as default nfsargs size is more
 *  than 256 and so it is not possible to edit it
 */
#define CONFIG_SYS_CBSIZE		(256 * 2) /* Console I/O Buffer Size */
/* Print Buffer Size */
#define CONFIG_SYS_PBSIZE		(CONFIG_SYS_CBSIZE + \
					sizeof(CONFIG_SYS_PROMPT) + 16)
#define CONFIG_SYS_MAXARGS		32	/* max number of command args */
/* Boot Argument Buffer Size */
#define CONFIG_SYS_BARGSIZE		(CONFIG_SYS_CBSIZE)

#define CONFIG_SYS_MEMTEST_START	(TEGRA_SDRC_CS0 + 0x600000)
#define CONFIG_SYS_MEMTEST_END		(CONFIG_SYS_MEMTEST_START + 0x100000)

#define CONFIG_PRE_CONSOLE_BUFFER

#define CONFIG_SYS_INIT_RAM_ADDR	CONFIG_STACKBASE
#define CONFIG_SYS_INIT_RAM_SIZE	CONFIG_SYS_MALLOC_LEN
#define RAM_TOP				(CONFIG_SYS_INIT_RAM_ADDR + \
						CONFIG_SYS_INIT_RAM_SIZE)
#define CONFIG_PRE_CON_BUF_SZ		1024
#define CONFIG_PRE_CON_BUF_ADDR		(RAM_TOP - CONFIG_PRE_CON_BUF_SZ)
#define CONFIG_SYS_INIT_SP_ADDR		(CONFIG_PRE_CON_BUF_ADDR - \
						GENERATED_GBL_DATA_SIZE)

/* kernel device tree booting support */
#define CONFIG_FIT	1

/*
 * 32M is what it takes the u-boot to allocate enough room for the kernel
 * loader to inflate the kernel and keep a copy of the device tree handy.
 */
#define CONFIG_SYS_BOOTMAPSZ	(1 << 25)

/* BCT starts at 0x100 in RAM */
#define BCT_OFFSET		0x100

/*
 * Defines the regen_all variable, which is used by other commands
 * defined in this file.  Usage is to override one or more of the environment
 * variables and then run regen_all to regenerate the environment.
 *
 * Args from other scipts in this file:
 *   bootdev_bootargs: Filled in by other commands below based on the boot
 *       device.
 *
 * Args:
 *   common_bootargs: A copy of the default bootargs so we can run regen_all
 *       more than once.
 *   dev_extras: Placeholder space for developers to put their own boot args.
 *   extra_bootargs: Filled in by update_firmware_vars.py script in some cases.
 */
#define CONFIG_REGEN_ALL_SETTINGS \
	"common_bootargs=cros_legacy " CONFIG_DIRECT_BOOTARGS "\0" \
	\
	"dev_extras=\0" \
	"extra_bootargs=\0" \
	"bootdev_bootargs=\0" \
	\
	"regen_all=" \
		"setenv bootargs " \
			"${common_bootargs} " \
			"${dev_extras} " \
			"${extra_bootargs} " \
			"${bootdev_bootargs}\0"

/*
 * Defines ext2_boot and run_disk_boot_script.
 *
 * The run_disk_boot_script runs a u-boot script on the boot disk.  At the
 * moment this is used to allow the boot disk to choose a partion to boot from,
 * but could theoretically be used for more complicated things.
 *
 * The ext2_boot script boots from an ext2 device.
 *
 * Args from other scipts in this file:
 *   devtype: The device type we're booting from, like "usb" or "mmc"
 *   devnum: The device number (depends on devtype).  If we're booting from
 *       extranal MMC (for instance), this would be 1
 *   devname: The linux device name that will be assigned, like "sda" or
 *       mmcblk0p
 *
 * Args expected to be set by the u-boot script in /u-boot/boot.scr.uimg:
 *   rootpart: The root filesystem partion; we default to 3 in case there are
 *       problems reading the boot script.
 *   cros_bootfile: The name of the kernel in the root partition; we default to
 *       "/boot/vmlinux.uimg"
 *
 * Other args:
 *   script_part: The FAT partion we'll look for a boot script in.
 *   script_img: The name of the u-boot script.
 *
 * When we boot from an ext2 device, we will look at partion 12 (0x0c) to find
 * a u-boot script (as /u-boot/boot.scr.uimg).  That script is expected to
 * override "rootpart" and "cros_bootfile" as needed to select which partition
 * to boot from.
 */
#define CONFIG_EXT2_BOOT_HELPER_SETTINGS \
	"rootpart=3\0" \
	"cros_bootfile=/boot/vmlinux.uimg\0" \
	\
	"script_part=c\0" \
	"script_img=/u-boot/boot.scr.uimg\0" \
	\
	"run_disk_boot_script=" \
		"if fatload ${devtype} ${devnum}:${script_part} " \
				"${loadaddr} ${script_img}; then " \
			"source ${loadaddr}; " \
		"fi\0" \
	\
	"regen_ext2_bootargs=" \
		"setenv bootdev_bootargs " \
		"root=/dev/${devname}${rootpart} rootwait ro; " \
		"run regen_all\0" \
	\
	"ext2_boot=" \
		"run regen_ext2_bootargs; " \
		"if ext2load ${devtype} ${devnum}:${rootpart} " \
			"${loadaddr} ${cros_bootfile}; then " \
			"bootm ${loadaddr};" \
		"fi\0"

/*
 * Network-boot related settings.
 *
 * At the moment, we support full network root booting (tftp kernel and initial
 * ramdisk) as well as nfs booting (tftp kernel and point root to NFS).
 *
 * Network booting is enabled if you have an ethernet adapter plugged in at boot
 * and also have set tftpserverip/nfsserverip to something other than 0.0.0.0.
 * For full network booting you just need tftpserverip.  For full NFS root
 * you neet to set both.
 */
#define CONFIG_NETBOOT_SETTINGS \
	"tftpserverip=0.0.0.0\0" \
	"nfsserverip=0.0.0.0\0" \
	\
	"rootaddr=0x12008000\0" \
	"initrd_high=0xffffffff\0" \
	\
	"regen_nfsroot_bootargs=" \
		"setenv bootdev_bootargs " \
			"dev=/dev/nfs4 rw nfsroot=${nfsserverip}:${rootpath} " \
			"ip=dhcp noinitrd; " \
		"run regen_all\0" \
	"regen_initrdroot_bootargs=" \
		"setenv bootdev_bootargs " \
			"rw root=/dev/ram0 ramdisk_size=294912; " \
		"run regen_all\0" \
	\
	"tftp_setup=" \
		"setenv tftpkernelpath " \
			"/tftpboot/vmlinux.uimg; " \
		"setenv tftprootpath " \
			"/tftpboot/initrd.uimg; " \
		"setenv rootpath " \
			"/export/nfsroot; " \
		"setenv autoload n\0" \
	"initrdroot_boot=" \
		"run tftp_setup; " \
		"run regen_initrdroot_bootargs; " \
		"bootp; " \
		"if tftpboot ${rootaddr} ${tftpserverip}:${tftprootpath} && " \
		"   tftpboot ${loadaddr} ${tftpserverip}:${tftpkernelpath}; " \
		"then " \
			"bootm ${loadaddr} ${rootaddr}; " \
		"else " \
			"echo 'ERROR: Could not load root/kernel from TFTP'; " \
			"exit; " \
		"fi\0" \
	"tftp_ext2_boot=" \
		"run tftp_setup; " \
		"run regen_ext2_bootargs; " \
		"bootp; " \
		"if tftpboot ${loadaddr} ${tftpserverip}:${tftpkernelpath}; " \
		"then " \
			"bootm ${loadaddr}; " \
		"else " \
			"echo 'ERROR: Could not load kernel from TFTP'; " \
			"exit; " \
		"fi\0" \
	"nfsroot_boot=" \
		"run tftp_setup; " \
		"run regen_nfsroot_bootargs; " \
		"bootp; " \
		"if tftpboot ${loadaddr} ${tftpserverip}:${tftpkernelpath}; " \
		"then " \
			"bootm ${loadaddr}; " \
		"else " \
			"echo 'ERROR: Could not load kernel from TFTP'; " \
			"exit; " \
		"fi\0" \
	\
	"net_boot=" \
		"if test ${ethact} != \"\"; then " \
			"if test ${tftpserverip} != \"0.0.0.0\"; then " \
				"run initrdroot_boot; " \
				"if test ${nfsserverip} != \"0.0.0.0\"; then " \
					"run nfsroot_boot; " \
				"fi; " \
			"fi; " \
		"fi\0" \

/*
 * Our full set of extra enviornment variables.
 *
 * A few notes:
 * - Right now, we can only boot from one USB device.  Need to fix this once
 *   usb works better.
 * - We define "non_verified_boot", which is the normal boot command unless
 *   it is overridden in the FDT.
 * - When we're running securely, the FDT will specify to call vboot_twostop
 *   directly.
 */

#define CONFIG_EXTRA_ENV_SETTINGS_COMMON \
	CONFIG_STD_DEVICES_SETTINGS \
	CONFIG_REGEN_ALL_SETTINGS \
	CONFIG_EXT2_BOOT_HELPER_SETTINGS\
	CONFIG_NETBOOT_SETTINGS \
	\
	"usb_boot=setenv devtype usb; " \
		"setenv devnum 0; " \
		"setenv devname sda; " \
		"run run_disk_boot_script;" \
		"run ext2_boot\0" \
	\
	"mmc_setup=" \
		"mmc rescan ${devnum}; " \
		"setenv devtype mmc; " \
		"setenv devname mmcblk${devnum}p\0" \
	"mmc_boot=" \
		"run mmc_setup; " \
		"run run_disk_boot_script;" \
		"run ext2_boot\0" \
	"mmc0_boot=setenv devnum 0; " \
		"run mmc_boot\0" \
	"mmc1_boot=setenv devnum 1; " \
		"run mmc_boot\0" \
	"mmc0_tftpboot=setenv devnum 0; " \
		"run mmc_setup; " \
		"run tftp_ext2_boot\0" \
	\
	"non_verified_boot=" \
		"usb start; " \
		"run net_boot; " \
		"run usb_boot; " \
		\
		"run mmc1_boot; " \
		"run mmc0_boot\0"

#define CONFIG_BOOTCOMMAND "run non_verified_boot"

#define CONFIG_CMD_ENTERRCM

#endif
