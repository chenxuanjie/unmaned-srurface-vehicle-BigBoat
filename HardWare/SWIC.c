#include "stm32f4xx.h"                  // Device header
#include "SWIC.h"
#include "TIM4.h"
#include "TIM5.h"
#include "USART1.h"

void SWIC_Init(void)
{
	TIM4_Init();
	TIM5_Init();
}

int8_t SWIC_GetRemoteState(uint8_t SWN)
{
	uint32_t PWM = 0;
	uint32_t (* Duty_ptr)(void) = NULL;
	
	if (SW1 == SWN)
		Duty_ptr = SWIC1_GetRemoteOriPWM;
	else if (SW2 == SWN)
		Duty_ptr = SWIC2_GetRemoteOriPWM;
	else if (SW3 == SWN)
		Duty_ptr = SWIC3_GetRemoteOriPWM;
	else if (SW4 == SWN)
		Duty_ptr = SWIC4_GetRemoteOriPWM;
	else
		return MID;		// 操作非4个开关，返回中间位（不操作）
	PWM = Duty_ptr();
	if (PWM >= SW_ON_PWM)	// 上档
		return UP;
	else if (PWM <= SW_OFF_PWM)	// 下档
		return DOWN;
	else				// 中档
		return MID;
}

uint32_t SWIC1_GetRemoteOriPWM(void)
{
	// rising edge / period
	return TIM_GetCapture2(TIM4);
}

uint32_t SWIC2_GetRemoteOriPWM(void)
{
	return TIM_GetCapture4(TIM4);
}

uint32_t SWIC3_GetRemoteOriPWM(void)
{
	return TIM_GetCapture2(TIM5);
}

uint32_t SWIC4_GetRemoteOriPWM(void)
{
	return TIM_GetCapture4(TIM5);
}


uint32_t Pump_GetPWM(void)
{
	uint32_t PWM = 0;
	if (UP == SWIC_GetRemoteState(SW_PUMP))
		PWM = 2000;	// 2ms
	else if (DOWN == SWIC_GetRemoteState(SW_PUMP))
//		PWM = 1500;	// 1.5ms
		PWM = 0;
	return PWM;	
}

uint32_t Conveyor_GetPWM(void)
{
	uint32_t PWM = 0;
	
	if (UP == SWIC_GetRemoteState(SW_CONVEYOR))
		PWM = 2000;	// 2ms
	else if (DOWN == SWIC_GetRemoteState(SW_CONVEYOR))
		PWM = 0;	// 1.5ms

	return PWM;
}

/**
  * @brief  水泵PWM设置
  * @param  PWM：？
  * @retval 
  */
void Pump_SetPWM(uint32_t PWM)
{
	TIM_SetCompare3(TIM8, PWM);
}

/**
  * @brief  传送带PWM设置
  * @param  PWM: 周期20ms，
				停止：1.5ms
				运行：大于2.0ms
  * @retval 
  */
void Conveyor_SetPWM(uint32_t PWM)
{
	TIM_SetCompare4(TIM8, PWM);
}

/**
  * @brief  启动水泵
  * @param  无
  * @retval 无
  */
void Pump_Start(void)
{
	TIM_CCxCmd(TIM8, TIM_Channel_3, TIM_CCx_Enable);
}	

/**
  * @brief  启动传送带
  * @param  无
  * @retval 无
  */
void Conveyor_Start(void)
{
	TIM_CCxCmd(TIM8, TIM_Channel_4, TIM_CCx_Enable);
}

/**
  * @brief  关闭水泵
  * @param  无
  * @retval 无
  */
void Pump_Stop(void)
{
	TIM_CCxCmd(TIM8, TIM_Channel_3, TIM_CCx_Disable);
}	

/**
  * @brief  关闭传送带
  * @param  无
  * @retval 无
  */
void Conveyor_Stop(void)
{
	TIM_CCxCmd(TIM8, TIM_Channel_4, TIM_CCx_Disable);
}
