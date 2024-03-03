/*
* Copyright (c) 2020 Vladimir Alemasov
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

#ifndef HAL_IO_H_
#define HAL_IO_H_

//--------------------------------------------
void hal_io_init(void);
void hal_io_m0(uint8_t val);
void hal_io_m1(uint8_t val);
void hal_io_aux(uint8_t val);

#endif /* HAL_IO_H_ */