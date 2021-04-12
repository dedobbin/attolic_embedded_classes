/**
*****************************************************************************
**
**  File        : main.c
**
**  Abstract    : PWM on pin PC6 of the STM32L discovery.
**  Auther:       R. Slokker
**/



/* Includes */
#include <stddef.h>
#include "stm32l1xx.h"
#include "discover_board.h"






/* Private function prototypes */
/* Private functions */
#ifdef USE_STM32L_DISCOVERY
 void Init_GPIOs (void);
 void  TIM_Config();
 void Delay(uint32_t nTime);

 static volatile uint32_t TimingDelay;
 RCC_ClocksTypeDef RCC_Clocks;

 void Config_Systick()
 {
   RCC_GetClocksFreq(&RCC_Clocks);
   SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);
 }
#endif

/**
**===========================================================================
**
**  Abstract: main program
**
**===========================================================================
*/

 TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
 TIM_OCInitTypeDef  TIM_OCInitStructure;
 uint16_t CCR1_Val =20;


int main(void)
{
  uint16_t button;
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC| RCC_AHBPeriph_GPIOD| RCC_AHBPeriph_GPIOE| RCC_AHBPeriph_GPIOH, ENABLE);


  /* Init I/O ports for the blue LED, green LED and user button*/
  Init_GPIOs();

  /* Switch on the leds at start */
  GPIOB->ODR|=0xC0;
  // of GPIO_HIGH(GPIOB,LD_GREEN);
  //    GPIO_HIGH(GPIOB,LD_BLUE);

  TIM_Config();


  /* Infinite loop */
  while (1)
  {

	button=(GPIOA->IDR)&0x1;;
	if((GPIOA->IDR&0x1))
	   {  GPIOB->ODR^=0x40;
		  // of GPIO_TOGGLE(LD_PORT,LD_BLUE);
	      if(TIM3->CCR1==20) TIM3->CCR1=620; else TIM3->CCR1=20;
        }


  };

  return 0;
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



/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in 10 ms.
  * @retval None
  */


void Delay(uint32_t nTime)
{
  TimingDelay = nTime;

  while(TimingDelay != 0);

}







void TIM_Config(void)
 {
   GPIO_InitTypeDef GPIO_InitStructure;

   /* TIM3 clock enable */
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

   /* GPIOD , GPIOC and GPIOB clock enable */
   RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
   GPIO_Init(GPIOC, &GPIO_InitStructure);



   /* Connect TIM3 pins to AF2 */
   GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);


   /* Time base configuration */
   TIM_TimeBaseStructure.TIM_Period = 665;
   TIM_TimeBaseStructure.TIM_Prescaler = 70;
   TIM_TimeBaseStructure.TIM_CounterMode = ..........;
   TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);


   /* PWM1 Mode configuration: Channel1 */
   TIM_OCInitStructure.TIM_OCMode = ..............;

   TIM_OCInitStructure.TIM_OutputState = ............;
   // TIM3_CCER TIMx capture/compare enable register   bit nummer 0
   TIM_OCInitStructure.TIM_OCPolarity = ..............;
   // TIM3_CCER TIMx capture/compare enable register   bit nummer 1

   TIM_OCInitStructure.TIM_Pulse = ............;
   //Waarde in het TIM3_CCR1 register

   TIM_OC1Init(TIM3, &TIM_OCInitStructure);

   TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
   //De waarde in CCR1_VAL wordt via een preload register geupdate

   TIM_Cmd(TIM3,ENABLE);
   // Laat de timer counter lopen
 }
