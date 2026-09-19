/*
 * APP_config.c
 *
 *  Created on: Jul 18, 2026
 *      Author: ANAS
 *
 *      File for init conifigurations for the project
 */


#include "APP_config.h"


// ****************************** Project specific variables **************************************************************


// ******************************  GPIO Configuration - START *************************************************************

/// The gpio pins needed to be configured for this project
const GPIO_PinConfig_t gpio_pin_config[] = {

		/// Generic GPIO Pins used in this project
		{	gpioa_ptr,	PIN0,	PIN_ANALOG_INPUT,	PORTA	,AF_INVALID},
		{	gpioa_ptr,	PIN1,	PIN_ANALOG_INPUT,	PORTA	,AF_INVALID},
		{	gpiob_ptr,	PIN1,	PIN_ANALOG_INPUT,	PORTB	,AF_INVALID},

		// GPIO Pin configurations for UART2
		{	gpioa_ptr , PIN2 , PIN_ALTERNATE_FUNCTION , PORTA, AF7},
		{	gpioa_ptr , PIN3 , PIN_ALTERNATE_FUNCTION , PORTA, AF7},
};

const uint8_t gpio_pin_config_size = sizeof(gpio_pin_config)/sizeof(gpio_pin_config[0]);

// ******************************  GPIO Configuration - END   *************************************************************




// ******************************  UART Configuration - START *************************************************************

const UART_Module_Config uart_config[] = {
		{
				usart2_ptr,				/// Module pointer UART - Pointer for uart module used
				USART_MODULE2,			/// Module number UART - Module number for uart module
				9600,					/// Uart_baud rate
				0,						/// Uart Oversampling
				16000000				/// Clock
		}
};

const uint8_t uart_config_size = sizeof(uart_config)/sizeof(uart_config[0]);


// ******************************  UART Configuration - END *************************************************************




// ****************************** WATCHDOG Configuration - START ********************************************************

const Watchdog_Module_Config watchdog_config = {
		iwdg_ptr,				/// Module Poniter
		6,						/// Prescalar
		10					    /// Watchdog set time
};

// ****************************** WATCHDOG Configuration - END **********************************************************

/// ADC Buffer initialisation
volatile uint32_t adc_measurement[3] = {0u};


// ***********************************************************************************************
/// The adc channels needed to be configured for this project
const ADC_Channel_config_t adc1_channel_config[] =
{
		{CHANNEL_1	, SAMPLES_480},
		{CHANNEL_3	, SAMPLES_480},
};
const ADC_Channel_config_t adc2_channel_config[] =
{
		{CHANNEL_9	, SAMPLES_480},
};

const uint8_t adc1_channel_count = sizeof(adc1_channel_config)/sizeof(adc1_channel_config[0]);
const uint8_t adc2_channel_count = sizeof(adc2_channel_config)/sizeof(adc2_channel_config[0]);

/// The ADC Modules needed to be configured for this project
const ADC_Module_config_t adc_module_config[] = {
		{	ADC1,	adc1_ptr,	ADC_RIGHT_ALIGN,	adc1_channel_config,	adc1_channel_count,	SCAN_MODE_ENABLED,	CONITNUOUS_MODE_ENABLED,	ADC_ENABLED,	EOC_AFTER_EACH_CONVERSION,	DMA_ENABLE,	},
		{	ADC2,	adc2_ptr,	ADC_RIGHT_ALIGN,	adc2_channel_config,	adc2_channel_count,	SCAN_MODE_DISABLED,	CONITNUOUS_MODE_DISABLED,	ADC_ENABLED,	EOC_AFTER_EACH_CONVERSION,	DMA_ENABLE,	},
};

const uint8_t adc_module_config_size = sizeof(adc_module_config)/sizeof(adc_module_config[0]);

// *******************************************************************************************
/// The dma streams to be configured for this project
const DMA_Stream_Config dma_stream_config[] = {
		{	DMA2,	dma2_ptr ,	STREAM0,	DMA2_STREAM0_ADC1,	PERIPHERAL_TO_MEMORY,	&(adc1_ptr->DR),	&adc_measurement[0],	adc1_channel_count,	PERIPHERAL_DATA_REG_32BIT,	MEMORY_SIZE_16BIT,	MEMORY_INCREMENT_ENABLE,	PERIPHERAL_INCREMENT_DISABLE,	CIRCULAR_MODE_ENABLE,	HIGH_PRIORITY,},
		{	DMA2,	dma2_ptr ,	STREAM3,	DMA2_STREAM3_ADC2,	PERIPHERAL_TO_MEMORY,	&(adc2_ptr->DR),	&adc_measurement[2],	adc2_channel_count,	PERIPHERAL_DATA_REG_32BIT,	MEMORY_SIZE_16BIT,	MEMORY_INCREMENT_ENABLE,	PERIPHERAL_INCREMENT_DISABLE,	CIRCULAR_MODE_ENABLE,	HIGH_PRIORITY,},
};

const uint8_t dma_stream_count = sizeof (dma_stream_config) / sizeof(dma_stream_config[0]);


