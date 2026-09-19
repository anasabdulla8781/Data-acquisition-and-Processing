/*
 * M4_FPU.c
 *
 *  Created on: Dec 9, 2025
 *      Author: ANAS
 */



#include "M4.h"


void FPU_Enable(void)
{
	scb_ptr->CPACR |= (0xF << 20);   // enable full access to CP10 and CP11 (FPU)
}

void software_reset(void)
{
	/// Step 1 - Read the aircr
	uint32_t aircr = *application_interrupt_reset_control_Register;
	/// Step 1 - Clear only the vector key and then set the vector key for reset
	aircr &= ~(0xFFFF<<16);
	aircr |= (0x5FA << 16);
	/// Step 2 - Set reset bit
	aircr |= (1 << 2);
	/// Step 3 - Write the new contents into the register
	*application_interrupt_reset_control_Register = aircr;
    /// Wait for the reset
    while (1)
    {
    }
}

