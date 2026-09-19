/*
 * STM32_EXTI.c
 *
 *  Created on: May 8, 2026
 *      Author: ANAS
 */


#include "STM32_EXTI.h"

void setup_exti(uint8_t pin)
{
	/// Enable the interrupt in the pin we gave
	exti_ptr->IMR |= (1<<pin);
	/// Rising edge is disabled for the pin
	exti_ptr->RTSR &= ~(0x01 << pin);
	/// Falling edge is enabled for the pin
	exti_ptr->FTSR |= (1<<pin);
}
