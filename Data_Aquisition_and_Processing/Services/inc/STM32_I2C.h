/*
 * STM32_I2C.h
 *
 *  Created on: Feb 21, 2026
 *      Author: ANAS
 */

#ifndef INC_STM32_I2C_H_
#define INC_STM32_I2C_H_


#include <stdint.h>

/// I2C Memory map strucutre

typedef struct
{
	volatile uint32_t	CR1;			/// Control register 1
	volatile uint32_t	CR2;			/// Control register 2
	volatile uint32_t	OAR1;
	volatile uint32_t	OAR2;
	volatile uint32_t	DR;
	volatile uint32_t	SR1;
	volatile uint32_t	SR2;
	volatile uint32_t	CCR;
	volatile uint32_t	TRISE;
	volatile uint32_t	FLTR;
}i2c_structure;


#define I2C1_BASEADDRESS	0x40005400
#define I2C2_BASEADDRESS	0x40005800
#define I2C3_BASEADDRESS	0x40005C00

#define i2c1_ptr	((i2c_structure*)I2C1_BASEADDRESS)
#define i2c2_ptr	((i2c_structure*)I2C2_BASEADDRESS)
#define i2c3_ptr	((i2c_structure*)I2C3_BASEADDRESS)

// MACROS

/// Differant I2C Chips
#define I2C_1	1
#define I2C_2	2
#define I2C_3	3


#endif /* INC_STM32_I2C_H_ */
