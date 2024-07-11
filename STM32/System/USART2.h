#ifndef __USART2_H__
#define __USART2_H__

#include <stdio.h>


/*USART TX Pin definition */
#define USART2_TX_RCC	RCC_AHB1Periph_GPIOD
#define USART2_TX_GPIO	GPIOD
#define USART2_TX_PIN	GPIO_Pin_5
/*USART RX Pin definition */
#define USART2_RX_RCC	RCC_AHB1Periph_GPIOD
#define USART2_RX_GPIO	GPIOD
#define USART2_RX_PIN	GPIO_Pin_6

void USART2_Config(uint32_t BaudRate);
void Usart2_SendChar(uint16_t c);
void Usart2_SendString(char *str);

#endif
