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

/* Import the definition of vboot_wrapper interfaces. */
#include <vboot_api.h>

uint64_t VbExGetTimer(void)
{
	return timer_get_us();
}
