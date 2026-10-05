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


// Run time configurations of the I2C Module - Not fixed . This will get updated based on the application request

I2C_Runtime i2c_runtime_config[I2C_MAX_CONFIGURATION];


/// **************************************** Module configurations - Start ************************************************


void i2c_init (const I2C_Config* config , const uint8_t i2c_module_count)
{
	if ((config == NULL) || (i2c_module_count > I2C_MAX_CONFIGURATION))
	{
		return;
	}
	for (uint8_t iter = 0 ; iter <i2c_module_count ; iter++)
	{

		if (config[iter].module_pointer != NULL)
		{
			i2c_structure* module_pointer = config[iter].module_pointer;

			i2c_clock_enable(config[iter].module_number);
			i2c_reset_peripheral(module_pointer);
			i2c_set_mode(module_pointer , config[iter].i2c_mode);
			i2c_set_clock_control_register(module_pointer , config[iter].ccr);
			i2c_set_risetime(module_pointer , config[iter].max_rise_time);
			i2c_set_acknoledgement(module_pointer , config[iter].ack_enable_disable);
			i2c_set_clockstrech(module_pointer,config[iter].clock_strech_enable_disable);
			i2c_set_frequency(module_pointer,config[iter].peripheral_clock_frequency);
			i2c_set_error_interrupt_enable(module_pointer , config[iter].error_interrupt_enable_disable);
			i2c_set_event_interrupt_enable(module_pointer , config[iter].event_interrupt_enable_disable);
			i2c_set_buffer_interrupt_enable(module_pointer , config[iter].buffer_interrupt_enable_disable);
			i2c_copy_contents(&config[iter]);
			i2c_enable(module_pointer , config[iter].i2c_enable_disable);
		}
	}
}

void i2c_enable(i2c_structure* module_pointer, uint8_t i2c_enable_disable)
{
	if(i2c_enable_disable > I2C_ENABLE)
	{
		return ;
	}
	module_pointer->CR1 &= ~(1<<0);											// Clear the current I2C Enable selection
	module_pointer->CR1 |= (i2c_enable_disable << 0);						// Set the configured I2C Enable selection
}

void i2c_set_lastmode (i2c_structure* module_pointer, uint8_t set_last_enable_disable)
{
	if (set_last_enable_disable > LAST_TRANSFER_ENABLED)
	{
		return;
	}
	module_pointer->CR2 &= ~(1<<12);											// Clear the last mode
	module_pointer->CR2 |= (set_last_enable_disable << 12);						// Set the last mode
}

void i2c_set_event_interrupt_enable(i2c_structure* module_pointer, uint8_t event_interrupt_enable_disable)
{
	if (event_interrupt_enable_disable > EVENT_INTERRUPT_ENABLE)
	{
		return;
	}
	module_pointer->CR2 &= ~(1<<9);												// Clear the current event interrupt enable selection
	module_pointer->CR2 |= (event_interrupt_enable_disable << 9);		// Set the configured event interrupt enable selection bit
}


void i2c_set_buffer_interrupt_enable(i2c_structure* module_pointer, uint8_t buffer_interrupt_enable_disable)
{
	if (buffer_interrupt_enable_disable > BUFFER_INTERRUPT_ENABLE)
	{
		return;
	}
	module_pointer->CR2 &= ~(1<<10);											// Clear the current buffer interrupt enable selection
	module_pointer->CR2 |= (buffer_interrupt_enable_disable << 10);				// Set the configured buffer interrupt enable selection bit
}

void i2c_set_frequency(i2c_structure* module_pointer, uint8_t peripheral_clock_frequency)
{
	if ((peripheral_clock_frequency <=50) && (peripheral_clock_frequency >=1))
	{
		module_pointer->CR2 &= ~(0x3F);										// Clear the current peripheral frequency settings
		module_pointer->CR2 |= peripheral_clock_frequency;					// Set the peripheral clock frequency
	}
}

void i2c_set_error_interrupt_enable (i2c_structure* module_pointer, uint8_t error_interrupt_enable_disable)
{
	if (error_interrupt_enable_disable > ERROR_INTERRUPT_ENABLE)
	{
		return;
	}
	module_pointer->CR2 &= ~(1<<8);											// Clear the current configuration of the error interrupt enable bit
	module_pointer->CR2 |= (error_interrupt_enable_disable << 8);			// Set the error interrupt enable bit
}

void i2c_set_clockstrech (i2c_structure* module_pointer, uint8_t clock_strech_enable_disable)
{
	if (clock_strech_enable_disable > CLOCK_STRECH_DISABLE)
	{
		return;
	}
	module_pointer->CR1 &= ~(1<<7);											// Clear the current configuration in clock strech bit
	module_pointer->CR1 |= (clock_strech_enable_disable << 7);				// Set or clear the Clock strech bit
}

void i2c_set_acknoledgement (i2c_structure* module_pointer, uint8_t ack_enable_disable)
{
	if (ack_enable_disable > ACK_ENABLED)
	{
		return;
	}
	module_pointer->CR1 &= ~(1<<10);								// Clear the current selection in the ack bit
	module_pointer->CR1 |= (ack_enable_disable << 10);				// Set or clear the ack bit
}


void i2c_set_risetime (i2c_structure* module_pointer, uint8_t max_rise_time)
{
    if ((module_pointer == NULL) || (max_rise_time > 63U))
    {
        return;
    }
	module_pointer->TRISE &= ~(0x3F);					// Cleared the current configuration for rise time
	module_pointer->TRISE |= max_rise_time;				// Max rise time
}

void i2c_set_clock_control_register (i2c_structure* module_pointer , uint16_t clock_control_register)
{
	clock_control_register &= (0x0FFF);					// Clear the msb 4 bits of the CCR value mentioned
	module_pointer->CCR &= ~(0x0FFF);					// Clear the first 11 bits in the CCR register
	module_pointer->CCR |= clock_control_register;		// Set the clock control register 12 bits
}

void i2c_set_mode (i2c_structure* module_pointer , uint8_t mode)
{
	if (mode<= FAST_MODE)
	{
		module_pointer->CCR &= ~(1U << 15);				// Clear the CCR bit for speed mode
		module_pointer->CCR |= (mode<<15);				// Set the mode mentioned
	}
	else
	{
		// Do nothing
	}
}

void i2c_reset_peripheral (i2c_structure* module_pointer)
{
	if ((module_pointer == i2c1_ptr) || (module_pointer == i2c2_ptr) || (module_pointer == i2c3_ptr))
	{
		module_pointer->CR1 |= (1<<15);					// Set the reset bit . Will reset the peripheral
		module_pointer->CR1 &= ~(1<<15);				// Clear the reset bit . No more reset happens
	}
	else
	{
		// Do nothing
	}
}

void i2c_copy_contents (const I2C_Config* config)
{
    if ((config == NULL) || (config->module_pointer == NULL) || (config->module_number >= I2C_MAX_CONFIGURATION))
    {
        return;
    }
	uint8_t module = config->module_number;
	i2c_structure* module_pointer = config->module_pointer;

	i2c_runtime_config[module].bus_state = I2C_BUS_IDLE;
	i2c_runtime_config[module].dma_config = config->dma_config;
	i2c_runtime_config[module].driver_status = I2C_DRIVER_IDLE;
	i2c_runtime_config[module].module_pointer = module_pointer;
}


void i2c_dma_enable(i2c_structure* module_pointer)
{
	module_pointer->CR2 &= ~(1<<11);										// Clear the current DMA selection
	module_pointer->CR2 |= (1 << 11);										// Set the configured DMA selection
}

/// **************************************** Module configurations - End ************************************************


/// **************************************** Module Functionalities - Start ************************************************

uint8_t i2c_start(I2C_Transaction transaction_structure)
{
	I2C_Runtime* i2c_runtime = NULL;

	if (i2c_get_driver_runtime(transaction_structure, &i2c_runtime) == E_OK)											//	Get the driver corresponding to the transaction mentioned
	{
		if ((i2c_runtime->driver_status == I2C_DRIVER_IDLE) && (i2c_runtime->bus_state == I2C_BUS_IDLE) && (i2c_runtime->module_pointer != NULL))
		{
			i2c_structure*module_ptr = i2c_runtime->module_pointer;
			i2c_runtime->active_transaction = transaction_structure;												// The driver is ready to take the transaction , so copied the contents to the driver structure

			i2c_runtime->driver_status = I2C_DRIVER_BUSY;										// Set the driver to busy to avoid further transactions ( if  another start request comes for the same bus )
			i2c_runtime->bus_state = I2C_BUS_START;												// Set the start bit for the module we wanted to communicate . This will clear automatically by HW after setting the start condition

			module_ptr->CR1 |= (1<<8);
			return E_OK;
		}
		else
		{
			return E_NOT_OK;
		}
	}
	else
	{
		return E_NOT_OK;
	}

}


uint8_t i2c_get_driver_runtime(I2C_Transaction transaction_structure , I2C_Runtime** i2c_runtime)
{
	for (uint8_t iter = 0 ; iter<I2C_MAX_CONFIGURATION; iter++)
	{
		if (transaction_structure.module_pointer == i2c_runtime_config[iter].module_pointer)
		{
			*i2c_runtime = &i2c_runtime_config[iter];
			return E_OK;
		}
	}
	return E_NOT_OK;
}

void i2c_stop (I2C_Runtime* i2c_runtime)
{
	if ((i2c_runtime == NULL) || (i2c_runtime->module_pointer == NULL))
	{
		return;
	}
	i2c_structure*module_ptr = i2c_runtime->module_pointer;
	module_ptr->CR1 |= (1<<9);

	i2c_runtime->bus_state = I2C_BUS_STOP;												// Send the stop command for the bus
}


/// **************************************** Callback and Event handler ( ISR Related ) - Start ************************************************

void i2c_dma_complete (void *context)										// Callback function which need to be called once the i2c dma event is completed
{
	I2C_Runtime *i2c_runtime= (I2C_Runtime*) context;
	i2c_stop(i2c_runtime);													// I2C Stop function
}


/* Events
 *
 *  1 - SB will genereate once the start bit setting is successful . Hint to move to Address sending with write mode .
 *  		Send the address in write mode + Move the bus into ADDR WRITE mode
 *  2 - ADDR will generate once the address setting was successful . Hint to move to register writing process
 *  		Clear the ADDR and send the address of register + Move the BUS into REGISTER MODE
 *  3 - BTF will generate once the transfer of the register address is fine .. Hint that we can read / write to the register
 *  		Check we want to read from register or write into the register ( Because both has differant flow )
 *  		If its read , we need to do repeated start of the transfer . Set the start bit to 1 + Move the bus to REPEATED STATE ( we will do the read mode only for the moment since our intention is to read the registers )
 *  4 - SB will generate once the repeated start bit setting was successful . Hint to move to the addresss sending in read mode
 *  		Send the address in READ mode + Move the bus into ADDR READ mode
 *  5 -	ADDR will generate once the repeated start with address read was fine .. Hint to move to the read mode
 *  		Enable the DMA here since the reading will be done through  DMA and move the bus to I2C_BUS_DATA_READ
 */


void i2c_eventhandler (uint8_t module)
{
	if (module < I2C_MAX_CONFIGURATION)
	{
		// Fetch the Runtime configuration of the driver
		I2C_Runtime *i2c_runtime = &i2c_runtime_config[module];
		// Local variables for Module pointer and active transaction and the DMA stream configurations
		i2c_structure* module_pointer = i2c_runtime->module_pointer;
		I2C_Transaction active_transaction =i2c_runtime->active_transaction;
		const DMA_Stream_Config* dma_stream =i2c_runtime->dma_config;

		if ((i2c_runtime->module_pointer == NULL) || (dma_stream == NULL ))
		{
		    return;
		}
		// Event handlings


		// Handling the SB Event
		if (module_pointer->SR1 & (1 << 0x00))													/// Interrupt triggered through start bit event
		{
			switch(i2c_runtime->bus_state)
			{
			case I2C_BUS_START:
				i2c_write_address( module_pointer , active_transaction.slave_address, I2C_WRITE);								/// Write the address in write mode and set the bus state to ADDR mode
				i2c_runtime->bus_state = I2C_BUS_ADDRESS_WRITE;
				break;
			case I2C_BUS_REPEATED_START:
				i2c_write_address(module_pointer , active_transaction.slave_address , I2C_READ);								/// Now entering in read mode from the register
				i2c_runtime->bus_state = I2C_BUS_ADDRESS_READ;
				break;
			default:
				break;
			}
		}
		// Handling the ADDR Event
		else if (module_pointer->SR1 & (1 << 0x01))
		{
			(void)module_pointer->SR1;
			(void)module_pointer->SR2;

			switch(i2c_runtime->bus_state)
			{
			case I2C_BUS_ADDRESS_WRITE:
				i2c_write_byte( module_pointer , active_transaction.register_address);							// Initial address writing was success , so adding register address and we can restart from here
				i2c_runtime->bus_state = I2C_BUS_REGISTER;
				break;
			case I2C_BUS_ADDRESS_READ:
																											// Need to enable the DMA Here
				if ((dma_stream != NULL) && (dma_stream->module_pointer != NULL))
				{
					i2c_runtime->bus_state = I2C_BUS_DATA_READ;
					i2c_set_lastmode (module_pointer, LAST_TRANSFER_ENABLED);

					dma_set_ndtr(dma_stream , active_transaction.data_length);

					if (active_transaction.direction == I2C_READ)
					{
						dma_set_memory_address(dma_stream,active_transaction.rx_buffer);
					    dma_set_direction(dma_stream, PERIPHERAL_TO_MEMORY);
					}
					else
					{
						dma_set_memory_address(dma_stream,active_transaction.tx_buffer);
					    dma_set_direction(dma_stream, MEMORY_TO_PERIPHERAL);
					}
					dma_enable(dma_stream);

					i2c_dma_enable(module_pointer);
				}
				break;
			default :
				break;
			}
		}
		// Handling the BTF event
		else if (module_pointer->SR1 & (1 << 0x02))
		{
			switch(i2c_runtime->bus_state)
			{
			case I2C_BUS_REGISTER:
				if (active_transaction.direction == I2C_READ)
				{
					module_pointer->CR1 |= (1<<8);												// Repeated start
					i2c_runtime->bus_state = I2C_BUS_REPEATED_START;
				}
				else
				{
					i2c_write(module_pointer , active_transaction.tx_buffer);
					i2c_runtime->bus_state = I2C_BUS_DATA_SEND;
				}
				break;
			case I2C_BUS_DATA_READ:
										//	No need to do anything here . DMA will handle it
				break;
			default :
				break;
			}
		}
		// Handling stop event
		else if (module_pointer->SR1 & (1 << 0x04))
		{
			switch(i2c_runtime->bus_state)
			{
			case I2C_BUS_STOP:
				i2c_runtime->bus_state = I2C_BUS_IDLE;
				i2c_runtime->driver_status = I2C_DRIVER_IDLE;
				break;
			default :
				break;
			}
		}
		else
		{
			// Do nothing
		}
	}
}


uint8_t i2c_write(i2c_structure *module_pointer, uint8_t *data)
{
	if (data == NULL)
	{
		return E_NOT_OK;
	}
    module_pointer->DR = *data;
    return E_OK;
}

void i2c_write_byte(i2c_structure *module_pointer, uint8_t data)
{
    module_pointer->DR = data;
}


void i2c_write_address (i2c_structure *module_pointer, uint8_t data , uint8_t mode)
{
	uint8_t request = (data << 1) | mode ;
	module_pointer->DR = request;
}


