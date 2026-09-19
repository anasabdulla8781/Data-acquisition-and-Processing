/*
 * STM32_EXTI.h
 *
 *  Created on: May 8, 2026
 *      Author: ANAS
 */

#ifndef INC_STM32_EXTI_H_
#define INC_STM32_EXTI_H_

#include "stdint.h"

typedef struct
{
	volatile uint32_t IMR;			/// Interrupt Mask Register			Offset : 0x00
	volatile uint32_t EMR;			/// Event Mask Register				Offset : 0x04
	volatile uint32_t RTSR;			/// Rising Trigger Selection Register	Offset : 0x08
	volatile uint32_t FTSR;			/// Falling Trigger Selection Register	Offset : 0x0C
	volatile uint32_t SWIER;		/// Software Interrupt Event Register	Offset : 0x10
	volatile uint32_t PR;			/// Pending Register					Offset : 0x14
}exti_structure;

/// Base address for EXTI peripheral
#define EXTI_BASEADDRESS	0x40013C00

/// Pointer to EXTI peripheral
#define exti_ptr	((exti_structure*)EXTI_BASEADDRESS)

void setup_exti(uint8_t pin);

#endif /* INC_STM32_EXTI_H_ */
