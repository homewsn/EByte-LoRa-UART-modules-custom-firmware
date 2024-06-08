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
#include "asr6601-hw.h"

//--------------------------------------------
#define GPIO_GPSZ                                 0x400U
#define GPIO_REG(base, port)                     (*((volatile uint32_t *)((uint32_t)&base + port * GPIO_GPSZ)))
#define GPIO_SET_0(base, port, pin)               GPIO_REG(base, port) &= ~(1 << pin)
#define GPIO_SET_1(base, port, pin)               GPIO_REG(base, port) |= (1 << pin)
#define GPIO_SET_BIT(base, port, pin, bit)        GPIO_REG(base, port) = (GPIO_REG(base, port) & ~(1 << pin)) | (bit)
#define GPIO_GET_BIT(base, port, pin)           ((GPIO_REG(base, port) & (1 << pin)) >> pin)
#define GPIO_GET_VALUE(base, port, pin)          (GPIO_REG(base, port) & (1 << pin))
#define GPIO_SET_MASK(base, port, mask, value)    GPIO_REG(base, port) = (GPIO_REG(base, port) & ~(mask)) | (value)

//--------------------------------------------
// configure operation mode of GPIO pin
void hw_cfg_pin(uint8_t gpio_pin, uint16_t gpiocfg)
{
	uint8_t port = gpio_pin / 16;
	uint8_t pin = gpio_pin % 16;

	if ((uint16_t)(gpiocfg & GPIOCFG_MODE_MASK) == (uint16_t)GPIOCFG_MODE_ALT)
	{
		// Port multiplexing configured as digital function port
		uint32_t mask;
		uint32_t value;
		uint8_t func_num = (gpiocfg & GPIO_AF_MASK);
		if (pin > 7)
		{
			uint32_t index = pin - 8;
			mask = (port == 3) ? (0x7 << (3 * index)) : (0xF << (4 * index));
			value = (port == 3) ? (func_num << (3 * index)) : (func_num << (4 * index));
			GPIO_SET_MASK(ASR_GPIOA->AFRH, port, mask, value);
		}
		else
		{
			mask = (0xF << (4 * pin));
			value = (func_num << (4 * pin));
			GPIO_SET_MASK(ASR_GPIOA->AFRL, port, mask, value);
		}
		return;
	}

	hw_cfg_pin(gpio_pin, GPIOCFG_MODE_ALT | GPIO_AF_NONE);

	if ((uint16_t)(gpiocfg & GPIOCFG_MODE_MASK) == (uint16_t)GPIOCFG_MODE_ANA)
	{
		// Port multiplexing configured as an analog port
		GPIO_SET_1(ASR_GPIOA->OER, port, pin);
		GPIO_SET_0(ASR_GPIOA->IER, port, pin);
		GPIO_SET_0(ASR_GPIOA->PER, port, pin);
		return;
	}

	// Port multiplexing configured as digital general purpose port
	switch ((uint16_t)(gpiocfg & GPIOCFG_GP_MODE_MASK))
	{
	case GPIOCFG_GP_MODE_INPUT_FLOATING:
		GPIO_SET_1(ASR_GPIOA->OER, port, pin);
		GPIO_SET_1(ASR_GPIOA->IER, port, pin);
		GPIO_SET_0(ASR_GPIOA->PER, port, pin);
		break;
	case GPIOCFG_GP_MODE_INPUT_PULL_UP:
		GPIO_SET_1(ASR_GPIOA->OER, port, pin);
		GPIO_SET_1(ASR_GPIOA->IER, port, pin);
		GPIO_SET_1(ASR_GPIOA->PER, port, pin);
		GPIO_SET_1(ASR_GPIOA->PSR, port, pin);
		break;
	case GPIOCFG_GP_MODE_INPUT_PULL_DOWN:
		GPIO_SET_1(ASR_GPIOA->OER, port, pin);
		GPIO_SET_1(ASR_GPIOA->IER, port, pin);
		GPIO_SET_1(ASR_GPIOA->PER, port, pin);
		GPIO_SET_0(ASR_GPIOA->PSR, port, pin);
		break;
	case GPIOCFG_GP_MODE_OUTPUT_PP_HIGH:
		GPIO_SET_0(ASR_GPIOA->OER, port, pin);
		GPIO_SET_0(ASR_GPIOA->IER, port, pin);
		GPIO_SET_1(ASR_GPIOA->ODR, port, pin);
		GPIO_SET_0(ASR_GPIOA->OTYPER, port, pin);
		break;
	case GPIOCFG_GP_MODE_OUTPUT_PP_LOW:
		GPIO_SET_0(ASR_GPIOA->OER, port, pin);
		GPIO_SET_0(ASR_GPIOA->IER, port, pin);
		GPIO_SET_0(ASR_GPIOA->ODR, port, pin);
		GPIO_SET_0(ASR_GPIOA->OTYPER, port, pin);
		break;
	case GPIOCFG_GP_MODE_OUTPUT_OD_HIZ:
		if (port == 3 && pin > 7)
		{
			GPIO_SET_1(ASR_GPIOA->OER, port, pin);
			GPIO_SET_0(ASR_GPIOA->IER, port, pin);
			GPIO_SET_0(ASR_GPIOA->ODR, port, pin);
			GPIO_SET_1(ASR_GPIOA->PSR, port, pin);
		}
		else
		{
			GPIO_SET_0(ASR_GPIOA->OER, port, pin);
			GPIO_SET_0(ASR_GPIOA->IER, port, pin);
			GPIO_SET_1(ASR_GPIOA->ODR, port, pin);
			GPIO_SET_1(ASR_GPIOA->OTYPER, port, pin);
		}
        break;
	case GPIOCFG_GP_MODE_OUTPUT_OD_LOW:
		if (port == 3 && pin > 7)
		{
			GPIO_SET_0(ASR_GPIOA->OER, port, pin);
			GPIO_SET_0(ASR_GPIOA->IER, port, pin);
			GPIO_SET_0(ASR_GPIOA->ODR, port, pin);
			GPIO_SET_1(ASR_GPIOA->PSR, port, pin);
		}
		else
		{
			GPIO_SET_0(ASR_GPIOA->OER, port, pin);
			GPIO_SET_0(ASR_GPIOA->IER, port, pin);
			GPIO_SET_0(ASR_GPIOA->ODR, port, pin);
			GPIO_SET_1(ASR_GPIOA->OTYPER, port, pin);
		}
        break;
	}

	if ((uint16_t)(gpiocfg & GPIOCFG_DRIVE_MASK) == (uint16_t)GPIOCFG_DRIVE_4MA)
	{
		GPIO_SET_0(ASR_GPIOA->DSR, port, pin);
	}
	else
	{
		GPIO_SET_1(ASR_GPIOA->DSR, port, pin);
	}
}

//--------------------------------------------
// set state of GPIO output pin
void hw_set_pin(uint8_t gpio_pin, uint8_t state)
{
	uint8_t port = gpio_pin / 16;
	uint8_t pin = gpio_pin % 16;

	if (port == 3 && pin > 7)
	{
		if ((GPIO_GET_VALUE(ASR_GPIOA->IER, port, pin) == 0) &&
			(GPIO_GET_VALUE(ASR_GPIOA->ODR, port, pin) == 0) &&
			(GPIO_GET_VALUE(ASR_GPIOA->PSR, port, pin) != 0))
		{
			if (GPIO_GET_VALUE(ASR_GPIOA->OER, port, pin) != 0)
			{
				if (state == 0)
				{
					GPIO_SET_0(ASR_GPIOA->OER, port, pin);
					GPIO_SET_0(ASR_GPIOA->IER, port, pin);
					GPIO_SET_0(ASR_GPIOA->ODR, port, pin);
					GPIO_SET_1(ASR_GPIOA->PSR, port, pin);
				}
			}
			else
			{
				if (state != 0)
				{
					GPIO_SET_1(ASR_GPIOA->OER, port, pin);
					GPIO_SET_0(ASR_GPIOA->IER, port, pin);
					GPIO_SET_0(ASR_GPIOA->ODR, port, pin);
					GPIO_SET_1(ASR_GPIOA->PSR, port, pin);
				}
			}
			return;
		}
	}

	if (state == 0)
	{
		GPIO_SET_1(ASR_GPIOA->BRR, port, pin);
	}
	else
	{
		GPIO_SET_1(ASR_GPIOA->BSR, port, pin);
	}
}

//--------------------------------------------
// get state of GPIO input pin
uint8_t hw_get_pin(uint8_t gpio_pin)
{
	uint8_t port = gpio_pin / 16;
	uint8_t pin = gpio_pin % 16;

	return GPIO_GET_BIT(ASR_GPIOA->IDR, port, pin);
}

//--------------------------------------------
// configure given line as external interrupt source
void hw_cfg_extirq(uint8_t gpio_pin, uint8_t irqcfg)
{
	uint8_t port = gpio_pin / 16;
	uint8_t pin = gpio_pin % 16;

	hw_clr_extirq(gpio_pin);
	GPIO_SET_MASK(ASR_GPIOA->ICR, port, 0x3 << 2 * pin, irqcfg << 2 * pin);
}

//--------------------------------------------
// clear external interrupt source flag
void hw_clr_extirq(uint8_t gpio_pin)
{
	uint8_t port = gpio_pin / 16;
	uint8_t pin = gpio_pin % 16;

	GPIO_REG(ASR_GPIOA->IFR, port) = (GPIO_REG(ASR_GPIOA->IFR, port) & 0x3 << 2 * pin);
}
