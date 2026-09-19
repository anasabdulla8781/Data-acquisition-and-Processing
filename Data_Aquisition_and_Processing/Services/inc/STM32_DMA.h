/*
 * STM32_DMA.h
 *
 *  Created on: Jul 24, 2026
 *      Author: ANAS
 */

#ifndef INC_STM32_DMA_H_
#define INC_STM32_DMA_H_


#include "stdint.h"
#include <STM32_RCC.h>

typedef struct DMA_stream_structure
{
    volatile uint32_t CR;     // Stream x configuration register
    volatile uint32_t NDTR;   // Stream x number of data register
    volatile uint32_t PAR;    // Stream x peripheral address register
    volatile uint32_t M0AR;   // Stream x memory 0 address register
    volatile uint32_t M1AR;   // Stream x memory 1 address register
    volatile uint32_t FCR;    // Stream x FIFO control register
}DMA_stream_structure;

typedef struct DMA_structure
{
    volatile uint32_t LISR;     // 0x00: Low interrupt status register
    volatile uint32_t HISR;     // 0x04: High interrupt status register
    volatile uint32_t LIFCR;    // 0x08: Low interrupt flag clear register
    volatile uint32_t HIFCR;    // 0x0C: High interrupt flag clear register
    DMA_stream_structure STREAM[8];	// Specific registers for all the streams
}DMA_structure;

typedef struct DMA_Combinations
{
	uint8_t request;
	uint8_t channel;
}DMA_Combinations;

typedef enum dma_stream_combinations
{
	DMA2_STREAM0_ADC1,
	DMA2_STREAM4_ADC1,
	DMA2_STREAM0_SPIRX,
	DMA2_STREAM3_ADC2

}dma_stream_combinations;


typedef struct dma_stream_config
{
	uint8_t module_number;
	DMA_structure* dma_module;
	uint8_t dma_stream;
	dma_stream_combinations request;
	uint8_t direction;
	volatile uint32_t* peripheral_address;
	volatile uint32_t* memory_address;
	uint16_t transfer_count;
	uint8_t psize;
	uint8_t msize;
	uint8_t memory_increment_mode;
	uint8_t peripheral_increment_mode;
	uint8_t circular_mode;
	uint8_t priority;
}DMA_Stream_Config;

#define CHANNEL0	0
#define CHANNEL1	1
#define CHANNEL3	3

#define STREAM0	0
#define STREAM1	1
#define STREAM3	3

#define dma2_ptr		(DMA_structure*)(0x40026400L)
#define dma1_ptr		(DMA_structure*)(0x40026000L)



/// Macros for direction
#define PERIPHERAL_TO_MEMORY	0
#define MEMORY_TO_PERIPHERAL	1

/// Macros for NDTR
#define NUMBER_OF_DATA_REGISTER_2	2

/// Macros for Peripheral size
#define PERIPHERAL_DATA_REG_32BIT 2

/// Macros for MEMORY Variable size
#define MEMORY_SIZE_16BIT	1
#define MEMORY_SIZE_8BIT	0

/// Macros for the memory increment mode
#define MEMORY_INCREMENT_ENABLE		1
#define MEMORY_INCREMENT_DISABLE	0

/// Macros for peripheral increment mode
#define PERIPHERAL_INCREMENT_ENABLE		1
#define PERIPHERAL_INCREMENT_DISABLE	0

/// Macros for the circular mode enable and disable
#define CIRCULAR_MODE_ENABLE	1
#define CIRCULAR_MODE_DISABLE	0

/// Macros for priority
#define HIGH_PRIORITY 2
#define VERY_HIGH_PRIORITY	3




/// Fucntion declarations
void dma_init(DMA_Stream_Config* config , uint8_t dma_stream_count);
void dma_set_channel(DMA_Stream_Config* config);
void dma_set_direction(DMA_Stream_Config* config);
void dma_set_peripheral_address(DMA_Stream_Config* config);
void dma_set_memory_address(DMA_Stream_Config* config);
void dma_set_ndtr(DMA_Stream_Config* config);
void dma_set_psize_msize(DMA_Stream_Config* config);
void dma_set_peripheral_memory_increment_mode(DMA_Stream_Config* config);
void dma_set_circular_mode(DMA_Stream_Config* config);
void dma_set_priority(DMA_Stream_Config* config);
void dma_enable(DMA_Stream_Config* config);
void dma_disable(DMA_Stream_Config* config);

#endif /* INC_STM32_DMA_H_ */
