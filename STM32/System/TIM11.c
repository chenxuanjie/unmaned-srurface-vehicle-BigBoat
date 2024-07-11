#include "stm32f4xx.h" 
#include "LED.h" 

void TIM11_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);	
	
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource7,GPIO_AF_TIM11);  //GPIOF0����ΪTIM9
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����������ԭ��ʹ��������
	GPIO_Init(GPIOF, &GPIO_InitStructure);

	TIM_InternalClockConfig(TIM11);
	
	// 10ms��һ���ж�
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period = 20000 - 1;
	TIM_TimeBaseStructure.TIM_Prescaler = 168 - 1;	
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM11, &TIM_TimeBaseStructure);
	
	/* ���ﲻ����TIM_OCIdleState/TIM_OCNIdleState����Ϊ�����TIM1-TIM8��Ч */
	TIM_OCInitTypeDef TIM_OCInitStructure;	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC1Init(TIM11, &TIM_OCInitStructure); 

//	NVIC_InitTypeDef NVIC_InitStructure;
//	NVIC_InitStructure.NVIC_IRQChannel = TIM1_TRG_COM_TIM11_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//	TIM_ClearITPendingBit(TIM11, TIM_IT_Update);
//	TIM_ITConfig(TIM11, TIM_IT_Update, ENABLE);
	
	TIM_Cmd(TIM11, ENABLE);
}

//void TIM1_TRG_COM_TIM11_IRQHandler(void)
//{
//	static int num = 0;
//	if (TIM_GetITStatus(TIM11, TIM_IT_Update) == SET)
//	{
//		num ++;
//		if (num > 50)
//		{
//			num = 0;
//			LED_Turn();
//		}
//	}
//	TIM_ClearITPendingBit(TIM11, TIM_IT_Update);
//}

