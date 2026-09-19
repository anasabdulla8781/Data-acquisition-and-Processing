/*
 * STM32_IWDG.c
 *
 *  Created on: Nov 26, 2025
 *      Author: uie83604
 *
 *      /// Impliment the current watchdog timer count
 */

#include <STM32_IWDG.h>


void independent_watchdog_init(volatile Watchdog_Module_Config config)
{
	volatile iwdg_structure* config_ptr = config.module_pointer;
	/// Step 1 - Enable the LSI for the watchdog module
	LSI_clock_enable();
	/// Step 2 - Enable the debug MCU module ( Ohterwise the counter will be decrementing even if we stop the controller using breakpoint )
	init_dbgmcu();

	/// Step 3 - Key for the prescalar and reload access
	config_ptr->KR = PRESCALAR_AND_RELOAD_ACCESS;						// Getting the access to prescalar and reload register
	config_ptr->PR = config.prescalar;   								// Setting the prescalar to 256

	/// Step 4 - Finding the current watchdog setting time
	IWDG_set_reload_Register(config);
						// Setting the timing for 1 second ( reset time )
//	while(iwdg_ptr->SR != 0);							// Waiting till the bit fillings are completed in the PR and RLR
	config_ptr->KR = REFRESH_WATCHDOG;						// Before starting , lets reset the reload register
	config_ptr->KR = IWDG_START;							// Starting the watchdog , counter ( RLR ) decrement is started now

}

void IWDG_set_reload_Register(volatile Watchdog_Module_Config config)
{
	uint16_t reload_count;
	uint16_t prescalar_devisor;

	switch (config.prescalar)
	{
	case 0 :
		prescalar_devisor = 4;
		break;
	case 1 :
		prescalar_devisor = 8;
		break;
	case 2 :
		prescalar_devisor = 16;
		break;
	case 6 :
		prescalar_devisor = 256;
		break;
	default :
		prescalar_devisor = 4;
		break;
	}

	reload_count = ((config.watchdog_set_time)*LSI_CLOCK_FREQUENCY / (prescalar_devisor)) - 1;
	config.module_pointer->RLR = reload_count;
}

void feed_watchdog(void)
{
	iwdg_ptr->KR = REFRESH_WATCHDOG;
}

void error_code(void)
{
//	while (1);
}
