#include "stm32f4xx.h" 
#include "LED.h" 

void TIM9_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);	
	
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource5,GPIO_AF_TIM9);  //GPIOE0复用为TIM9
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉（正点原子使用下拉）
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	TIM_InternalClockConfig(TIM9);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period = 65536 - 1;
	TIM_TimeBaseStructure.TIM_Prescaler = 168 - 1;	
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM9, &TIM_TimeBaseStructure);
	
	TIM_ICInitTypeDef TIM_ICInitStructure;	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInit(TIM9, &TIM_ICInitStructure);
	TIM_PWMIConfig(TIM9, &TIM_ICInitStructure);
  	
	TIM_SelectInputTrigger(TIM3, TIM_TS_TI1FP1);
	TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset);
	//	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
//	TIM_TimeBaseStructure.TIM_Period = 100 - 1;
//	TIM_TimeBaseStructure.TIM_Prescaler = 1680 - 1;	
//	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//	TIM_TimeBaseInit(TIM9, &TIM_TimeBaseStructure);
	
//	NVIC_InitTypeDef NVIC_InitStructure;
//	NVIC_InitStructure.NVIC_IRQChannel = TIM1_BRK_TIM9_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//	TIM_ClearFlag(TIM9, TIM_FLAG_Update);
//	TIM_ITConfig(TIM9, TIM_IT_Update, ENABLE);
	
	TIM_Cmd(TIM9, ENABLE);
}

//void TIM1_BRK_TIM9_IRQHandler(void)
//{
//	static int num = 0;
//	if (TIM_GetFlagStatus(TIM9, TIM_FLAG_Update) == SET)
//	{
//		num ++;
//		if (num > 1000)
//		{
//			num = 0;
//			LED_Turn();
//		}
//	}
//	TIM_ClearITPendingBit(TIM9, TIM_FLAG_Update);
//}
