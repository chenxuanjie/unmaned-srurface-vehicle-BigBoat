#include "stm32f4xx.h"                  // Device header
#include "ADC1.h"
#include "DMA2.h"
#include "stdio.h"

void ADC1_Init(void)
{
	RCC_AHB1PeriphClockCmd(VOLTAGE_RCC | VOLTAGE2_RCC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);     

	/* Voltage 1 gpio init */
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;  	/* analog in */
	GPIO_InitStructure.GPIO_Pin = VOLTAGE2_PIN; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 	/* no pull */
	GPIO_Init(VOLTAGE2_GPIO,&GPIO_InitStructure);

	/* Voltage 2 gpio init */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;  	/* analog in */
	GPIO_InitStructure.GPIO_Pin = VOLTAGE_PIN; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 	/* no pull */
	GPIO_Init(VOLTAGE_GPIO,&GPIO_InitStructure);
	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);  
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);    

	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4; /* 四分频， APB2的时钟为84MHz，分频后为21MHz */
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; 
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);

	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Resolution=ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode=ENABLE;  	/* 开启扫描模式*/
	ADC_InitStructure.ADC_ContinuousConvMode=ENABLE;	 /* 开启连续转换 */
	ADC_InitStructure.ADC_ExternalTrigConvEdge=ADC_ExternalTrigConvEdge_None; /* disable external trigger */
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;  /* 右对齐 */
	ADC_InitStructure.ADC_NbrOfConversion = ADC_ConvertValue; 	/* 3个转换在规则序列中 */
	ADC_Init(ADC1,&ADC_InitStructure); 
	

	ADC_RegularChannelConfig(ADC1,ADC_Channel_14, 1, ADC_SampleTime_480Cycles);	/* 设置ADC规则组通道，一个序列*/
	ADC_RegularChannelConfig(ADC1,ADC_Channel_15, 2, ADC_SampleTime_480Cycles);	/* 设置ADC规则组通道，一个序列*/
	ADC_RegularChannelConfig(ADC1,ADC_Channel_16, 3, ADC_SampleTime_480Cycles);	/* 设置ADC规则组通道，一个序列*/

	ADC_TempSensorVrefintCmd(ENABLE);
	
	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
	ADC_DMACmd(ADC1, ENABLE);	
	ADC_Cmd(ADC1,ENABLE); 
	
	ADC_SoftwareStartConv(ADC1);	/* start conversion */
//	while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));	/* wait for the end of conversion */

}
