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

#ifndef LORA_H_
#define LORA_H_

//--------------------------------------------
float lora_get_packet_length_in_symbols(lora_sf_t sf, lora_cr_t cr, uint8_t pl, uint8_t prl, bool crc, bool ih, bool ldro);

#endif /* LORA_H_ */