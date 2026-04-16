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

#ifndef STM8L_HW_H_
#define STM8L_HW_H_

//------------------------------------------------------
// GPIO by port number (A=0, B=1, ..)
#define GPIO_A                  0
#define GPIO_B                  1
#define GPIO_C                  2
#define GPIO_D                  3
#define GPIO_E                  4
#define GPIO_F                  5

//------------------------------------------------------
// GPIOCFG macros
#define GPIO_ODR_SIZE           1
#define GPIO_IDR_SIZE           1
#define GPIO_DDR_SHIFT          0
#define GPIO_DDR_SIZE           1
#define GPIO_DDR_MASK           (GPIO_DDR_SIZE << GPIO_DDR_SHIFT)
#define GPIO_DDR_INP            (0 << GPIO_DDR_SHIFT)    // 0: Input mode (reset state)
#define GPIO_DDR_OUT            (1 << GPIO_DDR_SHIFT)    // 1: Output mode
#define GPIO_CR1_SHIFT          1
#define GPIO_CR1_SIZE           1
#define GPIO_CR1_MASK           (GPIO_CR1_SIZE << GPIO_CR1_SHIFT)
#define GPIO_CR1_INP_FLOAT      (0 << GPIO_CR1_SHIFT)   // 0: Floating input (reset state)
#define GPIO_CR1_INP_PULLUP     (1 << GPIO_CR1_SHIFT)   // 1: Input with pull-up
#define GPIO_CR1_OUT_DRAIN      (0 << GPIO_CR1_SHIFT)   // 0: Pseudo open drain
#define GPIO_CR1_OUT_PUSHPULL   (1 << GPIO_CR1_SHIFT)   // 1: Push-pull
#define GPIO_CR2_SHIFT          2
#define GPIO_CR2_SIZE           1
#define GPIO_CR2_MASK           (GPIO_CR2_SIZE << GPIO_CR2_SHIFT)
#define GPIO_CR2_OUT_2MHZ       (0 << GPIO_CR2_SHIFT)   // 0: Output speed up to 2 MHz
#define GPIO_CR2_OUT_10MHZ      (1 << GPIO_CR2_SHIFT)   // 1: Output speed up to 10 MHz
#define GPIO_CR2_INP_EXTINTDIS  (0 << GPIO_CR2_SHIFT)   // 0: External input interrupt disabled (reset state)
#define GPIO_CR2_INP_EXTINTEN   (1 << GPIO_CR2_SHIFT)   // 1: External input interrupt enabled

//------------------------------------------------------
void hw_cfg_pin(uint8_t port, uint8_t pin, uint8_t gpiocfg);
void hw_set_pin(uint8_t port, uint8_t pin, uint8_t state);
uint8_t hw_get_pin(uint8_t port, uint8_t pin);

#endif // STM8L_HW_H_