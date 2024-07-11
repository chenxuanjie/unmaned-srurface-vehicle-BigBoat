#ifndef __ADC1_H
#define __ADC1_H

#define ADC_ConvertValue	3
/* Voltage definition */
#define VOLTAGE_VALUE		0
#define VOLTAGE_RCC			RCC_AHB1Periph_GPIOC
#define VOLTAGE_GPIO		GPIOC
#define VOLTAGE_PIN			GPIO_Pin_4
/* Voltage2 definition */
#define VOLTAGE2_VALUE		1
#define VOLTAGE2_RCC		RCC_AHB1Periph_GPIOC
#define VOLTAGE2_GPIO		GPIOC
#define VOLTAGE2_PIN		GPIO_Pin_5
/* Temperature definition */
#define TEMPERATURE_VALUE	2

void ADC1_Init(void);

#endif
