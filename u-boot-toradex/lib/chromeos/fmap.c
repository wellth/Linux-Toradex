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
#include <chromeos/fmap.h>

#define PREFIX "chromeos/fmap: "

static void
dump_fmap_entry(const char *path, struct fmap_entry *entry)
{
	VBDEBUG(PREFIX "%-20s %08x:%08x\n", path, entry->offset,
		entry->length);
}

static void
dump_fmap_firmware_entry(const char *name, struct fmap_firmware_entry *entry)
{
	VBDEBUG(PREFIX "%s\n", name);
	dump_fmap_entry("boot", &entry->boot);
	dump_fmap_entry("vblock", &entry->vblock);
	dump_fmap_entry("firmware_id", &entry->firmware_id);
	VBDEBUG(PREFIX "%-20s %08llx\n", "LBA", entry->block_lba);
}

void
dump_fmap(struct twostop_fmap *config)
{
	VBDEBUG(PREFIX "rw-a:\n");
	dump_fmap_entry("fmap", &config->readonly.fmap);
	dump_fmap_entry("gbb", &config->readonly.gbb);
	dump_fmap_entry("firmware_id", &config->readonly.firmware_id);
	dump_fmap_firmware_entry("rw-a", &config->readwrite_a);
	dump_fmap_firmware_entry("rw-b", &config->readwrite_b);
}
