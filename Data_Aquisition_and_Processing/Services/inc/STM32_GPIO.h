/*
 * STM32_GPIO.h
 *
 *  Created on: Oct 20, 2025
 *      Author: ANAS
 */

#ifndef INC_STM32_GPIO_H_
#define INC_STM32_GPIO_H_

#include <stdint.h>
#include <STM32_RCC.h>

/// GPIO Structure

typedef struct
{
    volatile uint32_t MODER;     // 0x00: GPIO port mode register
    volatile uint32_t OTYPER;    // 0x04: GPIO port output type register
    volatile uint32_t OSPEEDR;   // 0x08: GPIO port output speed register
    volatile uint32_t PUPDR;     // 0x0C: GPIO port pull-up/pull-down register
    volatile uint32_t IDR;       // 0x10: GPIO port input data register
    volatile uint32_t ODR;       // 0x14: GPIO port output data register
    volatile uint32_t BSRR;      // 0x18: GPIO port bit set/reset register
    volatile uint32_t LCKR;      // 0x1C: GPIO port configuration lock register
    volatile uint32_t AFRL;      // 0x20: GPIO alternate function low register (pins 0–7)
    volatile uint32_t AFRH;      // 0x24: GPIO alternate function high register (pins 8–15)
} GPIO_structure;

typedef struct GPIO_PinConfig_t				// Init structure for the gpio pins using in the project
{
	GPIO_structure* register_ptr;
	uint8_t pin_number;
	uint8_t mode;
	uint8_t port_number;
	uint8_t alternate_function_number;
}GPIO_PinConfig_t;


/// GPIO BASE ADDRESS

#define GPIOA_BASEADDRESS  (0x40020000UL)
#define GPIOB_BASEADDRESS  (0x40020400UL)
#define GPIOC_BASEADDRESS  (0x40020800UL)
#define GPIOD_BASEADDRESS  (0x40020C00UL)
#define GPIOE_BASEADDRESS  (0x40021000UL)
#define GPIOF_BASEADDRESS  (0x40021400UL)
#define GPIOG_BASEADDRESS  (0x40021800UL)
#define GPIOH_BASEADDRESS  (0x40021C00UL)
#define GPIOI_BASEADDRESS  (0x40022000UL)


/// POINTERS TO GPIO Structures

#define gpioa_ptr ((GPIO_structure *) GPIOA_BASEADDRESS)
#define gpiob_ptr ((GPIO_structure *) GPIOB_BASEADDRESS)
#define gpioc_ptr ((GPIO_structure *) GPIOC_BASEADDRESS)
#define gpiod_ptr ((GPIO_structure *) GPIOD_BASEADDRESS)
#define gpioe_ptr ((GPIO_structure *) GPIOE_BASEADDRESS)
#define gpiof_ptr ((GPIO_structure *) GPIOF_BASEADDRESS)
#define gpiog_ptr ((GPIO_structure *) GPIOG_BASEADDRESS)
#define gpioh_ptr ((GPIO_structure *) GPIOH_BASEADDRESS)
#define gpioi_ptr ((GPIO_structure *) GPIOI_BASEADDRESS)


/// MACROS

#define PORTD_BLUE_LED_PIN	15
#define PORTD_GREEN_LED_PIN	12
#define PORTD_ORANGE_LED_PIN	13
#define PORTD_RED_LED_PIN		14
#define PORTA_USER_INPUT_PIN	0


/// Macros for pins
#define PIN0   0
#define PIN1   1
#define PIN2   2
#define PIN3   3
#define PIN4   4
#define PIN5   5
#define PIN6   6
#define PIN7   7
#define PIN8   8
#define PIN9   9
#define PIN10  10
#define PIN11  11
#define PIN12  12
#define PIN13  13
#define PIN14  14
#define PIN15  15


/// Macros for differant operations
#define SET_LOW			0
#define SET_HIGH		1
#define SET_TOGGLE		2

/// Macros for modes
#define PIN_INPUT					0
#define PIN_OUTPUT 					1
#define PIN_ALTERNATE_FUNCTION		2
#define PIN_ANALOG_INPUT			3

/// Macros for differant alternate modes
#define AF0	0
#define AF2	2
#define AF3	3
#define AF4	4
#define AF5	5
#define AF6	6
#define AF7	7
#define AF15	15
#define AF_INVALID	0XFF

/// Macros for differant Ports
#define PORTA	0
#define PORTB	1
#define PORTC	2
#define PORTD	3

/// Macros for output types
#define OUTPUT_PUSH_PULL   0
#define OUTPUT_OPEN_DRAIN  1

/// Macros for output speed types
#define LOW_SPEED			0
#define MEDIUM_SPEED		1
#define HIGH_SPEED			2
#define VERY_HIGH_SPEED		3

/// Macros for pull up and pull down configuraions
#define NO_PULLUP_PULLDOWN		0
#define PULLUP					1
#define PULLDOWN				2
#define RESERVED				3

/// Macros for SPI1 Pins
#define SPI1_NSS	4
#define SPI1_SCLK	5
#define SPI1_MISO	6
#define SPI1_MOSI	7


/// FUNCTION DECLARATIONS
extern void gpio_pin_set_mode(uint8_t pin , uint8_t mode , GPIO_structure* gpio_ptr);
extern void gpio_output_operations (GPIO_structure* gpio_ptr , uint8_t pin , uint8_t state);
extern void gpio_set_alternate_function(GPIO_structure* gpio_ptr , uint8_t pin_number, uint8_t af_number);
extern void gpio_set_output_type(GPIO_structure* gpio_ptr , uint8_t pin_number , uint8_t output_type);
extern void gpio_set_output_speed(GPIO_structure* gpio_ptr , uint8_t pin_number , uint8_t output_speed);
extern void gpio_set_pullup_pulldown(GPIO_structure* gpio_ptr , uint8_t pin_number , uint8_t pullup_down_value);
extern void gpio_i2c_config(void);
extern void gpio_spi1_config(void);

/// Making generic init for all
extern void gpio_init(const GPIO_PinConfig_t* config ,  const uint8_t size);


#endif /* INC_STM32_GPIO_H_ */
