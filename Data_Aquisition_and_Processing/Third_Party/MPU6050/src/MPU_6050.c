/*
 * MPU_6050.c
 *
 *  Created on: Oct 3, 2026
 *      Author: ANAS
 */

#include <MPU_6050.h>


uint8_t who_am_i = 0;

I2C_Transaction transaction_6050_who_am_i  =
{
    .module_pointer = i2c1_ptr,
    .slave_address = 0x68,
    .register_address = 0x75,
    .direction = I2C_READ,
    .data_length = 1,
    .tx_buffer = NULL,
    .rx_buffer = &who_am_i
};
