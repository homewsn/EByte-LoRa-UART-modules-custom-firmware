/*
* Copyright (c) 2022 Vladimir Alemasov
* All rights reserved
*
* This program and the accompanying materials are distributed under 
* the terms of GNU General Public License version 2 
* as published by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*/

#ifndef LORA_DEFS_H_
#define LORA_DEFS_H_

//--------------------------------------------
typedef enum
{
	LORA_BW_7,
	LORA_BW_10,
	LORA_BW_15,
	LORA_BW_20,
	LORA_BW_31,
	LORA_BW_41,
	LORA_BW_62,
	LORA_BW_125,
	LORA_BW_250,
	LORA_BW_500
} lora_bw_t;

//--------------------------------------------
typedef enum
{
	LORA_SF5 = 5,
	LORA_SF6,
	LORA_SF7,
	LORA_SF8,
	LORA_SF9,
	LORA_SF10,
	LORA_SF11,
	LORA_SF12
} lora_sf_t;

//--------------------------------------------
typedef enum
{
	LORA_CR_4_5 = 1,
	LORA_CR_4_6,
	LORA_CR_4_7,
	LORA_CR_4_8
} lora_cr_t;

//--------------------------------------------
typedef enum
{
	LORA_PKT_EXPLICIT,
	LORA_PKT_IMPLICIT
} lora_pkt_len_modes_t;

//--------------------------------------------
typedef enum
{
	LORA_DIO_IRQ_OFF,
	LORA_DIO_IRQ_ON
} lora_dio_irq_modes_t;

//--------------------------------------------
typedef enum
{
	LORA_CRC_OFF,
	LORA_CRC_ON
} lora_crc_modes_t;

//--------------------------------------------
typedef enum
{
	LORA_IQ_STANDARD,
	LORA_IQ_INVERTED
} lora_iq_modes_t;

//--------------------------------------------
typedef enum
{
	LORA_STATUS_OK,
	LORA_STATUS_UNSUPPORTED_FEATURE,
	LORA_STATUS_UNKNOWN_VALUE,
	LORA_STATUS_ERROR,
} lora_status_t;

//--------------------------------------------
typedef enum
{
	LORA_IRQ_NONE,
	LORA_IRQ_RX_DONE,
	LORA_IRQ_TX_DONE,
	LORA_IRQ_CRC_ERROR,
	LORA_IRQ_CAD_DONE,
	LORA_IRQ_CAD_DETECTED,
	LORA_IRQ_TIMEOUT
} lora_irq_src_t;

//--------------------------------------------
typedef struct
{
	lora_pkt_len_modes_t pkt_len_mode;
	uint8_t pkt_len;
	lora_dio_irq_modes_t dio_irq_mode;
	lora_crc_modes_t crc_mode;
	lora_iq_modes_t iq_mode;
	uint16_t preamble_len;
	uint8_t sync_word;
} lora_modes_t;

//--------------------------------------------
typedef struct
{
	uint32_t freq_in_hz;
	lora_bw_t bw;
	lora_sf_t sf;
	lora_cr_t cr;
} lora_params_t;

//--------------------------------------------
#define LORA_SYNC_WORD_LORAWAN_PRIVATE   0x12
#define LORA_SYNC_WORD_LORAWAN_PUBLIC    0x34

#endif /* LORA_DEFS_H_ */