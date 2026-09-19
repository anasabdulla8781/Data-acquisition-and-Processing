/*
 * STM32_UART.c
 *
 *  Created on: Dec 7, 2025
 *      Author: ANAS
 */


#include "STM32_UART.h"

uint8_t i;
char *string = NULL;
uint8_t uart_tx_busy = 0;

/// *******************  Driver configuration for UART - START *******************

void uart_init(UART_Module_Config *config ,uint8_t uart_config_size )
{
	for (uint8_t iter = 0 ; iter< uart_config_size ; iter++)
	{
		// Step 1 - Enable the UART clock ( We need to enable the clock for gpio as well where uart is connected . Make sure clock init function for GPIO Pins configured are called )
		usart_clock_enable(config[iter].module_number_UART);
		// Step 2 - Set the baud rate
		uart_set_baud_rate(config[iter].module_pointer_UART , config[iter].baud_rate , config[iter].oversampling , config[iter].clock);
		// Step 3 - Enable UART
		uart_enable(config[iter].module_pointer_UART, config[iter].oversampling);
	}
}

void uart_set_baud_rate(usart_structure* uart_ptr, uint32_t baud_rate , uint8_t oversampling , uint32_t clock)
{
	uint32_t internal_clock = clock;																	// Setting for internal clock
	float usartdiv = (float)internal_clock / ((float)baud_rate * (8 * (2 - oversampling)));				// Finding the usartdiv using the equation
	uint32_t mantassa = (uint32_t)usartdiv;
	uint32_t fraction;
	if (oversampling == 1)		/// for oversampling of 8
	{
		fraction = (uint32_t)(( (float)(usartdiv - mantassa) * 8) + 0.5);
		fraction &= 0x07;      // keep only 3 bits
	}
	else if (oversampling == 0)/// for oversampling of 16
	{
		fraction = (uint32_t)(( (float)(usartdiv - mantassa) * 16) + 0.5);
		fraction &= 0x0F;      // keep only 4 bits
	}
	uart_ptr->BRR = 0x0000;
	uart_ptr->BRR |= (mantassa << 4);
	uart_ptr->BRR |= fraction;

}

void uart_enable(usart_structure* uart_ptr, uint8_t oversampling)
{
	/// Step 1 - Clearing CR Register for first time
	uart_ptr->CR1 = 0;
	/// Step 2 - Setting the oversampling mode
	uart_ptr->CR1 |= (oversampling << 15);
	/// Step 3 - Enabling UART Rx and Tx
	uart_ptr->CR1 |=  1<<3;				/// Setting the uart TX
	uart_ptr->CR1 |=  1<<2;				/// Setting the uart RX
	// Enable the interrupt for Trasmission
	uart_ptr->CR1 |= 1<<7;				/// Interrupt is enabled
	/// Step 4 - Enabling UART module
	uart_ptr->CR1 |=  1<<13;				/// Setting the uart enable bit
}


// ******************** Driver configuration for UART - END ********************



// ********************* Data writing over UART - START ********************

/// Polling method

// Step 5 - Function for writing the data
void uart2_write_char(uint8_t charecter)
{
	while (!(usart2_ptr->SR & (1<<7)));
	usart2_ptr->DR = charecter;
}

// Step 6 - Function for writing the data as the whole string
void uart_print(char* str)
{
	while (*str)
	{
		uart2_write_char(*str++);
	}
	uart2_write_char('\r');
	uart2_write_char('\n');
}

// Interrrupt method

void uart_print_interrupt_method(char* str)
{
	if (uart_tx_busy == 0)
	{
		/// Copied the address of the str to string
		string = str ;
		/// We can make the interrupt enable if we are writing anything .. So that ISR will trigger if the DR is empty
		usart2_ptr->CR1 |= 1<<7;			///Enabled TXIE
		uart_tx_busy = 1;
	}
	else
	{
		// Do nothing .. Uart is busy , we can skip the write request
	}
}

// Step 7 - Function for reading from UART
uint8_t uart_read_char()
{
	while (!((usart2_ptr->SR) & (1 << 5)));
	return (uint8_t)(usart2_ptr->DR);
}



// ************************* Queue Implimentation for UART - START ***************************

/// Linear TX Buffer


char TX_Buffer_Linear[100];
char* read_pointer_Linear = TX_Buffer_Linear;
char* write_pointer_Linear = TX_Buffer_Linear;

void uart_write_producer_linear(const char* str)		/// Here str is the string we are trhing to write . it shouldnt modify in middle
{
	/// Calculate the string length
    uint8_t string_length = 0;
    while (str[string_length] != '\0')
    {
        string_length++;
    }

	/// Condition for filling the buffer
	if ((write_pointer_Linear >= (TX_Buffer_Linear+100)) || (TX_Buffer_Linear+100-write_pointer_Linear < (string_length+2)))
	{
		/// Buffer is already full . Exit the function without doing anything
		return;
	}
	else
	{
		while (*str)
		{
			*write_pointer_Linear++ = *str;
			str++;
		}
		*write_pointer_Linear++ = '\r';
		*write_pointer_Linear++ = '\n';
		usart2_ptr->CR1 |= 1<<7;			///Enabled TXIE
	}
}


uint8_t uart_write_consumer_linear (char *str)
{
	if (read_pointer_Linear >= write_pointer_Linear)
	{
		return 0;
	}
	else
	{
		*str = *read_pointer_Linear ++;
		return 1;
	}
}


/// Circular Buffer

char TX_Buffer_circular[TX_BUFFER_SIZE];
char* read_pointer_circular = TX_Buffer_circular;
char* write_pointer_circular = TX_Buffer_circular;
volatile uint16_t current_count = 0;

Uart_status uart_write_producer_circular(const char*str)
{
	/// Found the Legth of the string
	uint8_t stringlength = 0;
	while(str[stringlength] != '\0')
	{
		stringlength++;
	}

	/// If max size - Current count is less than (stringlength + 2) , we cant have that string in the buffer
	if ((TX_BUFFER_SIZE - current_count) < (stringlength + 2))
	{
		return UART_BUFFER_FULL;
	}

	/// Add all the elements in the buffer
	while (*str)
	{
		if (write_pointer_circular >= (TX_Buffer_circular + TX_BUFFER_SIZE))
		{
			write_pointer_circular = TX_Buffer_circular;
		}
		*write_pointer_circular++ = *str;
		current_count++;
		str++;
	}

	/// Handling the start of the line
	if (write_pointer_circular >= (TX_Buffer_circular + TX_BUFFER_SIZE))
	{
		write_pointer_circular = TX_Buffer_circular;
	}
	*write_pointer_circular++ = '\r';
	current_count++;

	/// Handling new line
	if (write_pointer_circular >= (TX_Buffer_circular + TX_BUFFER_SIZE))
	{
		write_pointer_circular = TX_Buffer_circular;
	}
	*write_pointer_circular++ = '\n';
	current_count++;

	/// Enable TXIE
	usart2_ptr->CR1 |= 1<<7;

	return UART_BUFFER_AVAILABLE;
}


uint8_t uart_write_consumer_circular(char* str)
{
	if (current_count == 0)
	{
		return 0;
	}

	/// To pass the charecter
	*str = *read_pointer_circular;
	/// To point to the next charecter
	if (read_pointer_circular == (TX_Buffer_circular + TX_BUFFER_SIZE - 1))
	{
		read_pointer_circular = TX_Buffer_circular;
	}
	else
	{
		read_pointer_circular++;
	}
	current_count--;
	return 1 ;
}

// ************************* Queue Implimentation for UART - END ***************************


void uart_echo_test(uint8_t* ch , uint8_t size)
{
    while (!(usart2_ptr->SR & (1<<5)));

    uint8_t c = (uint8_t)usart2_ptr->DR;

    if (c == '\r')
    {
        ch[i] = '\0';   // ✅ terminate at current index

        uart2_write_char('\r');
        uart2_write_char('\n');

        uart_print((char*)ch);

        i = 0;
    }
    else
    {
        if (i < (size - 1))
        {
            ch[i++] = c;     // store safely
        }

        uart2_write_char(c); // echo anyway
    }
}


void itoa_custom(int32_t number , char* string)
{
	/// The idea is , we will have a buffer called string , lets have it with size 100 . we will pass it to the function
	/// we will have the number as well .. that also we will pass

	/// We are making one by one byte to charecters .. so we will start from LSB .. thats easy approach
	uint8_t number_is_negative = 0;
	char last_digit;
	uint8_t index = 0;

	/// handling for negative number
	if (number < 0)
	{
		/// For a negative number , we will set it as positive for the moment ... for doing all the calculations .. but once every calculaiton is over , then we can consider the fact that it was negative
		number = -number ;
		/// We need to remember that it was a negative number .. then only we can consider it at the end
		number_is_negative = 1;
	}
	else if (number == 0)
	{
		string[index++] = '0';
		string[index] = '\0';
		return;

	}

	/// as long as we have some values , we will keep on converting each of the last bytes to charecters and store it
	while (number)
	{
		/// We took the last byte and converted it into the charecter
		last_digit = ((number %10) + '0');
		/// Added the last digit to the string array
		string[index ++] = last_digit;
		/// We removed the last byte from the number
		number = number /10;
	}

	/// We added all the bytes , make the string buffer to proper string
	if (number == 0)
	{
		//// Adding the sign also at the end ( We are adding each bytes from LSB . so the numbers will be in reverse order )
		if (number_is_negative)
		{
			string[index++] = '-';
		}
		/// adding the string ending charecter in the end
		string[index] = '\0';
	}

	/// Now we need to reverse the string
	str_reverse_inplace(string);
}

void str_reverse_inplace(char*string)
{
	char temp;
	int i = 0, j  = 0;

	while (string[j] != '\0')
	{
		/// We reached the end of the string . now j is the location of '\0' and i is in the starting
		j++;
	}
	/// we are not going to reverse the null charecter .. so moved j one step back
	j = j-1;

	/// interchange the charecters in such a way that i should be always less than J
	while (i < j)
	{
		/// Interchange the charecters
		temp = string[i];
		string[i] = string[j];
		string[j] = temp;
		/// Moving the indexes in both direction
		i++;
		j--;
	}
}



void strcat_custom(char* buffer , char* string)
{
	uint8_t index_buffer = 0;
	uint8_t index_string = 0;

	while (buffer[index_buffer] != '\0')
	{
		index_buffer ++;
	}

	while (string[index_string] != '\0')
	{
		buffer[index_buffer++] = string[index_string++];
	}
	buffer[index_buffer] = '\0';
}
