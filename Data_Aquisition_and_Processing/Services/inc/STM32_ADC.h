/*
 * STM32_ADC.h
 *
 *  Created on: Nov 20, 2025
 *      Author: uie83604
 */

#ifndef INC_STM32_ADC_H_
#define INC_STM32_ADC_H_

#include <stdint.h>
#include <STM32_RCC.h>
#include "STM32_GPIO.h"


typedef struct
{
    volatile uint32_t CSR;   // 0x00: Common status register
    volatile uint32_t CCR;   // 0x04: Common control register
    volatile uint32_t CDR;   // 0x08: Common regular data register
} ADC_Common_structure;

typedef struct ADC_structure
{
    volatile uint32_t SR;       // 0x00: Status register
    volatile uint32_t CR1;      // 0x04: Control register 1
    volatile uint32_t CR2;      // 0x08: Control register 2
    volatile uint32_t SMPR1;    // 0x0C: Sample time register 1 (channels 10–18)
    volatile uint32_t SMPR2;    // 0x10: Sample time register 2 (channels 0–9)

    volatile uint32_t JOFR1;    // 0x14: Injected channel data offset register 1
    volatile uint32_t JOFR2;    // 0x18: Injected channel data offset register 2
    volatile uint32_t JOFR3;    // 0x1C: Injected channel data offset register 3
    volatile uint32_t JOFR4;    // 0x20: Injected channel data offset register 4

    volatile uint32_t HTR;      // 0x24: Watchdog high threshold register
    volatile uint32_t LTR;      // 0x28: Watchdog low threshold register

    volatile uint32_t SQR1;     // 0x2C: Regular sequence register 1
    volatile uint32_t SQR2;     // 0x30: Regular sequence register 2
    volatile uint32_t SQR3;     // 0x34: Regular sequence register 3

    volatile uint32_t JSQR;     // 0x38: Injected sequence register

    volatile uint32_t JDR1;     // 0x3C: Injected data register 1
    volatile uint32_t JDR2;     // 0x40: Injected data register 2
    volatile uint32_t JDR3;     // 0x44: Injected data register 3
    volatile uint32_t JDR4;     // 0x48: Injected data register 4

    volatile uint32_t DR;       // 0x4C: Regular data register
} ADC_structure;

// ---------- ADC BASE ADDRESSES ----------
#define ADC1_BASEADDRESS         (0x40012000UL)
#define ADC2_BASEADDRESS         (0x40012100UL)
#define ADC3_BASEADDRESS         (0x40012200UL)
#define ADC_COMMON_BASEADDRESS   (0x40012300UL)


// ---------- POINTERS TO ADC STRUCTURES ----------
#define adc1_ptr      ((ADC_structure *) ADC1_BASEADDRESS)
#define adc2_ptr      ((ADC_structure *) ADC2_BASEADDRESS)
#define adc3_ptr      ((ADC_structure *) ADC3_BASEADDRESS)

// Pointer to ADC Common Block
#define adc_common_ptr   ((ADC_Common_structure *) ADC_COMMON_BASEADDRESS)


typedef struct adc_channel_config
{
	uint8_t channel_number;
	uint8_t sample_time;
}ADC_Channel_config_t;


typedef struct adc_module_config
{
	uint8_t module_number;
	ADC_structure* register_ptr;
	uint8_t alignment;
	const ADC_Channel_config_t* channel_ptr;
	uint8_t number_of_channels;
	uint8_t scan_mode;
	uint8_t continuous_mode;
	uint8_t enable;
	uint8_t EOC_bit;
	uint8_t dma_enable;
}ADC_Module_config_t;

/// Macros

	// Modes
#define EXTERNAL_CHANNEL	0
#define INTERNAL_CHANNEL_TEMPERATURE_SENSOR	1
#define INTERNAL_CHANNEL_VBAT	2

/// Macros for alignment
#define ADC_RIGHT_ALIGN	(0 << 11)
#define ADC_LEFT_ALIGN	(1 << 11)

/// Macros for Channels
#define CHANNEL_0   0
#define CHANNEL_1   1
#define CHANNEL_2   2
#define CHANNEL_3   3
#define CHANNEL_4   4
#define CHANNEL_5   5
#define CHANNEL_6   6
#define CHANNEL_7   7
#define CHANNEL_8   8
#define CHANNEL_9   9
#define CHANNEL_10  10
#define CHANNEL_11  11
#define CHANNEL_12  12
#define CHANNEL_13  13
#define CHANNEL_14  14
#define CHANNEL_15  15
#define CHANNEL_16  16
#define CHANNEL_17  17
#define CHANNEL_18  18


/// Macros for Sample time
#define SAMPLES_3     0
#define SAMPLES_15    1
#define SAMPLES_28    2
#define SAMPLES_56    3
#define SAMPLES_84    4
#define SAMPLES_112   5
#define SAMPLES_144   6
#define SAMPLES_480   7

/// Macros for scan mode
#define SCAN_MODE_DISABLED	0
#define SCAN_MODE_ENABLED	1

/// Macros for continous mode
#define CONITNUOUS_MODE_DISABLED	0
#define CONITNUOUS_MODE_ENABLED	1

/// Macros for enable or disable
#define ADC_DISABLED	0
#define ADC_ENABLED		1

/// Macros for EOC
#define EOC_AFTER_EACH_CONVERSION   1
#define EOC_AFTER_SEQUENCE          0


#define ADC_INDEPENDENT_MODE	0
#define ADC_MAX_VALUE	4095.00
#define MAX_VOLTAGE		3.3

/// Temperature sensor specific macros
#define ADC_VOLTAGE_AT_25_DEGREE	0.760
#define TEMPERATURE_SENSOR_SLOPE	0.0025
#define INNER_TEMPERATURE_SENSOR	1
#define REFERANCE_TEMPERATURE	25.00

/// EOS Related macros
#define ALL_CYCLE_CONVERSION	1
#define SEQUENCE_CONVERSION		2

/// MACROS for DMA enable
#define DMA_ENABLE		1
#define DMA_DISABLE	0

/// Macros for data register address ( Useful for DMA )
#define ADC1_DR		(uint32_t)( 0x40012000UL + 0x4CL)






/// Global function declarations
void adc_init_common(uint8_t mode);
void adc_convert_value(volatile uint16_t adc_measured_value, float* converted_value ,uint8_t mode);
void adc_get_value(ADC_structure * adc_ptr , volatile uint16_t* adc_measured_value);
void adc_init_module(ADC_structure * adc_ptr , uint8_t channel, uint8_t mode , uint8_t conversion_behaviour , uint8_t converion_order);


void adc_init(const ADC_Module_config_t* config , const uint8_t size);
void adc_set_alignment ( ADC_structure* adc_ptr , uint8_t alignment );
void adc_set_sample_time (ADC_structure* adc_ptr , ADC_Channel_config_t channel_ptr);
void adc_set_sequence_and_length(ADC_structure* adc_ptr , ADC_Channel_config_t channel_ptr, uint8_t conversion_order);
void adc_set_scan_mode(ADC_structure* adc_ptr , uint8_t scan_mode);
void adc_set_continuous_mode(ADC_structure* adc_ptr , uint8_t continuous_mode);
void adc_set_enable ( ADC_structure* adc_ptr , uint8_t enablebit );
void adc_start_conversion (ADC_structure* adc_ptr);
void adc_get_value(ADC_structure * adc_ptr , volatile uint16_t* adc_measured_value);
void adc_set_EOC ( ADC_structure* adc_ptr , uint8_t eoc_bit );
void adc_configure_dma(const ADC_Module_config_t* config);

/// Global variable declarions

#endif /* INC_STM32_ADC_H_ */
