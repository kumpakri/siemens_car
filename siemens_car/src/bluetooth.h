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
void configurePinsPC6andPC7();
void initUART2(int baudrate);
void initUART6(int baudrate);
void USART_puts(USART_TypeDef* USARTx, volatile char *s);
void setNewMessage(int b);
int isNewMessage();
char getCommand();

#endif /* BLUETOOTH_H_ */
