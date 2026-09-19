/*
 * APP_feature.h
 *
 *  Created on: Nov 16, 2025
 *      Author: ANAS
 */

#ifndef INC_APP_FEATURE_H_
#define INC_APP_FEATURE_H_

#include <stdint.h>

extern volatile uint8_t upcounter;
extern volatile uint8_t bmp_280_ID;

extern void fade_led_program(void);
extern uint8_t app_spi_loopback_test (void);
extern void app_bmp_280_read_temp(void);
extern void app_bmp_read_id (void);

#endif /* INC_APP_FEATURE_H_ */
