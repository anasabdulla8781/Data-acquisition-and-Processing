/*
 * STM32_I2C.c
 *
 *  Created on: Feb 21, 2026
 *      Author: ANAS
 */


#include <STM32_I2C.h>







































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
