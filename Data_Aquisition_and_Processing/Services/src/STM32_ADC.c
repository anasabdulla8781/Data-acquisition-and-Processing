/*
 * STM32_ADC.c
 *
 *  Created on: Nov 20, 2025
 *      Author: uie83604
 */


/// Version 1.1

#include <STM32_ADC.h>
#include "STM32_RCC.h"


void adc_set_alignment ( ADC_structure* adc_ptr , uint8_t alignment )
{
	adc_ptr->CR2 &= ~(1 << 11);
	adc_ptr->CR2 |= alignment;
}

void adc_set_sample_time (ADC_structure* adc_ptr , ADC_Channel_config_t channel_ptr)
{
	uint8_t channel = channel_ptr.channel_number;
	uint8_t sample_time = channel_ptr.sample_time;
	if (channel < 10)
	{

		adc_ptr->SMPR2 &= ~(0x07 << ( channel *3)) ;
		adc_ptr->SMPR2 |= (sample_time << ( channel *3)) ;						/// Clearing and setting the Sample time to 480 cycle
	}
	else if (channel <= 18)
	{
		adc_ptr->SMPR1 &= ~(0x07 << (( channel - 10 )*3)) ;
		adc_ptr->SMPR1 |= (sample_time << ((channel - 10 )*3)) ;				/// Clearing and settting the sample time to 480 cycle
	}
}

void adc_set_sequence_and_length(ADC_structure* adc_ptr , ADC_Channel_config_t channel_ptr, uint8_t conversion_order)
{
	uint8_t shift = (conversion_order-1)*5;
	uint8_t channel = channel_ptr.channel_number;

	if((conversion_order < 1 || conversion_order > 16) ||channel > 18 )
	{
	    return;
	}


    if (conversion_order >=1 && conversion_order <= 6)
    {
        adc_ptr->SQR3 &= ~(0x1F << shift);
        adc_ptr->SQR3 |= (channel << shift);
    }
    else if (conversion_order >=7 && conversion_order <= 12)
    {
        shift = (conversion_order-7)*5;
        adc_ptr->SQR2 &= ~(0x1F << shift);
        adc_ptr->SQR2 |= (channel << shift);
    }
    else if (conversion_order >=13 && conversion_order <=16)
    {
        shift = (conversion_order-13)*5;
        adc_ptr->SQR1 &= ~(0x1F << shift);
        adc_ptr->SQR1 |= (channel << shift);
    }

    // Update sequence length
    adc_ptr->SQR1 &= ~(0xF << 20);
    adc_ptr->SQR1 |= ((conversion_order-1) << 20);
}

void adc_set_scan_mode(ADC_structure* adc_ptr , uint8_t scan_mode)
{
	if (scan_mode > SCAN_MODE_ENABLED)
	{
		return;
	}

	adc_ptr->CR1 &= ~(1<<8);		// Clearing the Scan mode bit
	adc_ptr->CR1 |= (scan_mode << 8);
}

void adc_set_continuous_mode(ADC_structure* adc_ptr , uint8_t continuous_mode)
{
	if(continuous_mode > CONITNUOUS_MODE_ENABLED)
	{
		return ;
	}

	adc_ptr->CR2 &= ~(1 << 1);	// Clearing the continous mode
	adc_ptr->CR2 |= (continuous_mode << 1);
}

void adc_set_enable ( ADC_structure* adc_ptr , uint8_t enablebit )
{
	if (enablebit>ADC_ENABLED)
	{
		return ;
	}

	adc_ptr->CR2 &= ~(1 << 0);	// Clearing the enable bit
	adc_ptr->CR2 |= (enablebit << 0);
}

void adc_set_EOC ( ADC_structure* adc_ptr , uint8_t eoc_bit )
{
	if (eoc_bit > EOC_AFTER_EACH_CONVERSION)
	{
		return ;
	}

	adc_ptr->CR2 &= ~(1 << 10);	// Clearing the CR2 eoc bit
	adc_ptr->CR2 |= (eoc_bit << 10);
}

void adc_start_conversion (ADC_structure* adc_ptr)
{
	adc_ptr->CR2 |= (1<<30);
}

void adc_get_value(ADC_structure * adc_ptr , volatile uint16_t* adc_measured_value)
{
	while (!(adc_ptr->SR & (1 << 1)));  // wait for EOC
	*adc_measured_value = adc_ptr->DR;
}

void adc_configure_dma(const ADC_Module_config_t* config)
{
	ADC_structure* adc_ptr = (config->register_ptr);
	if ((config->dma_enable) == DMA_ENABLE)
	{
		// Enable continuous DMA requests (DDS)
		adc_ptr->CR2 |= 1<<9;
		/// enable DMA mode
		adc_ptr->CR2 |= 1<<8;
	}
	else
	{
		// Enable continuous DMA requests (DDS)
		adc_ptr->CR2 &= ~(1<<9);
		/// Disable DMA mode
		adc_ptr->CR2 &= ~(1<<8);
	}
}

void adc_init(const ADC_Module_config_t* config , const uint8_t size)
{
	ADC_structure* adc_ptr = 0;
	const ADC_Channel_config_t* channel_ptr = 0;

	for (uint8_t iter = 0; iter<size ; iter++)
	{
		/// Taking the pointers
		adc_ptr = config[iter].register_ptr;
		channel_ptr = config[iter].channel_ptr;

		/// Step 1 - Initialise the clock for each modules in the project
		adc_clock_enable(config[iter].module_number);
		/// Step 2 - Set the alignment as right alignment
		adc_set_alignment(adc_ptr,config[iter].alignment);
		/// Step 4 - set the scan mode for the adc module
		adc_set_scan_mode ( adc_ptr , config[iter].scan_mode);
		/// Step 5 - set continuous mode
		adc_set_continuous_mode(adc_ptr , config[iter].continuous_mode);
		/// Step 6 - Set EOC pattern ( Do we need the EOC after each conversion or not  )
		adc_set_EOC(adc_ptr , config[iter].EOC_bit);
		for (uint8_t iter_channel = 0; iter_channel <config[iter].number_of_channels ; iter_channel++)
		{
			/// Step 3 - Set the sample time for each channels
			adc_set_sample_time(adc_ptr , channel_ptr[iter_channel]);
			/// Step 4 - Setting the sequence and length
			adc_set_sequence_and_length(adc_ptr,channel_ptr[iter_channel],(iter_channel+1));
		}
		/// Step 7 - Configure DMA
		adc_configure_dma(&config[iter]);
		/// Step 8 - Enable the ADC Module
		adc_set_enable ( adc_ptr , config[iter].enable );
	}
}





