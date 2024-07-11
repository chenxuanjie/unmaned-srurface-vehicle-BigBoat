#ifndef __GPS_H
#define __GPS_H

#define SAVE_MESSAGE_SIZE		16		//�������ñ��ĵĴ�С

#define BOTE_MESSAGE_SIZE		16		//�����ʱ��ĵĴ�С

#define START_MESSAGE_SIZE		9		//�������ĵĴ�С

#define SPEED_MESSAGE_SIZE		28		//Ƶ�ʱ��ĵĴ�С

#define SYSTEM_SELECTON_SIZE	12		//ѡ��ϵͳ���Ĵ�С

#define GET_MESSAGE_SIZE		11		//���ĵı��Ĵ�С

void GPS_Init(uint32_t BaudRate);
void GPS_SendMesssage(uint8_t* message, const uint8_t size);
void GPS_ConvMeg(void);


#endif
