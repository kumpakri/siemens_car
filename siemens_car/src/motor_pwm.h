/*
 * motor_pwm.h
 *
 *  Created on: 29. 4. 2017
 *      Author: Jan Holy
 */

#ifndef MOTOR_PWM_H_
#define MOTOR_PWM_H_
#include "stm32f4xx.h"

/*
 * define
 */

#define PRESCALE 21000000
#define PERIODE 666

/*
 * function
 */
void init_motors(void);
void set_speed_motorLeft(int);
void set_speed_motorRight(int);
int get_speed_motor1(void);
int get_speed_motor2(void);

#endif /* MOTOR_PWM_H_ */
