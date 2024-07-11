/**
*Program: 	无人艇下位机
*History
*Date:		2023/11/29	
*Name: 		无人艇下位机
*By:		Shane
*Version:	v1.03	 release
			v1.02 	2023/11/29
**/
#include "stm32f4xx.h" 
#include "LED.h" 
#include "Delay.h" 
#include "USART_DMA.h"
#include "USART3.h"
#include "SWIC.h"
#include "Servo.h"
#include "Motor.h"
#include "Pushrod.h"
#include "main.h"
#include "Bluetooth.h"
#include "PID.h"
#include "Measure.h"
#include "TIM8.h"
#include <string.h>

static void Controller_Config(ControllerInit* ControllerStructure);
PID mypid;

void test(void)
{
//	printf("%f\t", Measure_GetVoltage2());

//	printf("%d\t", MotorY_GetRemoteOriPWM());
//	printf("%d\t", MotorX_GetRemoteOriPWM());
//	printf("%d\t", Pushrod_GetRemoteOriPWM());
//	printf("%d\t", Servo_GetRemoteOriPWM());
//	
//	printf("%d\t", SWIC1_GetRemoteOriPWM());
//	printf("%d\t", SWIC2_GetRemoteOriPWM());
//	printf("%d\t", SWIC3_GetRemoteOriPWM());
//	printf("%d\t", SWIC4_GetRemoteOriPWM());
	
	
	printf("%d\t", MotorLeft_GetRemotePWM());
	printf("%d\t", MotorRight_GetRemotePWM());
	printf("%d\t", Pushrod_GetRemotePWM());
	printf("%d\t", Servo_GetRemotePWM());
	
	printf("%d\t", Conveyor_GetPWM());
	printf("%d\t", Pump_GetPWM());
	
	printf("\r\n");
}

int main(void)
{
	// define Struct
	MotorDataInit LMainMotor, RMainMotor;	/* 定义左主推，右主推结构体 */
	ServoDataInit LServo, RServo;			/* 定义左舵机，右舵机结构体 */
	PushrodDataInit LPushrod, RPushrod;		/* 定义左推杆，右推杆结构体 */
	PumpDataInit Pump;						/* 定义水泵结构体 */
	ConveyorDataInit Conveyor;				/* 定义传送带结构体 */
	ControllerInit Controller;				/* 定义遥控模式结构体 */
	Info myInfo;							/* 定义追踪点数据结构体 */
	double V1, V2;							/* 需要测量的左电池电压V1、右电池电压V2 */
	char To_server_buf[100];				/* 发送至服务器端的字符串 */
	
	// test
	LED_Init();
	Bluetooth_Init(RESET, RESET);
	USART_DMA_Init(115200);
	Motor_Init();
	Servo_Init();
	Pushrod_Init();
	SWIC_Init();
	Measure_Init();
	while(1)
	{
		test();
		
		/*-- Bluetooth monitor number */
		Bluetooth_SetNum_f(&(mypid.kd), 'd');
		Bluetooth_UpdateBuf();
		
		/* Get string from PC */
		USART_DMA_GetInfo(&myInfo);
		USART_DMA_UpdateBuf();
		
		/* Get voltage of left and right battery */
		V1 = Measure_GetVoltage();
		V2 = Measure_GetVoltage();
		
		/* Send string to PC following protocol */
		if (ready_send_server)
		{
			ready_send_server = 0;
			sprintf(To_server_buf, "Q,%f,%f,", V1, V2);
			USART_DMA_SendString(To_server_buf);				
		}
		Controller_Config(&Controller);
		/*-- remote mode ------------------------------------------------------------------*/
		if (SET == Controller.RemoteModeEN)
		{
			/* start work */
			Motor_Start();
			Servo_Start();
			Pushrod_Start();
			Pump_Start();
			Conveyor_Start();
			
			/*-- remote mode 2 --------------------------------*/
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
			/*-- end of remote mode 2 -------------------------*/
			
			LServo.PWM = Servo_GetRemotePWM();
			RServo.PWM = Servo_GetRemotePWM();
			LPushrod.PWM = Pushrod_GetRemotePWM();
			RPushrod.PWM = Pushrod_GetRemotePWM();
			Pump.PWM = Conveyor_GetPWM();
			Conveyor.PWM = Conveyor_GetPWM();
		}
		/*-- auto mode --------------------------------------------------------------------*/
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
	Pump_SetPWM(Pump.PWM);		
	Conveyor_SetPWM(Conveyor.PWM);
	}
}

static void Controller_Config(ControllerInit* ControllerStructure)
{
	/* Switch 2: enable remote control or not */
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
	
	/* Switch 1: open auto control mode or not */
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
