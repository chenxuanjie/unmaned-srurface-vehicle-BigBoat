#include "stm32f4xx.h" 
#include "USART3.h"
#include "LED.h"

void (*IRQHandler)(void);

void USART3_Config(uint32_t BaudRate)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	
	// AF config
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource8,GPIO_AF_USART3);  
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource9,GPIO_AF_USART3); 
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;   //����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART3, &USART_InitStructure);
	
	//�ж�ʹ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);
	
	USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);
	USART_Cmd(USART3, ENABLE);
}

///�ض���c�⺯��printf�����ڣ��ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{
		/* ����һ���ֽ����ݵ����� */
		USART_SendData(USART3, (uint8_t) ch);
		
		/* �ȴ�������� */
		while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

///�ض���c�⺯��scanf�����ڣ���д����ʹ��scanf��getchar�Ⱥ���
int fgetc(FILE *f)
{
		/* �ȴ������������� */
		while (USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USART3);
}

void USART3_SetHandler(void (*Func)(void))
{
	IRQHandler = Func;
}

void USART3_IRQHandler(void)
{
	IRQHandler();
}
