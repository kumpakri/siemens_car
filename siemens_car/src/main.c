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
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);

	//there initialize other function
	//init_motors();
	//init_Hall();

}


/*
 * function
 */
int main(void)
{
	/*
	 * initialize peripheries
	 */
	init();
	/*
	 * main loop
	 */
	while(1)
	{

	}
}
