#ifndef __REMOTE_H
#define __REMOTE_H

typedef struct {
	int16_t MotorYPWM;
	int16_t MotorXPWM;
	int16_t ServoPWM;
	int16_t PushrodPWM;
	int16_t SW1ON;
	int16_t SW2ON;
	int16_t SW3ON;
	int16_t SW4ON;
} RemoteStatesInit;

void Remote_GetOrignalData(RemoteStatesInit* RemoteStatesStructure);

#endif
