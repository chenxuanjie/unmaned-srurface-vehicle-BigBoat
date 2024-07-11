#ifndef __MAIN_H
#define __MAIN_H

/* @ref remote value definition ********/
#define REMOTE_PWM_MIN		900		/* rocker up and down */
#define REMOTE_PWM_MIN2		1000	/* rocker left and right */
#define REMOTE_PWM_MID		1500
#define REMOTE_PWM_MAX		2100	/* rocker up and down */
#define REMOTE_PWM_MAX2		2000	/* rocker left and right */

#define REMOTE_PWM_LEFTHALF	((REMOTE_PWM_MID - REMOTE_PWM_MIN2) / 2 + REMOTE_PWM_MIN2)		/* A point 1/4 of the way down the X-axis */
#define REMOTE_PWM_RIGHTHALF	((REMOTE_PWM_MID - REMOTE_PWM_MIN2) / 2 + REMOTE_PWM_MID)	/* A point 3/4 of the way down the X-axis */
/* @ref remote value definition ********/

typedef struct{
	int16_t RemoteModeEN;
	int16_t AutoModeEN;
	int16_t RemoteMode2;
	
} ControllerInit;

#endif
