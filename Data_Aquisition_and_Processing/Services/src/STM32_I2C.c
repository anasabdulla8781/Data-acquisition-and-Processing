/*
 * STM32_I2C.c
 *
 *  Created on: Feb 21, 2026
 *      Author: ANAS
 */


#include <STM32_I2C.h>



// Pin configurations for the I2C
// Step 1 - Configure the clock for I2C ( Enable the bit21 in APB1enr register )
// Step 2 - Configure the Pin pb6 and pb7 in alternate function mode (Set Mode register bit [12:13] and [14:15] to 0x10 )
// Step 3 - Configure the AF4 Mode in AFRL register for both  SCL and SDA ( Set [24:27] and [28:31] t0 0x0100 )
// Step 4 - Setup the output type to Open drain


// I2C Peripheral configurations
// Step 1 - Set the software reset bit in init ( This will make sure the Peripheral get reset , eliminate unnecessory i2c errors - Bit 15 in the CR1) . And then clear it to avoid further resets in the peripheral
// Step 2 - Set the mode in standard mode for the moement ( 100khz is enough - Bit 15 in ccr set to 0 )
// Step 3 - Set the clock control register in CCR [0-11] to 80 since we are trying for the standard speed with 16mhz clock speed
// Step 4 - Configure the maximum rise time in trise register ( For standard mode , its 17 ( 16MHZ clock ) )
// Step 5 - Set the acknolodge bit in CR1- Set 1 ( CR1 - BIT 10 - So ackoledgement is enabled in runtime )
// Step 6 - Set the clock streching to enabled ( CR1 - Bit 7 to 0 )
// Step 7 - Set the error interrupt enable in CR2 - So we can handle all sort of the errors in handler
// Step 8 - Set the frequency to 16MHZ since thats the APB frequency in the freq bits in CR2
// Step 9 - Enable DMA for so transission and reception can happen torhough that  Bit 11 in CR2 ( DMA Need to configured seperately )
// Step 10 - Set ITBUFEN to 0 since we dont need to handle the data through Interrpts . Handling throguh DMA
// Step 11 - Set the ITEVTEN . Event interrupt enable in CR2 . This will make sure the interrupts are triggering for events
// Step 11 - Set the PE bit to 1 ( Bit 0 - Peripheral enable bit in CR1 , This will make the peripheral work .. and hence the DMA should be configured before this)


// DMA Configurations - Will do it later

void i2c_init (const i2c_module_configuration* config , uint8_t i2c_module_count)
{
	for (uint8_t iter = 0 ; iter <i2c_module_count ; iter++)
	{
		i2c_clock_enable(config[iter].module_number);
		i2c_reset_peripheral(config[iter].module_pointer);
		i2c_set_mode(&config[iter]);
		i2c_set_clock_control_register(&config[iter]);
		i2c_set_risetime(&config[iter]);
		i2c_set_acknoledgement(&config[iter]);
		i2c_set_clockstrech(&config[iter]);
		i2c_set_error_interrupt_enable(&config[iter]);
	}
}


void i2c_set_error_interrupt_enable (const i2c_module_configuration* config)
{
	i2c_structure* module_pointer = config->module_pointer;

	module_pointer->CR2 &= ~(1<<8);											// Clear the current configuration of the error interrupt enable bit
	module_pointer->CR2 |= ((config->error_interrupt_enable_disable) << 8);	// Set the error interrupt enable bit
}

void i2c_set_clockstrech (const i2c_module_configuration* config)
{
	i2c_structure* module_pointer = config->module_pointer;

	module_pointer->CR1 &= ~(1<<7);											// Clear the current configuration in clock strech bit
	module_pointer->CR1 |= ((config->clock_strech_enable_disable) << 7);	// Set or clear the Clock strech bit
}

void i2c_set_acknoledgement (const i2c_module_configuration* config)
{
	i2c_structure* module_pointer = config->module_pointer;

	module_pointer->CR1 &= ~(1<<10);								// Clear the current selection in the ack bit
	module_pointer->CR1 |= ((config->ack_enable_disable) << 10);	// Set or clear the ack bit
}


void i2c_set_risetime (const i2c_module_configuration* config)
{
	i2c_structure* module_pointer = config->module_pointer;

	module_pointer->TRISE &= ~(0x3F);					// Cleared the current configuration for rise time
	module_pointer->TRISE |= config->max_rise_time;		// Max rise time
}

void i2c_set_clock_control_register (const i2c_module_configuration* config)
{
	uint16_t clock_control_register = config->ccr;
	i2c_structure* module_pointer = config->module_pointer;


	clock_control_register &= (0x0FFF);					// Clear the msb 4 bits of the CCR value mentioned
	module_pointer->CCR &= ~(0x0FFF);					// Clear the first 11 bits in the CCR register
	module_pointer->CCR |= clock_control_register;		// Set the clock control register 12 bits


}

void i2c_set_mode (const i2c_module_configuration* config)
{
	uint8_t i2c_mode = config->i2c_speed_mode;
	i2c_structure* module_pointer = config->module_pointer;
	if ((i2c_mode == STANDARD_MODE) || (i2c_mode == FAST_MODE))
	{
		module_pointer->CCR &= ~(1U << 15);				// Clear the CCR bit for speed mode
		module_pointer->CCR |= (i2c_mode<<15);			// Set the mode mentioned
	}
	else
	{
		// Do nothing
	}
}

void i2c_reset_peripheral (i2c_structure* module_pointer)
{
	if ((module_pointer == i2c1_ptr) ||(module_pointer == i2c2_ptr) || (module_pointer == i2c3_ptr))
	{
		module_pointer->CR1 |= (1<<15);					// Set the reset bit . Will reset the peripheral
		module_pointer->CR1 &= ~(1<<15);				// Clear the reset bit . No more reset happens
	}
	else
	{
		// Do nothing
	}
}





























void i2c_config_init(i2c_structure* i2c_ptr)
{
	/// Step 1 - Reset I2C
	i2c_ptr->CR1 |= (1<<15);				/// Set the 15th bit to 1 to reset the I2C Configurations.
	i2c_ptr->CR1 &= ~ (1<<15);				/// Now cleared the bit for releasing i2c from reset state

	/// Step 2 - Set the frequency for APB (APB Frequency)
	i2c_ptr->CR2 &= ~ (0x3F); 				/// Clearing the first 6 bits to add frequecy here
	i2c_ptr->CR2 |= 16;						/// We are using HSI Without any prescalar . So 16MHZ frequency

	/// Step 3 - Set the clock speed of the I2C
	i2c_ptr->CCR &= ~(0xFFF);				/// Clear all the bits for CCR
	i2c_ptr->CCR |= 80;						/// Set the clock frequency to 80
	i2c_ptr->CCR &= ~(1 << 15);				/// Purposefully clearing the Fast mode

	/// Step 4 - Configure TRISE (Standard mode, 16 MHz)
	i2c_ptr->TRISE = 17;

	/// Step 5 - Turning ON i2c
	i2c_ptr->CR1 |= (1U << 0);

}
