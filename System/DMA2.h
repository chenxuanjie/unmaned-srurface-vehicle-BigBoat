#ifndef __DMA2_H__
#define __DMA2_H__

#include "ADC1.h"
#include "USART_DMA.h"

/* USART1 DMA Channel */
#define USART1_RX_Channel	DMA_Channel_4
#define USART1_RX_Stream	DMA2_Stream5
/* ADC1 DMA Channel */
#define ADC1_Channel	DMA_Channel_0
#define ADC1_Stream		DMA2_Stream0

extern char Datas2[USART_DMA_DATA_MAXSIZE];
extern __IO uint16_t TempDatas_ADC1[ADC_ConvertValue];

void DMA2_USART1_Init(void);
void DMA2_ADC1_Init(void);
void DMA2_USART1_Start(DMA_Stream_TypeDef* DMAy_Streamx);
void DMA2_ADC1_Start(void);

#endif
