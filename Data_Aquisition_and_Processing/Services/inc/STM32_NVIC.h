/*
 * STM32_NVIC.h
 *
 *  Created on: Nov 10, 2025
 *      Author: ANAS
 */

#ifndef INC_STM32_NVIC_H_
#define INC_STM32_NVIC_H_

#include <stdint.h>


typedef struct
{
    volatile uint32_t ISER[8];     // 0x000 - Interrupt Set-Enable Registers (enable interrupts 0–239)
    uint32_t RESERVED0[24];        // Reserved space (to align next section)

    volatile uint32_t ICER[8];     // 0x080 - Interrupt Clear-Enable Registers (disable interrupts)
    uint32_t RESERVED1[24];        // Reserved space

    volatile uint32_t ISPR[8];     // 0x100 - Interrupt Set-Pending Registers (set pending flag)
    uint32_t RESERVED2[24];        // Reserved space

    volatile uint32_t ICPR[8];     // 0x180 - Interrupt Clear-Pending Registers (clear pending flag)
    uint32_t RESERVED3[24];        // Reserved space

    volatile uint32_t IABR[8];     // 0x200 - Interrupt Active Bit Registers (shows which interrupts are active)
    uint32_t RESERVED4[56];        // Reserved space

    volatile uint8_t  IPR[60];    // 0x300 - Interrupt Priority Registers (set priority 0–239)
} NVIC_structure;


/// NVIC base address
#define NVIC_BASEADDRESS	0xE000E100


/// Poniter to the NVIC Strucutre
#define nvic_ptr	((NVIC_structure*)NVIC_BASEADDRESS)

/// Macros
#define	ENABLE_TIMER2_INTERRUPT	1<<28
#define ENABLE_TIMER4_INTERRUPT	1<<30
#define ENABLE_EXTIO_INTERRUPT	1<<6
#define ENABLE_USART2_INTERRUPT 1<<6

/// Nested vector table

#define TIM2_ISR_NUMBER	28
#define TIM4_ISR_NUMBER	30
#define EXTIO_ISR_NUMBER 6


void nvic_init(uint8_t interrupt_number);



#endif /* INC_STM32_NVIC_H_ */
