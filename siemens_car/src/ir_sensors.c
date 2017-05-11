/*
 * ir_sensors.c
 *
 *  Created on: 5. 5. 2017
 *      Author: PC
 */

#include "ir_sensors.h"


__IO uint16_t uhADCxConvertedValue = 0;


const int SIZE = 5;
const int IR_BOTTOM_ORD = 1;
const int IR_FRONT_RIGHT_ORD = 2;
const int IR_FRONT_LEFT_ORD = 3;
const int IR_LEFT_SIDE_FRONT_ORD = 4;
const int IR_RIGHT_SIDE_FRONT_ORD = 5;

__IO uint16_t aADCDualConvertedValue[5];


void adc_init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	DMA_InitTypeDef DMA_InitStructure;


	DMA_InitStructure.DMA_Channel = DMA_Channel_0;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(ADC1->DR) ;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&aADCDualConvertedValue;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = SIZE;
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

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);



	  /* DMA2 Stream0 channel 0 configuration */

	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_1;
	ADC_CommonInitStructure.ADC_Mode = ADC_DualMode_RegSimult;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
	ADC_CommonInit(&ADC_CommonInitStructure);

	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = SIZE;

	ADC_Init(ADC1, &ADC_InitStructure);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, IR_BOTTOM_ORD, ADC_SampleTime_3Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, IR_FRONT_LEFT_ORD, ADC_SampleTime_3Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, IR_FRONT_RIGHT_ORD, ADC_SampleTime_3Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_12, IR_LEFT_SIDE_FRONT_ORD, ADC_SampleTime_3Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_14, IR_RIGHT_SIDE_FRONT_ORD, ADC_SampleTime_3Cycles);

	ADC_MultiModeDMARequestAfterLastTransferCmd(ENABLE);


	ADC_Cmd(ADC1, ENABLE);
	delay2(100);

	ADC_SoftwareStartConv(ADC1);

}




int get_ir_buttom(){
	ADC_SoftwareStartConv(ADC1);
	return aADCDualConvertedValue[IR_BOTTOM_ORD];
}

int get_ir_front_left(){
	ADC_SoftwareStartConv(ADC1);
	return aADCDualConvertedValue[IR_FRONT_LEFT_ORD];
}

int get_ir_front_right(){
	ADC_SoftwareStartConv(ADC1);
	return aADCDualConvertedValue[IR_FRONT_RIGHT_ORD];
}

int get_ir_right_side_front(){
	return aADCDualConvertedValue[IR_RIGHT_SIDE_FRONT_ORD];
}

int get_ir_left_side_front(){
	return aADCDualConvertedValue[IR_LEFT_SIDE_FRONT_ORD];
}

