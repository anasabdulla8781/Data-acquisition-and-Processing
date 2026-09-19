/*
 * STM32_GPIO.c
 *
 *  Created on: Oct 20, 2025
 *      Author: ANAS
 */

#include <STM32_GPIO.h>
#include <STM32_RCC.h>


void gpio_init(const GPIO_PinConfig_t* config ,  const uint8_t size)
{
	for (uint8_t iter = 0; iter<size ; iter++)
	{
		// Step 1 - Clock init for all the pins
		gpio_clock_enable(config[iter].port_number);
		// Step 2 - Set the configuration for the register
		gpio_pin_set_mode(config[iter].pin_number , config[iter].mode , config[iter].register_ptr);
		// Step 3 - Set to alternate function mode if its not invalid
		if ((config[iter].alternate_function_number) != AF_INVALID)
		{
			gpio_set_alternate_function (config[iter].register_ptr , config[iter].pin_number , config[iter].alternate_function_number);
		}
		else
		{
			// Do nothing
		}
	}
}


void gpio_pin_set_mode(uint8_t pin , uint8_t mode , GPIO_structure* gpio_ptr)
{
	if (pin > 15 || mode > PIN_ANALOG_INPUT) return;			/// Safty logic , Dont give pin number more than 15 or no other modes than than 3

	gpio_ptr->MODER &= ~(0X03<<(pin*2));		/// clearing the mode bits for that pin
	gpio_ptr->MODER |= (mode << (pin*2));		/// Setting the mode bits for different kind of modes ( Input , Output , Alternate function , Analog input )
}


void gpio_output_operations (GPIO_structure* gpio_ptr , uint8_t pin , uint8_t state)
{
	if(state == SET_TOGGLE)
	{
		gpio_ptr->ODR ^= (1<<pin);					/// Toggle the pin
	}
	else if (state == SET_HIGH)
	{
		gpio_ptr->ODR |= (1<<pin);					/// Drive the pin to High
	}
	else if(state == SET_LOW)
	{
		gpio_ptr->ODR &= ~(1<<pin);					/// Drive the pin to Low
	}
}

void gpio_set_alternate_function(GPIO_structure* gpio_ptr , uint8_t pin_number, uint8_t af_number)
{
	if (af_number > AF15)
	    return;
	if (pin_number <8)
	{
		gpio_ptr->AFRL &= ~ ( 0x0F <<(pin_number*4));						/// Clearing the alternate functions bits
		gpio_ptr->AFRL |=   ( af_number  <<(pin_number*4));
	}
	else if (pin_number < 16 )
	{
		gpio_ptr->AFRH &= ~ ( 0x0F <<((pin_number - 8)*4));					/// Clearing the alternate functions bits
		gpio_ptr->AFRH |=   ( af_number <<((pin_number - 8)*4));
	}

}

void gpio_set_output_type(GPIO_structure* gpio_ptr , uint8_t pin_number , uint8_t output_type)
{
	if (pin_number>15 || output_type > 1)	return;						/// we need to check only upto 15 bits
	gpio_ptr->OTYPER &= ~(1 << pin_number);								/// Cleared the particular pin we want to set the output state
	gpio_ptr->OTYPER |= (output_type << pin_number);					/// We have Pushpull and open drain states
}

void gpio_set_output_speed(GPIO_structure* gpio_ptr , uint8_t pin_number , uint8_t output_speed)
{
	if (pin_number>15 || output_speed > 3)	return;						/// Safety check - Pin shouldnt be more than 15 and we have maximum 4 configured speeds
	gpio_ptr->OSPEEDR &= ~ (0x03 << (pin_number*2));					/// Clearing the particular pins needed to set the speed
	gpio_ptr->OSPEEDR |= ( output_speed << (pin_number*2));				/// We set the output speed
}

void gpio_set_pullup_pulldown(GPIO_structure* gpio_ptr , uint8_t pin_number , uint8_t pullup_down_value)
{
	if (pin_number>15 || pullup_down_value > 3)	return;						/// Safety check - Pin shouldnt be more than 15 and we have maximum 4 configured pull up or downs
	gpio_ptr->PUPDR &= ~ (0x03 << (pin_number*2));							/// Clearing the particular pins needed to set the pull up or down
	gpio_ptr->PUPDR |= ( pullup_down_value << (pin_number*2));				/// We set the output speed
}

void gpio_spi1_config(void)
{
	/// Step 1 - Setting the pins in alternate function mode
	gpio_pin_set_mode(SPI1_NSS,PIN_OUTPUT,gpioa_ptr);								/// PA4 Set in Output mode for SPI1 NSS (Chip select pin , we need control over it)
	gpio_pin_set_mode(SPI1_SCLK,PIN_ALTERNATE_FUNCTION,gpioa_ptr);					/// PA5 Set in alternate function mode for SPI1 SCLK
	gpio_pin_set_mode(SPI1_MISO,PIN_ALTERNATE_FUNCTION,gpioa_ptr);					/// PA6 Set in alternate function mode for SPI1 MISO
	gpio_pin_set_mode(SPI1_MOSI,PIN_ALTERNATE_FUNCTION,gpioa_ptr);					/// PA7 Set in alternate function mode for SPI1 MOSI
	/// Step 2 - Setting these pins to AF5
	gpio_set_alternate_function(gpioa_ptr,SPI1_SCLK,AF5);							/// PA5 - AF5
	gpio_set_alternate_function(gpioa_ptr,SPI1_MISO,AF5);							/// PA6 - AF5
	gpio_set_alternate_function(gpioa_ptr,SPI1_MOSI,AF5);							/// PA7 - AF5
	/// Step 3 - Setting the Output types as pushpull - SPI needs it (Miso is input , leave it)
	gpio_set_output_type(gpioa_ptr,SPI1_NSS,OUTPUT_PUSH_PULL);
	gpio_set_output_type(gpioa_ptr,SPI1_SCLK,OUTPUT_PUSH_PULL);
	gpio_set_output_type(gpioa_ptr,SPI1_MOSI,OUTPUT_PUSH_PULL);
	/// Step 4 - Setting the output pin speeds
	gpio_set_output_speed(gpioa_ptr,SPI1_NSS,VERY_HIGH_SPEED);
	gpio_set_output_speed(gpioa_ptr,SPI1_SCLK,VERY_HIGH_SPEED);
	gpio_set_output_speed(gpioa_ptr,SPI1_MOSI,VERY_HIGH_SPEED);
	/// Step 5 - Configure pull-up/pull-down (none required for SPI)
	gpio_set_pullup_pulldown(gpioa_ptr,SPI1_NSS,NO_PULLUP_PULLDOWN);
	gpio_set_pullup_pulldown(gpioa_ptr,SPI1_SCLK,NO_PULLUP_PULLDOWN);
	gpio_set_pullup_pulldown(gpioa_ptr,SPI1_MOSI,NO_PULLUP_PULLDOWN);
	gpio_set_pullup_pulldown(gpioa_ptr, SPI1_MISO, PULLUP);
	/// Step 6 - Making the default Cs pin high
	gpio_output_operations(gpioa_ptr,SPI1_NSS,SET_HIGH);
}

void gpio_i2c_config(void)
{
	/// Step 1 - Set the pins we are using to alternate function mode ( PB6 and PB7 )
	gpio_pin_set_mode(6,PIN_ALTERNATE_FUNCTION,gpiob_ptr);		/// To set PB6 to alternate function mode
	gpio_pin_set_mode(7,PIN_ALTERNATE_FUNCTION,gpiob_ptr);		/// To set PB7 to alternate function mode
	/// Step 2 - Set the alternate modes properly
	gpio_set_alternate_function(gpiob_ptr,6,AF4);
	gpio_set_alternate_function(gpiob_ptr,7,AF4);
	/// Step 3 - Set the gpio pins to open drain configuration
	gpio_set_output_type(gpiob_ptr,6,OUTPUT_OPEN_DRAIN);
	gpio_set_output_type(gpiob_ptr,7,OUTPUT_OPEN_DRAIN);
	/// Step 4 - Set the output speeds
	gpio_set_output_speed(gpiob_ptr, 6 , VERY_HIGH_SPEED);
	gpio_set_output_speed(gpiob_ptr, 7 , VERY_HIGH_SPEED);
	/// Step 5 - Set the Pull up resistors
	gpio_set_pullup_pulldown(gpiob_ptr, 6 , PULLUP);
	gpio_set_pullup_pulldown(gpiob_ptr, 7 , PULLUP);
}

