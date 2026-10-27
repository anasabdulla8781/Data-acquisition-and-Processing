/*
 * STM32_SPI.h
 *
 *  Created on: Feb 28, 2026
 *      Author: ANAS
 */

#ifndef INC_STM32_SPI_H_
#define INC_STM32_SPI_H_

#include <stdint.h>
#include <STM32_GPIO.h>
#include "STM32_RCC.h"

/// Macros

/// Macros for SPI1 Pins
#define SPI1_NSS	4
#define SPI1_SCLK	5
#define SPI1_MISO	6
#define SPI1_MOSI	7


typedef struct
{
	volatile uint32_t	CR1;
	volatile uint32_t	CR2;
	volatile uint32_t	SR;
	volatile uint32_t	DR;
	volatile uint32_t	CRCPR;
	volatile uint32_t	RXCRCR;
	volatile uint32_t	TXCRCR;
	volatile uint32_t	I2SCFGR;
	volatile uint32_t	I2SPR;
}spi_structure;

typedef struct
{
	spi_structure* spi_ptr;
	GPIO_structure *gpio_ptr;
	uint8_t cs_pin;
	uint8_t miso_pin;
	uint8_t mosi_pin;
	uint8_t clock_pin;
}spi_device;

typedef struct
{
	uint8_t module_number;
	spi_structure* module_pointer;
	uint8_t polarity_and_phase;
	uint8_t mode;
	uint8_t prescalar;
	uint8_t frame_format;
	uint8_t ssm_config;
	uint8_t ssi_config;
}SPI_Config;


// SPI Base Addresses
#define SPI1_BASEADDRESS    0x40013000U
#define SPI2_BASEADDRESS    0x40003800U
#define SPI3_BASEADDRESS    0x40003C00U

// Pointers to SPI peripheral registers
#define spi1_ptr    ((spi_structure *)SPI1_BASEADDRESS)
#define spi2_ptr    ((spi_structure *)SPI2_BASEADDRESS)
#define spi3_ptr    ((spi_structure *)SPI3_BASEADDRESS)

// Macros for Module numbers
#define SPI1        0U
#define SPI2        1U
#define SPI3        2U

// Macros for polarity_and_phase
#define POLARITY_PHASE0		0
#define POLARITY_PHASE1		1
#define POLARITY_PHASE2		2
#define POLARITY_PHASE3		3

// Macros for SPI controller mode
#define SLAVE_MODE	0
#define MASTER_MODE	1

// Macros for prescalar
#define PRESCALAR2		0
#define PRESCALAR4		1
#define PRESCALAR8		2
#define PRESCALAR16		3
#define PRESCALAR32		4
#define PRESCALAR64		5
#define PRESCALAR128	6
#define PRESCALAR256	7

// Macros for frame_format
#define BITFORMART8		0
#define BITFORMART16	1

// Macros for ssm_config
#define SSM_HARDWARE    0U
#define SSM_SOFTWARE    1U

// Macros for ssi_config
#define SSI_LOW  0U
#define SSI_HIGH 1U

extern spi_device spi1_config;

/// Function declarations
extern uint8_t spi_transfer_polling(spi_structure* spi_ptr , uint8_t data);
extern void spi_transfer_buffer(spi_structure* spi_ptr, uint8_t *tx_buffer, uint8_t *rx_buffer, uint32_t length);
extern void spi_init(const SPI_Config* config , const uint8_t module_count );
extern void spi_reset_config(spi_structure* module_pointer);
extern void spi_set_polarity_phase (spi_structure* module_pointer , uint8_t polarity_and_phase);
extern void spi_set_mode (spi_structure* module_pointer , uint8_t mode);
extern void spi_set_prescalar (spi_structure* module_pointer , uint8_t prescalar);
extern void spi_set_frameformat (spi_structure* module_pointer , uint8_t frame_format);
extern void spi_set_software_slave_management(spi_structure *module_pointer, uint8_t ssm_config);
extern void spi_set_internal_slave_select(spi_structure *module_pointer, uint8_t ssi_config);
extern void spi_enable(spi_structure* module_pointer);

#endif /* INC_STM32_SPI_H_ */
