/*
 * STM32_DBGMCU.h
 *
 *  Created on: Nov 30, 2025
 *      Author: ANAS
 */

#ifndef INC_STM32_DBGMCU_H_
#define INC_STM32_DBGMCU_H_

#include <stdint.h>
#include "STM32_RCC.h"

typedef struct
{
	volatile uint32_t IDCODE;
	volatile uint32_t CR;
	volatile uint32_t APB1_FZ;
	volatile uint32_t APB2_FZ;
}dgbmcu_structure;


/// Base address
#define DBGMCU_BASEADDRESS	0xE0042000


#define dbgmcu_ptr	((dgbmcu_structure*)DBGMCU_BASEADDRESS)


extern void init_dbgmcu (void);

#endif /* INC_STM32_DBGMCU_H_ */
