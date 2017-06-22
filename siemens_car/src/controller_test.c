/*
 * controller_test.c
 *
 *  Created on: Jun 22, 2017
 *      Author: kumpakri
 */
#include <sys/time.h>
#include "main.h"
#define FRONT_LIMIT 100
#define SIDE_LIMIT 150
#define P_RIGHT 0.4
#define P_LEFT 0.5
#define DEFAULT_SPEED 50

/*working variables*/
double Input, Output, Setpoint;
double errSum, lastErr;
double kp, ki, kd;
struct timeval now, lastTime;

void TM_TIMER_Init(void) {
    TIM_TimeBaseInitTypeDef TIM_BaseStruct;

    /* Enable clock for TIM5 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

    TIM_BaseStruct.TIM_Prescaler = 0;
    TIM_BaseStruct.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_BaseStruct.TIM_Period = 8399; /* 10kHz PWM */
    TIM_BaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_BaseStruct.TIM_RepetitionCounter = 0;

    TIM_TimeBaseInit(TIM5, &TIM_BaseStruct);
    TIM_Cmd(TIM5, ENABLE);
}

int main(void) {

	kp=1000;
	ki=30;
	kd=0;

	TM_TIMER_Init();
	init_ir_sensors();
	init_motors();
	userButtonInit();

	gettimeofday(&lastTime, NULL);

	while(1){
		if(get_ir_front_left()>FRONT_LIMIT ){
			set_speed_motorLeft(0);
			set_speed_motorRight(70);
		}
		if(get_ir_front_right()>FRONT_LIMIT+10 ){
			set_speed_motorLeft(30);
			set_speed_motorRight(60);
		}
		if(get_ir_right_side_front()>SIDE_LIMIT ){
			set_speed_motorLeft(53);
			set_speed_motorRight(57);
		}
		if(get_ir_right_side_front()<SIDE_LIMIT-20 ){
			set_speed_motorLeft(57);
			set_speed_motorRight(53);
		}
		if( get_ir_right_side_front()>SIDE_LIMIT-20 && get_ir_right_side_front()<SIDE_LIMIT ){
			set_speed_motorLeft(60);
			set_speed_motorRight(60);
		}


		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)){
			set_speed_motorLeft(0);
			set_speed_motorRight(0);

			return 0;
		}
	}

}
