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
#define nope
/* vars */
static volatile uint32_t TimingDelay;
/* function prototypes */
void Delay( uint16_t);

/**
 **===========================================================================
 **
 **  Abstract: main program
 **
 **===========================================================================
 */
int main(void) {
	//Configure
	int i = 0;
#ifdef dingen
	char *p = (char*) 0x40023800; //pointer naar adres 40023800 is reset control register
	int *q = (int *) 0x40020400;//int pointer naar port B, base adres
	*((int*) (p + 28)) |= 0x3f;//zet porten A B C D E en aan
	*((int*) (p + 32)) |= 1;//zet  sysconfig controller clock aan
	*q = 0x5000;//q point naar base adres van GPIOB, en zet de register op 0101 0000 0000 0000, dus pin b7 en b8 zijn nu output
	while (1) {
		*(int*) (0x40020414) ^= 0x80;
		//flip een gpio pin hoog en laag, lampje
		while(i++<100000);//delay
		i = 0;}
#endif
#ifndef dingen
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	GPIOB ->MODER = 0x5000;
	while (1) {
		GPIOB->ODR ^= 0x80;
		while (i++ < 100000)
			; //delay
		i = 0;
	}

#endif

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

