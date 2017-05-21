/*
 * debug_support.h
 *
 *  Created on: May 6, 2017
 *      Author: kumpakri
 */

#ifndef SRC_DEBUG_SUPPORT_H_
#define SRC_DEBUG_SUPPORT_H_
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void userButtonInit();
void LEDinit();
void buttonITR();


#endif /* SRC_DEBUG_SUPPORT_H_ */
