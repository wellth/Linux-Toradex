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
#ifdef CONFIG_LCD
#define HAVE_DISPLAY
#include <lcd.h>
#endif
#ifdef CONFIG_CFB_CONSOLE
#include <video.h>
#define HAVE_DISPLAY
#endif
#include <chromeos/common.h>
#include <chromeos/crossystem_data.h>
#include <chromeos/fdt_decode.h>
#include <lzma/LzmaTypes.h>
#include <lzma/LzmaDec.h>
#include <lzma/LzmaTools.h>

#define PRINT_MAX_ROW	20
#define PRINT_MAX_COL	80

DECLARE_GLOBAL_DATA_PTR;

struct display_callbacks {
	int (*dc_get_pixel_width) (void);
	int (*dc_get_pixel_height) (void);
	int (*dc_get_screen_columns) (void);
	int (*dc_get_screen_rows) (void);
	void (*dc_position_cursor) (unsigned col, unsigned row);
	void (*dc_puts) (const char *s);
	int (*dc_display_bitmap) (ulong, int, int);
	int (*dc_display_clear) (void);
};

#ifdef HAVE_DISPLAY
static struct display_callbacks display_callbacks_ = {
#ifdef CONFIG_LCD
	.dc_get_pixel_width = lcd_get_pixel_width,
	.dc_get_pixel_height = lcd_get_pixel_height,
	.dc_get_screen_columns = lcd_get_screen_columns,
	.dc_get_screen_rows = lcd_get_screen_rows,
	.dc_position_cursor = lcd_position_cursor,
	.dc_puts = lcd_puts,
	.dc_display_bitmap = lcd_display_bitmap,
	.dc_display_clear = lcd_clear,

#elif defined(CONFIG_VIDEO)
	.dc_get_pixel_width = video_get_pixel_width,
	.dc_get_pixel_height = video_get_pixel_height,
	.dc_get_screen_columns = video_get_screen_columns,
	.dc_get_screen_rows = video_get_screen_rows,
	.dc_position_cursor = video_position_cursor,
	.dc_puts = video_puts,
	.dc_display_bitmap = video_display_bitmap,
	.dc_display_clear = video_clear
#endif
};
#endif

VbError_t VbExDisplayInit(uint32_t *width, uint32_t *height)
{
#ifdef HAVE_DISPLAY
	*width = display_callbacks_.dc_get_pixel_width();
	*height = display_callbacks_.dc_get_pixel_height();
#else
	*width = 640;
	*height = 480;
#endif
	return VBERROR_SUCCESS;
}

VbError_t VbExDisplayBacklight(uint8_t enable)
{
	/* TODO(waihong@chromium.org) Implement it later */
	return VBERROR_SUCCESS;
}

#ifdef HAVE_DISPLAY
/* Print the message on the center of LCD. */
static void print_on_center(const char *message)
{
	int i, room_before_text;
	int screen_size = display_callbacks_.dc_get_screen_columns() *
		display_callbacks_.dc_get_screen_rows();
	int text_length = strlen(message);

	room_before_text = (screen_size - text_length) / 2;

	display_callbacks_.dc_position_cursor(0, 0);

	for (i = 0; i < room_before_text; i++)
		display_callbacks_.dc_puts(".");

	display_callbacks_.dc_puts(message);

	for (i = i + text_length; i < screen_size; i++)
		display_callbacks_.dc_puts(".");
}
#endif

VbError_t VbExDisplayScreen(uint32_t screen_type)
{
#ifdef HAVE_DISPLAY
	/*
	 * Show the debug messages for development. It is a backup method
	 * when GBB does not contain a full set of bitmaps.
	 */
	switch (screen_type) {
		case VB_SCREEN_BLANK:
			/* clear the screen */
			display_clear();
			break;
		case VB_SCREEN_DEVELOPER_WARNING:
			print_on_center("developer mode warning");
			break;
		case VB_SCREEN_DEVELOPER_EGG:
			print_on_center("easter egg");
			break;
		case VB_SCREEN_RECOVERY_REMOVE:
			print_on_center("remove inserted devices");
			break;
		case VB_SCREEN_RECOVERY_INSERT:
			print_on_center("insert recovery image");
			break;
		case VB_SCREEN_RECOVERY_NO_GOOD:
			print_on_center("insert image invalid");
			break;
		default:
			VBDEBUG("Not a valid screen type: %08x.\n",
					screen_type);
			return 1;
	}
#endif
	return VBERROR_SUCCESS;
}

VbError_t VbExDecompress(void *inbuf, uint32_t in_size,
                         uint32_t compression_type,
                         void *outbuf, uint32_t *out_size)
{
	SizeT input_size = in_size;
	SizeT output_size = *out_size;
	int ret;

	switch (compression_type) {
	case COMPRESS_NONE:
		memcpy(outbuf, inbuf, in_size);
                *out_size = in_size;
		return VBERROR_SUCCESS;

	case COMPRESS_LZMA1:
		ret = lzmaBuffToBuffDecompress(outbuf, &output_size,
					       inbuf, input_size);
		if (ret != SZ_OK) {
			return ret;
		}
                *out_size = output_size;
		return VBERROR_SUCCESS;
	}

	VBDEBUG("Unsupported compression format: %08x\n", compression_type);
	return VBERROR_INVALID_PARAMETER;
}


VbError_t VbExDisplayImage(uint32_t x, uint32_t y,
                           void *buffer, uint32_t buffersize)
{
#ifdef HAVE_DISPLAY
	int ret;

	ret = display_callbacks_.dc_display_bitmap((ulong)buffer, x, y);
	if (ret) {
		VBDEBUG("LCD display error.\n");
		return VBERROR_UNKNOWN;
	}
#endif
	return VBERROR_SUCCESS;
}

VbError_t VbExDisplayDebugInfo(const char *info_str)
{
#ifdef HAVE_DISPLAY
	crossystem_data_t *cdata;
	size_t size;

	display_callbacks_.dc_position_cursor(0, 0);
	display_callbacks_.dc_puts(info_str);


	cdata = fdt_decode_chromeos_alloc_region(gd->blob, "cros-system-data",
						 &size);
	if (!cdata) {
		VBDEBUG("cros-system-data missing "
				"from fdt, or malloc failed\n");
		return VBERROR_UNKNOWN;
	}

	display_callbacks_.dc_puts("read-only firmware id: ");
	display_callbacks_.dc_puts((char *)cdata->readonly_firmware_id);
	display_callbacks_.dc_puts("\nactive firmware id: ");
	display_callbacks_.dc_puts((char *)cdata->firmware_id);
	display_callbacks_.dc_puts("\n");
#endif
	return VBERROR_SUCCESS;
}

/* this function is not technically part of the vboot interface */
int display_clear(void)
{
#ifdef HAVE_DISPLAY
	return display_callbacks_.dc_display_clear();
#else
	return 0;
#endif
}
