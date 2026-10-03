/*
 * STM32_I2C.h
 *
 *  Created on: Feb 21, 2026
 *      Author: ANAS
 */

#ifndef INC_STM32_I2C_H_
#define INC_STM32_I2C_H_


#include <stdint.h>
#include "STM32_RCC.h"
#include "STM32_DMA.h"

/// I2C Memory map strucutre

/// Structure for the Registers
typedef struct
{
	volatile uint32_t	CR1;			/// Control register 1
	volatile uint32_t	CR2;			/// Control register 2
	volatile uint32_t	OAR1;
	volatile uint32_t	OAR2;
	volatile uint32_t	DR;
	volatile uint32_t	SR1;
	volatile uint32_t	SR2;
	volatile uint32_t	CCR;
	volatile uint32_t	TRISE;
	volatile uint32_t	FLTR;
}i2c_structure;

/// Structure for the project configurations
typedef struct
{
	uint8_t module_number;
	i2c_structure* module_pointer;
	uint8_t i2c_speed_mode;
	uint16_t ccr;
	uint8_t max_rise_time;
	uint8_t ack_enable_disable;
	uint8_t clock_strech_enable_disable;
	uint8_t error_interrupt_enable_disable;
	uint8_t peripheral_clock_frequency;
	uint8_t buffer_interrupt_enable_disable;
	uint8_t event_interrupt_enable_disable;
	uint8_t dma_enable_disable;
	uint8_t i2c_enable_disable;
	const DMA_Stream_Config* dma_config;
	uint8_t set_last_enable_disable;
}I2C_Config;

/// Structure for the transactions  ( Application layer has to use to share the slave and register info to the driver ) - Differant structures for each sensors

typedef struct
{
	i2c_structure* module_pointer;
	uint8_t slave_address;
	uint8_t start_register_address;
	uint8_t direction;
	uint8_t data_length;
	uint8_t* result_array;
}I2C_Transaction;


/// Structure hold by I2C for transaction control ( Each driver will have seperate onces , we will arrange in common array )
typedef struct
{
	I2C_Transaction active_transaction;
	i2c_structure* module_pointer;
	uint8_t bus_state;
	uint8_t driver_status;
    const DMA_Stream_Config *dma_config;
}I2C_Runtime;


#define I2C1_BASEADDRESS	0x40005400
#define I2C2_BASEADDRESS	0x40005800
#define I2C3_BASEADDRESS	0x40005C00

#define i2c1_ptr	((i2c_structure*)I2C1_BASEADDRESS)
#define i2c2_ptr	((i2c_structure*)I2C2_BASEADDRESS)
#define i2c3_ptr	((i2c_structure*)I2C3_BASEADDRESS)

// MACROS

/// Differant I2C Chips
#define I2C_1	0
#define I2C_2	1
#define I2C_3	2

// Macro to declare the runtime array
#define I2C_MAX_CONFIGURATION	3

/// Differant speed modes
#define STANDARD_MODE	0
#define FAST_MODE		1

// ACKNOLEDGEMENT enable or disable
#define ACK_DISABLED	0
#define ACK_ENABLED		1

// Clock streching enabled / Disabled
#define CLOCK_STRECH_ENABLE		0
#define CLOCK_STRECH_DISABLE	1

// Error interrupts macros
#define ERROR_INTERRUPT_DISABLE		0
#define ERROR_INTERRUPT_ENABLE		1

// Buffer interrupts enable and disable ( Interrupts for TXE and RXNE )
#define BUFFER_INTERRUPT_ENABLE		1
#define BUFFER_INTERRUPT_DISABLE	0

// Event interrupt enable and disable ( Interrupts for various events)
#define EVENT_INTERRUPT_ENABLE		1
#define EVENT_INTERRUPT_DISABLE		0

// DMA Enable / Disable
#define DMA_DISABLE	0
#define DMA_ENABLE	1

// I2C Enabled / Disabled
#define I2C_ENABLE	1
#define I2C_DISABLE	0

// OK and NOT OK
#define E_NOT_OK	0
#define E_OK		1

// Last transfer enabled / Disabled
#define LAST_TRANSFER_DISABLED	0
#define LAST_TRANSFER_ENABLED	1

// Bus status - What exactly the status of the bus now

#define I2C_BUS_IDLE			0
#define I2C_BUS_START			1
#define I2C_BUS_ADDRESS_WRITE	2
#define I2C_BUS_REGISTER		3
#define I2C_BUS_REPEATED_START	4
#define I2C_BUS_ADDRESS_READ	5
#define I2C_BUS_DATA_READ		6
#define I2C_BUS_DATA_SEND		7
#define I2C_BUS_STOP			8

// Driver state - In what state the driver is currently
#define I2C_DRIVER_IDLE	0
#define I2C_DRIVER_BUSY	1

// Read and write mode in I2C
#define I2C_WRITE	0
#define I2C_READ	1
// Global variables declaration

/// Function declarations
extern void i2c_init (const I2C_Config* config , uint8_t i2c_module_count);
extern void i2c_reset_peripheral (i2c_structure* module_pointer);
extern void i2c_set_mode (const I2C_Config* config);
extern void i2c_set_clock_control_register (const I2C_Config* config);
extern void i2c_set_risetime (const I2C_Config* config);
extern void i2c_set_acknoledgement (const I2C_Config* config);
extern void i2c_set_clockstrech (const I2C_Config* config);
extern void i2c_set_error_interrupt_enable (const I2C_Config* config);
extern void i2c_set_frequency(const I2C_Config* config);
extern void i2c_set_buffer_interrupt_enable(const I2C_Config* config);
extern void i2c_set_event_interrupt_enable(const I2C_Config* config);
extern void i2c_dma_enable(i2c_structure* module_pointer);
extern void i2c_enable(const I2C_Config* config);
extern void i2c_dma_complete (void *context);


extern void i2c_start(I2C_Transaction transaction_structure);
extern uint8_t i2c_get_driver_runtime(I2C_Transaction transaction_structure , I2C_Runtime** i2c_runtime);


#endif /* INC_STM32_I2C_H_ */
