/*
 * STM32_SYSCFG.h
 *
 *  Created on: May 7, 2026
 *      Author: ANAS
 */

#ifndef INC_STM32_SYSCFG_H_
#define INC_STM32_SYSCFG_H_

#include <stdint.h>

/// Structure for memory map
typedef struct
{
	volatile uint32_t MEMRMP;
	volatile uint32_t PMC;
	volatile uint32_t EXTICR1;
	volatile uint32_t EXTICR2;
	volatile uint32_t EXTICR3;
	volatile uint32_t EXTICR4;
	volatile uint32_t CMPCR;
}syscfg_structure;


#define SYSCFG_BASEADDRESS	0x40013800

#define syscfg_ptr	((syscfg_structure*)SYSCFG_BASEADDRESS)


#define PORTA	0
#define PORTB	1
#define PORTC	2
#define PORTD	3
#define PORTE	4
#define PORTF	5
#define PORTG	6
#define PORTH	7


void exti_interrupt_enable(uint8_t port , uint8_t pin);

#endif /* INC_STM32_SYSCFG_H_ */


