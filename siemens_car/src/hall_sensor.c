/*
 * hall_sensor.c
 *
 *  Created on: May 4, 2017
 *      Author: kumpakri
 */

/*	- COMMENT ------------------------------------------------------------------
 * 	when implementing distance measurement, beware of integer variables overflow
 */

/* Includes ------------------------------------------------------------------*/
#include "hall_sensor.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
GPIO_InitTypeDef GPIO_InitStruct;
EXTI_InitTypeDef EXTI_InitStruct;
NVIC_InitTypeDef NVIC_InitStruct;

volatile long revolutionsR=0;
volatile long revolutionsL=0;

/* Private function prototypes -----------------------------------------------*/
/* initialize Hall sensors */

/* Private functions ---------------------------------------------------------*/
/**
 * @brief	Set GPIO pins PD13, PD14 as inputs for interrupts
 */
void init_Hall(){
	Configure_PD13();
	Configure_PD14();
}

/**
 * @brief	Configure pin PD13 as input for right Hall sensor interrupts on falling edge.
 */
void Configure_PD13() {
    /* Enable clock for GPIOD */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    /* Enable clock for SYSCFG */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    /* Set pin as input */
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* Tell system that you will use PD13 for EXTI_Line13 */
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource13);

    /* PD13 is connected to EXTI_Line13 */
    EXTI_InitStruct.EXTI_Line = EXTI_Line13;
    /* Enable interrupt */
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    /* Interrupt mode */
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    /* Triggers on falling edge */
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
    /* Add to EXTI */
    EXTI_Init(&EXTI_InitStruct);

    /* Add IRQ vector to NVIC */
    /* PD13 is connected to EXTI_Line13, which has EXTI15_10_IRQn vector */
    NVIC_InitStruct.NVIC_IRQChannel = EXTI15_10_IRQn;
    /* Set priority */
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
    /* Set sub priority */
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x01;
    /* Enable interrupt */
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    /* Add to NVIC */
    NVIC_Init(&NVIC_InitStruct);
}

/**
 * @brief	Configure pin PD14 as input for left Hall sensor interrupts on falling edge.
 */
void Configure_PD14() {
    /* Enable clock for GPIOD */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    /* Enable clock for SYSCFG */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    /* Set pin as input */
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* Tell system that you will use PD14 for EXTI_Line14 */
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource14);

    /* PD14 is connected to EXTI_Line14 */
    EXTI_InitStruct.EXTI_Line = EXTI_Line14;
    /* Enable interrupt */
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    /* Interrupt mode */
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    /* Triggers on falling edge */
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
    /* Add to EXTI */
    EXTI_Init(&EXTI_InitStruct);

    /* Add IRQ vector to NVIC */
    /* PD14 is connected to EXTI_Line14, which has EXTI15_10_IRQn vector */
    NVIC_InitStruct.NVIC_IRQChannel = EXTI15_10_IRQn;
    /* Set priority */
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
    /* Set sub priority */
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x01;
    /* Enable interrupt */
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    /* Add to NVIC */
    NVIC_Init(&NVIC_InitStruct);
}

/* Handle Hall sensors interrupt */
void EXTI15_10_IRQHandler(void) {
	int i = 0;
    /* if interrupt flag EXTI_Line13 (right wheel) is set: */
    if (EXTI_GetITStatus(EXTI_Line13) != RESET) {
    	revolutionsR++;

        /* Clear interrupt flag */
        EXTI_ClearITPendingBit(EXTI_Line13);
    }

    /* if interrupt flag EXTI_Line14 (left wheel) is set: */
    if (EXTI_GetITStatus(EXTI_Line14) != RESET) {
    	revolutionsL++;

        /* Clear interrupt flag */
        EXTI_ClearITPendingBit(EXTI_Line14);
    }
}


int get_revolutionsR()
{
	return revolutionsR;
}

int get_revolutionsL()
{
	return revolutionsL;
}




