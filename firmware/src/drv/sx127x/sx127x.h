/*
* Copyright (c) 2026 Vladimir Alemasov
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

#ifndef SX127X_H_
#define SX127X_H_

//--------------------------------------------
uint32_t sx127x_convert_freq_in_hz_to_pll_step(uint32_t freq_in_hz);
void sx127x_write_buf(uint8_t reg_addr, uint8_t *data, uint8_t data_length);
void sx127x_read_buf(uint8_t reg_addr, uint8_t *data, uint8_t data_length);
void sx127x_write_register(uint8_t reg_addr, uint8_t data);
uint8_t sx127x_read_register(uint8_t reg_addr);
void sx127x_set_device_mode(uint8_t device_mode);
void sx127x_set_lora_sync_word(uint8_t sync_word);
void sx127x_set_frequency(uint32_t freq_in_hz);
void sx127x_set_spreading_factor(uint8_t sf);
void sx127x_set_detection_optimize(uint8_t dov);
void sx127x_set_detection_threshold(uint8_t dt);
void sx127x_set_preamble_length(uint16_t len);

#endif // SX127X_H_
