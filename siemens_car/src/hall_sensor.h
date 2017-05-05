/*
 * hall_sensor.h
 *
 *  Created on: May 4, 2017
 *      Author: kumpakri
 */

#ifndef SRC_HALL_SENSOR_H_
#define SRC_HALL_SENSOR_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/* initialize Hall sensors */
void init_Hall();
/* sets GPIO pin PD13 as input for Hall sensor interrupts */
void Configure_PD13();
/* sets GPIO pin PD14 as input for Hall sensor interrupts */
void Configure_PD14();
/* interrupt handler for interrupts from Hall sensors */
void EXTI15_10_IRQHandler(void);

#endif /* SRC_HALL_SENSOR_H_ */
