#import "ir_sensors.h"



/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

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
void pwm_ir_led(void);

const int SIZE = 5;
const int IR_BOTTOM_ORD = 1;
const int IR_FRONT_RIGHT_ORD = 2;
const int IR_FRONT_LEFT_ORD = 3;
const int IR_LEFT_SIDE_FRONT_ORD = 4;
const int IR_RIGHT_SIDE_FRONT_ORD = 5;

__IO uint16_t aADCDualConvertedValue[10];

/**
  * @brief  Configure the TIM3 Output Channels.
  * @param  None
  * @retval None
  */
void init_ir_sensors()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOC, ENABLE);
	adc_init();
	pwm_ir_led();
}

void adc_init()
{
	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_Channel = DMA_Channel_0;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(ADC1->DR) ;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&aADCDualConvertedValue;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = 2*SIZE;
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


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_4;
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
	ADC_InitStructure.ADC_NbrOfConversion = 2*SIZE;
	ADC_Init(ADC1, &ADC_InitStructure);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, IR_BOTTOM_ORD, ADC_SampleTime_3Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, IR_FRONT_LEFT_ORD, ADC_SampleTime_3Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, IR_FRONT_RIGHT_ORD, ADC_SampleTime_3Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_12, IR_LEFT_SIDE_FRONT_ORD, ADC_SampleTime_3Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_14, IR_RIGHT_SIDE_FRONT_ORD, ADC_SampleTime_3Cycles);

	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, IR_BOTTOM_ORD + SIZE, ADC_SampleTime_3Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, IR_FRONT_LEFT_ORD + SIZE, ADC_SampleTime_3Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, IR_FRONT_RIGHT_ORD + SIZE, ADC_SampleTime_3Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_12, IR_LEFT_SIDE_FRONT_ORD +SIZE, ADC_SampleTime_3Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_14, IR_RIGHT_SIDE_FRONT_ORD + SIZE, ADC_SampleTime_3Cycles);
	ADC_Cmd(ADC1, ENABLE);
	ADC_DMACmd(ADC1, ENABLE);
	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
	ADC_SoftwareStartConv(ADC1);
	for (int i = 0; i < 100 * 10000; i++) {}
}

int get_ir_buttom(){
	return (aADCDualConvertedValue[IR_BOTTOM_ORD-1] + aADCDualConvertedValue[IR_BOTTOM_ORD-1+SIZE])/2;
}

int get_ir_front_left(){
	return (aADCDualConvertedValue[IR_FRONT_LEFT_ORD-1]+aADCDualConvertedValue[IR_FRONT_LEFT_ORD-1+SIZE])/2;
}

int get_ir_front_right(){
	return (aADCDualConvertedValue[IR_FRONT_RIGHT_ORD-1] + aADCDualConvertedValue[IR_FRONT_RIGHT_ORD-1+SIZE])/2;
}

int get_ir_right_side_front(){
	return (aADCDualConvertedValue[IR_RIGHT_SIDE_FRONT_ORD-1]+aADCDualConvertedValue[IR_RIGHT_SIDE_FRONT_ORD-1+SIZE])/2;
}

int get_ir_left_side_front(){
	return (aADCDualConvertedValue[IR_LEFT_SIDE_FRONT_ORD-1]+aADCDualConvertedValue[IR_LEFT_SIDE_FRONT_ORD-1+SIZE])/2;
}

void pwm_ir_led()
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructureMotor;
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	int PrescalerValue = (uint16_t) ((SystemCoreClock / 2) / 21000000) - 1;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource15, GPIO_AF_TIM2);

	TIM_TimeBaseStructureMotor.TIM_Period = 2997 - 1;
	TIM_TimeBaseStructureMotor.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructureMotor.TIM_ClockDivision = 0;
	TIM_TimeBaseStructureMotor.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructureMotor);

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 1498;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM2, ENABLE);

	TIM_Cmd(TIM2, ENABLE);

	TIM_CtrlPWMOutputs(TIM2,ENABLE);

}
