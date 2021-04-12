#include "stm32l1xx.h"
#include "discover_board.h"

static volatile uint32_t TimingDelay;
void Delay(__IO uint32_t nCount);
void t4pmconf();

TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
TIM_OCInitTypeDef TIM_OCInitStructure;
GPIO_InitTypeDef GPIO_InitStructure;


void init_greenled() {
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = LD_GREEN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIOB ->ODR = 0x80; //groene led aan
}

int delayval=100000;//var voor dynamische delay
int i=0;
int faster = 0;

void run(void);
void initUserButton(void);

int main(void) {
	initUserButton();//toegevoegd om pwm snelheid aan te passen.
	init_greenled();

	run();
/* dode code
     while (1) {
      Delay(1000);
 }*/
}

void run() {
	t4pmconf();

	while (1) {
		Delay(delayval);
		 if ((GPIOA->IDR & USER_GPIO_PIN) != 0x0){
			 TIM4->CCR1 -=100;
		 }
		GPIO_TOGGLE(LD_PORT, GPIO_Pin_7);//toggle groene led
		if (i>9 && !faster){/*na 10 keer lopen wordt delay gehalveerd, de gpio toggle functie wordt dus dubbel zo snel aangeroepen, ook wordt timer 4 geinitialisateerd*/
			delayval /= 2;
			TIM_DeInit(TIM4);
			i=0;
			faster = 1;

		}
		i++;
	}
}

void t4pmconf() {
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; /*gedeelte van de init structure van GPIO wordt ingvuld, hier wordt aangegeven dat het om pin 6 gaat */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; /*Zet de pin op type push/pull, geen open drain */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; /*Hier wordt aangegeven dat de pin in pull up modes moet gezet worden*/
	GPIO_Init(GPIOB, &GPIO_InitStructure); /*De init structure wordt ingevoerd, als alles goed is ingevuld kan GPIOB 6 hierna gebruikt worden*/
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4 );
	TIM_TimeBaseStructure.TIM_Period = 1000; /* Het invullen van de init struct voor een timer, period is als het ware 1 cycle, als de helft van een period een signaal hoog is, is de duty cycle 50% */
	TIM_TimeBaseStructure.TIM_Prescaler = 1000; /*getal wordt gebruikt gebruikt om de counter clock te dividen*/
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; /*zorgt ervoor dat timer output compare enabled is, dit wordt gebruikt voor onderandere PWM*/
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_Pulse = 500; /*De tijd in de period dat er een hoog signaal wordt afgegeven, deze is (na aanpassing) 50% van de tijd hoog*/
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);
	TIM_Cmd(TIM4, ENABLE);
}
void initUserButton() {
	/* zorgt ervoor dt user button gebruikt kan worden*/
	GPIO_InitStructure.GPIO_Pin = USER_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(BUTTON_GPIO_PORT, &GPIO_InitStructure);
}

void Delay(__IO uint32_t nCount) {
	while (nCount--) {
	}
}

void TimingDelay_Decrement(void) {
	if (TimingDelay != 0x00) {
		TimingDelay--;
	}
}
