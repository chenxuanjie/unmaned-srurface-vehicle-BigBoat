#ifndef __SERVO_H
#define __SERVO_H

typedef struct {
	uint32_t RemoteDuty;
	int16_t PWM;
} ServoDataInit;

void Servo_Init(void);
uint32_t Servo_GetRemoteOriPWM(void);
uint32_t Servo_GetRemotePWM(void);
/* Set PWM *******************************/
void ServoLeft_SetPWM(uint32_t PWM);
void ServoRight_SetPWM(uint32_t PWM);
void Servo_Reset(void);
void Servo_Start(void);

#endif
