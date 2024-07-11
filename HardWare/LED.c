#include "stm32f4xx.h" 
#include "LED.h" 

// LED1
#define LED_RCC		RCC_AHB1Periph_GPIOG
#define LED_GPIO	GPIOG
#define LED_PIN		GPIO_Pin_13

void LED_Init(void)
{
	RCC_AHB1PeriphClockCmd(LED_RCC, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = LED_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LED_GPIO, &GPIO_InitStructure);
	
	GPIO_SetBits(LED_GPIO, LED_PIN);
}

void LED_ON(void)
{
	GPIO_ResetBits(LED_GPIO, LED_PIN);
}

void LED_OFF(void)
{
	GPIO_SetBits(LED_GPIO, LED_PIN);
}

void LED_Turn(void)
{
	if (GPIO_ReadOutputDataBit(LED_GPIO, LED_PIN) == 0)
	{
		GPIO_SetBits(LED_GPIO, LED_PIN);
	}
	else
	{
		GPIO_ResetBits(LED_GPIO, LED_PIN);
	}
}

