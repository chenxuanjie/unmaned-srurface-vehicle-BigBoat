#include "stm32f4xx.h"                  // Device header
#include "Pushrod.h"
#include "TIM3.h"
#include "TIM8.h"

void Pushrod_Init(void)
{
	TIM3_Init();
	TIM8_Init();
}

uint32_t Pushrod_GetRemoteFreq(void)
{
	// unit: Hz
	return 1000000 / TIM_GetCapture1(TIM3);
}

float Pushrod_GetRemoteOriDuty(void)
{
	// rising edge / period
	return (float)TIM_GetCapture2(TIM3) * 100 / TIM_GetCapture1(TIM3); // unit: % (the reson for "*100")
}

uint32_t Pushrod_GetRemoteDuty(void)
{
	// rising edge / period
	return Pushrod_GetRemoteOriDuty() * (2000 / 100); // unit: % (the reson for "*100")
}

void PushrodLeft_SetPWM(int32_t PWM)
{
	TIM_SetCompare1(TIM8, PWM);
}

void PushrodRight_SetPWM(int32_t PWM)
{
	TIM_SetCompare2(TIM8, PWM);
}