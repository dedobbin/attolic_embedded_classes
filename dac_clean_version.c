**===========================================================================
*/
int main(void)
{
	uint16_t output=0;

    GPIO_InitTypeDef  GPIO_InitStructure;


  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

  /* Configure the GPIO_LED pin */
  GPIO_InitStructure.GPIO_Pin = LD_GREEN |LD_BLUE;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_32MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);



   DAC_Configuration();


   GPIOB->ODR|=0xC0;
   GPIOB->ODR^=0x40;

   /* Infinite loop */
   while (1)
    {

      if (output==0) output=3000;else output=0;
	  ConvertDAC(output);
	  //if (analogresult<100) GPIOB->ODR^=0x40;
      Delay(1000);
    }
}

void DAC_Configuration(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC , ENABLE);

	/* Configure Pa52 as an ADC input ?????????*/
    /* Pin 4 of port A (PCA) is internally connected with DAC channel 1 */
    /* See page 24 of the stm32L discovery user manual */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_400KHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

    DAC_DeInit();
	//dac channel 1 trigger, set to software trigger
    DAC->CR|=0x38;

}



void ConvertDAC(uint16_t value)
{

	    DAC_SetChannel1Data(DAC_Align_12b_R ,value);
	    //enable dac channel 1
		DAC->CR|=0x1;

}

void Delay(__IO uint32_t nCount)
{
  while(nCount--)
  {
  }
}



