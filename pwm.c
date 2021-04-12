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

/* defines*/
RCC_ClocksTypeDef RCC_Clocks;
/* vars */
static volatile uint32_t TimingDelay;
/* function prototypes */
void pwmConfig();
void Delay(uint32_t nTime);

/**
 **===========================================================================
 **
 **  Abstract: main program
 **
 **===========================================================================
 */

int main(void) {
	pwmConfig();

	while (1)
		;
	Delay(700);
	return 0;
}
void pwmConfig() {
	//a5, tim2
	//STAPPENPLAN PWM **************/
	// 1) enable clocks, using diffrent kinds of clocks, onderzoek his? (timer4 and gpiob)
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	// 2) declare init structures, gpio, timer, pwm
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	// 3) fill in GPIO init structures
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//pin is in, or ouput, but for pwm it needs to be alt
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//?
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	//pull up or pull down
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//4) connect timer to gpio
	/* Connect TIM4 pins to GPIOB Pin6 (blue Led) TIM4_CH1 function (zie STM32Ldiscovery manual tabel 7) */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_TIM9);

	//5) fill in time init structure
	TIM_TimeBaseStructure.TIM_Period = 1600;	//als je dit dus hoger doet is de period relatief korter
	TIM_TimeBaseStructure.TIM_Prescaler = 700;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM9, &TIM_TimeBaseStructure);

	//6)fill in pwm init, can also be used for other timing functions
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//pwm channel 1, onderzoek welke wanneer kunnen
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//onderzoek
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	//Waarde in het TIM4_CCR1 register (opgave: duty cycle up time 70%)
	TIM_OCInitStructure.TIM_Pulse = 420; //0,7 * 600 = 420; duty cycle meaning how much % pulse is of total period
	TIM_OC1Init(TIM9, &TIM_OCInitStructure);

	//7)gogogogo
	TIM_Cmd(TIM9, ENABLE);
	// Laat de timer counter lopen

	//8) om te gebruiken, oproepen is het enige wat nodig is
}

/**
 * @brief  Configures the different system clocks.
 * @param  None
 * @retval None
 */

void Delay(uint32_t nTime) {
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

