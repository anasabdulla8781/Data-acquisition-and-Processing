
/*
 * STM32_GPT.c
 *
 *  Created on: Oct 19, 2025
 *      Author: ANAS
 */


#include <STM32_GPT.h>
#include <stdint.h>


void timer2_init(void)
{
	rcc_ptr->APB1ENR |= SET_TIM2_CLOCK_ENABLE;						/// enabled the clock of TIMER2
	gpt2_ptr->TIMx_CR1 |= ENABLE_ARR_BUFFER;						/// Enabling ARR buffering ( will not update all of a sudden )
	gpt2_ptr->TIMx_DIER |= ENABLE_UPDATE_INTERRUPT;					/// Enabling the update interrupt
}

void timer2_set(uint32_t arr , uint32_t psc)
{
	gpt2_ptr->TIMx_ARR = arr;										/// Setting auto reload register value
	gpt2_ptr->TIMx_PSC = psc;										/// Setting prescalar value
	gpt2_ptr->TIMx_EGR |= 1;          								/// <<< Generate update event (IMPORTANT)
	gpt2_ptr->TIMx_CR1 |= TIMER_ON;									/// Turning on the timer (counter enabled)
}

void timer4_init(void)
{
	rcc_ptr->APB1ENR |= SET_TIM4_CLOCK_ENABLE;						/// Enabled the clock of TIMER4
	gpt4_ptr->TIMx_CR1 |= ENABLE_ARR_BUFFER;						/// Enabling ARR buffering ( will not update all of a sudden )
	gpt4_ptr->TIMx_DIER |= ENABLE_UPDATE_INTERRUPT;					/// Enabling the update interrupt
}

void timer4_set_generic(uint32_t freqeuncy , uint32_t prescalar)
{
	gpt4_ptr->TIMx_ARR = freqeuncy - 1;										/// Setting auto reload register value
	gpt4_ptr->TIMx_PSC = prescalar;											/// Setting prescalar value
}

void timer4_set_pwm(uint8_t channel)
{
	if(channel == 1)
	{
		gpt4_ptr->TIMx_CCMR1 &= ~ (0x03 << 0);					/// setting the PWM mode in the Capture compare select bits
		gpt4_ptr->TIMx_CCMR1 &= ~ (0x0c << 0);					/// clearing the bits for fast enable and buffering from other register
		gpt4_ptr->TIMx_CCMR1 |=   (0x08 << 0);					/// setting the buffering
		gpt4_ptr->TIMx_CCMR1 &= ~ (0x70 << 0);					/// Clearing output compare mode
		gpt4_ptr->TIMx_CCMR1 |=   (0x60 << 0);					/// Setting in PWM mode 1
		gpt4_ptr->TIMx_CCER  &= ~ (0x03 << 0);					/// Clearing the bits for enabling and polarity
		gpt4_ptr->TIMx_CCER  |=   (0x01 << 0);					/// Setting enable bit for timer4 channel 1 to handle the connected pin
		gpt4_ptr->TIMx_CCR1   = 0;								/// Setting the CCR register to 0 initially

	}
	else if (channel == 2)
	{
		gpt4_ptr->TIMx_CCMR1 &= ~ (0x03 << 8);					/// setting the PWM mode in the Capture compare select bits
		gpt4_ptr->TIMx_CCMR1 &= ~ (0x0c << 8);					/// clearing the bits for fast enable and buffering from other register
		gpt4_ptr->TIMx_CCMR1 |=   (0x08 << 8);					/// setting the buffering
		gpt4_ptr->TIMx_CCMR1 &= ~ (0x70 << 8);					/// Clearing output compare mode
		gpt4_ptr->TIMx_CCMR1 |=   (0x60 << 8);					/// Setting in PWM mode 1
		gpt4_ptr->TIMx_CCER  &= ~ (0x03 << 4);					/// Clearing the bits for enabling and polarity
		gpt4_ptr->TIMx_CCER  |=   (0x01 << 4);					/// Setting enable bit for timer4 channel 2 to handle the connected pin
		gpt4_ptr->TIMx_CCR2   = 0;								/// Setting the CCR register to 0 initially
	}
	else if (channel == 3)
	{
		gpt4_ptr->TIMx_CCMR2 &= ~ (0x03 << 0);					/// setting the PWM mode in the Capture compare select bits
		gpt4_ptr->TIMx_CCMR2 &= ~ (0x0c << 0);					/// clearing the bits for fast enable and buffering from other register
		gpt4_ptr->TIMx_CCMR2 |=   (0x08 << 0);					/// setting the buffering
		gpt4_ptr->TIMx_CCMR2 &= ~ (0x70 << 0);					/// Clearing output compare mode
		gpt4_ptr->TIMx_CCMR2 |=   (0x60 << 0);					/// Setting in PWM mode 1
		gpt4_ptr->TIMx_CCER  &= ~ (0x03 << 8);					/// Clearing the bits for enabling and polarity
		gpt4_ptr->TIMx_CCER  |=   (0x01 << 8);					/// Setting enable bit for timer4 channel 3 to handle the connected pin
		gpt4_ptr->TIMx_CCR3   = 0;								/// Setting the CCR register to 0 initially
	}
	else if (channel == 4)
	{
		gpt4_ptr->TIMx_CCMR2 &= ~ (0x03 << 8);					/// setting the PWM mode in the Capture compare select bits
		gpt4_ptr->TIMx_CCMR2 &= ~ (0x0c << 8);					/// clearing the bits for fast enable and buffering from other register
		gpt4_ptr->TIMx_CCMR2 |=   (0x08 << 8);					/// setting the buffering
		gpt4_ptr->TIMx_CCMR2 &= ~ (0x70 << 8);					/// Clearing output compare mode
		gpt4_ptr->TIMx_CCMR2 |=   (0x60 << 8);					/// Setting in PWM mode 1
		gpt4_ptr->TIMx_CCER  &= ~ (0x03 << 12);					/// Clearing the bits for enabling and polarity
		gpt4_ptr->TIMx_CCER  |=   (0x01 << 12);					/// Setting enable bit for timer4 channel 4 to handle the connected pin
		gpt4_ptr->TIMx_CCR4   = 0;								/// Setting the CCR register to 0 initially
	}
}

void timer4_start(void)
{
	gpt4_ptr->TIMx_EGR |= 1;          								/// <<< Generate update event (IMPORTANT)
	gpt4_ptr->TIMx_CR1 |= TIMER_ON;									/// Turning on the timer (counter enabled)
}

void delay_ms_generic(uint32_t ms)
{
    volatile uint32_t count;

    while(ms--)
    {
        count = 8000;

        while(count--)
        {
            __asm__("nop");
        }
    }
}

