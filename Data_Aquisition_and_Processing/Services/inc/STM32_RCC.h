/*
 * STM32_RCC.h
 *
 *  Created on: Oct 20, 2025
 *      Author: ANAS
 */

#ifndef INC_STM32_RCC_H_
#define INC_STM32_RCC_H_


#include <stdint.h>

/// RCC Structure


typedef struct
{
    volatile uint32_t CR;                 // 0x00: Clock control register
    volatile uint32_t PLLCFGR;            // 0x04: PLL configuration register
    volatile uint32_t CFGR;               // 0x08: Clock configuration register
    volatile uint32_t CIR;                // 0x0C: Clock interrupt register
    volatile uint32_t AHB1RSTR;           // 0x10: AHB1 peripheral reset register
    volatile uint32_t AHB2RSTR;           // 0x14: AHB2 peripheral reset register
    volatile uint32_t AHB3RSTR;           // 0x18: AHB3 peripheral reset register
    volatile uint32_t RESERVED0;          // 0x1C
    volatile uint32_t APB1RSTR;           // 0x20: APB1 peripheral reset register
    volatile uint32_t APB2RSTR;           // 0x24: APB2 peripheral reset register
    volatile uint32_t RESERVED1[2];       // 0x28–0x2C
    volatile uint32_t AHB1ENR;            // 0x30: AHB1 peripheral clock enable register
    volatile uint32_t AHB2ENR;            // 0x34: AHB2 peripheral clock enable register
    volatile uint32_t AHB3ENR;            // 0x38: AHB3 peripheral clock enable register
    volatile uint32_t RESERVED2;          // 0x3C
    volatile uint32_t APB1ENR;            // 0x40: APB1 peripheral clock enable register
    volatile uint32_t APB2ENR;            // 0x44: APB2 peripheral clock enable register
    volatile uint32_t RESERVED3[2];       // 0x48–0x4C
    volatile uint32_t AHB1LPENR;          // 0x50: AHB1 clock enable in low-power mode
    volatile uint32_t AHB2LPENR;          // 0x54: AHB2 clock enable in low-power mode
    volatile uint32_t AHB3LPENR;          // 0x58: AHB3 clock enable in low-power mode
    volatile uint32_t RESERVED4;          // 0x5C
    volatile uint32_t APB1LPENR;          // 0x60: APB1 clock enable in low-power mode
    volatile uint32_t APB2LPENR;          // 0x64: APB2 clock enable in low-power mode
    volatile uint32_t RESERVED5[2];       // 0x68–0x6C
    volatile uint32_t BDCR;               // 0x70: Backup domain control register
    volatile uint32_t CSR;                // 0x74: Clock control & status register
    volatile uint32_t RESERVED6[2];       // 0x78–0x7C
    volatile uint32_t SSCGR;              // 0x80: Spread spectrum clock generation register
    volatile uint32_t PLLI2SCFGR;         // 0x84: PLLI2S configuration register
    volatile uint32_t PLLSAICFGR;         // 0x88: PLLSAI configuration register
    volatile uint32_t DCKCFGR;            // 0x8C: Dedicated clocks configuration register
    volatile uint32_t CKGATENR;           // 0x90: Clocks gated enable register
    volatile uint32_t DCKCFGR2;           // 0x94: Dedicated clocks configuration register 2
} rcc_structure;



/// RCC BASE ADDRESS

#define RCC_BASEADDRESS		0x40023800

/// POINTER TO RCC STRUCTURE

#define rcc_ptr		((rcc_structure*)RCC_BASEADDRESS)


/// MACROS


/// Macros for ADC and its clock enable

#define ADC1	0
#define ADC2	1
#define ADC3	2

#define ADC_CLOCK_ENABLE 	8

/// Macros for DMA and Its clock enable
#define DMA1	1
#define DMA2	2



/// Macros for GPIO and its clock enable

#define GPIOA	0
#define GPIOB	1
#define GPIOC	2
#define GPIOD	3
#define GPIOE	4
#define GPIOF	5
#define GPIOG	6
#define GPIOH	7
#define GPIOI	8

#define GPIO_CLOCK_ENABLE	0


#define SET_GPTIO_CLOCK_ENABLE			0x1FF
#define SET_TIM2_CLOCK_ENABLE			0x01
#define SET_TIM4_CLOCK_ENABLE			0x04
#define SET_SYSTEM_CONFIG_CLOCK_ENABLE	0x4000

/// Macros for the reset reasons
#define INDEPENDENT_WATCHDOG_RESET		1
#define WINDOW_WATCHDOG_RESET			2
#define RESET_BUTTON_RESET				3
#define POWER_ON_RESET					4
#define BROWN_OUT_RESET					5
#define SOFTWARE_RESET					6
#define UNKNOWN_RESET					7


struct ADC_structure;


/// Function declarations

extern void clock_enable_LSI(void);
extern void clock_enable_HSE(void);
extern void reset_reason_check(volatile uint8_t *reset_reason_ptr , char* reset_reson_string);
extern void adc_clock_enable(uint8_t module);
extern void usart_clock_enable(uint8_t module_number);
extern void i2c_clock_enable(uint8_t module);
extern void gpio_clock_enable(uint8_t port);
extern void spi_clock_enable(uint8_t module);
extern void syscfg_clock_enable(void);
extern void dma_clock_enable(uint8_t module);
extern void print_reset_reason (void);


#endif /* INC_STM32_RCC_H_ */
