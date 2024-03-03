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

#include <stdbool.h>
#include "platform.h"
#include "lora-defs.h"

//--------------------------------------------
float lora_get_packet_length_in_symbols(lora_sf_t sf, lora_cr_t cr, uint8_t pl, uint8_t prl, bool crc, bool ih, bool ldro)
{
	int32_t num;
	uint32_t den;
	uint32_t div_res;
	uint32_t pl_in_symb;
	float pkt_in_symb;

	// numerator
	num = (8 * pl) + (crc ? 16 : 0) - (4 * sf) + (ih ? 0 : 16) + ((sf == 5 || sf == 6) ? 0 : 8);
	num = num > 0 ? num : 0;
	// denominator
	den = 4 * (sf - (ldro ? 2 : 0));
	// division result
	div_res = num / den + 1;
	// payload in symbols
	pl_in_symb = 8 + div_res * (cr + 4);
	// packet in symbols
	pkt_in_symb = prl + ((sf == 5 || sf == 6) ? 6.25 : 4.25) + pl_in_symb;
	return pkt_in_symb;
}
