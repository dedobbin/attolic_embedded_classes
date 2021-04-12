/*
*****************************************************************************
**
**  File        : main.c
**
**  Abstract    : Knipperlicht met timer 3.
**  Author      : R.Slokker
*
*/

/* Includes */
#include <stddef.h>
#include "stm32l1xx.h"
#include "discover_board.h"




 void Init_GPIOs (void);
 void RCC_Configuration(void);




/**
**===========================================================================
**
**  Abstract: main program
**
**===========================================================================
*/
 TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
 TIM_OCInitTypeDef  TIM_OCInitStructure;
 __IO uint16_t CCR1_Val = 18;



 /* Private function prototypes -----------------------------------------------*/
 void TIM_Config(void);



 int main(void)
 {
	 RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC| RCC_AHBPeriph_GPIOD| RCC_AHBPeriph_GPIOE| RCC_AHBPeriph_GPIOH, ENABLE);
	 Init_GPIOs();

	 //Zet de groene led aan
	 GPIOB->ODR=0x80;

	 /* TIM Configuration */
     TIM_Config();

     while (1);
 }



 void TIM_Config(void)
 {
	 NVIC_InitTypeDef NVIC_InitStructure;
	 uint16_t prescalervalue;

	 /* TIM3 clock enable */
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);


	     /* -----------------------------------------------------------------------
	      TIM3 Configuration: Output Compare Timing Mode:

	      In this example TIM3 input clock (TIM3CLK) is set to 2 * APB1 clock (PCLK1),
	      since APB1 prescaler is different from 1.
	        TIM3CLK = 2 * PCLK1
	        PCLK1 = HCLK / 4
	        => TIM3CLK = HCLK / 2 = SystemCoreClock /2

	      Note:
	       SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f4xx.c file.
	       Each time the core clock (HCLK) changes, user had to call SystemCoreClockUpdate()
	       function to update SystemCoreClock variable value. Otherwise, any configuration
	       based on this variable will be incorrect.
	    ----------------------------------------------------------------------- */


	    /* Compute the prescaler value */
	    prescalervalue = (uint16_t) ((SystemCoreClock / 2) / 500000) - 1;


	    /* Time base configuration */
	    // Vul in de maximale waarde van het timer counter register
	    TIM_TimeBaseStructure.TIM_Period = .........;

	    TIM_TimeBaseStructure.TIM_CounterMode = ............;
	    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	    /* Prescaler value is loaded immediate */
	    TIM_PrescalerConfig(TIM3, prescalervalue, TIM_PSCReloadMode_Immediate);

	    /* Output Compare Timing Mode configuration: Channel1 */
	    TIM_OCInitStructure.TIM_OCMode = ..........;
	    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;

	    TIM_OC1Init(TIM3, &TIM_OCInitStructure);



	    /*  Timer interrupt bij een update event  */
	    TIM_ITConfig(TIM3, ........ , ENABLE);

	    /* TIM3 enable counter */
	    TIM_Cmd(TIM3, ENABLE);



           /* Enable the TIM3 gloabal Interrupt */
           NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
           NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
           NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
           NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
           NVIC_Init(&NVIC_InitStructure);

 }


 void TIM3_IRQHandler(void)
  {
	 TIM_ClearITPendingBit(TIM3, TIM_IT_Update);

	 GPIO_TOGGLE(LD_PORT,LD_BLUE);
	 GPIO_TOGGLE(LD_PORT,LD_GREEN);
  }




 void  Init_GPIOs (void)
 {
   GPIO_InitTypeDef GPIO_InitStructure;

   /* Configure User Button pin as input */
   GPIO_InitStructure.GPIO_Pin = USER_GPIO_PIN;
   //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
   GPIO_Init(GPIOA, &GPIO_InitStructure);


   /* Configure the GPIO_LED pins  LD3 & LD4*/
   GPIO_InitStructure.GPIO_Pin = LD_GREEN|LD_BLUE;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
   GPIO_Init(GPIOB, &GPIO_InitStructure);
 }






