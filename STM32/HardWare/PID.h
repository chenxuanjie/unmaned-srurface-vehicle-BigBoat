#ifndef __PID_H
#define __PID_H

typedef struct {
	float kp, ki, kd;
    float error,lastError;//???????
    float integral,maxIntegral;//积分、积分限幅
    float output,maxOutput;//输出、输出限幅
}PID;	


void PID_Init(PID *pid,float p,float i,float d,float maxI,float maxOut);
void PID_Calc(PID *pid,float reference,float feedback);

#endif
