#include "stm32f4xx.h" 
#include "LED.h" 

void TIM10_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);	
	
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource6,GPIO_AF_TIM10);  //GPIOE0复用为TIM9
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉（正点原子使用下拉）
	GPIO_Init(GPIOF, &GPIO_InitStructure);

	TIM_InternalClockConfig(TIM10);
	
	// 5ms进一次中断
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period = 20000 - 1;
	TIM_TimeBaseStructure.TIM_Prescaler = 168 - 1;	
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM10, &TIM_TimeBaseStructure);

	/* 这里不配置TIM_OCIdleState/TIM_OCNIdleState，因为其仅对TIM1-TIM8生效 */
	TIM_OCInitTypeDef TIM_OCInitStructure;	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC1Init(TIM10, &TIM_OCInitStructure); 

//	NVIC_InitTypeDef NVIC_InitStructure;
//	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//	TIM_ClearITPendingBit(TIM10, TIM_IT_Update);
//	TIM_ITConfig(TIM10, TIM_IT_Update, ENABLE);
	
	TIM_Cmd(TIM10, ENABLE);
}

//void TIM1_UP_TIM10_IRQHandler(void)
//{
//	static int num = 0;
//	if (TIM_GetITStatus(TIM10, TIM_IT_Update) == SET)
//	{
//		num ++;
//		if (num > 50)
//		{
//			num = 0;
//			LED_Turn();
//		}
//	}
//	TIM_ClearITPendingBit(TIM10, TIM_IT_Update);
//}
