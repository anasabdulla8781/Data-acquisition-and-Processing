/*
 * MPU_6050.c
 *
 *  Created on: Oct 3, 2026
 *      Author: ANAS
 */

#include <MPU_6050.h>

uint8_t mpu_6050_rx_buffer[MAX_BUFFER_SIZE] = {0};


I2C_Transaction transaction_6050_who_am_i =
{
	.module_pointer = CONNECTED_I2C_BUS,
	.data_length = 1,
	.direction = I2C_READ,
	.result_array = mpu_6050_rx_buffer,
	.slave_address = 0x68,
	.register_address = 0x75,
};


