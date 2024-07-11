#ifndef __PUSHROD_H
#define __PUSHROD_H

typedef struct {
	uint32_t RemoteDuty;
	int16_t PWM;
} PushrodDataInit;

void Pushrod_Init(void);
void Pushrod_Start(void);
void Pushrod_Stop(void);
uint32_t Pushrod_GetRemoteOriPWM(void);
uint32_t Pushrod_GetRemotePWM(void);
/* output ********************************/
void PushrodLeft_SetPWM(int32_t PWM);
void PushrodRight_SetPWM(int32_t PWM);

#endif
