#include "stm32f4xx.h"                  // Device header
#include "Servo.h"
#include "TIM1.h"
#include "TIM3.h"
#include "main.h"

void Servo_Init(void)
{
	// 定时器初始化
	TIM1_Init();
	TIM3_Init();
}

uint32_t Servo_GetRemoteOriPWM(void)
{
	// rising edge / period
	return TIM_GetCapture4(TIM3); 
}

/**
  * @brief  
  * @param  
  * @retval PWM :the range of Servo PWM is 0.5ms~2.5ms.
			the range of high level of remote is 1ms~2ms.
  */
uint32_t Servo_GetRemotePWM(void)
{
	uint32_t PWM = Servo_GetRemoteOriPWM();
	/* input data error */
	if (Servo_GetRemoteOriPWM() < REMOTE_PWM_MIN || Servo_GetRemoteOriPWM() > REMOTE_PWM_MAX)
		return 0;
	
	PWM = (PWM - REMOTE_PWM_MIN) * ((float)2000 / (REMOTE_PWM_MAX - REMOTE_PWM_MIN)) + 500;
	return PWM;
}

/**
  * @brief  
  * @param  PWM :the range of Servo PWM is 0.5ms~2.5ms.
  * @retval 
  */
void ServoLeft_SetPWM(uint32_t PWM)
{
	/* input data error */
	if (PWM < 500 || PWM > 2500)
		PWM = 0;

	TIM_SetCompare1(TIM1, PWM);
}

/**
  * @brief  
  * @param  PWM :the range of Servo PWM is 0.5ms~2.5ms.
  * @retval 
  */
void ServoRight_SetPWM(uint32_t PWM)
{
	/* input data error */
	if (PWM < 500 || PWM > 2500)
		PWM = 0;

	TIM_SetCompare2(TIM1, PWM);
}

/**
  * @brief  暂停输出
  * @param  
  * @retval 
  */
void Servo_Reset(void)
{
//	ServoLeft_SetPWM(150);	// 90 degree
//	ServoRight_SetPWM(150);
	TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Disable);
	TIM_CCxCmd(TIM1, TIM_Channel_2, TIM_CCx_Disable);
}

void Servo_Start(void)
{
	TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Enable);
	TIM_CCxCmd(TIM1, TIM_Channel_2, TIM_CCx_Enable);
}	
