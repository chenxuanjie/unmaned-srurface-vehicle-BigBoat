#include "stm32f4xx.h"                  // Device header
#include "Pushrod.h"
#include "TIM3.h"
#include "TIM8.h"

void Pushrod_Init(void)
{
	TIM3_Init();
	TIM8_Init();
}

uint32_t Pushrod_GetRemoteOriPWM(void)
{
	// rising edge / period
	return (float)TIM_GetCapture2(TIM3);
}

uint32_t Pushrod_GetRemotePWM(void)
{
	uint32_t PWM = Pushrod_GetRemoteOriPWM();
	return PWM; 
}

void PushrodLeft_SetPWM(int32_t PWM)
{
	TIM_SetCompare1(TIM8, PWM);
}

void PushrodRight_SetPWM(int32_t PWM)
{
	TIM_SetCompare2(TIM8, PWM);
}

void Pushrod_Start(void)
{
	TIM_CCxCmd(TIM8, TIM_Channel_1, TIM_CCx_Enable);
	TIM_CCxCmd(TIM8, TIM_Channel_2, TIM_CCx_Enable);
}
	
void Pushrod_Stop(void)
{
	TIM_CCxCmd(TIM8, TIM_Channel_1, TIM_CCx_Disable);
	TIM_CCxCmd(TIM8, TIM_Channel_2, TIM_CCx_Disable);
}
