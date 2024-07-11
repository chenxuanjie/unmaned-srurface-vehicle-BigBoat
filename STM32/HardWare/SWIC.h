#ifndef __SWIC_H
#define __SWIC_H


#define UP		1
#define MID		0
#define DOWN	-1

#define SW_ON_PWM		1600
#define SW_OFF_PWM		1400
/* Switch function assign *******/
#define SW_AUTO			SW1
#define SW_REMOTE_EN	SW2
#define SW_CONVEYOR	 	SW3
#define SW_PUMP			SW4
#define SW1				1
#define SW2				2
#define SW3				3
#define SW4				4

typedef struct {
	uint32_t RemoteDuty;
	int16_t PWM;
} PumpDataInit;

typedef struct {
	uint32_t RemoteDuty;
	int16_t PWM;
} ConveyorDataInit;

void SWIC_Init(void);
int8_t SWIC_GetRemoteState(uint8_t SWN);
void Pump_Start(void);
void Conveyor_Start(void);
void Pump_Stop(void);
void Conveyor_Stop(void);
/* Get remote data************************/
uint32_t SWIC1_GetRemoteOriPWM(void);
uint32_t SWIC2_GetRemoteOriPWM(void);
uint32_t SWIC3_GetRemoteOriPWM(void);
uint32_t SWIC4_GetRemoteOriPWM(void);
/* pwm setting *****************************************/
uint32_t Conveyor_GetPWM(void);
uint32_t Pump_GetPWM(void);
void Pump_SetPWM(uint32_t PWM);
void Conveyor_SetPWM(uint32_t PWM);

#endif
