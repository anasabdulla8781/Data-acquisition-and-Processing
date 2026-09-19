/*
 * STM32_DBGMCU.c
 *
 *  Created on: Nov 30, 2025
 *      Author: ANAS
 */


#include "STM32_DBGMCU.h"



void init_dbgmcu (void)
{
	dbgmcu_ptr->APB1_FZ |= 1<<12;				/// IWDG is in freeze once we do debugging (breakpoint)
}
