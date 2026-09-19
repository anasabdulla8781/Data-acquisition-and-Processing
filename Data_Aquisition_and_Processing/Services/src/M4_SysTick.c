/*
 * M4_SysTick.c
 *
 *  Created on: Nov 23, 2025
 *      Author: ANAS
 */


#include <M4_SysTick.h>



void systick_init (void)
{
	systick_ptr->CSR |= (1 << 2);					/// Enabling the internal clock source for this application
	systick_ptr->CSR |= (1 << 1);					/// Interrupt enable in systick module
	systick_ptr->RVR = 15999;						/// Setting the reload register according to 16Mhz , so that it will get completed in one mill second
	systick_ptr->CVR = 0;							/// Resetting the counter . doing this since we gave a new reload value . so let it affect in the timer
	systick_ptr->CSR |= (1 << 0);					/// Enabling the counting
}
