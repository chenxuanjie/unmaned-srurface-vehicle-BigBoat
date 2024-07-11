#ifndef __USART_H__
#define __USART_H__

#include <stdio.h>

extern uint16_t Data;

void USART1_Config(uint32_t BaudRate);
void Usart_SendString( USART_TypeDef * pUSARTx, char *str);
void USART1_SetIRQHandler(void (* Function)(void));

#endif
