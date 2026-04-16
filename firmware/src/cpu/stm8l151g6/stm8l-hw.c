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

#include "cpu.h"
#include "stm8l-hw.h"

//--------------------------------------------
typedef struct
{
	volatile uint8_t ODR;
	volatile uint8_t IDR;
	volatile uint8_t DDR;
	volatile uint8_t CR1;
	volatile uint8_t CR2;
} GPIO_TypeDef;

//--------------------------------------------
#define GPIOx(no)             ((GPIO_TypeDef*)((volatile uint8_t *)(0x5000 + no * 5)))

//--------------------------------------------
// configure operation mode of GPIO pin
void hw_cfg_pin(uint8_t port, uint8_t pin, uint8_t gpiocfg)
{
	GPIO_TypeDef *GPIOx = GPIOx(port);

	GPIOx->DDR = (GPIOx->DDR & ~(GPIO_DDR_SIZE << pin)) | (((gpiocfg >> GPIO_DDR_SHIFT) & GPIO_DDR_SIZE) << pin);
	GPIOx->CR1 = (GPIOx->CR1 & ~(GPIO_CR1_SIZE << pin)) | (((gpiocfg >> GPIO_CR1_SHIFT) & GPIO_CR1_SIZE) << pin);
	GPIOx->CR2 = (GPIOx->CR2 & ~(GPIO_CR2_SIZE << pin)) | (((gpiocfg >> GPIO_CR2_SHIFT) & GPIO_CR2_SIZE) << pin);
}

//--------------------------------------------
// set state of GPIO output pin
void hw_set_pin(uint8_t port, uint8_t pin, uint8_t state)
{
	GPIO_TypeDef *GPIOx = GPIOx(port);

	GPIOx->ODR = (GPIOx->ODR & ~(GPIO_ODR_SIZE << pin)) | ((state & 1) << pin);
}

//--------------------------------------------
// get state of GPIO input pin
uint8_t hw_get_pin(uint8_t port, uint8_t pin)
{
	GPIO_TypeDef *GPIOx = GPIOx(port);

	return (GPIOx->IDR & (GPIO_IDR_SIZE << pin)) >> pin;
}
