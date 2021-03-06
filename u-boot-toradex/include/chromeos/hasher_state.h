/*
 * Copyright (c) 2011 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 *
 * Alternatively, this software may be distributed under the terms of the
 * GNU General Public License ("GPL") version 2 as published by the Free
 * Software Foundation.
 */

#ifndef CHROMEOS_HASHER_STATE_H_
#define CHROMEOS_HASHER_STATE_H_

#include <chromeos/firmware_storage.h>

typedef struct {
	firmware_storage_t *file;
	struct {
		void *vblock;
		uint32_t offset;
		uint32_t size;
		void *cache;
	} fw[2];
} hasher_state_t;

#endif /* CHROMEOS_HASHER_STATE_H_ */
