/*
 * bluetooth_test.c
 *
 *  Created on: Jun 17, 2017
 *      Author: kumpakri
 */

#ifndef BLUETOOTH_TEST_C_
#define BLUETOOTH_TEST_C_

#define NUM_OF_QUANTILES 21

#include "main.h"
#include <stdio.h>

int main(void) {
	configurePinsPA2andPA3();
	initUART2(9600);
	LEDinit();

	char cmdLeftArray[NUM_OF_QUANTILES] = {'a','b','c','d','e','f','g','h','i','j','k','B','C','D','E','F','G','H','I','J','K'};
	char cmdRightArray[NUM_OF_QUANTILES] = {'l','m','n','o','p','q','r','s','t','u','v','M','N','O','P','Q','R','S','T','U','V'};
	int cmdValues[NUM_OF_QUANTILES] = {0,-10,-20,-30,-40,-50,-60,-70,-80,-90,-100,10,20,30,40,50,60,70,80,90,100};
	volatile int number;

	char *ptr = "hello\r\n";
	USART_puts(USART2, ptr);

	char cmd;


	while(1){

		int i = 0;
		while(i<100000){i++;}
		if (  isNewMessage() ) {
			cmd = getCommand();


			for(i=0;i<NUM_OF_QUANTILES;i++){
				/* Left motor */
				if(cmd == cmdLeftArray[i]) {
					set_speed_motor1(cmdValues[i]);
				}
				/* Right motor */
				if(cmd == cmdRightArray[i]) {
					set_speed_motor2(cmdValues[i]);
				}
			}

			/* STOP command */
			if(cmd == 'x') {
				set_speed_motor1(0);
				set_speed_motor2(0);
			}


			setNewMessage(0);
		}

		char str[] = "L:%i\r\n";
		char str2[12];
		number = get_speed_motor1();
		sprintf(str2,str,number);
		USART_puts(USART2,str2);

	}
}


#endif /* BLUETOOTH_TEST_C_ */
