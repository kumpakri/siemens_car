/*
 * ir_sensors.h
 *
 *  Created on: 5. 5. 2017
 *      Author: PC
 */

#ifndef IR_SENSORS_H_
#define IR_SENSORS_H_

#include "stm32f4xx.h"

void adc_init(void);
int get_ir_buttom();
int get_ir_front_left();
int get_ir_front_right();
int get_ir_right_side_front();
int get_ir_left_side_front();


#endif /* IR_SENSORS_H_ */
