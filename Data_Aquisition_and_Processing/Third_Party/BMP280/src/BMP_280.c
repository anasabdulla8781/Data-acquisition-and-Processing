/*
 * BMP_280.c
 *
 *  Created on: Mar 3, 2026
 *      Author: ANAS
 */

#include <stddef.h>
#include <BMP_280.h>
#include <STM32_SPI.h>

static int32_t t_fine;
calibration_structure calib_data;
raw_measrement_structure raw_data;
int32_t bmp_280_temperature;

void bmp280_burst_read(spi_device* spi_config , uint8_t reg, uint8_t * rx_buffer , uint8_t length)
{
	/// Saftty check
	if(spi_config == NULL || rx_buffer == NULL || length == 0)
	    return;
	/// STEP 1 - Send the CS bit low in the starting
	gpio_output_operations(spi_config->gpio_ptr,spi_config->cs_pin,SET_LOW);
	/// Added a small delay after making that pin
	for(volatile int i=0;i<200;i++);
	/// STEP 2 - Send the first byte address in read mode so that the reading will start . No need to read the return value . That will be garbage
	spi_transfer(spi_config->spi_ptr , (reg |BMP280_SPI_READ_MASK));
	/// STEP 3 - recieve the buffers in the bursting way
	for (uint16_t i=0 ; i<length ; i++)
	{
		rx_buffer[i] = spi_transfer(spi_config->spi_ptr , 0xFF);
	}
	/// STEP 4 - Completed the data transfer . Now make the CS pin high
	gpio_output_operations(spi_config->gpio_ptr,spi_config->cs_pin,SET_HIGH);
}

/// Reading compensation values
void bmp280_read_calibration(spi_device* spi_config , calibration_structure* calib_data)
{
	if(spi_config == NULL || calib_data == NULL)
	    return;
	uint8_t rx_buffer[24];
	uint8_t reg = BMP280_CALIBERATION_REG_START;
	/// Read the length number of bytes to rx buffer
	bmp280_burst_read(spi_config ,reg , rx_buffer ,24 );
	/// Fill the structure based on the rx buffer and caliberation structure
		/* Temperature calibration */
	calib_data->dig_T1 = (rx_buffer[1] << 8) | rx_buffer[0];
	calib_data->dig_T2 = (rx_buffer[3] << 8) | rx_buffer[2];
	calib_data->dig_T3 = (rx_buffer[5] << 8) | rx_buffer[4];
		/* Pressure calibration */
	calib_data->dig_P1 = (rx_buffer[7] << 8) | rx_buffer[6];
	calib_data->dig_P2 = (rx_buffer[9] << 8) | rx_buffer[8];
	calib_data->dig_P3 = (rx_buffer[11] << 8) | rx_buffer[10];
	calib_data->dig_P4 = (rx_buffer[13] << 8) | rx_buffer[12];
	calib_data->dig_P5 = (rx_buffer[15] << 8) | rx_buffer[14];
	calib_data->dig_P6 = (rx_buffer[17] << 8) | rx_buffer[16];
	calib_data->dig_P7 = (rx_buffer[19] << 8) | rx_buffer[18];
	calib_data->dig_P8 = (rx_buffer[21] << 8) | rx_buffer[20];
	calib_data->dig_P9 = (rx_buffer[23] << 8) | rx_buffer[22];
}

/// Reading raw adc values
void bmp280_read_raw_data(spi_device* spi_config ,raw_measrement_structure* raw_data)
{
	if(spi_config == NULL || raw_data == NULL )
		return;
	uint8_t rx_buffer[6];
	uint8_t reg = BMP280_RAW_DATA_REG_START;
	bmp280_burst_read(spi_config ,reg , rx_buffer ,6 );

	raw_data->raw_pressure = ( ((uint32_t)rx_buffer[0]) <<12 | ((uint32_t)rx_buffer[1]) <<4 | ((uint32_t)rx_buffer[2]) >> 4 );
	raw_data->raw_temp = 	 ( ((uint32_t)rx_buffer[3]) <<12 | ((uint32_t)rx_buffer[4]) <<4 | ((uint32_t)rx_buffer[5]) >> 4 );
}

// Copy of the compensate temperature function based on the data sheet
int32_t bmp280_compensate_temperature(calibration_structure* calib_data, raw_measrement_structure* raw_data)
{
	if(calib_data == NULL || raw_data == NULL)
	    return 0;
    int32_t var1, var2, T;

    var1 = ((((raw_data->raw_temp >> 3) -
             ((int32_t)calib_data->dig_T1 << 1))) *
            ((int32_t)calib_data->dig_T2)) >> 11;

    var2 = (((((raw_data->raw_temp >> 4) -
              ((int32_t)calib_data->dig_T1)) *
             ((raw_data->raw_temp >> 4) -
              ((int32_t)calib_data->dig_T1))) >> 12) *
            ((int32_t)calib_data->dig_T3)) >> 14;

    t_fine = var1 + var2;

    T = (t_fine * 5 + 128) >> 8;
    return T;
}

/// Copy of the compensate pressure function based on the data sheet
uint32_t bmp280_compensate_pressure(calibration_structure* calib_data, raw_measrement_structure* raw_data)
{
    if(calib_data == NULL || raw_data == NULL)
        return 0;

    int64_t var1, var2, p;
    int32_t adc_P = raw_data->raw_pressure;

    var1 = ((int64_t)t_fine) - 128000;

    var2 = var1 * var1 * (int64_t)calib_data->dig_P6;
    var2 = var2 + ((var1 * (int64_t)calib_data->dig_P5) << 17);
    var2 = var2 + (((int64_t)calib_data->dig_P4) << 35);

    var1 = ((var1 * var1 * (int64_t)calib_data->dig_P3) >> 8) +
           ((var1 * (int64_t)calib_data->dig_P2) << 12);

    var1 = (((((int64_t)1) << 47) + var1)) *
           ((int64_t)calib_data->dig_P1) >> 33;

    if (var1 == 0)
        return 0;   // avoid division by zero

    p = 1048576 - adc_P;

    p = (((p << 31) - var2) * 3125) / var1;

    var1 = (((int64_t)calib_data->dig_P9) *
           (p >> 13) * (p >> 13)) >> 25;

    var2 = (((int64_t)calib_data->dig_P8) * p) >> 19;

    p = ((p + var1 + var2) >> 8) +
        (((int64_t)calib_data->dig_P7) << 4);

    return (uint32_t)p;
}

void bmp280_init()
{
	/// Setting the sensor out of the sleep mode
	bmp280_write_register(&spi1_config , 0xF4 , 0x27);
	/// Reading the caliberation registers
	bmp280_read_calibration(&spi1_config , &calib_data);
}

void bmp280_get_temperature()
{
    uint8_t status;
    /// Wait until measurement completes
    do
    {
        bmp280_burst_read(&spi1_config, BMP280_STATUS_REG, &status, 1);
    }
    while(status & (1 << 3));
	/// Reading the raw registers adc reading
	bmp280_read_raw_data(&spi1_config ,&raw_data);
	if(raw_data.raw_temp == 0)
	    return;

	int32_t bmp_280_temp =  bmp280_compensate_temperature(&calib_data,&raw_data);
	bmp_280_temperature = bmp_280_temp;

}

void bmp280_write_register(spi_device* spi_config , uint8_t reg , uint8_t value)
{
	/// STEP 1 - Send the CS bit low in the starting
	gpio_output_operations(spi_config->gpio_ptr,spi_config->cs_pin,SET_LOW);
	for(volatile int i=0;i<50;i++);
	/// STEP 2 - Set the configuration address to write mode
	spi_transfer(spi_config->spi_ptr , (reg & BMP280_SPI_WRITE_MASK));
	/// STEP 3 - Send the actual data
	spi_transfer(spi_config->spi_ptr , value);
	/// STEP 4 - Make the pin high
	gpio_output_operations(spi_config->gpio_ptr,spi_config->cs_pin,SET_HIGH);

}
