/**
 *****************************************************************************
 **
 **  File        : main.c
 **
 **  Abstract    : main function.
 **
 **  Functions   : main
 **
 **  Environment : Atollic TrueSTUDIO/STM32
 **                STMicroelectronics STM32Lxx Standard Peripherals Library
 **
 **  Distribution: The file is distributed “as is,” without any warranty
 **                of any kind.
 **
 **  (c)Copyright Atollic AB.
 **  You may use this file as-is or modify it according to the needs of your
 **  project. This file may only be built (assembled or compiled and linked)
 **  using the Atollic TrueSTUDIO(R) product. The use of this file together
 **  with other tools than Atollic TrueSTUDIO(R) is not permitted.
 **
 *****************************************************************************
 */
/* Includes */
#include <stddef.h>
#include "stm32l1xx.h"
#include "discover_board.h"
#include "stm32l_discovery_lcd.h"

/* defines and typedefs */
RCC_ClocksTypeDef RCC_Clocks;
/* vars */
static volatile uint32_t TimingDelay;
/* function prototypes */
void Delay(uint16_t);

/**
 **===========================================================================
 **
 **  Abstract: main program
 **  pin a5, tim2, dac
 **
 **===========================================================================
 */
uint16_t toconvert=3000;
uint16_t someval;
int debug;
void configPWM();
void configDAC();
void convertDAC(uint16_t);

int main(void) {
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	//Configure
	configPWM();
	configDAC();

	while (1) {
		ConvertDAC(toconvert);
		TIM2->CR1 =  ((float)DAC->DOR1/4096) * 600;;
		//TIM2->CR1 = 1;
		Delay(10000);

	}
}
void configPWM() {
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Connect TIM4 pins to GPIOB Pin6 (blue Led) TIM4_CH1 function (zie STM32Ldiscovery manual tabel 7) */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_TIM2 );

	/* Time base configuration */
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period = 600;
	TIM_TimeBaseStructure.TIM_Prescaler = 700;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	/* PWM1 Mode configuration: Channel1 */
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_Pulse = 300;
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);

	TIM_Cmd(TIM2, ENABLE);

}
void ConvertDAC(uint16_t value)
{

	    DAC_SetChannel1Data(DAC_Align_12b_R ,value);
	    //enable dac channel 1
		DAC->CR|=0x1;

}

void configDAC() {
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

	DAC_DeInit();
	DAC ->CR |= 0x38;
}

/**
 * @brief  Configures the different system clocks.
 * @param  None
 * @retval None
 */

void Delay(uint16_t nTime) {
	TimingDelay = nTime;

	while (TimingDelay != 0)
		;

}

/**
 * @brief  Decrements the TimingDelay variable.
 * @param  None
 * @retval None
 */
void TimingDelay_Decrement(void) {

	if (TimingDelay != 0x00) {
		TimingDelay--;
	}
}

