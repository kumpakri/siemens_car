/*
 * motor_pwm.c
 *
 *  Created on: 29. 4. 2017
 *      Author: Jan Holy
 */

#include "stm32f4xx.h"

const int period = 666;

void init_motors(void);
void init_motor1(void);
void init_motor2(void);
void set_speed_motor(int, TIM_OCInitTypeDef*, TIM_OCInitTypeDef*);
void set_speed_motorLeft(int);
void set_speed_motorRight(int);
int get_speed_motor1(void);
int get_speed_motor2(void);

TIM_OCInitTypeDef motor1_control_back;
TIM_OCInitTypeDef motor1_control_forward;

TIM_OCInitTypeDef motor2_control_back;
TIM_OCInitTypeDef motor2_control_forward;

void init_motors() {
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	init_motor1();
	init_motor2();
}

void init_motor1() {
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructureMotor;
	GPIO_InitTypeDef GPIO_InitStructure;
	int PrescalerValue = (uint16_t) ((SystemCoreClock / 2) / 21000000) - 1;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOE, GPIO_PinSource10, GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_TIM1);
	TIM_TimeBaseStructureMotor.TIM_Period = period - 1;
	TIM_TimeBaseStructureMotor.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructureMotor.TIM_ClockDivision = 0;
	TIM_TimeBaseStructureMotor.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructureMotor);
	motor1_control_back.TIM_OCMode = TIM_OCMode_PWM1;
	motor1_control_back.TIM_OutputNState = TIM_OutputNState_Disable;
	motor1_control_back.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC2Init(TIM1, &motor1_control_back);
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
	motor1_control_forward.TIM_OCMode = TIM_OCMode_PWM1;
	motor1_control_forward.TIM_OutputState = TIM_OutputState_Disable;
	motor1_control_forward.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC3Init(TIM1, &motor1_control_forward);
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM1, ENABLE);
	TIM_Cmd(TIM1, ENABLE);
	TIM_CtrlPWMOutputs(TIM1, ENABLE);

}

void init_motor2() {
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	/* GPIOC clock enable */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructureMotor;
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Compute the prescaler value */
	int PrescalerValue = (uint16_t) ((SystemCoreClock / 2) / 21000000) - 1;

	/* Time base configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_TIM3);

	TIM_TimeBaseStructureMotor.TIM_Period = period - 1;
	TIM_TimeBaseStructureMotor.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructureMotor.TIM_ClockDivision = 0;
	TIM_TimeBaseStructureMotor.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructureMotor);

	motor2_control_back.TIM_OCMode = TIM_OCMode_PWM1;
	motor2_control_back.TIM_OutputState = TIM_OutputState_Disable;
	motor2_control_back.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_OC1Init(TIM3, &motor2_control_back);
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

	motor2_control_forward.TIM_OCMode = TIM_OCMode_PWM1;
	motor2_control_forward.TIM_OutputState = TIM_OutputState_Disable;
	motor2_control_forward.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_OC2Init(TIM3, &motor2_control_forward);
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM3, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
	TIM_CtrlPWMOutputs(TIM3, ENABLE);
}

/*
 *	@reval speed of motor 1 in percent, from -100 to 100
 */
int get_speed_motor1() {

	int pulse = 0;
	if (motor1_control_back.TIM_OutputState == TIM_OutputState_Disable
			&& motor1_control_forward.TIM_OutputState == TIM_OutputState_Disable) {
		return pulse;
	}

	if (motor1_control_back.TIM_OutputState == TIM_OutputState_Enable) {
		pulse = motor1_control_back.TIM_Pulse;
	} else {
		pulse = -motor1_control_back.TIM_Pulse;
	}
	return pulse / (period) * 100;
}

/*
 *	@reval speed of motor 2 in percent, from -100 to 100
 */
int get_speed_motor2() {
	int pulse = 0;
	if (motor2_control_back.TIM_OutputState == TIM_OutputState_Disable
			&& motor2_control_forward.TIM_OutputState == TIM_OutputState_Disable) {
		return pulse;
	}

	if (motor2_control_back.TIM_OutputState == TIM_OutputState_Enable) {
		pulse = motor2_control_back.TIM_Pulse;
	} else {
		pulse = -motor2_control_back.TIM_Pulse;
	}
	return pulse / (period) * 100;
}

void set_speed_motorLeft(int speed) {

	int pulse_forward = 0;
	int pulse_back = 0;
	int TIM_OutputState_Forward;
	int TIM_OutputState_Back;
	if (speed > 100)
		speed = 100;
	if (speed < -100)
		speed = -100;
	if (speed == 0) {
		TIM_OutputState_Forward = TIM_OutputState_Disable;
		TIM_OutputState_Back = TIM_OutputNState_Disable;

	} else if (speed > 0) {
		pulse_forward = speed * period / 100;
		TIM_OutputState_Forward = TIM_OutputState_Enable;
		TIM_OutputState_Back = TIM_OutputNState_Disable;
	} else {
		pulse_back = -speed * period / 100;
		TIM_OutputState_Forward = TIM_OutputState_Disable;
		TIM_OutputState_Back = TIM_OutputNState_Enable;
	}
	motor1_control_back.TIM_OutputNState = TIM_OutputState_Back;
	motor1_control_back.TIM_Pulse = pulse_back;
	motor1_control_forward.TIM_OutputState = TIM_OutputState_Forward;
	motor1_control_forward.TIM_Pulse = pulse_forward;

	TIM_OC2Init(TIM1, &motor1_control_back);
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC3Init(TIM1, &motor1_control_forward);
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
}

void set_speed_motorRight(int speed) {

	int pulse_forward = 0;
	int pulse_back = 0;
	int TIM_OutputState_Forward;
	int TIM_OutputState_Back;
	if (speed > 100)
		speed = 100;
	if (speed < -100)
		speed = -100;
	if (speed == 0) {
		TIM_OutputState_Forward = TIM_OutputState_Disable;
		TIM_OutputState_Back = TIM_OutputState_Disable;

	} else if (speed > 0) {
		pulse_forward = speed * period / 100;
		TIM_OutputState_Forward = TIM_OutputState_Enable;
		TIM_OutputState_Back = TIM_OutputState_Disable;
	} else {
		pulse_back = -speed * period / 100;
		TIM_OutputState_Forward = TIM_OutputState_Disable;
		TIM_OutputState_Back = TIM_OutputState_Enable;
	}
	motor2_control_back.TIM_OutputState = TIM_OutputState_Back;
	motor2_control_back.TIM_Pulse = pulse_back;
	motor2_control_forward.TIM_OutputState = TIM_OutputState_Forward;
	motor2_control_forward.TIM_Pulse = pulse_forward;

	TIM_OC1Init(TIM3, &motor2_control_back);
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC2Init(TIM3, &motor2_control_forward);
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
}

void set_speed_motor(int speed, TIM_OCInitTypeDef* motorForward,
		TIM_OCInitTypeDef* motorBack) {
	int pulse_forward = 0;
	int pulse_back = 0;
	int TIM_OutputState_Forward;
	int TIM_OutputState_Back;
	if (speed > 100)
		speed = 100;
	if (speed < -100)
		speed = -100;
	if (speed == 0) {
		TIM_OutputState_Forward = TIM_OutputState_Disable;
		TIM_OutputState_Back = TIM_OutputState_Disable;

	} else if (speed > 0) {
		pulse_forward = speed * period / 100.0;
		TIM_OutputState_Forward = TIM_OutputState_Enable;
		TIM_OutputState_Back = TIM_OutputState_Disable;
	} else {
		pulse_forward = -speed * period / 100.0;
		TIM_OutputState_Forward = TIM_OutputState_Disable;
		TIM_OutputState_Back = TIM_OutputState_Enable;
	}
	motorForward->TIM_OutputState = TIM_OutputState_Forward;
	motorForward->TIM_Pulse = pulse_forward;
	motorBack->TIM_OutputState = TIM_OutputState_Back;
	motorBack->TIM_Pulse = pulse_back;
}

