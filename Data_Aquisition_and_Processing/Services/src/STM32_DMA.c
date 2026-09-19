/*
 * STM32_DMA.c
 *
 *  Created on: Jul 25, 2026
 *      Author: ANAS
 */


#include "stdint.h"
#include "STM32_DMA.h"

// STM32F407 VECTOR TABLE


const DMA_Combinations DMA_RequestChannelMap[] =
{
		{DMA2_STREAM0_ADC1	,	CHANNEL0},
		{DMA2_STREAM4_ADC1	,	CHANNEL0},
		{DMA2_STREAM0_SPIRX	,	CHANNEL3},
		{DMA2_STREAM3_ADC2	,	CHANNEL1},

};


void dma_init(DMA_Stream_Config* config , uint8_t dma_stream_count)
{
	for (uint8_t iter = 0; iter< dma_stream_count ; iter++)
	{
		dma_clock_enable(config[iter].module_number);
		dma_disable(& config[iter]);
		dma_set_channel (& config[iter]);
		dma_set_direction(&config[iter]);
		dma_set_peripheral_address(&config[iter]);
		dma_set_memory_address(&config[iter]);
		dma_set_ndtr(&config[iter]);
		dma_set_psize_msize(&config[iter]);
		dma_set_peripheral_memory_increment_mode(&config[iter]);
		dma_set_circular_mode(&config[iter]);
		dma_set_priority(&config[iter]);
		dma_enable(&config[iter]);
	}
}

void dma_set_channel(DMA_Stream_Config* config)
{
	DMA_structure* dma_module = (config->dma_module);
	uint8_t stream = config->dma_stream;

	/// Clear the CHSEL Bits
	dma_module->STREAM[stream].CR &= ~(0x07u << 25);
	/// Set the channel select bit to the channel we want
	dma_module->STREAM[stream].CR |= ( ( DMA_RequestChannelMap [config->request].channel)<< 25);
}


void dma_set_direction(DMA_Stream_Config * config)
{
	DMA_structure* dma_module = (config->dma_module);
	uint8_t stream = config->dma_stream;

	/// Clear the direction
	dma_module->STREAM[stream].CR &= ~( 0x03 << 6 );
	/// Set the direction
	dma_module->STREAM[stream].CR |= ((config->direction) << 6);
}


void dma_set_peripheral_address(DMA_Stream_Config * config)
{
	DMA_structure* dma_module = (config->dma_module);
	uint8_t stream = config->dma_stream;

	/// add the peripheral address
	dma_module->STREAM[stream].PAR = (uint32_t)config->peripheral_address;

}

void dma_set_memory_address(DMA_Stream_Config * config)
{
	DMA_structure* dma_module = (config->dma_module);
	uint8_t stream = config->dma_stream;

	/// add the peripheral address
	dma_module->STREAM[stream].M0AR = (uint32_t)config->memory_address;
}

void dma_set_ndtr(DMA_Stream_Config * config)
{
	DMA_structure* dma_module = (config->dma_module);
	uint8_t stream = config->dma_stream;

	/// Set the NDTR
	dma_module->STREAM[stream].NDTR = config->transfer_count;
}

void dma_set_psize_msize(DMA_Stream_Config * config)
{
	DMA_structure* dma_module = (config->dma_module);
	uint8_t stream = config->dma_stream;

	/// Clear the psize and msize
	dma_module->STREAM[stream].CR &= ~(3 << 13);
	dma_module->STREAM[stream].CR &= ~(3 << 11);

	/// Set the PSIZE and MSIZE
	dma_module->STREAM[stream].CR |= ((config->psize) << 11);
	dma_module->STREAM[stream].CR |= ((config->msize) << 13);
}


void dma_set_peripheral_memory_increment_mode(DMA_Stream_Config * config)
{
	DMA_structure* dma_module = (config->dma_module);
	uint8_t stream = config->dma_stream;

	/// Clear the peripheral and memory incremental modes
	dma_module->STREAM[stream].CR &= ~(1 << 9);
	dma_module->STREAM[stream].CR &= ~(1 << 10);

	/// Set the incremental modes
	dma_module->STREAM[stream].CR |= (config->memory_increment_mode << 10);
	dma_module->STREAM[stream].CR |= (config->peripheral_increment_mode << 9);
}

void dma_set_circular_mode ( DMA_Stream_Config * config )
{
	DMA_structure* dma_module = (config->dma_module);
	uint8_t stream = config->dma_stream;

	/// Clear the circular mode bit
	dma_module->STREAM[stream].CR &= ~(1<<8);
	/// Set the circular mode
	dma_module->STREAM[stream].CR |= ((config->circular_mode) << 8);
}

void dma_set_priority ( DMA_Stream_Config * config )
{
	DMA_structure* dma_module = (config->dma_module);
	uint8_t stream = config->dma_stream;

	/// Clear the priority bits
	dma_module->STREAM[stream].CR &= ~(3 << 16);
	/// Set the priority bits
	dma_module->STREAM[stream].CR |= ((config->priority) << 16);
}

void dma_enable ( DMA_Stream_Config * config )
{
	DMA_structure* dma_module = (config->dma_module);
	uint8_t stream = config->dma_stream;

	volatile uint32_t test1 = dma_module->STREAM[stream].CR;
	/// Enable the DMA after all the configurations
	dma_module->STREAM[stream].CR |= (1 << 0);

	volatile uint32_t test2 = dma_module->STREAM[stream].CR;

	dma_module->STREAM[stream].CR &= ~(3 << 13);
	dma_module->STREAM[stream].CR &= ~(3 << 11);

	/// Set the PSIZE and MSIZE
	dma_module->STREAM[stream].CR |= ((config->psize) << 11);
	dma_module->STREAM[stream].CR |= ((config->msize) << 13);

}

void dma_disable ( DMA_Stream_Config * config )
{
	DMA_structure* dma_module = (config->dma_module);
	uint8_t stream = config->dma_stream;

	/// Disable the DMA stream before doing any configuraitons
	dma_module->STREAM[stream].CR &= ~(1 << 0);

	while ((dma_module->STREAM[stream].CR) & (1u<<0));
}
