/*
 * STM32_rcc_ptr.c
 *
 *  Created on: Oct 20, 2025
 *      Author: ANAS
 */


#include <STM32_RCC.h>
#include <STM32_SPI.h>
#include <STM32_UART.h>


uint8_t reset_reason = 0;
char reset_reason_string[50] = {0};
char temp_csr_print[50] = {0};


void LSI_clock_enable (void)
{
	rcc_ptr->CSR |= 1<<0;					/// Set the LSI clock
	while (!(rcc_ptr->CSR & (1<<1)));
}


// Function to enable the clock for UART module . We need to enable the clock of USART2 module and GPIOA module
void usart_clock_enable(uint8_t module_number)
{
	switch(module_number)
	{
	case USART_MODULE2:
		rcc_ptr->APB1ENR |= 1<<17;				/// Set the USART2 clock enable bit
		break;
	default:
		break;
	}
}


void i2c_clock_enable(uint8_t module)
{
	if ((module <= I2C_3) && (module >= I2C_1))
	{
		rcc_ptr->APB1ENR |= (1<<(21+module));						/// Enabled the clock for I2C1 Module
	}
	else
	{
		/// Do nothing
	}
}

void gpio_clock_enable(uint8_t gpio_module)
{

	if (gpio_module >= GPIOA && gpio_module <= GPIOI)
	{
		rcc_ptr->AHB1ENR |= 1<<(GPIO_CLOCK_ENABLE + gpio_module);							//// Enabled the clock for GPIO Module
	}
	else
	{
		;
	}
}


void adc_clock_enable(uint8_t adc_module)
{

	if (( adc_module <= ADC3) && (adc_module>=ADC1) )
	{
		rcc_ptr->APB2ENR |= (1 << (adc_module + ADC_CLOCK_ENABLE));   							/// Enabled ADC clock
	}
	else
	{
		;
	}
}

void spi_clock_enable(uint8_t module)
{
	switch(module)
	{
	case SPI1:
		rcc_ptr->APB2ENR |= (1<<12);
		break;
	case SPI2:
		rcc_ptr->APB1ENR |= (1<<14);
		break;
	case SPI3:
		rcc_ptr->APB1ENR |= (1<<15);
		break;
	default:
		break;
	}
}

void reset_reason_check( volatile uint8_t *reset_reason_ptr, char *reset_reason_string)
{
    uint32_t csr = rcc_ptr->CSR;
    itoa_custom(csr , temp_csr_print);

    const char *message;

    if (csr & (1U << 29))
    {
        *reset_reason_ptr = INDEPENDENT_WATCHDOG_RESET;
        message = "INDEPENDENT_WATCHDOG_RESET";
    }
    else if (csr & (1U << 30))
    {
        *reset_reason_ptr = WINDOW_WATCHDOG_RESET;
        message = "WINDOW_WATCHDOG_RESET";
    }
    else if (csr & (1U << 27))
    {
        *reset_reason_ptr = POWER_ON_RESET;
        message = "POWER_ON_RESET";
    }
    else if (csr & (1U << 25))
    {
        *reset_reason_ptr = BROWN_OUT_RESET;
        message = "BROWN_OUT_RESET";
    }
    else if (csr & (1U << 28))
    {
        *reset_reason_ptr = SOFTWARE_RESET;
        message = "SOFTWARE_RESET";
    }
    else if (csr & (1U << 26))
    {
        *reset_reason_ptr = RESET_BUTTON_RESET;
        message = "RESET_BUTTON_RESET";
    }
    else
    {
        *reset_reason_ptr = UNKNOWN_RESET;
        message = "UNKNOWN_RESET";
    }

    while (*message != '\0')
    {
        *reset_reason_string++ = *message++;
    }

    *reset_reason_string++ = '\0';
    *reset_reason_string++ = '\r';
    *reset_reason_string = '\n';

    rcc_ptr->CSR |= (1U << 24);
}

void clock_enable_HSE(void)											/// Default HSE Clock is coming with 8MHZ . that improves the accuracy
{
	// 1 . Enabling the HSE Clock
	rcc_ptr->CR |= 1<<16;

	// 2 . Wait until HSE oscillater based clock is ready
	while (!(rcc_ptr->CR & (1 << 17)));

    // 3. Clear SW[1:0] bits
    rcc_ptr->CFGR &= ~(3 << 0);

    // 4. Select HSE (01)
    rcc_ptr->CFGR |= (1 << 0);

    // 5. Wait until switch is done (VERY IMPORTANT)
    while (!((rcc_ptr->CFGR &  0x0C ) == 0x04));
}

void syscfg_clock_enable(void)
{
	rcc_ptr->APB2ENR |= (1<<14);
}

void dma_clock_enable(uint8_t module)
{
	rcc_ptr->AHB1ENR |= 1<< (20 + module);
}


void print_reset_reason (void)
{
	/// Temporary buffer to store what to print
	char temporary_buffer[100] = {0};

	strcat_custom(temporary_buffer , "        ");
	strcat_custom(temporary_buffer , reset_reason_string);
	strcat_custom(temporary_buffer , "        ");
	uart_write_producer_circular(temporary_buffer);
    uart_write_producer_circular("========================================");
	temporary_buffer[0] = '\0';
}




