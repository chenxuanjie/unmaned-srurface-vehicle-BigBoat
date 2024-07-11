#ifndef __USART1_H__
#define __USART1_H__

#include <stdio.h>

extern uint16_t Data;

void USART1_Config(void);
void Usart_SendString( USART_TypeDef * pUSARTx, char *str);
uint16_t USART1_ReturnData(void);

#endif
