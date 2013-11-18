/*
 * NVIDIA Tegra2 I2C controller
 *
 * Copyright 2010-2011 NVIDIA Corporation
 *
 * This software may be used and distributed according to the
 * terms of the GNU Public License, Version 2, incorporated
 * herein by reference.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * Version 2 as published by the Free Software Foundation.
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

#ifndef _TEGRA_I2C_H_
#define _TEGRA_I2C_H_

#include <asm/types.h>

/* Convert the number of bytes to word. */
#define BYTES_TO_WORDS(size)		(((size) + 3) >> 2)

/* Convert i2c slave address to be put on bus  */
#define I2C_ADDR_ON_BUS(chip)		(chip << 1)

#ifndef CONFIG_OF_CONTROL
enum {
	I2CSPEED_KHZ = 100,		/* in KHz */
};
#endif

enum {
	I2C_TIMEOUT_USEC = 10000,	/* Wait time for completion */
	I2C_FIFO_DEPTH = 8,		/* I2C fifo depth */
};

enum i2c_transaction_flags {
	I2C_IS_WRITE = 0x1,		/* for I2C write operation */
	I2C_IS_10_BIT_ADDRESS = 0x2,	/* for 10-bit I2C slave address */
	I2C_USE_REPEATED_START = 0x4,	/* for repeat start */
	I2C_NO_ACK = 0x8,		/* for slave that won't generate ACK */
	I2C_SOFTWARE_CONTROLLER	= 0x10,	/* for I2C transfer using GPIO */
	I2C_NO_STOP = 0x20,
};

/* Contians the I2C transaction details */
struct i2c_trans_info {
	/* flags to indicate the transaction details */
	enum i2c_transaction_flags flags;
	u32 address;	/* I2C slave device address */
	u32 num_bytes;	/* number of bytes to be transferred */
	/* Send/receive buffer. For I2C send operation this buffer should be
	 * filled with the data to be sent to the slave device. For I2C receive
	 * operation this buffer is filled with the data received from the
	 * slave device. */
	u8 *buf;
	int is_10bit_address;
};

struct i2c_control {
	u32 tx_fifo;
	u32 rx_fifo;
	u32 packet_status;
	u32 fifo_control;
	u32 fifo_status;
	u32 int_mask;
	u32 int_status;
};

struct dvc_ctlr {
	u32 ctrl1;			/* 00: DVC_CTRL_REG1 */
	u32 ctrl2;			/* 04: DVC_CTRL_REG2 */
	u32 ctrl3;			/* 08: DVC_CTRL_REG3 */
	u32 status;			/* 0C: DVC_STATUS_REG */
	u32 ctrl;			/* 10: DVC_I2C_CTRL_REG */
	u32 addr_data;			/* 14: DVC_I2C_ADDR_DATA_REG */
	u32 reserved_0[2];		/* 18: */
	u32 req;			/* 20: DVC_REQ_REGISTER */
	u32 addr_data3;			/* 24: DVC_I2C_ADDR_DATA_REG_3 */
	u32 reserved_1[6];		/* 28: */
	u32 cnfg;			/* 40: DVC_I2C_CNFG */
	u32 cmd_addr0;			/* 44: DVC_I2C_CMD_ADDR0 */
	u32 cmd_addr1;			/* 48: DVC_I2C_CMD_ADDR1 */
	u32 cmd_data1;			/* 4C: DVC_I2C_CMD_DATA1 */
	u32 cmd_data2;			/* 50: DVC_I2C_CMD_DATA2 */
	u32 reserved_2[2];		/* 54: */
	u32 i2c_status;			/* 5C: DVC_I2C_STATUS */
	struct i2c_control control;	/* 60 ~ 78 */
};

struct i2c_ctlr {
	u32 cnfg;			/* 00: I2C_I2C_CNFG */
	u32 cmd_addr0;			/* 04: I2C_I2C_CMD_ADDR0 */
	u32 cmd_addr1;			/* 08: I2C_I2C_CMD_DATA1 */
	u32 cmd_data1;			/* 0C: I2C_I2C_CMD_DATA2 */
	u32 cmd_data2;			/* 10: DVC_I2C_CMD_DATA2 */
	u32 reserved_0[2];		/* 14: */
	u32 status;			/* 1C: I2C_I2C_STATUS */
	u32 sl_cnfg;			/* 20: I2C_I2C_SL_CNFG */
	u32 sl_rcvd;			/* 24: I2C_I2C_SL_RCVD */
	u32 sl_status;			/* 28: I2C_I2C_SL_STATUS */
	u32 sl_addr1;			/* 2C: I2C_I2C_SL_ADDR1 */
	u32 sl_addr2;			/* 30: I2C_I2C_SL_ADDR2 */
	u32 reserved_1[2];		/* 34: */
	u32 sl_delay_count;		/* 3C: I2C_I2C_SL_DELAY_COUNT */
	u32 reserved_2[4];		/* 40: */
	struct i2c_control control;	/* 50 ~ 68 */
};

/* bit fields definitions for IO Packet Header 1 format */
#define PKT_HDR1_TYPE_RANGE			2 : 0
#define PKT_HDR1_PROTOCOL_RANGE			7 : 4
#define PKT_HDR1_CTLR_ID_RANGE			15 : 12
#define PKT_HDR1_PKT_ID_RANGE			23 : 16
#define PKT_HDR1_HDRSZ_RANGE			29 : 28
#define PROTOCOL_TYPE_I2C			1

/* bit fields definitions for IO Packet Header 2 format */
#define PKT_HDR2_PAYLOAD_SIZE_RANGE		11 : 0

/* bit fields definitions for IO Packet Header 3 format */
#define PKT_HDR3_HS_MODE_RANGE			22 : 22
#define PKT_HDR3_CONT_ON_NACK_RANGE		21 : 21
#define PKT_HDR3_SEND_START_BYTE_RANGE		20 : 20
#define PKT_HDR3_READ_MODE_RANGE		19 : 19
#define PKT_HDR3_ADDR_MODE_RANGE		18 : 18
#define PKT_HDR3_IE_RANGE			17 : 17
#define PKT_HDR3_REPEAT_START_STOP_RANGE	16 : 16
#define PKT_HDR3_HS_MASTER_ADDR_RANGE		14 : 12
#define PKT_HDR3_SLAVE_ADDR_RANGE		9 : 0

#define DVC_CTRL_REG3_I2C_HW_SW_PROG_RANGE	26 : 26

/* pin_mux selections for I2Cs */
#define I2CP_SEL_RANGE				9 : 8
#define RM_SEL_RANGE				15 : 14
#define PTA_SEL_RANGE				23 : 22
#define DDC_SEL_RANGE				1 : 0
#define DTF_SEL_RANGE				31 : 30

/* I2C_CNFG */
#define I2C_CNFG_NEW_MASTER_FSM_RANGE		11 : 11
#define I2C_CNFG_PACKET_MODE_RANGE		10 : 10

/* I2C_SL_CNFG */
#define I2C_SL_CNFG_NEWSL_RANGE			2 : 2

/* I2C_FIFO_CONTROL */
#define I2C_RX_FIFO_FLUSH_RANGE			0 : 0
#define I2C_TX_FIFO_FLUSH_RANGE			1 : 1

/* I2C_FIFO_STATUS */
#define RX_FIFO_FULL_CNT_RANGE			3 : 0
#define TX_FIFO_EMPTY_CNT_RANGE			7 : 4

/* I2C_INTERRUPT_STATUS */
#define I2C_INT_PACKET_XFER_COMPLETE_RANGE	7 : 7
#define I2C_INT_NO_ACK_RANGE			3 : 3
#define I2C_INT_ARBITRATION_LOST_RANGE		2 : 2

/**
 * Low level, hopefully temporary, functions to write values to the
 * Tegra DVC I2C controller. These are used by T30 init, when running
 * on the AVP CPU, before the Cortex-A9s are up. It is not easy to
 * have the i2c infrastructure up that early, but we still want to put
 * this code in the driver
 */

/**
 * Write an address (with config) to the DVC I2C
 *
 * @param addr		Address to write
 * @param config	Config to write
 */
void tegra_i2c_ll_write_addr(uint addr, uint config);

/**
 * Write a data word (with config) to the DVC I2C
 *
 * @param data		Data to write
 * @param config	Config to write
 */
void tegra_i2c_ll_write_data(uint data, uint config);

#endif
