/*
 * STM32_UART.h
 *
 *  Created on: Dec 7, 2025
 *      Author: ANAS
 */

#ifndef INC_STM32_UART_H_
#define INC_STM32_UART_H_


#include <STM32_GPIO.h>
#include <STM32_RCC.h>
#include <stddef.h>


/// Function prototypes

typedef enum
{
	UART_BUFFER_FULL,
	UART_BUFFER_AVAILABLE
}Uart_status;

typedef struct usart_structure
{
    volatile uint32_t SR;     // Status register           (offset 0x00)
    volatile uint32_t DR;     // Data register             (offset 0x04)
    volatile uint32_t BRR;    // Baud rate register        (offset 0x08)
    volatile uint32_t CR1;    // Control register 1        (offset 0x0C)
    volatile uint32_t CR2;    // Control register 2        (offset 0x10)
    volatile uint32_t CR3;    // Control register 3        (offset 0x14)
    volatile uint32_t GTPR;   // Guard time/prescaler      (offset 0x18)
} usart_structure;


typedef struct UART_Module_Config
{
	usart_structure* module_pointer_UART;
	uint8_t module_number_UART;
	uint32_t baud_rate;
	uint8_t oversampling;
	uint32_t clock;

}UART_Module_Config;

#define USART2_BASEADDRESS	0x40004400U
#define TX_BUFFER_SIZE 1000


// Macros

#define SET_OVERSAMPLING_8_SAMPLE	1
#define SET_OVERSAMPLING_16_SAMPLE	0

/// Macros for module numbers
#define USART_MODULE2	2

/// Macros for module pointers
#define usart2_ptr		((usart_structure*)USART2_BASEADDRESS)


/// variables
extern uint8_t i;
extern char *string;
extern uint8_t uart_tx_busy;

extern char TX_Buffer_Linear[100];
extern char* read_pointer_Linear ;
extern char* write_pointer_Linear;


extern char TX_Buffer_circular[TX_BUFFER_SIZE];
extern char* read_pointer_circular;
extern char* write_pointer_circular;
extern volatile uint16_t current_count;


/// Function declarations

void uart_init(UART_Module_Config *config ,uint8_t uart_config_size);
void uart_set_baud_rate(usart_structure* uart_ptr, uint32_t baud_rate , uint8_t oversampling , uint32_t clock);
void uart_enable(usart_structure* uart_ptr, uint8_t oversampling);
void uart_print_interrupt_method(char* str);
void uart_write_producer_linear(const char* str);
uint8_t uart_write_consumer_linear (char *str);
Uart_status uart_write_producer_circular(const char*str);
uint8_t uart_write_consumer_circular(char* str);

void uart2_write_char(uint8_t charecter);
void uart_print(char* str);
uint8_t uart_read_char();
void uart_echo_test(uint8_t* ch , uint8_t size);
void itoa_custom(int32_t number , char* string);
void str_reverse_inplace(char*string);
void strcat_custom(char* buffer , char* string);



#endif /* INC_STM32_UART_H_ */
