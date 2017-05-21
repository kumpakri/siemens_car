/*
 * dist_measurement.c
 *
 *  Created on: May 13, 2017
 *      Author: kumpakri
 */

/* Includes ------------------------------------------------------------------*/
#include "dist_measurement.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
long getRightDistInCM();
long getLeftDistInCM();
long getRightDistInMM();
long getLeftDistInMM();

/* Private functions ---------------------------------------------------------*/
long getRightDistInCM(){
	long revolutionsTotal=(getRightRevolutions()+(REVOLUTIONS_OVERFLOW_LIMIT*getRevolutionsROverflow()));
	return (long)((revolutionsTotal*WHEEL_PERIMETER*0.1)/SPROCKET_TO_WHEEL_REVOLUTIONS_RATIO);
}

long getLeftDistInCM(){
	long revolutionsTotal=(getLeftRevolutions()+REVOLUTIONS_OVERFLOW_LIMIT*getRevolutionsLOverflow());
	return (long)((revolutionsTotal*WHEEL_PERIMETER*0.1)/SPROCKET_TO_WHEEL_REVOLUTIONS_RATIO);
}

long getRightDistInMM(){
	long revolutionsTotal=(getRightRevolutions()+REVOLUTIONS_OVERFLOW_LIMIT*getRevolutionsROverflow());
	return (long)((revolutionsTotal*WHEEL_PERIMETER)/SPROCKET_TO_WHEEL_REVOLUTIONS_RATIO);
}

long getLeftDistInMM(){
	long revolutionsTotal=(getLeftRevolutions()+REVOLUTIONS_OVERFLOW_LIMIT*getRevolutionsLOverflow());
	return (long)((revolutionsTotal*WHEEL_PERIMETER)/SPROCKET_TO_WHEEL_REVOLUTIONS_RATIO);
}
