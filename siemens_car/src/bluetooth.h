/*
 * bluetooth.h
 *
 *  Created on: Jun 17, 2017
 *      Author: kumpakri
 */

#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void configurePinsPA2andPA3();
void initUART2(int baudrate);
void configurePinsPC6andPC7();
void initUART6(int baudrate);
char* getRecievedString();
int getITRbyl();

#endif /* BLUETOOTH_H_ */
