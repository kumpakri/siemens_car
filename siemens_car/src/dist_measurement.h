/*
 * dist_measurement.h
 *
 *  Created on: May 13, 2017
 *      Author: kumpakri
 */

#ifndef USER_INC_DIST_MEASUREMENT_H_
#define USER_INC_DIST_MEASUREMENT_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

#define SPROCKET_TO_WHEEL_REVOLUTIONS_RATIO 6

//perimeter of the wheel in mm
#define WHEEL_PERIMETER 220
#define REVOLUTIONS_OVERFLOW_LIMIT 2000000000

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
long getRightDistInCM();
long getLeftDistInCM();

#endif /* USER_INC_DIST_MEASUREMENT_H_ */
