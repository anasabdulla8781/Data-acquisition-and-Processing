/*
 * STM32_SYSCFG.c
 *
 *  Created on: May 7, 2026
 *      Author: ANAS
 */


#include "STM32_SYSCFG.h"

/// Enable the interrupts

void exti_interrupt_enable(uint8_t port , uint8_t pin)
{
	if (pin >= 0 && pin < 4)
	{
		syscfg_ptr->EXTICR1 &= ~ ((0x0F)<<(pin*4));
		syscfg_ptr->EXTICR1 |= (port << (pin*4));
	}
	else if (pin >3 && pin < 8)
	{
		syscfg_ptr->EXTICR2 &= ~ ((0x0F)<<((pin-4)*4));
		syscfg_ptr->EXTICR2 |= (port << ((pin-4)*4));
	}
	else if (pin >7 && pin < 12)
	{
		syscfg_ptr->EXTICR3 &= ~ ((0x0F)<<((pin-8)*4));
		syscfg_ptr->EXTICR3 |= (port << ((pin-8)*4));
	}
	else if (pin >11 && pin < 16)
	{
		syscfg_ptr->EXTICR4 &= ~ ((0x0F)<<((pin-12)*4));
		syscfg_ptr->EXTICR4 |= (port << ((pin-12)*4));
	}
	else
	{
		/// Do nothing .. Invalid condition
	}
}
