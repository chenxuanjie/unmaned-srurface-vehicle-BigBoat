#include "stm32f4xx.h" 
#include "USART2.h"
#include "USART3.h"
#include "LED.h"


void USART2_Config(uint32_t BaudRate)
{
	RCC_AHB1PeriphClockCmd(USART2_TX_RCC | USART2_RX_RCC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
	// AF config
	GPIO_PinAFConfig(USART2_TX_GPIO, GPIO_PinSource5,GPIO_AF_USART2);  
	GPIO_PinAFConfig(USART2_RX_GPIO, GPIO_PinSource6,GPIO_AF_USART2); 
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = USART2_TX_PIN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;   //上拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART2_TX_GPIO, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = USART2_RX_PIN;	
	GPIO_Init(USART2_RX_GPIO, &GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = BaudRate;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART2, &USART_InitStructure);

	USART_ClearFlag(USART2, USART_FLAG_TC);
	USART_ClearFlag(USART2, USART_FLAG_RXNE);
	
	//中断使能
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	
	USART_Cmd(USART2, ENABLE);
}

void Usart2_SendChar(uint16_t c)
{
	/* 发送一个字节数据到USART */
	USART_SendData(USART2, c);
		
	/* 等待发送数据寄存器为空 */
	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET) ;		
}

void Usart2_SendString(char *str)
{
	unsigned int k=0;
  do 
  {
	/* 发送一个字节数据到USART */
	USART_SendData(USART2,*(str + k));
		
	/* 等待发送数据寄存器为空 */
	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET) ;	
    k++;
  } while(*(str + k)!='\0');
  
  /* 等待发送完成 */
  while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET)
  {}
}

void USART2_IRQHandler(void)
{
	static uint16_t Data = 0;

	if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
	{
		Data = USART_ReceiveData(USART2);

		USART_SendData(USART3, Data);
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET);
	}
	USART_ClearITPendingBit(USART2, USART_IT_RXNE);
}
