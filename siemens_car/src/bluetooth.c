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

volatile int newMessage = 0;
volatile char command;

/* Private function prototypes -----------------------------------------------*/
void configurePinsPA2andPA3();
void configurePinsPC6andPC7();
void initUART2(int baudrate);
void initUART6(int baudrate);
void USART_puts(USART_TypeDef* USARTx, volatile char *s);
void setNewMessage(int b);
int isNewMessage();

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
		*s++;
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

void setNewMessage(int b){
	newMessage = b;
}

int isNewMessage(){
	return newMessage;
}

char getCommand(){
	return command;
}

//Interrupt handler implementation
void USART2_IRQHandler(void)
{
	// check if the USART2 receive interrupt flag was set
	if( USART_GetITStatus(USART2, USART_IT_RXNE) ){
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		newMessage = 1;

		char t = USART_ReceiveData(USART2);
		command = (char) t;
		/*left backward*/
	/*	if(t=='a'){command = 'a';} //0
		if(t=='b'){command = 'b';} //-10
		if(t=='c'){command = 'c';} //-20
		if(t=='d'){command = 'd';} //-30
		if(t=='e'){command = 'e';} //-40
		if(t=='f'){command = 'f';} //-50
		if(t=='g'){command = 'g';} //-60
		if(t=='h'){command = 'h';} //-70
		if(t=='i'){command = 'i';} //-80
		if(t=='j'){command = 'j';} //-90
		if(t=='k'){command = 'k';} //-100
		/*right backward*/
	/*	if(t=='l'){command = 'l';} //0
		if(t=='m'){command = 'm';} //-10
		if(t=='n'){command = 'n';} //-20
		if(t=='o'){command = 'o';} //-30
		if(t=='p'){command = 'p';} //-40
		if(t=='q'){command = 'q';} //-50
		if(t=='r'){command = 'r';} //-60
		if(t=='s'){command = 's';} //-70
		if(t=='t'){command = 't';} //-80
		if(t=='u'){command = 'u';} //-90
		if(t=='v'){command = 'v';} //-100

		/*left forward*/
	/*	if(t=='B'){command = 'B';} //10
		if(t=='C'){command = 'C';} //20
		if(t=='D'){command = 'D';} //30
		if(t=='E'){command = 'E';} //40
		if(t=='F'){command = 'F';} //50
		if(t=='G'){command = 'G';} //60
		if(t=='H'){command = 'H';} //70
		if(t=='I'){command = 'I';} //80
		if(t=='J'){command = 'J';} //90
		if(t=='K'){command = 'K';} //100
		/*right forward*/
	/*	if(t=='M'){command = 'M';} //10
		if(t=='N'){command = 'N';} //20
		if(t=='O'){command = 'O';} //30
		if(t=='P'){command = 'P';} //40
		if(t=='Q'){command = 'Q';} //50
		if(t=='R'){command = 'R';} //60
		if(t=='S'){command = 'S';} //70
		if(t=='T'){command = 'T';} //80
		if(t=='U'){command = 'U';} //90
		if(t=='V'){command = 'V';} //100
		/* Stop */
	/*	if(t=='x'){command = 'x';} //stop both motors
*/
	}
}

