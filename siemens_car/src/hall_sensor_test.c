/*
 * hall_sensor_test.c
 *
 *  Created on: May 6, 2017
 *      Author: kumpakri
 */

#include "main.h"

void EXTI0_IRQHandler(void) {
    if (EXTI_GetITStatus(EXTI_Line0) != RESET) {
    	count++;

        /* Clear interrupt flag */
        EXTI_ClearITPendingBit(EXTI_Line0);
    }

}



int main(void) {
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    LEDinit();
    init_Hall();

    while (1) {
        if (getRightRevolutions()%5 == 0) {
            GPIO_SetBits(GPIOD, GPIO_Pin_12);
        } else {
            GPIO_ResetBits(GPIOD, GPIO_Pin_12);
        }

        if (getLeftRevolutions()%5 == 0) {
            GPIO_SetBits(GPIOD, GPIO_Pin_15);
        } else {
            GPIO_ResetBits(GPIOD, GPIO_Pin_15);
        }
    }
}

