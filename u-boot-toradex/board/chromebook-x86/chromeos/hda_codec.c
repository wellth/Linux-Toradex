/*
 * Copyright (c) 2011 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 *
 * Alternatively, this software may be distributed under the terms of the
 * GNU General Public License ("GPL") version 2 as published by the Free
 * Software Foundation.
 */

/* Implementation of per-board codec beeping */

#include <common.h>
#include <asm/global_data.h>
#include <asm/io.h>
#include <pci.h>
#include <chromeos/hda_codec.h>

#define HDA_CMD_REG 0x5C
#define HDA_ICII_REG 0x64
#define   HDA_ICII_BUSY (1 << 0)
#define   HDA_ICII_VALID  (1 << 1)

#define BEEP_FREQ_MAGIC 0x00C70A00
#define BEEP_FREQ_BASE 12000

/**
 *  Wait 50usec for the codec to indicate it is ready
 *  no response would imply that the codec is non-operative
 */
static int wait_for_ready(uint32_t base)
{
	/* Use a 50 usec timeout - the Linux kernel uses the
	 * same duration
	 */

	int timeout = 50;

	while (timeout--) {
		uint32_t reg32 = readl(base +  HDA_ICII_REG);
		asm("" ::: "memory");
		if (!(reg32 & HDA_ICII_BUSY))
			return 0;
		udelay(1);
	}

	return -1;
}

/**
 *  Wait 50usec for the codec to indicate that it accepted
 *  the previous command.  No response would imply that the code
 *  is non-operative
 */
static int wait_for_valid(uint32_t base)
{
	uint32_t reg32;

	/* Send the verb to the codec */
	reg32 = readl(base + HDA_ICII_REG);
	reg32 |= HDA_ICII_BUSY | HDA_ICII_VALID;
	writel(reg32, base + HDA_ICII_REG);

	/* Use a 50 usec timeout - the Linux kernel uses the
	 * same duration
	 */

	int timeout = 50;
	while (timeout--) {
		reg32 = readl(base + HDA_ICII_REG);
		if ((reg32 & (HDA_ICII_VALID | HDA_ICII_BUSY)) ==
				HDA_ICII_VALID)
			return 0;
		udelay(1);
	}

	return -1;
}

/* Wait for the codec to be ready, write the verb, then wait for the
 * codec to be valid.
 */
int write_one_verb(uint32_t base, uint32_t val)
{
	if (wait_for_ready(base) == -1)
		return -1;

	writel(val, base + HDA_CMD_REG);

	if (wait_for_valid(base) == -1)
		return -1;

	return 0;
}

/* Supported sound devices.
 */
static struct pci_device_id supported[] = {
	{PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_COUGARPOINT_HDA},
	{}
};

/* Find the base address to talk tot he HDA codec.
 */
static u32 get_hda_base(void)
{
	pci_dev_t devbusfn;
	u32 pci_mem_base;

	devbusfn = pci_find_devices(supported, 0);
	if (devbusfn < 0) {
		printf("Audio: Controller not found !\n");
		return 0;
	}

	pci_read_config_dword(devbusfn, PCI_BASE_ADDRESS_0, &pci_mem_base);
	pci_mem_base = pci_mem_to_phys(devbusfn, pci_mem_base);
	return pci_mem_base;
}

static const u32 beep_cmd[] = {
	0x00170500,			/* power up codec */
	0x00270500,			/* power up DAC */
	0x00670500,			/* power up speaker */
	0x00670740,			/* enable speaker output */
	0x0023B04B,			/* set DAC gain */
};					/* and follow with BEEP_FREQ_MAGIC */

void enable_beep(uint32_t frequency)
{
	uint32_t base;
	uint8_t divider_val;
	int i;

	if (0 == frequency)
		divider_val = 0;	/* off */
	else if (frequency > BEEP_FREQ_BASE)
		divider_val = 1;
	else if (frequency < BEEP_FREQ_BASE / 0xFF)
		divider_val = 0xff;
	else
		divider_val = (uint8_t)(0xFF & (BEEP_FREQ_BASE / frequency));

	base = get_hda_base();
	for (i = 0; i < sizeof(beep_cmd)/sizeof(beep_cmd[0]); i++) {
		if (write_one_verb(base, beep_cmd[i]))
			return;
	}
	write_one_verb(base, BEEP_FREQ_MAGIC|divider_val);
}

void disable_beep(void)
{
	uint32_t base;

	base = get_hda_base();
	write_one_verb(base, 0x00C70A00); /* Disable beep gen */
}
