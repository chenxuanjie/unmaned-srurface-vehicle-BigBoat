#include "stm32f4xx.h"                  // Device header
#include "TIM1.h"
#include "TIM2.h"
#include "Motor.h"
#include "main.h"
#include "USART1.h"
#include "TIM10.h"
#include "TIM11.h"

void Motor_Init(void)
{
	TIM1_Init();
	TIM2_Init();
}

void SideMotor_Init(void)
{
	TIM10_Init();
	TIM11_Init();
}

uint32_t MotorY_GetRemoteOriPWM(void)
{
	// rising edge / period
	return (TIM_GetCapture2(TIM2) + 1 );
}

uint32_t MotorX_GetRemoteOriPWM(void)
{
	// rising edge / period
	return (TIM_GetCapture4(TIM2) + 1);
}

/**
  * @brief  
  * @param  
  * @retval LPWM :the range of LPWM is 1.6~2.2ms if mainmotor go,
			and 0.8ms~1.4ms if mainmotor back.
			the range of high level of remote is 1ms~2ms.
  */
uint32_t MotorLeft_GetRemotePWM(void)
{
	int32_t LPWM = MotorY_GetRemoteOriPWM();
	int32_t PWM_Down = 0;
	
	/* input data error */
	if (MotorY_GetRemoteOriPWM() < (REMOTE_PWM_MIN - 50) || MotorY_GetRemoteOriPWM() > (REMOTE_PWM_MAX + 50))
		return 0;
	
	/* if boat turn right */
	if ((PWM_Down = REMOTE_PWM_MID  - MotorX_GetRemoteOriPWM()) < 0)
		PWM_Down = 0;
	
	if ((int32_t)(MotorY_GetRemoteOriPWM() - REMOTE_PWM_MID) >= 0)	/* if boat advance */
	{
		LPWM -= PWM_Down / 2;
	}
	else if ((int32_t)(MotorY_GetRemoteOriPWM() - REMOTE_PWM_MID) < 0)		/* boat backward */
	{
		LPWM += PWM_Down / 2;
	}
	else
		LPWM = 0;

	return LPWM;
}

/**
  * @brief  利用比例方法获取增量PWM，从而实现一电机正转，一电机反转
  * @param  None
  * @retval LPWM :the range of LPWM is 1.6~2.2ms if mainmotor go,
			and 0.8ms~1.4ms if mainmotor back.
			the range of high level of remote is 1ms~2ms.
  */
uint32_t MotorLeft_GetRemotePWM2(void)
{
	/* formula: (dynamic value - direction middle) / (half range / 2) */
	int32_t LPWM_Add = MotorY_GetRemoteOriPWM() - REMOTE_PWM_MID;
	int32_t LPWM = 0;
	float Ratio = 0;
	
	/* input data error */
	if (MotorY_GetRemoteOriPWM() < (REMOTE_PWM_MIN - 50) || MotorY_GetRemoteOriPWM() > (REMOTE_PWM_MAX + 50))
		return 0;
	
	/* if boat turn right */
	if ((int32_t)(REMOTE_PWM_MID  - MotorX_GetRemoteOriPWM()) < 0)
		return MotorY_GetRemoteOriPWM();
	/* X值在左边时，距离左半中点位置对应的比例系数 */
	Ratio = ((float)MotorX_GetRemoteOriPWM() - REMOTE_PWM_LEFTHALF) / ((REMOTE_PWM_MID - REMOTE_PWM_MIN2) / 2);
	LPWM_Add *= Ratio;
	LPWM = REMOTE_PWM_MID + LPWM_Add;
	return LPWM;
}

uint32_t SideMotorLeft_GetRemotePWM(void)
{
	return MotorX_GetRemoteOriPWM();
}

/**
  * @brief  
  * @param  
  * @retval RPWM :the range of LPWM is 1.6~2.2ms if mainmotor go,
			and 0.8ms~1.4ms if mainmotor back.
			the range of high level of remote is 1ms~2ms.
  */
uint32_t MotorRight_GetRemotePWM(void)
{
	int32_t RPWM = MotorY_GetRemoteOriPWM();
	int32_t PWM_Down = 0;
	
	/* input data error */
	if (MotorY_GetRemoteOriPWM() < (REMOTE_PWM_MIN - 50) || MotorY_GetRemoteOriPWM() > (REMOTE_PWM_MAX + 50))
		return 0;
	
	/* if boat turn left */
	if ((PWM_Down = MotorX_GetRemoteOriPWM() - REMOTE_PWM_MID) < 0)	
		PWM_Down = 0;

	if ((int32_t)(MotorY_GetRemoteOriPWM() - REMOTE_PWM_MID) >= 0)	/* if boat advance */
	{
		RPWM -= PWM_Down / 2;
		
	}
	else if ((int32_t)(MotorY_GetRemoteOriPWM() - REMOTE_PWM_MID) < 0)		/* boat backward */
	{
		RPWM += PWM_Down / 2;
	}
	else
		RPWM = 0;

	return RPWM;
}

/**
  * @brief  利用比例方法获取增量PWM，从而实现一电机正转，一电机反转
  * @param  None
  * @retval RPWM :the range of LPWM is 1.6~2.2ms if mainmotor go,
			and 0.8ms~1.4ms if mainmotor back.
			the range of high level of remote is 1ms~2ms.
  */
uint32_t MotorRight_GetRemotePWM2(void)
{
	/* formula: (dynamic value - direction middle) / (half range / 2) */
	int32_t RPWM_Add = MotorY_GetRemoteOriPWM() - REMOTE_PWM_MID;
	int32_t RPWM = 0;
	float Ratio = 0;
	
	/* input data error */
	if (MotorY_GetRemoteOriPWM() < (REMOTE_PWM_MIN - 50) || MotorY_GetRemoteOriPWM() > (REMOTE_PWM_MAX + 50))
		return 0;

	/* if boat turn left */
	if (((int32_t)MotorX_GetRemoteOriPWM() - REMOTE_PWM_MID) < 0)	
		return MotorY_GetRemoteOriPWM();
	/* X值在右边时，距离右半中点位置对应的比例系数 */
	Ratio = (REMOTE_PWM_RIGHTHALF - (float)MotorX_GetRemoteOriPWM()) / ((REMOTE_PWM_MID - REMOTE_PWM_MIN2) / 2);
	RPWM_Add *= Ratio;
	RPWM = REMOTE_PWM_MID + RPWM_Add;
	return RPWM;
}

uint32_t SideMotorRight_GetRemotePWM(void)
{
	return MotorX_GetRemoteOriPWM();
}

void MotorLeft_SetPWM(int32_t PWM)
{
	/* input data error */
	if (PWM < 750 || PWM > 2250)
		PWM = 0;

	TIM_SetCompare3(TIM1, PWM);
}

void MotorRight_SetPWM(int32_t PWM)
{
	/* input data error */
	if (PWM < 750 || PWM > 2250)
		PWM = 0;

	TIM_SetCompare4(TIM1, PWM);
}

void SideMotorLeft_SetPWM(int32_t PWM)
{
	/* input data error */
	if (PWM < 750 || PWM > 2250)
		PWM = 0;

	TIM_SetCompare1(TIM10, PWM);
}

void SideMotorRight_SetPWM(int32_t PWM)
{
	/* input data error */
	if (PWM < 750 || PWM > 2250)
		PWM = 0;

	TIM_SetCompare1(TIM11, PWM);
}

void Motor_Start(void)
{
	TIM_CCxCmd(TIM1, TIM_Channel_3, TIM_CCx_Enable);
	TIM_CCxCmd(TIM1, TIM_Channel_4, TIM_CCx_Enable);
}

void SideMotor_Start(void)
{
	TIM_CCxCmd(TIM10, TIM_Channel_1, TIM_CCx_Enable);
	TIM_CCxCmd(TIM11, TIM_Channel_1, TIM_CCx_Enable);	
}

void Motor_Stop(void)
{
	MotorLeft_SetPWM(0);
	MotorRight_SetPWM(0);
	TIM_CCxCmd(TIM1, TIM_Channel_3, TIM_CCx_Disable);
	TIM_CCxCmd(TIM1, TIM_Channel_4, TIM_CCx_Disable);
}

void SideMotor_Stop(void)
{
	MotorLeft_SetPWM(0);
	MotorRight_SetPWM(0);
	TIM_CCxCmd(TIM10, TIM_Channel_1, TIM_CCx_Disable);
	TIM_CCxCmd(TIM11, TIM_Channel_1, TIM_CCx_Disable);
}

///**
//  * @brief  利用差值法调整PWM。左右差值，向上负变正，向下正变负。
//  * @param  
//  * @retval LPWM :the range of LPWM is 1.6~2.2ms if mainmotor go,
//			and 0.8ms~1.4ms if mainmotor back.
//			the range of high level of remote is 1ms~2ms.
//  */
//uint32_t MotorLeft_GetRemotePWM(void)
//{
//	int32_t LPWM = MotorY_GetRemoteOriPWM();
//	int32_t PWM_Difference = 0, PWM_Add_Value = 0;
//	
//	/* input data error */
//	if (MotorY_GetRemoteOriPWM() < (REMOTE_PWM_MIN - 50) || MotorY_GetRemoteOriPWM() > (REMOTE_PWM_MAX + 50))
//		return 0;

//	/* add pwm value if boat go or back */
//	PWM_Add_Value = (int32_t)(MotorY_GetRemoteOriPWM() - REMOTE_PWM_MID)
//	LPWM += PWM_Add_Value;	
//		
//	/* Calculate difference value */
//	PWM_Difference = (REMOTE_PWM_MID  - MotorX_GetRemoteOriPWM());	
//	
//	/* if boat turn left */
//	if (PWM_Difference > 0)	
//	{
//		if (PWM_Add_Value > 0)			/* boat forward */
//			LPWM -= PWM_Difference;		/* substract difference value when y add */
//	}	
//	
//	/* if boat turn right */
//	if (PWM_Difference < 0)
//	{
//		/* if boat forward, PWM+, and if boat backward, PWM- */
//		LPWM += PWM_Difference;	
//	}

//	return LPWM;
//}

///**
//  * @brief  
//  * @param  
//  * @retval RPWM :the range of LPWM is 1.6~2.2ms if mainmotor go,
//			and 0.8ms~1.4ms if mainmotor back.
//			the range of high level of remote is 1ms~2ms.
//  */
//uint32_t MotorRight_GetRemotePWM(void)
//{
//	int32_t RPWM = MotorY_GetRemoteOriPWM();
//	int32_t PWM_Difference = 0, PWM_Add_Value = 0;
//	
//	/* input data error */
//	if (MotorY_GetRemoteOriPWM() < (REMOTE_PWM_MIN - 50) || MotorY_GetRemoteOriPWM() > (REMOTE_PWM_MAX + 50))
//		return 0;

//	/* add pwm value if boat go or back */
//	PWM_Add_Value = (int32_t)(MotorY_GetRemoteOriPWM() - REMOTE_PWM_MID)
//	RPWM += PWM_Add_Value;	
//		
//	/* Calculate difference value */
//	PWM_Difference = (REMOTE_PWM_MID  - MotorX_GetRemoteOriPWM());	
//	
//	/* if boat turn left */
//	if (PWM_Difference > 0)	
//	{
//		/* if boat forward, PWM+, and if boat backward, PWM- */
//		RPWM += PWM_Difference;	
//	}	
//	
//	/* if boat turn right */
//	if (PWM_Difference < 0)
//	{
//		/* if boat forward, PWM+, and if boat backward, PWM- */
//		LPWM += PWM_Difference;	
//	}

//	return RPWM;
//}
