/*
 * STM32_NVIC.c
 *
 *  Created on: Nov 10, 2025
 *      Author: ANAS
 */


#include <STM32_NVIC.h>
#include <STM32_GPT.h>
#include <STM32_GPIO.h>
#include <APP_feature.h>
#include <STM32_EXTI.h>
#include "APP_task.h"
#include "STM32_UART.h"


uint32_t tim2_interrupt_count;
uint8_t eight_s_delay;
uint8_t twelve_s_delay;

uint32_t button_count;




void nvic_init(uint8_t interrupt_number)
{
	switch(interrupt_number)
	{
		case 28:
			/// Setting the priority for this interrupt
			nvic_ptr->IPR[28] = (5 << 4);
			/// Enabling the interrupt
			nvic_ptr->ISER[0] |= ENABLE_TIMER2_INTERRUPT;
			break;

		case 30:
			nvic_ptr->ISER[0] |= ENABLE_TIMER4_INTERRUPT;
			break;

		case 6:
			/// Setting the priority
			nvic_ptr->IPR[6] = (5 << 4);
			// enabling the interrput
			nvic_ptr->ISER[0] |= ENABLE_EXTIO_INTERRUPT;
			break;

		case 38:
			nvic_ptr->ISER[1] |= ENABLE_USART2_INTERRUPT;

		default:
			break;

	}
}

void TIM2_IRQHandler(void)
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	if (gpt2_ptr->TIMx_SR & 1U) 			/// Last bit in the SR is 1 indicating there is an interrupt happened
	{
		/// Cleared the interrupt
		gpt2_ptr->TIMx_SR &= ~(1U << 0);
//		///Counted the interrupt
//		tim2_interrupt_count++;
//		/// Connect to the task1ms .. and share the counter to print
//
//		if (task1Handle != NULL)
//		{
//			xTaskNotifyFromISR(task1Handle,tim2_interrupt_count,eSetValueWithOverwrite,&xHigherPriorityTaskWoken);
//			// Perform context switching if needed
//			portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
//		}



	}
}

void TIM4_IRQHandler(void)
{
	if(gpt4_ptr->TIMx_SR &1u)
	{
		gpt4_ptr->TIMx_SR &= ~(1U << 0);

		fade_led_program();
	}
}

void EXTI0_IRQHandler(void)
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	measurement isr_measure;
	if (exti_ptr->PR & (1<<0))
	{
		/// Cleared the interrupt happened
		exti_ptr->PR |= (1<<0);

		/// Disable EXTI0 temporarily
		exti_ptr->IMR &= ~(1<<0);

		/// Give notification to the tasks
		vTaskNotifyGiveFromISR(buttontaskHandle,&xHigherPriorityTaskWoken);

		/// Debounce delay
		for(volatile uint32_t i=0; i<20000; i++);
//
		/// Enable EXTI0 again
		exti_ptr->IMR |= (1<<0);
		// Perform context switching if needed
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}
}


void USART2_IRQHandler(void)
{
	char str;

	/// Check whether the TXE bit is set or not
	if (usart2_ptr->SR & (1<<7))
	{
		if (uart_write_consumer_circular(&str))
		{
			usart2_ptr->DR = str;
		}
		else
		{
			/// Disable the interrupt .. Ohterwise this ISR will be always calling
			usart2_ptr->CR1 &= ~(1<<7);
		}
	}
}
