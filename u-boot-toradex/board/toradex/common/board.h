/*
 *  (C) Copyright 2012
 *  Toradex, Inc.
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

#ifndef _BOARD_H_
#define _BOARD_H_

void gpio_config_uart(const void *blob);
void gpio_early_init_uart(const void *blob);
void gpio_config_mmc(void);
int tegra_mmc_init(const void *blob);
void lcd_early_init(const void *blob);

/**
 * Perform the next stage of the LCD init if it is time to do so.
 *
 * LCD init can be time-consuming because of the number of delays we need
 * while waiting for the backlight power supply, etc. This function can
 * be called at various times during U-Boot operation to advance the
 * initialization of the LCD to the next stage if sufficient time has
 * passed since the last stage. It keeps track of what stage it is up to
 * and the time that it is permitted to move to the next stage.
 *
 * The final call should have wait=1 to complete the init.
 *
 * @param blob	fdt blob containing LCD information
 * @param wait	1 to wait until all init is complete, and then return
 *		0 to return immediately, potentially doing nothing if it is
 *		not yet time for the next init.
 */
int tegra_lcd_check_next_stage(const void *blob, int wait);

#endif	/* BOARD_H */
