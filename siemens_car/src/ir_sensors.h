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
int readValue();

#endif /* IR_SENSORS_H_ */
