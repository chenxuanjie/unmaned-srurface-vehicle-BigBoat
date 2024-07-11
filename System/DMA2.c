#include "stm32f4xx.h"                  // Device header
#include "DMA2.h"
#include "LED.h"
#include "stdio.h"

char Datas2[USART_DMA_DATA_MAXSIZE];
__IO uint16_t TempDatas_ADC1[ADC_ConvertValue] = {1,2};


void DMA2_USART1_Init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	
	DMA_DeInit(USART1_RX_Stream);

  /* ensure DMA reset completion */
  while (DMA_GetCmdStatus(USART1_RX_Stream) != DISABLE);
  
	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_Channel = USART1_RX_Channel;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(USART1->DR));
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)Datas2;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;	//来自外设
	DMA_InitStructure.DMA_BufferSize = USART_DMA_DATA_MAXSIZE;	/* SET size of buff */
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; /* 工作在正常模式，而非循环 */
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

	DMA_Init(USART1_RX_Stream, &DMA_InitStructure);
	DMA_Cmd(USART1_RX_Stream, ENABLE);
	
	/* wait for stream accessable */
	while(DMA_GetCmdStatus(USART1_RX_Stream) != ENABLE);
}

void DMA2_ADC1_Init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	
	DMA_DeInit(ADC1_Stream);
	/* ensure DMA reset completion */
	while (DMA_GetCmdStatus(ADC1_Stream) != DISABLE);
  
	/* USART1 Initialization*/
	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_Channel = ADC1_Channel;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(ADC1->DR));
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)TempDatas_ADC1;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;	//来自外设
	DMA_InitStructure.DMA_BufferSize = ADC_ConvertValue;	/* SET size of buff */
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(ADC1_Stream, &DMA_InitStructure);
	
	DMA_ClearFlag(ADC1_Stream,DMA_FLAG_TCIF0);
	DMA_Cmd(ADC1_Stream, ENABLE);

	/* wait for stream accessable */
	while(DMA_GetCmdStatus(ADC1_Stream) != ENABLE);		
}

void DMA2_USART1_Start(DMA_Stream_TypeDef* DMAy_Streamx)
{ 
	DMA_Cmd(DMAy_Streamx, DISABLE);
	DMA_ClearFlag(DMAy_Streamx,DMA_FLAG_TCIF5 | DMA_FLAG_FEIF5 | DMA_FLAG_DMEIF5 | DMA_FLAG_TEIF5 | DMA_FLAG_HTIF5);
	DMA_SetCurrDataCounter(DMAy_Streamx, USART_DMA_DATA_MAXSIZE);
	DMA_Cmd(DMAy_Streamx, ENABLE);  
}  

void DMA2_ADC1_Start(void)
{ 
	DMA_Cmd(ADC1_Stream, DISABLE);
	DMA_ClearFlag(ADC1_Stream,DMA_FLAG_TCIF0);
	DMA_SetCurrDataCounter(ADC1_Stream, ADC_ConvertValue);
	DMA_Cmd(ADC1_Stream, ENABLE);  
}
