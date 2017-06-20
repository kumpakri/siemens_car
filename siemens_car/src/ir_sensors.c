#import "ir_sensors.h"

#define COUNT_MEASURE 5
#define COUNT_CYCLE 1000

/**
 * @brief  Main program
 * @param  None
 * @retval None
 */

void adc_init(void);
void init_ir_sensors(void);
int get_ir_buttom(void);
int get_ir_front_left(void);
int get_ir_front_right(void);
int get_ir_right_side_front(void);
int get_ir_left_side_front(void);
void pwm_ir_led(int);

const int IR_BOTTOM_ORD = 1;
const int IR_FRONT_RIGHT_ORD = 2;
const int IR_FRONT_LEFT_ORD = 3;
const int IR_LEFT_SIDE_FRONT_ORD = 4;
const int IR_RIGHT_SIDE_FRONT_ORD = 5;



__IO uint16_t aADCDualConvertedValue[COUNT_MEASURE*COUNT_CYCLE];

/**
 * @brief  Configure the TIM3 Output Channels.
 * @param  None
 * @retval None
 */

int measure_led_on[COUNT_MEASURE];
int measure_led_off[COUNT_MEASURE];
int on = 1;

void TIM2_IRQHandler(void) {
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {

		int j, i;
		int sum_measure = 0;
		for (i = 0; i < COUNT_MEASURE; i++) {
			sum_measure = 0;
			for (j = 0; j < COUNT_CYCLE; j++) {
				sum_measure += aADCDualConvertedValue[(i) + COUNT_MEASURE * j];
			}
			if (on == 1) {
				measure_led_on[i] = sum_measure / (COUNT_CYCLE);
			} else {
				measure_led_off[i] = sum_measure / (COUNT_CYCLE);
			}
		}
		on ^= 1;
		GPIO_ToggleBits(GPIOA, GPIO_Pin_15);
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

	}
}

void interruption_init() {
	NVIC_InitTypeDef nvicStructure;
	nvicStructure.NVIC_IRQChannel = TIM2_IRQn;
	nvicStructure.NVIC_IRQChannelPreemptionPriority = 0;
	nvicStructure.NVIC_IRQChannelSubPriority = 1;
	nvicStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvicStructure);
}

void init_ir_sensors() {
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOC, ENABLE);
	adc_init();
	pwm_ir_led(50);
	interruption_init();
}

void adc_init() {
	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_Channel = DMA_Channel_0;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &(ADC1->DR);
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) &aADCDualConvertedValue;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = COUNT_MEASURE * COUNT_CYCLE;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream0, &DMA_InitStructure);

	DMA_Cmd(DMA2_Stream0, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2
			| GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_1;
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;

	ADC_CommonInit(&ADC_CommonInitStructure);

	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = COUNT_MEASURE;
	ADC_Init(ADC1, &ADC_InitStructure);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_15, IR_BOTTOM_ORD,
	ADC_SampleTime_3Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, IR_FRONT_RIGHT_ORD,
			ADC_SampleTime_3Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, IR_FRONT_LEFT_ORD,
			ADC_SampleTime_3Cycles);

	ADC_RegularChannelConfig(ADC1, ADC_Channel_12, IR_LEFT_SIDE_FRONT_ORD,
			ADC_SampleTime_3Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_14, IR_RIGHT_SIDE_FRONT_ORD,
			ADC_SampleTime_3Cycles);
	ADC_Cmd(ADC1, ENABLE);
	ADC_DMACmd(ADC1, ENABLE);
	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
	ADC_SoftwareStartConv(ADC1);
	for (int i = 0; i < 100 * 10000; i++) {
	}
}

int get_intensity(int order){
	return measure_led_on[order-1]-measure_led_off[order-1];
}

int get_ir_buttom() {
	return get_intensity(IR_BOTTOM_ORD);
}

int get_ir_front_left() {
	return get_intensity(IR_FRONT_LEFT_ORD);
}

int get_ir_front_right() {
	return get_intensity(IR_RIGHT_SIDE_FRONT_ORD);
}

int get_ir_right_side_front() {
	return get_intensity(IR_LEFT_SIDE_FRONT_ORD);

}

int get_ir_left_side_front() {
	return get_intensity(IR_FRONT_RIGHT_ORD);
}



void pwm_ir_led(int freq) {
	int TimerPeriod = (SystemCoreClock / (2 * freq));
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructureMotor;
	GPIO_InitTypeDef GPIO_InitStructure;
	int PrescalerValue = 1;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource15, GPIO_AF_TIM2);

	TIM_TimeBaseStructureMotor.TIM_Period = TimerPeriod;
	TIM_TimeBaseStructureMotor.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructureMotor.TIM_ClockDivision = 0;
	TIM_TimeBaseStructureMotor.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructureMotor);
	TIM_Cmd(TIM2, ENABLE);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	GPIO_InitTypeDef gpio;

	GPIO_StructInit(&gpio);
	gpio.GPIO_Pin = GPIO_Pin_15;
	gpio.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOA, &gpio);

	GPIO_SetBits(GPIOA, GPIO_Pin_15);

}
