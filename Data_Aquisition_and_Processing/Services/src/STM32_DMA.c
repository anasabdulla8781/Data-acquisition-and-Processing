/*
 * STM32_DMA.c
 *
 *  Created on: Jul 25, 2026
 *      Author: ANAS
 */


#include "stdint.h"
#include "STM32_DMA.h"

/****************************************************** Global and static variable - START  ********************************************/

static DMA_Stream_Config* DMA_stream_ptr;
static uint8_t DMA_stream_count;

/****************************************************** Global and static variable - START  ********************************************/

/****************************************************** DMA LOOKUP TABLE - START  ********************************************/

const DMA_Request_Config DMA_RequestChannelMap[] =
{
		{DMA2_STREAM0_ADC1	,	CHANNEL0	},
		{DMA2_STREAM4_ADC1	,	CHANNEL0	},
		{DMA2_STREAM0_SPIRX	,	CHANNEL3	},
		{DMA2_STREAM3_ADC2	,	CHANNEL1	},
		{DMA1_STREAM0_IC21RX,	CHANNEL1	},

};

/****************************************************** DMA LOOKUP TABLE - END   ********************************************/


void dma_init(DMA_Stream_Config* config , uint8_t dma_stream_count)
{
	for (uint8_t iter = 0; iter< dma_stream_count ; iter++)
	{
		dma_clock_enable(config[iter].module_number);
		dma_disable(& config[iter]);
		dma_set_channel (& config[iter]);
		dma_set_direction(&config[iter] , config[iter].direction);
		dma_set_peripheral_address(&config[iter]);
		dma_set_memory_address(&config[iter] , config[iter].memory_address);
		dma_set_ndtr(&config[iter] , config[iter].transfer_count);
		dma_set_psize_msize(&config[iter]);
		dma_set_peripheral_memory_increment_mode(&config[iter]);
		dma_set_circular_mode(&config[iter]);
		dma_set_priority(&config[iter]);

		DMA_stream_ptr = config;
		DMA_stream_count = dma_stream_count;
	}
}

void dma_set_channel(DMA_Stream_Config* config)
{
	DMA_structure* module_pointer = (config->module_pointer);
	uint8_t stream = config->dma_stream;

	/// Clear the CHSEL Bits
	module_pointer->STREAM[stream].CR &= ~(0x07u << 25);
	/// Set the channel select bit to the channel we want
	module_pointer->STREAM[stream].CR |= ( ( DMA_RequestChannelMap [config->channel_request].channel)<< 25);
}


void dma_set_direction(const DMA_Stream_Config *config, uint8_t direction)
{
    DMA_stream_structure *stream = &config->module_pointer->STREAM[config->dma_stream];

    stream->CR &= ~(3U << 6);
    stream->CR |= ((direction & 0x03U) << 6);
}


void dma_set_peripheral_address(DMA_Stream_Config * config)
{
	DMA_structure* module_pointer = (config->module_pointer);
	uint8_t stream = config->dma_stream;

	/// add the peripheral address
	module_pointer->STREAM[stream].PAR = (uint32_t)config->peripheral_address;

}

void dma_set_memory_address(const DMA_Stream_Config *config, uint8_t *address)
{
    config->module_pointer->STREAM[config->dma_stream].M0AR = (uint32_t)address;
}

void dma_set_ndtr(const DMA_Stream_Config *config, uint16_t count)
{
    config->module_pointer->STREAM[config->dma_stream].NDTR = count;
}

void dma_set_psize_msize(DMA_Stream_Config * config)
{
	DMA_structure* module_pointer = (config->module_pointer);
	uint8_t stream = config->dma_stream;

	/// Clear the psize and msize
	module_pointer->STREAM[stream].CR &= ~(3 << 13);
	module_pointer->STREAM[stream].CR &= ~(3 << 11);

	/// Set the PSIZE and MSIZE
	module_pointer->STREAM[stream].CR |= ((config->psize) << 11);
	module_pointer->STREAM[stream].CR |= ((config->msize) << 13);
}


void dma_set_peripheral_memory_increment_mode(DMA_Stream_Config * config)
{
	DMA_structure* module_pointer = (config->module_pointer);
	uint8_t stream = config->dma_stream;

	/// Clear the peripheral and memory incremental modes
	module_pointer->STREAM[stream].CR &= ~(1 << 9);
	module_pointer->STREAM[stream].CR &= ~(1 << 10);

	/// Set the incremental modes
	module_pointer->STREAM[stream].CR |= (config->memory_increment_mode << 10);
	module_pointer->STREAM[stream].CR |= (config->peripheral_increment_mode << 9);
}

void dma_set_circular_mode ( DMA_Stream_Config * config )
{
	DMA_structure* module_pointer = (config->module_pointer);
	uint8_t stream = config->dma_stream;

	/// Clear the circular mode bit
	module_pointer->STREAM[stream].CR &= ~(1<<8);
	/// Set the circular mode
	module_pointer->STREAM[stream].CR |= ((config->circular_mode) << 8);
}

void dma_set_priority ( DMA_Stream_Config * config )
{
	DMA_structure* module_pointer = (config->module_pointer);
	uint8_t stream = config->dma_stream;

	/// Clear the priority bits
	module_pointer->STREAM[stream].CR &= ~(3 << 16);
	/// Set the priority bits
	module_pointer->STREAM[stream].CR |= ((config->priority) << 16);
}

void dma_enable ( const DMA_Stream_Config * config )
{
	DMA_structure* module_pointer = (config->module_pointer);
	uint8_t stream = config->dma_stream;

	/// Enable the DMA after all the configurations
	module_pointer->STREAM[stream].CR |= (1 << 0);
}

void dma_disable ( DMA_Stream_Config * config )
{
	DMA_structure* module_pointer = (config->module_pointer);
	uint8_t stream = config->dma_stream;

	/// Disable the DMA stream before doing any configuraitons
	module_pointer->STREAM[stream].CR &= ~(1 << 0);

	while ((module_pointer->STREAM[stream].CR) & (1u<<0));
}


void dma_eventhandler(DMA_structure *module_pointer, uint8_t stream)
{
    const DMA_Stream_Config *config = NULL;

    /* Find the configuration matching this DMA module and stream */
    for (uint8_t i = 0; i < DMA_stream_count; i++)
    {
        if ((DMA_stream_ptr[i].module_pointer == module_pointer) && (DMA_stream_ptr[i].dma_stream == stream))
        {
            config = &DMA_stream_ptr[i];
            break;
        }
    }

    /* No matching configuration found */
    if (config == NULL)
    {
        return;
    }

    /*
     * Check Transfer Complete flag
     *
     * DMA1 Stream 0 uses DMA1->LISR
     * Stream 0 Transfer Complete Flag = TCIF0 (bit 5)
     */
    if (module_pointer->LISR & (1U << 5))
    {
        /* Clear Transfer Complete flag */
        module_pointer->LIFCR |= (1U << 5);

        /* Call registered callback */
        if (config->callback_ptr != NULL)
        {
            config->callback_ptr(config->context);
        }
    }
}
