#ifndef __DMA1_H__
#define __DMA1_H__

#include "Bluetooth.h"

#define USART3_RX_Channel	DMA_Channel_4
#define USART3_RX_Stream	DMA1_Stream1
extern char Datas[BLUETOOTH_DATA_MAXSIZE];

void DMA1_Init(void);
void DMA1_Start(DMA_Stream_TypeDef* DMAy_Streamx);

#endif
