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

void spi_init_config(spi_structure* spi_ptr)
{
	/// STEP 1 - We reset the CR1 register in starting ( to avoid previous pending things )
	spi_ptr->CR1 = 0;
	/// STEP 2 - We set the SPI in the board in master mode
	spi_ptr->CR1 |= (1 << 2);
	/// STEP 3 - Setting the baud rate needed in starting
	spi_ptr->CR1 &= ~(0x07 << 3);					/// Clearing the baud rate bits for SPI
	spi_ptr->CR1 |= (3 << 3);						/// Setting the baud rate with 011
	/// STEP 4 - Setting the polarity and phase to 0
	spi_ptr->CR1 &= ~(0x03 << 0);					/// Clearing the first 2 bits for Polarity and Phase ( No need of setting it to 0 manually )
	/// Step 5 - 8 bit data format
	spi_ptr->CR1 &= ~(0x01 << 11);					/// Clearing the 11th bit , no need of explicitly
	/// Step 6 - Explicitly mentioning that NSS pins are manual controlled (SSM and SSI bits to be 1)
	spi_ptr->CR1 |= (0x03 << 8);
	/// Step 7 - Turn On SPI (Enable SPI Peripheral)
	spi_ptr->CR1 |= (0x01 << 6);

}

/// Make sure CS pin is low before calling the function and make CS pin to high after completing the function ( after return )
uint8_t spi_transfer(spi_structure* spi_ptr , uint8_t data)
{
	/// Step 1 - Wait till the transmit buffer become empty
	while (!(spi_ptr->SR & (1<<1)));
	/// Step 2 - Write the data (one byte to the DR)
	spi_ptr->DR = data;
	/// Step 3 - Wait till the recieve buffet become non empty
	while(!(spi_ptr->SR & (1<<0)));
	/// Step 4 - Read the data (One byte from the read buffer)
	uint8_t received = spi_ptr->DR;
	/// Step 5 - Wait till the busy flag get over .
	while(spi_ptr->SR & (1<<7));
	/// Step 6 - Return the function
	return received;
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

        uint8_t received = spi_transfer(spi_ptr, tx_data);

        if(rx_buffer != NULL)
        {
        	rx_buffer[i] = received;
        }
    }
}
