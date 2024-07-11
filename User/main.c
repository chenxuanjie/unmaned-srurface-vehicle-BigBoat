/*
Program: 无人艇下位机
History:
	2023/10/9	无人艇下位机v1	Shane	1th release
*/
#include "stm32f4xx.h" 
#include "LED.h" 
#include "Delay.h" 
#include "USART1.h"
#include "USART3.h"
#include "SWIC.h"
#include "Servo.h"
#include "Motor.h"
#include "Pushrod.h"
#include "main.h"
#include "Bluetooth.h"
#include "PID.h"

static void Controller_Config(ControllerInit* ControllerStructure);
PID mypid;

int main(void)
{
	// define Struct
	MotorDataInit LMainMotor, RMainMotor;
	ServoDataInit LServo, RServo;
	PushrodDataInit LPushrod, RPushrod;
	PumpDataInit Pump;
	ConveyorDataInit Conveyor;
	ControllerInit Controller;
	
	// test
	LED_Init();
	Bluetooth_Init(RESET, RESET);
	Motor_Init();
	Servo_Init();
	Pushrod_Init();
	SWIC_Init();
	USART1_Config();
	while(1)
	{
		Bluetooth_SetNum_f(&(mypid.kd), 'd');
		Bluetooth_UpdateBuf();
		
		Controller_Config(&Controller);
		/* remote mode *******************************************************************/
		if (SET == Controller.RemoteModeEN)
		{
			/* start work */
			Motor_Start();
			Servo_Start();
			Pushrod_Start();
			Pump_Start();
			Conveyor_Start();
			
			/* remote mode 2 ********************************/
			if (Controller.RemoteMode2 != RESET)
			{
				LMainMotor.PWM = MotorLeft_GetRemotePWM();
				RMainMotor.PWM = MotorRight_GetRemotePWM();				
			}
			else
			{
				LMainMotor.PWM = MotorLeft_GetRemotePWM2();
				RMainMotor.PWM = MotorRight_GetRemotePWM2();
			}
			/* end of remote mode 2 *************************/
			LServo.PWM = Servo_GetRemotePWM();
			RServo.PWM = Servo_GetRemotePWM();
			LPushrod.PWM = Pushrod_GetRemotePWM();
			RPushrod.PWM = Pushrod_GetRemotePWM();
			Pump.PWM = Conveyor_GetPWM();
			Conveyor.PWM = Conveyor_GetPWM();
		}
		/* auto mode ********************************************************************/
		else if (Controller.AutoModeEN != RESET && Controller.RemoteModeEN != RESET)
		{
			;
		}
		else	// reset
		{
			LMainMotor.PWM = 0;
			RMainMotor.PWM = 0;
			LServo.PWM = 0;
			RServo.PWM = 0;
			LPushrod.PWM = 0;
			RPushrod.PWM = 0;
			Pump.PWM = 0;
			Conveyor.PWM = 0;
			
			Motor_Stop();
			Servo_Reset();
			Pushrod_Stop();
			Pump_Stop();
			Conveyor_Stop();
		}
	// set pwm
	MotorLeft_SetPWM(LMainMotor.PWM);
	MotorRight_SetPWM(RMainMotor.PWM);
	ServoLeft_SetPWM(LServo.PWM);
	ServoRight_SetPWM(3000 - RServo.PWM);
	PushrodLeft_SetPWM(LPushrod.PWM);
	PushrodRight_SetPWM(RPushrod.PWM);
	Pump_SetPWM(Pump.PWM);			// 待测试
	Conveyor_SetPWM(Conveyor.PWM);
	}
}

static void Controller_Config(ControllerInit* ControllerStructure)
{
	switch(SWIC_GetRemoteState(SW_REMOTE_EN))
	{
		case UP:
			ControllerStructure->RemoteModeEN = SET;
			break;
		case DOWN:
			ControllerStructure->RemoteModeEN = RESET;
			break;
		
		default:
			ControllerStructure->RemoteModeEN = RESET;
	}
	
	switch(SWIC_GetRemoteState(SW_AUTO))
	{
		case UP:
			ControllerStructure->AutoModeEN = SET;
			ControllerStructure->RemoteMode2 = RESET;
		break;
		case MID:
			ControllerStructure->AutoModeEN = RESET;
			ControllerStructure->RemoteMode2 = RESET;
		break;
		case DOWN:
			ControllerStructure->AutoModeEN = RESET;
			ControllerStructure->RemoteMode2 = SET;
			break;

		default:
			ControllerStructure->AutoModeEN = RESET;
			ControllerStructure->RemoteMode2 = RESET;
	}
}
