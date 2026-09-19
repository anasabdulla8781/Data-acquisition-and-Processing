/*
 * BMP_280.h
 *
 *  Created on: Mar 3, 2026
 *      Author: ANAS
 */

#ifndef BMP280_INC_BMP_280_H_
#define BMP280_INC_BMP_280_H_

#include <stdint.h>
#include <BMP_280.h>
#include "STM32_SPI.h"
#include "STM32_GPIO.h"


/// Strucutre for caliberation values
typedef struct
{
    /// Temperature calibration parameters
    uint16_t dig_T1;
    int16_t  dig_T2;
    int16_t  dig_T3;

    /// Pressure calibration parameters
    uint16_t dig_P1;
    int16_t  dig_P2;
    int16_t  dig_P3;
    int16_t  dig_P4;
    int16_t  dig_P5;
    int16_t  dig_P6;
    int16_t  dig_P7;
    int16_t  dig_P8;
    int16_t  dig_P9;
} calibration_structure;

/// Structure for configuration of the devices
typedef struct
{
	spi_structure* spi_ptr;
	GPIO_structure* gpio_ptr;
}BMP280_device;


typedef struct
{
	int32_t	raw_temp;
	int32_t raw_pressure;
}raw_measrement_structure;


/// Chip IDs
#define BMP280_CHIP_ID_REG     0xD0
#define BMP280_RESET_REG       0xE0
#define BMP280_STATUS_REG      0xF3
#define BMP280_CTRL_MEAS_REG   0xF4
#define BMP280_CONFIG_REG      0xF5

/// Read and write
#define BMP280_SPI_READ_MASK	0x80			// (Use or with BMP280_SPI_READ_MASK)
#define BMP280_SPI_WRITE_MASK  	0x7F			// (Use and with BMP280_SPI_WRITE_MASK)

// Compensation registers
#define BMP280_CALIBERATION_REG_START	0x88

// Raw data registers
#define BMP280_RAW_DATA_REG_START		0xF7

extern calibration_structure calib_data;
extern raw_measrement_structure raw_data;

extern int32_t bmp_280_temperature;

/// Function declarations ///
extern void bmp280_burst_read(spi_device* spi_config , uint8_t reg, uint8_t * rx_buffer , uint8_t length);
extern void bmp280_read_calibration(spi_device* spi_config , calibration_structure* calib_data);
extern void bmp280_read_raw_data(spi_device* spi_config ,raw_measrement_structure* raw_data);
extern int32_t bmp280_compensate_temperature(calibration_structure* calib_data, raw_measrement_structure* raw_data);
extern uint32_t bmp280_compensate_pressure(calibration_structure* calib_data, raw_measrement_structure* raw_data);
extern void bmp280_init(void);
extern void bmp280_get_temperature(void);
extern void bmp280_write_register(spi_device* spi_config , uint8_t reg , uint8_t value);

#endif /* BMP280_INC_BMP_280_H_ */
