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


void init(){
	 init_motors();
	 init_ir_sensors();
}


int main(void) {
	blink();
	/*int i1,i2,i3,i4,i5,j = 0;
	while(1)
	{
		i1 = get_ir_front_left();
		i2 = get_ir_front_right();
		i3 = get_ir_left_side_front();
		i4 = get_ir_right_side_front();
		i5 = get_ir_buttom();

		j = 1;
	}*/
}
