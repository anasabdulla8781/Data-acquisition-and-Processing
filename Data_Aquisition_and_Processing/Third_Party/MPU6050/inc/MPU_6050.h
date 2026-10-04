/*
 * MPU_6050.h
 *
 *  Created on: Oct 3, 2026
 *      Author: ANAS
 */


#ifndef MPU6050_INC_MPU_6050_H_
#define MPU6050_INC_MPU_6050_H_


#include "stdint.h"
#include "service.h"
#include "MPU_6050_Config.h"

// Macros for the device address ( BASED ON AD0 )

#define MPU_6050_ADDRESS_1	0x68
#define MPU_6050_ADDRESS_2	0x69

// Macros for register address
#define Who_am_i_register	0x75


// Variables

// RX buffer
extern uint8_t mpu_6050_rx_buffer[MAX_BUFFER_SIZE];

#endif /* MPU6050_INC_MPU_6050_H_ */
