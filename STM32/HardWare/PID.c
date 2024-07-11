#include "stm32f4xx.h"                  // Device header
#include "PID.h"

void PID_Init(PID *pid,float p,float i,float d,float maxI,float maxOut)
{
    pid->kp=p;
    pid->ki=i;
    pid->kd=d;
    pid->maxIntegral=maxI;
    pid->maxOutput=maxOut;
}


void PID_Calc(PID *pid,float reference,float feedback)
{
 	//????
    pid->lastError=pid->error;//??error???
    pid->error=reference-feedback;//???error
    //????
    float dout=(pid->error-pid->lastError)*pid->kd;
    //????
    float pout=pid->error*pid->kp;
    //????
    pid->integral+=pid->error*pid->ki;
    //????
    if(pid->integral > pid->maxIntegral) pid->integral=pid->maxIntegral;
    else if(pid->integral < -pid->maxIntegral) pid->integral=-pid->maxIntegral;
    //????
    pid->output=pout+dout+pid->integral;
    //????
    if(pid->output > pid->maxOutput) pid->output=pid->maxOutput;
    else if(pid->output < -pid->maxOutput) pid->output=-pid->maxOutput;
}
