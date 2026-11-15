/*
 * MPU_6050_Config.h
 *
 *  Created on: Oct 4, 2026
 *      Author: ANAS
 */

#ifndef MPU6050_INC_MPU_6050_CONFIG_H_
#define MPU6050_INC_MPU_6050_CONFIG_H_

// Address of i2c bus where its connected - CHANGE THIS MACRO BASED ON WHERE THE SENSOR IS CONFIGURED IN PROJECT
#define CONNECTED_I2C_BUS	i2c1_ptr

// Macros for the buffer size of the RX Array - CHANGE THE SIZE INCASE PROJECT NEED SOME MORE BIGGER ARRAY
#define MAX_BUFFER_SIZE	50

#endif /* MPU6050_INC_MPU_6050_CONFIG_H_ */
