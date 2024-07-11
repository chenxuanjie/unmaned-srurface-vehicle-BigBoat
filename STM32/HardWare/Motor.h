#ifndef __MOTOR_H
#define __MOTOR_H


typedef struct {
	uint32_t RemoteDuty;
	int16_t PWM;
} MotorDataInit;

void Motor_Init(void);
void Motor_Start(void);
void Motor_Stop(void);
/* Get Remote data ************************/
uint32_t MotorY_GetRemoteOriPWM(void);
uint32_t MotorX_GetRemoteOriPWM(void);
uint32_t MotorLeft_GetRemotePWM(void);
uint32_t MotorRight_GetRemotePWM(void);
uint32_t MotorLeft_GetRemotePWM2(void);
uint32_t MotorRight_GetRemotePWM2(void);

/* output ********************************/
void MotorLeft_SetPWM(int32_t PWM);
void MotorRight_SetPWM(int32_t PWM);

/* Side Motor Config ************************/
void SideMotor_Init(void);
void SideMotor_Start(void);
void SideMotor_Stop(void);
uint32_t SideMotorLeft_GetRemotePWM(void);
uint32_t SideMotorRight_GetRemotePWM(void);
void SideMotorLeft_SetPWM(int32_t PWM);
void SideMotorRight_SetPWM(int32_t PWM);

#endif
