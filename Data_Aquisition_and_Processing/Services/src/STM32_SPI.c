/*
 * STM32_SPI.c
 *
 *  Created on: Feb 28, 2026
 *      Author: ANAS
 */


#include <stddef.h>
#include <STM32_SPI.h>

/*        spi_structure  		 GPIO_structure		 cs_pin			 miso_pin		 mosi_pin		clock_pin*/
spi_device spi1_config = {	spi1_ptr , 			gpioa_ptr , 		SPI1_NSS,		SPI1_MISO,		SPI1_MOSI , 	SPI1_SCLK};


/// **************************************** Module configurations - Start ************************************************

void spi_init(const SPI_Config* config , const uint8_t module_count )
{
	if ((config == NULL) || (module_count == 0))
	{
		return;
	}

	for (uint8_t iter = 0u; iter <module_count ; iter++)
	{
		if (config[iter].module_pointer != NULL)
		{
			spi_structure* module_pointer = config[iter].module_pointer;
			// Set the SPI Clock ( Clock enable )
			spi_clock_enable(config[iter].module_number);
			// Reset the whole CR1 and CR2 for the module
			spi_reset_config(module_pointer);
			// Set polarity and phase
			spi_set_polarity_phase(module_pointer,config[iter].polarity_and_phase);
			// Set the mode for the controller
			spi_set_mode(module_pointer , config[iter].mode);
			// Set the baudrate (prescalar) for the module
			spi_set_prescalar(module_pointer , config[iter].prescalar);
			// Set the frameformat
			spi_set_frameformat(module_pointer , config[iter].frame_format);
			// Set the software slave management
			spi_set_software_slave_management(module_pointer , config[iter].ssm_config);
			// Set the slave select initial value
			spi_set_internal_slave_select(module_pointer , config[iter].ssi_config);
			// Enable SPI
			spi_enable(module_pointer);
		}
	}

}

void spi_reset_config(spi_structure* module_pointer)
{
	module_pointer->CR1 = 0u;
	module_pointer->CR2 = 0u;
}

void spi_set_polarity_phase (spi_structure* module_pointer , uint8_t polarity_and_phase)
{
	if (polarity_and_phase > POLARITY_PHASE3)
	{
		return ;
	}
	module_pointer->CR1 &= ~(0x03 << 0);					// Cleared the polarity and phase bits from the CR1
	module_pointer->CR1 |= (polarity_and_phase << 0);		// Set the polarity and phase
}

void spi_set_mode (spi_structure* module_pointer , uint8_t mode)
{
	if (mode > MASTER_MODE)
	{
		return;
	}

	module_pointer->CR1 &= ~(0x01 << 2);						// Clear the mode
	module_pointer->CR1 |= (mode << 2);							// Set the mode
}

void spi_set_prescalar (spi_structure* module_pointer , uint8_t prescalar)
{
	if (prescalar > PRESCALAR256)
	{
		return;
	}

	module_pointer->CR1 &= ~(0x07 << 3);						// Clear the Prescalar setting
	module_pointer->CR1 |= (prescalar << 3);					// Set the Prescalar
}

void spi_set_frameformat (spi_structure* module_pointer , uint8_t frame_format)
{
	if (frame_format > BITFORMART16)
	{
		return;
	}

	module_pointer->CR1 &= ~(0x01 << 11);						// Clear the current frameformat setting
	module_pointer->CR1 |= (frame_format << 11);				// Set the frame format setting
}


void spi_set_software_slave_management(spi_structure *module_pointer, uint8_t ssm_config)
{
    if (ssm_config > SSM_SOFTWARE)
    {
        return;
    }

    module_pointer->CR1 &= ~(0x01U << 9);       				// Clear SSM bit
    module_pointer->CR1 |=  (ssm_config << 9);  				// Set SSM bit
}


void spi_set_internal_slave_select(spi_structure *module_pointer, uint8_t ssi_config)
{
    if (ssi_config > SSI_HIGH)
    {
        return;
    }

    module_pointer->CR1 &= ~(0x01U << 8);						// Clear SSI bit
    module_pointer->CR1 |= ((uint32_t)ssi_config << 8);			// Set SSI bit
}

void spi_enable(spi_structure* module_pointer)
{
	module_pointer->CR1 |= (0x01 << 6);
}


/// **************************************** Module configurations - END ************************************************



/// **************************************** Module Functionalities - START ************************************************

/// Make sure CS pin is low before calling the function and make CS pin to high after completing the function ( after return )
uint8_t spi_transfer_polling(spi_structure* spi_ptr , uint8_t write_byte)
{
	/// Step 1 - Wait till the transmit buffer become empty
	while (!(spi_ptr->SR & (1<<1)));
	/// Step 2 - Write the data (one byte to the DR)
	spi_ptr->DR = write_byte;
	/// Step 3 - Wait till the recieve buffet become non empty
	while(!(spi_ptr->SR & (1<<0)));
	/// Step 4 - Read the data (One byte from the read buffer)
	uint8_t read_byte = spi_ptr->DR;
	/// Step 5 - Wait till the busy flag get over .
	while(spi_ptr->SR & (1<<7));
	/// Step 6 - Return the function
	return read_byte;
}


void spi_transfer_buffer(spi_structure* spi_ptr, uint8_t *tx_buffer, uint8_t *rx_buffer, uint32_t length)
{
    for(uint32_t i = 0; i < length; i++)
    {
        uint8_t tx_data = 0xFF;

        if(tx_buffer != NULL)
        {
            tx_data = tx_buffer[i];
        }

        uint8_t received = spi_transfer_polling(spi_ptr, tx_data);

        if(rx_buffer != NULL)
        {
        	rx_buffer[i] = received;
        }
    }
}
