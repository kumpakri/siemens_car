/*
 * hall_sensor_test.c
 *
 *  Created on: May 6, 2017
 *      Author: kumpakri
 */



/*
 * hall_sensor_test.c
 *
 *  Created on: May 6, 2017
 *      Author: kumpakri
 */

#include "main.h"

RCC_ClocksTypeDef RCC_Clocks;

int main(void) {

    LEDinit();
    init_Hall();


    while (1) {

    	/*
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
        /**/

    	if(getRightDistInCM()>20){
            GPIO_SetBits(GPIOD, GPIO_Pin_12);
		} else {
			GPIO_ResetBits(GPIOD, GPIO_Pin_12);
		}
    	/**/

    }
}

