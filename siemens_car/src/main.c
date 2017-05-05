/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

// to main.h include all other header files
#include "main.h"


/*
 *  initialize peripheries, this function will be called just once after start program
 */




void init()
{

	// there call functions RCC_APB1PeriphClockCmd and RCC_APB1PeriphClockCmd

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA , ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

}



/*
 * function
 */

int main(void)
{
	init_motors();
	return 0;


}

