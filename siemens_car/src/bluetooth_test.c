/*
 * bluetooth_test.c
 *
 *  Created on: Jun 17, 2017
 *      Author: kumpakri
 */

#ifndef BLUETOOTH_TEST_C_
#define BLUETOOTH_TEST_C_


#include "main.h"


//RCC_ClocksTypeDef RCC_Clocks;

int main(void) {
	//configurePinsPA2andPA3();
	//configurePinsPC6andPC7();
	//initUART2(9600);
	LEDinit();
	userButtonInit();
	buttonITR();


	while(1){
		//printf("UART Message: %c", getRecievedString());
/*		if (getRecievedString() == "a") {
			GPIO_SetBits(GPIOD,GPIO_Pin_12);
		}
		if (getRecievedString() == "b") {
			GPIO_ResetBits(GPIOD,GPIO_Pin_12);
		}

		if(getITRbyl()>0){
			GPIO_SetBits(GPIOD,GPIO_Pin_12);
		}
*/
		if(getPushNum()>0){
			GPIO_SetBits(GPIOD,GPIO_Pin_12);
		}
	}

}


#endif /* BLUETOOTH_TEST_C_ */
