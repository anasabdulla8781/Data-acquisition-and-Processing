/*
 * APP_feature.c
 *
 *  Created on: Nov 16, 2025
 *      Author: ANAS
 */


#include <APP_feature.h>
#include <service.h>


volatile uint8_t upcounter = 1;
volatile uint8_t bmp_280_ID = 0;

void fade_led_program(void)
{
    if (upcounter == 1)    // increasing brightness
    {
        gpt4_ptr->TIMx_CCR1++;
        gpt4_ptr->TIMx_CCR2++;
        gpt4_ptr->TIMx_CCR3++;
        gpt4_ptr->TIMx_CCR4++;

        if (gpt4_ptr->TIMx_CCR1 >= gpt4_ptr->TIMx_ARR)
        {
            upcounter = 0; // switch direction at MAX
        }
    }
    else                   // decreasing brightness
    {
        gpt4_ptr->TIMx_CCR1--;
        gpt4_ptr->TIMx_CCR2--;
        gpt4_ptr->TIMx_CCR3--;
        gpt4_ptr->TIMx_CCR4--;

        if (gpt4_ptr->TIMx_CCR1 == 0)
        {
            upcounter = 1; // switch direction at MIN
        }
    }
}


uint8_t app_spi_loopback_test (void)
{
	uint8_t rx_byte;
	/// This is a loopback test . So connect PA6 to PA7 for testing ( Mosi to Miso )
	gpio_output_operations(gpioa_ptr,SPI1_NSS,SET_LOW);
	spi_transfer(spi1_ptr, 0xD0); /// To read the ID
	rx_byte = spi_transfer(spi1_ptr, 0xFF);		// Dummy byte
	gpio_output_operations(gpioa_ptr,SPI1_NSS,SET_HIGH);

	return rx_byte;
}

void app_bmp_280_read_temp(void)
{
	bmp280_get_temperature();
	delay_ms_generic(500);
}

void app_bmp_read_id (void)
{
	gpio_output_operations(gpioa_ptr,SPI1_NSS,SET_LOW);
	spi_transfer(spi1_ptr, 0xD0); /// To read the ID
	bmp_280_ID = spi_transfer(spi1_ptr, 0xFF);		// Dummy byte
	gpio_output_operations(gpioa_ptr,SPI1_NSS,SET_HIGH);
}

