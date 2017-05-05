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




int blink2(void)
{
    GPIO_InitTypeDef gpio;



    GPIO_StructInit(&gpio);
    gpio.GPIO_Pin = GPIO_Pin_15;
    gpio.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_Init(GPIOD, &gpio);

    GPIO_SetBits(GPIOD, GPIO_Pin_15);

    while (1)
    {
    	GPIO_SetBits(GPIOD, GPIO_Pin_15); // LED ON
        delay2(400);
        GPIO_ResetBits(GPIOD, GPIO_Pin_15); // LED OFF
        delay2(400);
    }
}
