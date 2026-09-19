/*
 * M4_SysTick.h
 *
 *  Created on: Nov 23, 2025
 *      Author: ANAS
 */

#ifndef INC_M4_SYSTICK_H_
#define INC_M4_SYSTICK_H_

#include <stdint.h>
/// Structure
typedef struct
{
	volatile uint32_t CSR;
	volatile uint32_t RVR;
	volatile uint32_t CVR;
	volatile uint32_t CALIB;
}systick_structure;


// Base address

#define SYSTICK_BASEADDRESS		0xE000E010

/// Pointer to the structure
#define systick_ptr		((systick_structure*)SYSTICK_BASEADDRESS)


void systick_init (void);

#endif /* INC_M4_SYSTICK_H_ */
