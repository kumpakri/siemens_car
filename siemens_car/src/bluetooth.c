/*
 * bluetooth.c
 *
 *  Created on: Jun 17, 2017
 *      Author: kumpakri
 */

/* Includes ------------------------------------------------------------------*/
#include "bluetooth.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define MAX_STRLEN 12 // this is the maximum string length of our string in characters

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
GPIO_InitTypeDef GPIO_InitStruct;
USART_InitTypeDef USART_InitStruct;
NVIC_InitTypeDef NVIC_InitStruct;
USART_ClockInitTypeDef USART_ClockInitstructure;

volatile char received_string[MAX_STRLEN+1]; // this will hold the recieved string
volatile int interuptbyl = 0;

/* Private function prototypes -----------------------------------------------*/
void configurePinsPA2andPA3();
void initUART2(int baudrate);
void configurePinsPC6andPC7();
void initUART6(int baudrate);
char* getRecievedString();
int getITRbyl();
void USART2_IRQHandler(void);

/* Private functions ---------------------------------------------------------*/
void configurePinsPA2andPA3(){
	// Enable clock for GPIOA
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	/**
	* Tell pins PA2 and PA3 which alternating function you will use
	* @important Make sure, these lines are before pins configuration!
	*/
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
	// Initialize pins as alternating function
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void configurePinsPC6andPC7(){
	// Enable clock for GPIOC
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	/**
	* Tell pins PC6 and P67 which alternating function you will use
	* @important Make sure, these lines are before pins configuration!
	*/
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6);
	// Initialize pins as alternating function
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
}

/* This function is used to transmit a string of characters via
 * the USART specified in USARTx.
 *
 * It takes two arguments: USARTx --> can be any of the USARTs e.g. USART1, USART2 etc.
 * 						   (volatile) char *s is the string you want to send
 *
 * Note: The string has to be passed to the function as a pointer because
 * 		 the compiler doesn't know the 'string' data type. In standard
 * 		 C a string is just an array of characters
 *
 * Note 2: At the moment it takes a volatile char because the received_string variable
 * 		   declared as volatile char --> otherwise the compiler will spit out warnings
 * */
void USART_puts(USART_TypeDef* USARTx, volatile char *s){

	while(*s){
		// wait until data register is empty
		while( !(USARTx->SR & 0x00000040) );
		USART_SendData(USARTx, *s);
		*s+=1;
	}
}

void initUART2(int baudrate){

	//Enable clock for USART2 peripheral
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	/**
	 * Set Baudrate to value you pass to function
	 * Disable Hardware Flow control
	 * Set Mode To TX and RX, so USART will work in full-duplex mode
	 * Disable parity bit
	 * Set 1 stop bit
	 * Set Data bits to 8
	 *
	 * Initialize USART2
	 * Activate USART2
	 */
	USART_InitStruct.USART_BaudRate = baudrate;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART2, &USART_InitStruct);

	/* USART Clock Initialization  */
	 USART_ClockInitstructure.USART_Clock   = USART_Clock_Disable ;
	 USART_ClockInitstructure.USART_CPOL    = USART_CPOL_Low ;
	 USART_ClockInitstructure.USART_LastBit = USART_LastBit_Enable;
	 USART_ClockInitstructure.USART_CPHA    = USART_CPHA_1Edge;

	 USART_ClockInit(USART2, &USART_ClockInitstructure);

	/**
	 * Enable RX interrupt
	 */
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

	/**
	 * Set Channel to USART2
	 * Set Channel Cmd to enable. That will enable USART2 channel in NVIC
	 * Set Both priorities to 0. This means high priority
	 *
	 * Initialize NVIC
	 */
	NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStruct);


	// This enables the complete USART2 peripheral
	USART_Cmd(USART2, ENABLE);
}

void initUART6(int baudrate){

	//Enable clock for USART2 peripheral
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);

	/**
	 * Set Baudrate to value you pass to function
	 * Disable Hardware Flow control
	 * Set Mode To TX and RX, so USART will work in full-duplex mode
	 * Disable parity bit
	 * Set 1 stop bit
	 * Set Data bits to 8
	 *
	 * Initialize USART6
	 * Activate USART6
	 */
	USART_InitStruct.USART_BaudRate = baudrate;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART6, &USART_InitStruct);

	/* USART Clock Initialization  */
	 USART_ClockInitstructure.USART_Clock   = USART_Clock_Disable ;
	 USART_ClockInitstructure.USART_CPOL    = USART_CPOL_Low ;
	 USART_ClockInitstructure.USART_LastBit = USART_LastBit_Enable;
	 USART_ClockInitstructure.USART_CPHA    = USART_CPHA_1Edge;

	 USART_ClockInit(USART6, &USART_ClockInitstructure);

	/**
	 * Enable RX interrupt
	 */
	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);

	/**
	 * Set Channel to USART6
	 * Set Channel Cmd to enable. That will enable USART6 channel in NVIC
	 * Set Both priorities to 0. This means high priority
	 *
	 * Initialize NVIC
	 */
	NVIC_InitStruct.NVIC_IRQChannel = USART6_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStruct);


	// This enables the complete USART6 peripheral
	USART_Cmd(USART6, ENABLE);
}

char* getRecievedString(){
	return received_string;
}

int getITRbyl(){
	return interuptbyl;
}
/*
//Interrupt handler implementation
void USART2_IRQHandler(void)
{
	interuptbyl +=1;
/*	// check if the USART2 receive interrupt flag was set
	if( USART_GetITStatus(USART2, USART_IT_RXNE) ){
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);


		static uint8_t cnt = 0; // this counter is used to determine the string length
		char t = USART2->DR; // the character from the USART2 data register is saved in t

		/* check if the received character is not the LF character (used to determine end of string)
		 * or the if the maximum string length has been been reached
		 *//*
		if( (t != '\n') && (cnt < MAX_STRLEN) ){
			received_string[cnt] = t;
			cnt++;
		}
		else{ // otherwise reset the character counter and print the received string
			cnt = 0;
			USART_puts(USART2, received_string);
		}
	}*//*
}
*/
