#ifndef __USART3_H__
#define __USART3_H__

#include <stdio.h>

void USART3_Config(uint32_t BaudRate);
void USART3_SetHandler(void (*Func)(void));

#endif
