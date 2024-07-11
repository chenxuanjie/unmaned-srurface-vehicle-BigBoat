#include "stm32f4xx.h"                  // Device header
#include "DMA1.h"
#include "Bluetooth.h"

char Datas[BLUETOOTH_DATA_MAXSIZE] = {'a', '\0'};

void DMA1_Init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
	
	DMA_DeInit(USART3_RX_Stream);

  /* ensure DMA reset completion */
  while (DMA_GetCmdStatus(USART3_RX_Stream) != DISABLE);
  
	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_Channel = USART3_RX_Channel;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(USART3->DR));
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)Datas;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;	//来自外设
	DMA_InitStructure.DMA_BufferSize = BLUETOOTH_DATA_MAXSIZE;	/* SET size of buff */
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; /* 工作在正常模式，而非循环 */
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

	DMA_Init(USART3_RX_Stream, &DMA_InitStructure);
	DMA_Cmd(USART3_RX_Stream, ENABLE);
	
	/* wait for stream accessable */
	while(DMA_GetCmdStatus(USART3_RX_Stream) != ENABLE);
}

void DMA1_Start(DMA_Stream_TypeDef* DMAy_Streamx)
{ 
	DMA_Cmd(DMAy_Streamx, DISABLE);
	DMA_ClearFlag(DMAy_Streamx,DMA_FLAG_TCIF1 | DMA_FLAG_FEIF1 | DMA_FLAG_DMEIF1 | DMA_FLAG_TEIF1 | DMA_FLAG_HTIF1);//??DMA1_Steam5??????
	DMA_SetCurrDataCounter(DMAy_Streamx, BLUETOOTH_DATA_MAXSIZE);
	DMA_Cmd(DMAy_Streamx, ENABLE);  
}  
