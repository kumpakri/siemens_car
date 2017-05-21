/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "test_blink_led.h"

void delay2(int counter) {
    int i;
    for (i = 0; i < counter * 10000; i++) {}
}




int blink(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
    GPIO_InitTypeDef gpio;



    GPIO_StructInit(&gpio);
    gpio.GPIO_Pin = GPIO_Pin_15;
    gpio.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_Init(GPIOA, &gpio);

    GPIO_SetBits(GPIOA, GPIO_Pin_15);
   //GPIO_ResetBits(GPIOA, GPIO_Pin_15);
    while (1)
    {


    }
}
