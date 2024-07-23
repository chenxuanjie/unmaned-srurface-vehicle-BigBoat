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
#include <stdlib.h>
#include "FormatConversion.h"


PID mypid;

void test(void)
{
//	printf("%f\t", Measure_GetVoltage2());
//	printf("M:%f,%f,%f,%f,\t", Measure_GetVoltage(), Measure_GetVoltage2(),Measure_GetOriVoltage(),Measure_GetOriVoltage2());

//	printf("%d\t", MotorY_GetRemoteOriPWM());
//	printf("%d\t", MotorX_GetRemoteOriPWM());
//	printf("%d\t", Pushrod_GetRemoteOriPWM());
//	printf("%d\t", Servo_GetRemoteOriPWM());
//	
//	printf("%d\t", SWIC1_GetRemoteOriPWM());
//	printf("%d\t", SWIC2_GetRemoteOriPWM());
//	printf("%d\t", SWIC3_GetRemoteOriPWM());
//	printf("%d\t", SWIC4_GetRemoteOriPWM());
	
	
//	printf("%d\t", MotorLeft_GetRemotePWM());
//	printf("%d\t", MotorRight_GetRemotePWM());
//	printf("%d\t", Pushrod_GetRemotePWM());
//	printf("%d\t", Servo_GetRemotePWM());
	
//	printf("%d\t", Conveyor_GetPWM());
//	printf("%d\t", Pump_GetPWM());
	
//	printf("\r\n");
	
//	Measure_GetR();
}

int main(void)
{
	// define Struct
	MotorDataInit LMainMotor, RMainMotor;	/* 定义左主推，右主推结构体 */
	MotorDataInit LSideMotor, RSideMotor;	/* 定义左侧推，右侧推结构体 */
	ServoDataInit LServo, RServo;			/* 定义左舵机，右舵机结构体 */
	PushrodDataInit LPushrod, RPushrod;		/* 定义左推杆，右推杆结构体 */
	PumpDataInit Pump;						/* 定义水泵结构体 */
	ConveyorDataInit Conveyor;				/* 定义传送带结构体 */
	MessageFromNXInit MessageFromNX;		/* 定义追踪点数据结构体 */
	double V1, V2;							/* 需要测量的左电池电压V1、右电池电压V2 */
	char To_server_buf[100];				/* 发送至服务器端的字符串 */
	char str_from_nx[USART_DMA_DATA_MAXSIZE];
	char* sub_strings[SUB_STRING_MAX_SIZE];
	int8_t boat_stop_flag;
	
	LED_Init();
	Bluetooth_Init(9600, RESET, RESET);
	USART_DMA_Init(115200);
	Motor_Init();
	SideMotor_Init();
	Servo_Init();
	Pushrod_Init();
	SWIC_Init();
	Measure_Init();
	while(1)
	{

		test();
		
		/*-- Bluetooth/2.4G monitor number */
		Bluetooth_SetNum_f(&(mypid.kd), 'd');
		Bluetooth_UpdateBuf();
		
		/* Get string from PC */
		USART_DMA_GetStringFromNX(str_from_nx);
		USART_DMA_UpdateBuf();
		
		int8_t count = SplitString(str_from_nx, sub_strings);
		if (count == SUB_STRING_SIZE)
		{
			MessageFromNX.LMainMotor_PWM = atoi(sub_strings[1]);		
			MessageFromNX.RMainMotor_PWM = atoi(sub_strings[2]);
			MessageFromNX.LSideMotor_PWM = atoi(sub_strings[3]);
			MessageFromNX.RSideMotor_PWM = atoi(sub_strings[4]);
			printf("%d,%d,%d,%d\r\n", MessageFromNX.LMainMotor_PWM, MessageFromNX.RMainMotor_PWM, MessageFromNX.LSideMotor_PWM, MessageFromNX.RSideMotor_PWM);
		}		
		for (int j = 0; j < count; j++) {
			free(sub_strings[j]);
		}
		
		/* Get voltage of left and right battery */
		V1 = Measure_GetVoltage();
		V2 = Measure_GetVoltage2();
		
		/* Send string to PC following protocol */
		if (ready_send_server)
		{
			ready_send_server = RESET;
			sprintf(To_server_buf, "M,%.3f,%.3f,\n", V1, V2);
			USART_DMA_SendString(To_server_buf);				
		}

		/* Switch 1: check control mode */
		boat_stop_flag = RESET;		// clear flag
		
		switch(SWIC_GetRemoteState(SW1))
		{
			/*-- auto mode --------------------------------------------------------------------*/
			case UP:
				printf("1111\r\n");
				LMainMotor.PWM = MessageFromNX.LMainMotor_PWM;
				RMainMotor.PWM = MessageFromNX.RMainMotor_PWM;
				LSideMotor.PWM = MessageFromNX.LSideMotor_PWM;
				RSideMotor.PWM = MessageFromNX.RSideMotor_PWM;
			break;
			/*-- end of auto mode -------------------------------------------------------------*/
			
			/*-- test mode --------------------------------------------------------------------*/
			case MID:			
				LMainMotor.PWM = MotorLeft_GetRemotePWM2();
				RMainMotor.PWM = MotorRight_GetRemotePWM2();
			
				LServo.PWM = Servo_GetRemotePWM();
				RServo.PWM = Servo_GetRemotePWM();
				LPushrod.PWM = Pushrod_GetRemotePWM();
				RPushrod.PWM = Pushrod_GetRemotePWM();
				Pump.PWM = Pump_GetPWM();
				Conveyor.PWM = Conveyor_GetPWM();
			break;
			/*-- end of test mode -------------------------------------------------------------*/
			
			/*-- remote mode ------------------------------------------------------------------*/
			case DOWN:
				LMainMotor.PWM = MotorLeft_GetRemotePWM();
				RMainMotor.PWM = MotorRight_GetRemotePWM();
				LSideMotor.PWM = SideMotorLeft_GetRemotePWM();
				RSideMotor.PWM = SideMotorRight_GetRemotePWM();
			
				LServo.PWM = Servo_GetRemotePWM();
				RServo.PWM = Servo_GetRemotePWM();
				LPushrod.PWM = Pushrod_GetRemotePWM();
				RPushrod.PWM = Pushrod_GetRemotePWM();
				Pump.PWM = Pump_GetPWM();
				Conveyor.PWM = Conveyor_GetPWM();
			break;
			/*-- end of remote mode -----------------------------------------------------------*/
			
			default:
				boat_stop_flag = SET;
		}

		/* Switch2: check remote enable or not */
		switch(SWIC_GetRemoteState(SW2))
		{
			case UP:	// start work
				boat_stop_flag = RESET;
			break;
			
			case DOWN:	// reset
				boat_stop_flag = SET;
			break;
			
			default:
				boat_stop_flag = SET;
		}
	
	/*-- Boat Overall Pwm Configuration -----------------------------------------------------------*/
	if (boat_stop_flag)		// stop and reset config
	{
		LMainMotor.PWM = 0;
		RMainMotor.PWM = 0;
		LSideMotor.PWM = 1500;
		RSideMotor.PWM = 1500;
		LServo.PWM = 0;
		RServo.PWM = 0;
		LPushrod.PWM = 0;
		RPushrod.PWM = 0;
		Pump.PWM = 0;
		Conveyor.PWM = 0;
		
		Motor_Stop();
		SideMotor_Stop();
		Servo_Reset();
		Pushrod_Stop();
		Pump_Stop();
		Conveyor_Stop();
	}
	else
	{
		Motor_Start();
		SideMotor_Start();
		Servo_Start();
		Pushrod_Start();
		Pump_Start();
		Conveyor_Start();		
	}
	
	// set pwm
	MotorLeft_SetPWM(LMainMotor.PWM);
	MotorRight_SetPWM(RMainMotor.PWM);
	SideMotorLeft_SetPWM(LSideMotor.PWM);
	SideMotorRight_SetPWM(RSideMotor.PWM);
	ServoLeft_SetPWM(LServo.PWM);
	ServoRight_SetPWM(3000 - RServo.PWM);
	PushrodLeft_SetPWM(LPushrod.PWM);
	PushrodRight_SetPWM(RPushrod.PWM);
	Pump_SetPWM(Pump.PWM);		
	Conveyor_SetPWM(Conveyor.PWM);
	}
}
