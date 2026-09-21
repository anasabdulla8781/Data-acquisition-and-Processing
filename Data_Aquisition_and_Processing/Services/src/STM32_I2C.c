/*
 * STM32_I2C.c
 *
 *  Created on: Feb 21, 2026
 *      Author: ANAS
 */


#include <STM32_I2C.h>
#include <stddef.h>



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


/// **************************************** Module configurations - Start ************************************************


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
		i2c_set_frequency(&config[iter]);
		i2c_set_error_interrupt_enable(&config[iter]);
		i2c_set_event_interrupt_enable(&config[iter]);
		i2c_set_buffer_interrupt_enable(&config[iter]);
		i2c_dma_enable(&config[iter]);
		i2c_enable(&config[iter]);
	}
}

void i2c_enable(const i2c_module_configuration* config)
{
	i2c_structure* module_pointer = config->module_pointer;
	module_pointer->CR1 &= ~(1<<0);											// Clear the current I2C Enable selection
	module_pointer->CR1 |= ((config->i2c_enable_disable) << 0);				// Set the configured I2C Enable selection
}


void i2c_DMA_enable(const i2c_module_configuration* config)
{
	i2c_structure* module_pointer = config->module_pointer;
	module_pointer->CR2 &= ~(1<<11);											// Clear the current DMA selection
	module_pointer->CR2 |= ((config->dma_enable_disable) << 11);				// Set the configured DMA selection
}

void i2c_set_event_interrupt_enable(const i2c_module_configuration* config)
{
	i2c_structure* module_pointer = config->module_pointer;
	module_pointer->CR2 &= ~(1<<9);												// Clear the current event interrupt enable selection
	module_pointer->CR2 |= ((config->event_interrupt_enable_disable) << 9);		// Set the configured event interrupt enable selection bit
}


void i2c_set_buffer_interrupt_enable(const i2c_module_configuration* config)
{
	i2c_structure* module_pointer = config->module_pointer;
	module_pointer->CR2 &= ~(1<<10);											// Clear the current buffer interrupt enable selection
	module_pointer->CR2 |= ((config->buffer_interrupt_enable_disable) << 10);	// Set the configured buffer interrupt enable selection bit
}

void i2c_set_frequency(const i2c_module_configuration* config)
{
	i2c_structure* module_pointer = config->module_pointer;

	if ((config->peripheral_clock_frequency <=50) && (config->peripheral_clock_frequency >=1))
	{
		module_pointer->CR2 &= ~(0x3F);										// Clear the current peripheral frequency settings
		module_pointer->CR2 |= (config->peripheral_clock_frequency);		// Set the peripheral clock frequency
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

/// **************************************** Module configurations - End ************************************************


/// **************************************** Module Functionalities - Start ************************************************


// Run time configurations of the I2C Module - Not fixed . This will get updated based on the application request

static i2c_driver i2c_drivers_configured[] = {
		{
				.module_pointer = i2c1_ptr,
				.bus_state = I2C_BUS_IDLE,
				.driver_status = I2C_DRIVER_IDLE,
		},
};

static const uint8_t configured_i2c_devices = sizeof(i2c_drivers_configured)/sizeof(i2c_drivers_configured[0]);

void i2c_start(i2c_transaction transaction_structure)
{
	i2c_driver* driver_info = NULL;

	if (i2c_get_driver_info(transaction_structure, &driver_info) == E_OK)											//	Get the driver corresponding to the transaction mentioned
	{
		if (driver_info->driver_status == I2C_DRIVER_IDLE)
		{
			i2c_structure*module_ptr = driver_info->module_pointer;
			driver_info->active_transaction = transaction_structure;												// The driver is ready to take the transaction , so copied the contents to the driver structure
			driver_info->driver_status = I2C_DRIVER_BUSY;															// Set the driver to busy to avoid further transactions ( if  another start request comes for the same bus )
			module_ptr->CR1 |= (1<<8);																				// Set the start bit for the module we wanted to communicate . This will clear automatically by HW after setting the start condition
		}
	}

}


uint8_t i2c_get_driver_info(i2c_transaction transaction_structure , i2c_driver** driver_info)
{
	for (uint8_t iter = 0 ; iter<configured_i2c_devices; iter++)
	{
		if (transaction_structure.module_pointer == i2c_drivers_configured[iter].module_pointer)
		{
			*driver_info = &i2c_drivers_configured[iter];
			return E_OK;
		}
	}
	return E_NOT_OK;
}
