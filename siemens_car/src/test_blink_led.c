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
    GPIO_InitTypeDef gpio;



    GPIO_StructInit(&gpio);
    gpio.GPIO_Pin = GPIO_Pin_15;
    gpio.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_Init(GPIOA, &gpio);

    GPIO_SetBits(GPIOA, GPIO_Pin_15);
    GPIO_SetBits(GPIOA, GPIO_Pin_15); // LED ON
    while (1)
    {


    }
}
