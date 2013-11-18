/*
 * Copyright (c) 2011, Google Inc. All rights reserved.
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

#ifndef __BOOTSTAGE_H
#define __BOOTSTAGE_H

/*
 * These are the things that can be timestamped. There are some pre-defined
 * by U-Boot, and some which are user defined.
 */
enum bootstage_id {
	BOOTSTAGE_AWAKE,
	BOOTSTAGE_START_UBOOT_F,
	BOOTSTAGE_START_RELOC,
	BOOTSTAGE_START_UBOOT_R,
	BOOTSTAGE_USB_START,
	BOOTSTAGE_ETH_START,
	BOOTSTAGE_BOOTP_START,
	BOOTSTAGE_BOOTP_STOP,
	BOOTSTAGE_KERNELREAD_START,
	BOOTSTAGE_KERNELREAD_STOP,
	BOOTSTAGE_BOOTM_START,
	BOOTSTAGE_BOOTM_HANDOFF,
	BOOTSTAGE_MAIN_LOOP,

	BOOTSTAGE_CPU_AWAKE,
	BOOTSTAGE_MAIN_CPU_AWAKE,
	BOOTSTAGE_MAIN_CPU_READY,

	BOOTSTAGE_TPM_SEND_RECV,
	BOOTSTAGE_LCD_WAIT,

	/* a few spare for the user, from here */
	BOOTSTAGE_USER,

	/*
	 * Total number of entries - increase this at the cost of some space
	 * in the data region of the U-Boot image.
	 */
	BOOTSTAGE_COUNT = 30
};

#ifdef CONFIG_BOOTSTAGE

/**
 * Mark a time stamp for the current boot stage.
 *
 * @param id	Bootstage id to record this timestamp against
 * @param name	Textual name to display for this id in the report (maybe NULL)
 * @return recorded timestamp in microseconds
 */
uint32_t bootstage_mark(enum bootstage_id id, const char *name);

/**
 * Mark the start of a bootstage activity. The end will be marked later with
 * bootstage_accum() and at that point we accumulate the time taken. Calling
 * this function turns the given id into a accumulator rather than and
 * absolute mark in time. Accumulators record the total amount of time spent
 * in an activty during boot.
 *
 * @param id	Bootstage id to record this timestamp against
 * @param name	Textual name to display for this id in the report (maybe NULL)
 * @return start timestamp in microseconds
 */
uint32_t bootstage_start(enum bootstage_id id, const char *name);

/**
 * Mark the start of a bootstage activity. The end will be marked later with
 * bootstage_accum() and at that point we accumulate the time taken. Calling
 * this function turns the given id into a accumulator rather than and
 * absolute mark in time. Accumulators record the total amount of time spent
 * in an activty during boot.
 *
 * @param id	Bootstage id to record this timestamp against
 * @param name	Textual name to display for this id in the report (maybe NULL)
 * @return time spent in this iteration of the activity (i.e. the time now
 *		less the start time recorded in the last bootstage_start() call
 *		with this id.
 */
uint32_t bootstage_accum(enum bootstage_id id);

/* Print a report about boot time */
void bootstage_report(void);

#else

static inline uint32_t bootstage_mark(enum bootstage_id id, const char *name)
{ return 0; }
static inline uint32_t bootstage_start(enum bootstage_id id, const char *name)
{ return 0; }
static inline uint32_t bootstage_accum(enum bootstage_id id)
{ return 0; }

#endif

#endif

