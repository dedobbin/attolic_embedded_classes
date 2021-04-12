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

/* Private typedef */
/* Private define  */

#define MESSAGE5   " program built with "
#define MESSAGE6   " Atollic TrueSTUDIO "

/* Private macro */
/* Private variables */
USART_InitTypeDef USART_InitStructure;

/* Private function prototypes */
/* Private functions */
#ifdef USE_STM32L_DISCOVERY
void Init_GPIOs(void);
void RCC_Configuration(void);

static volatile uint32_t TimingDelay;
RCC_ClocksTypeDef RCC_Clocks;

void Config_Systick() {
	RCC_GetClocksFreq(&RCC_Clocks);
	SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);
}
#endif

void Delay(__IO uint32_t nCount);

void ADC_Configuration(void);
uint16_t readADC1();

/**
 **===========================================================================
 **
 **  Abstract: main program
 **
 **===========================================================================
 */
int main(void) {

	/**
	 *  IMPORTANT NOTE!
	 *  See the <system_*.c> file and how/if the SystemInit() function updates
	 *  SCB->VTOR register. Sometimes the symbol VECT_TAB_SRAM needs to be defined
	 *  when building the project if code has been located to RAM and interrupts
	 *  are used. Otherwise the interrupt table located in flash will be used.
	 *  E.g.  SCB->VTOR = 0x20000000;
	 */

	/**
	 *  At this stage the microcontroller clock setting is already configured,
	 *  this is done through SystemInit() function which is called from startup
	 *  file (startup_stm32l1xx_md.s) before to branch to application main.
	 *  To reconfigure the default setting of SystemInit() function, refer to
	 *  system_stm32l1xx.c file
	 */

	/* TODO - Add your application code here */

	RCC_Configuration();
	/* Init I/O ports */
	Init_GPIOs();

	/* Init Systick */
	Config_Systick();

	/* Initializes the LCD glass */
	LCD_GLASS_Init();
	LCD_GLASS_DisplayString((uint8_t*) "STM32L-DISCOVERY");
	LCD_GLASS_ScrollSentence((uint8_t*) "  ** Atollic TrueSTUDIO **  ", 3, 200);

	/* Switch on the leds at start */
	GPIO_HIGH(LD_PORT, LD_GREEN);
	GPIO_HIGH(LD_PORT, LD_BLUE);

	/* the stuff */
	uint16_t analogresult = 0;

	GPIO_InitTypeDef GPIO_InitStructure;

	/**
	 *  IMPORTANT NOTE!
	 *  The symbol VECT_TAB_SRAM needs to be defined when building the project
	 *  if code has been located to RAM and interrupts are used.
	 *  Otherwise the interrupt table located in flash will be used.
	 *  See also the <system_*.c> file and how the SystemInit() function updates
	 *  SCB->VTOR register.
	 *  E.g.  SCB->VTOR = 0x20000000;
	 */

	/* TODO - Add your application code here */

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

	/* Configure the GPIO_LED pin */
	GPIO_InitStructure.GPIO_Pin = LD_GREEN | LD_BLUE;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_32MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	ADC_Configuration();

	GPIOB ->ODR |= 0xC0;
	GPIOB ->ODR ^= 0x40;

	/* Infinite loop */
	while (1) {

		Delay(0xFFFF);

		analogresult = readADC1();
		if (analogresult < 100)
			GPIOB ->ODR ^= 0x40;

	}

	return 0;
}

/*
 * Minimal __assert_func used by the assert() macro
 * */
void __assert_func(const char *file, int line, const char *func,
		const char *failedexpr) {
	while (1) {
	}
}

/*
 * Minimal __assert() uses __assert__func()
 * */
void __assert(const char *file, int line, const char *failedexpr) {
	__assert_func(file, line, NULL, failedexpr);
}

/**
 * @brief  Configures the different system clocks.
 * @param  None
 * @retval None
 */
void RCC_Configuration(void) {
	/* Enable the GPIOs Clock */
	RCC_AHBPeriphClockCmd(
			RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC
					| RCC_AHBPeriph_GPIOD | RCC_AHBPeriph_GPIOE
					| RCC_AHBPeriph_GPIOH, ENABLE);

	/* Enable comparator clock */
	RCC_APB1PeriphClockCmd(
			RCC_APB1Periph_COMP | RCC_APB1Periph_LCD | RCC_APB1Periph_PWR,
			ENABLE);

	/* Enable SYSCFG */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/* Allow access to the RTC */
	PWR_RTCAccessCmd(ENABLE);

	/* Reset Backup Domain */
	RCC_RTCResetCmd(ENABLE);
	RCC_RTCResetCmd(DISABLE);

	/*!< LSE Enable */
	RCC_LSEConfig(RCC_LSE_ON );

	/*!< Wait till LSE is ready */
	while (RCC_GetFlagStatus(RCC_FLAG_LSERDY ) == RESET) {
	}

	/*!< LCD Clock Source Selection */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE );
}

/**
 * @brief  To initialize the I/O ports
 * @caller main
 * @param None
 * @retval None
 */
void Init_GPIOs(void) {
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Configure User Button pin as input */
	GPIO_InitStructure.GPIO_Pin = USER_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(BUTTON_GPIO_PORT, &GPIO_InitStructure);

	/* Configure the GPIO_LED pins  LD3 & LD4*/
	GPIO_InitStructure.GPIO_Pin = LD_GREEN | LD_BLUE;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(LD_PORT, &GPIO_InitStructure);
	GPIO_LOW(LD_PORT, LD_GREEN);
	GPIO_LOW(LD_PORT, LD_BLUE);

	/* Configure Output for LCD */
	/* Port A */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3
			| GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_LCD );
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_LCD );
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_LCD );
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_LCD );
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_LCD );
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_LCD );
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource15, GPIO_AF_LCD );

	/* Configure Output for LCD */
	/* Port B */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5
			| GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12
			| GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_LCD );
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_LCD );
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_LCD );
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_LCD );
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_LCD );
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_LCD );
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_LCD );
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_LCD );
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_LCD );
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_LCD );

	/* Configure Output for LCD */
	/* Port C*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2
			| GPIO_Pin_3 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9
			| GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOC, GPIO_PinSource0, GPIO_AF_LCD );
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource1, GPIO_AF_LCD );
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource2, GPIO_AF_LCD );
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource3, GPIO_AF_LCD );
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_LCD );
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_LCD );
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_LCD );
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_LCD );
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_LCD );
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_LCD );
}

/**
 * @brief  Inserts a delay time.
 * @param  nTime: specifies the delay time length, in 10 ms.
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
void ADC_Configuration(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_InitTypeDef  ADC_InitStructure;


	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE);
	RCC_HSICmd(ENABLE);


	/* Configure Pa52 as an ADC input */
    /* Pin 5 van poort A (PCA) is internally connected with ADC channel 5 */
    /* See page 25 of the stm32L discovery user manual */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_400KHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);


    ADC_DeInit(ADC1);

    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConvEdge_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;



        /* Now do the setup */
    ADC_Init(ADC1, &ADC_InitStructure);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_192Cycles);

    /* Enable ADC1 */
    ADC_Cmd(ADC1, ENABLE);   // ADC1->CR1|=0x20000;

    //Wacht totdat de ADC gereed is
    while(ADC_GetFlagStatus(ADC1,ADC_FLAG_ADONS)==RESET);
}



int readADC1()
{
       // Start the conversion
       ADC_SoftwareStartConv(ADC1);

       // Wait until conversion completion
       while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0);

       // Get the conversion value
       return ADC_GetConversionValue(ADC1);
}

