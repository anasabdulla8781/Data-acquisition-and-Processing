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

// The gpio pins needed to be configured for this project
const GPIO_PinConfig_t gpio_pin_config[] = {

		// GPIO Pin configurations for I2C - FOR SCL line
		{
			gpiob_ptr,								// Register_ptr - Pointer to the structure B
			PIN6,									// Pin_number - Pin 6 of Port B
			PIN_ALTERNATE_FUNCTION,					// Mode - Alternate function mode
			PORTB,									// Port_number - Port B
			AF4,									// Alternate_function_number - AF4
			OUTPUT_OPEN_DRAIN,						// Output_type - Open drain
			HIGH_SPEED,								// output_speed - Set High speed
			NO_PULLUP_PULLDOWN,						// pullup_pulldown_config - No pullup or pulldown needed since it will come with the sensor

		},

		// GPIO Pin configurations for I2C - FOR SDA line
		{
			gpiob_ptr,								// Register_ptr - Pointer to the structure B
			PIN7,									// Pin_number - Pin 7 of Port B
			PIN_ALTERNATE_FUNCTION,					// Mode - Alternate function mode
			PORTB,									// Port_number - Port B
			AF4,									// Alternate_function_number - AF4
			OUTPUT_OPEN_DRAIN,						// Output_type - Open drain
			HIGH_SPEED,								// output_speed - Set High speed
			NO_PULLUP_PULLDOWN,						// pullup_pulldown_config - No pullup or pulldown needed since it will come with the sensor
		},
};

const uint8_t gpio_pin_config_size = sizeof(gpio_pin_config)/sizeof(gpio_pin_config[0]);

// ******************************  GPIO Configuration - END   *************************************************************

// ******************************  I2C Configuration - START *************************************************************

// Init configurations for I2C Module - Fixed - Configuration of I2C1
const I2C_Config i2c_config[] = {
		{
				I2C_1,						// Module number - Module number of I2C Module configured in the project
				i2c1_ptr,					// Module pointer - Pointer to the address of the register map
				STANDARD_MODE,				// i2c Speed mode - Standard or Fast mode
				80,							// CCR Value . Calculated based on the formula in datasheet . CCR = PCLK1 / (2 × I2C clock) = 16,000,000 / (2 × 100,000)
				17,							// Max Rise time. Calculated based on the formula . For standard speed , rise time is frequency + 1 = 16+1
				ACK_ENABLED,				// Acknoledgement is enabled
				CLOCK_STRECH_ENABLE,		// Clock streching is enabled
				ERROR_INTERRUPT_ENABLE,		// Enabled the interrupts logging on errors
				16,							// Peripheral clock frequency - 16 Mhz frequency
				BUFFER_INTERRUPT_DISABLE,	// Buffer interrupt is disabled - We dont need that since we are using DMA
				EVENT_INTERRUPT_ENABLE,		// Event interrupt is enabeld - We need to check events in ISR to write the next operations
				DMA_ENABLE,					// Enabled the DMA ( Make sure DMA is configured before enabling it )
				I2C_ENABLE,					// Enabled I2C . Last step , the peripheral will be working now
				&dma_stream_config[0],		// DMA_Stream_Config - Pointer to the configuration for DMA stream of I2CRX
				LAST_TRANSFER_ENABLED,		// set_last_enable_disable - Enabled - Enabled / Disabled the last transfer mode in I2C
		},
};

const uint8_t i2c_config_size = sizeof (i2c_config) / sizeof(i2c_config[0]);


// ******************************  I2C Configuration - END *************************************************************




///// ADC Buffer initialisation
//volatile uint32_t adc_measurement[3] = {0u};
//
//
//// ***********************************************************************************************
///// The adc channels needed to be configured for this project
//const ADC_Channel_config_t adc1_channel_config[] =
//{
//		{CHANNEL_1	, SAMPLES_480},
//		{CHANNEL_3	, SAMPLES_480},
//};
//const ADC_Channel_config_t adc2_channel_config[] =
//{
//		{CHANNEL_9	, SAMPLES_480},
//};
//
//const uint8_t adc1_channel_count = sizeof(adc1_channel_config)/sizeof(adc1_channel_config[0]);
//const uint8_t adc2_channel_count = sizeof(adc2_channel_config)/sizeof(adc2_channel_config[0]);
//
///// The ADC Modules needed to be configured for this project
//const ADC_Module_config_t adc_module_config[] = {
//		{	ADC1,	adc1_ptr,	ADC_RIGHT_ALIGN,	adc1_channel_config,	adc1_channel_count,	SCAN_MODE_ENABLED,	CONITNUOUS_MODE_ENABLED,	ADC_ENABLED,	EOC_AFTER_EACH_CONVERSION,	DMA_ENABLE,	},
//		{	ADC2,	adc2_ptr,	ADC_RIGHT_ALIGN,	adc2_channel_config,	adc2_channel_count,	SCAN_MODE_DISABLED,	CONITNUOUS_MODE_DISABLED,	ADC_ENABLED,	EOC_AFTER_EACH_CONVERSION,	DMA_ENABLE,	},
//};
//
//const uint8_t adc_module_config_size = sizeof(adc_module_config)/sizeof(adc_module_config[0]);

// *******************************************************************************************
/// The dma streams to be configured for this project
uint8_t mpu_6050_data[6];
const DMA_Stream_Config dma_stream_config[] =
{
		/// Configuration for the DMA1 - stream 0 - channel 1 for 12c1 RX
		{
				DMA1,											// module_number - Needed to do the clock init for specific module
				dma1_ptr,										// module_pointer - Pointer to the full register strucutre , Needed for accessing each modules
				STREAM0,										// dma_stream - Configured stream
				DMA1_STREAM0_IC21RX,							// channel_request - Application can request the channel needed , DMA will fetch the suitable channel based on the lookup table
				PERIPHERAL_TO_MEMORY,							// Direction - Peripheral to memory here since we need to copy the contents from I2C Dr to memory
				&(i2c1_ptr->DR),								// Peripheral address - The register address from where DMA need to copy the data
				&(mpu_6050_data[0]),							// memory_address - The data will be copied to here
				6,												// transfer_count - NDTR . Number of bytes to be transferred
				PERIPHERAL_DATA_REG_8BIT,						// psize - Peripheral data size ( Need to read 8bits from DR at a time)
				MEMORY_SIZE_8BIT,								// msize - Need to copy 8 bits to the memory at a time
				MEMORY_INCREMENT_ENABLE,						// memory_increment_mode - Enabled - Need to copy the data in all index
				PERIPHERAL_INCREMENT_DISABLE,					// peripheral_increment_mode - Disabled - We are taking everything from DR ( Common address )
				CIRCULAR_MODE_DISABLE,							// circular_mode - Disabled - No need of circular mode , DMA need to stop after completing the transfer .. we are stoping the i2c as well
				HIGH_PRIORITY,									// priority - Priority is given as high priority
				i2c_dma_complete,								// callback_ptr - Function which need to be called after completing i2c dma event
		},
};

const uint8_t dma_stream_count = sizeof (dma_stream_config) / sizeof(dma_stream_config[0]);


