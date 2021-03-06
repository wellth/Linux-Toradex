/*
 * Copyright (c) 2011 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 *
 * Alternatively, this software may be distributed under the terms of the
 * GNU General Public License ("GPL") version 2 as published by the Free
 * Software Foundation.
 */

#include <common.h>
#include <chromeos/common.h>
#include <chromeos/gbb.h>

#include <gbb_header.h>

#define PREFIX "gbb: "

int gbb_init(read_buf_type gbb, firmware_storage_t *file, uint32_t gbb_offset,
	     size_t gbb_size)
{
#ifndef CONFIG_HARDWARE_MAPPED_SPI
	GoogleBinaryBlockHeader *gbbh = (GoogleBinaryBlockHeader *)gbb;
	uint32_t hwid_end;
	uint32_t rootkey_end;

	if (file->read(file, gbb_offset, sizeof(*gbbh), gbbh)) {
		VBDEBUG(PREFIX "failed to read GBB header\n");
		return 1;
	}

	hwid_end = gbbh->hwid_offset + gbbh->hwid_size;
	rootkey_end = gbbh->rootkey_offset + gbbh->rootkey_size;
	if (hwid_end < gbbh->hwid_offset || hwid_end > gbb_size ||
			rootkey_end < gbbh->rootkey_offset ||
			rootkey_end > gbb_size) {
		VBDEBUG(PREFIX "%s: invalid gbb header entries\n", __func__);
		VBDEBUG(PREFIX "   hwid_end=%x\n", hwid_end);
		VBDEBUG(PREFIX "   gbbh->hwid_offset=%x\n", gbbh->hwid_offset);
		VBDEBUG(PREFIX "   gbb_size=%x\n", gbb_size);
		VBDEBUG(PREFIX "   rootkey_end=%x\n", rootkey_end);
		VBDEBUG(PREFIX "   gbbh->rootkey_offset=%x\n",
			gbbh->rootkey_offset);
		VBDEBUG(PREFIX "   %d, %d, %d, %d\n",
			hwid_end < gbbh->hwid_offset,
			hwid_end >= gbb_size,
			rootkey_end < gbbh->rootkey_offset,
			rootkey_end >= gbb_size);
		return 1;
	}

	if (file->read(file, gbb_offset + gbbh->hwid_offset,
				gbbh->hwid_size,
				gbb + gbbh->hwid_offset)) {
		VBDEBUG(PREFIX "failed to read hwid\n");
		return 1;
	}

	if (file->read(file, gbb_offset + gbbh->rootkey_offset,
				gbbh->rootkey_size,
				gbb + gbbh->rootkey_offset)) {
		VBDEBUG(PREFIX "failed to read root key\n");
		return 1;
	}
#else
	/* No data is actually moved in this case so no bounds checks. */
	if (file->read(file, gbb_offset,
		       sizeof(GoogleBinaryBlockHeader), gbb)) {
		VBDEBUG(PREFIX "failed to read GBB header\n");
		return 1;
	}
#endif

	return 0;
}

#ifndef CONFIG_HARDWARE_MAPPED_SPI
int gbb_read_bmp_block(void *gbb, firmware_storage_t *file, uint32_t gbb_offset,
		       size_t gbb_size)
{
	GoogleBinaryBlockHeader *gbbh = (GoogleBinaryBlockHeader *)gbb;
	uint32_t bmpfv_end = gbbh->bmpfv_offset + gbbh->bmpfv_size;

	if (bmpfv_end < gbbh->bmpfv_offset || bmpfv_end > gbb_size) {
		VBDEBUG(PREFIX "%s: invalid gbb header entries\n", __func__);
		return 1;
	}

	if (file->read(file, gbb_offset + gbbh->bmpfv_offset,
				gbbh->bmpfv_size,
				gbb + gbbh->bmpfv_offset)) {
		VBDEBUG(PREFIX "failed to read bmp block\n");
		return 1;
	}

	return 0;
}

int gbb_read_recovery_key(void *gbb, firmware_storage_t *file,
			  uint32_t gbb_offset, size_t gbb_size)
{
	GoogleBinaryBlockHeader *gbbh = (GoogleBinaryBlockHeader *)gbb;
	uint32_t rkey_end = gbbh->recovery_key_offset +
		gbbh->recovery_key_size;

	if (rkey_end < gbbh->recovery_key_offset || rkey_end > gbb_size) {
		VBDEBUG(PREFIX "%s: invalid gbb header entries\n", __func__);
		VBDEBUG(PREFIX "   gbbh->recovery_key_offset=%x\n",
			gbbh->recovery_key_offset);
		VBDEBUG(PREFIX "   gbbh->recovery_key_size=%x\n",
			gbbh->recovery_key_size);
		VBDEBUG(PREFIX "   rkey_end=%x\n", rkey_end);
		VBDEBUG(PREFIX "   gbb_size=%x\n", gbb_size);
		return 1;
	}

	if (file->read(file, gbb_offset + gbbh->recovery_key_offset,
				gbbh->recovery_key_size,
				gbb + gbbh->recovery_key_offset)) {
		VBDEBUG(PREFIX "failed to read recovery key\n");
		return 1;
	}

	return 0;
}
#endif

int gbb_check_integrity(uint8_t *gbb)
{
	/*
	 * Avoid a second "$GBB" signature in the binary. Some utility programs
	 * that parses the contents of firmware image could fail if there are
	 * multiple signatures.
	 */
	if (gbb[0] == '$' && gbb[1] == 'G' && gbb[2] == 'B' && gbb[3] == 'B')
		return 0;
	else
		return 1;
}
