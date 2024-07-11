#ifndef __GPS_H
#define __GPS_H

#define SAVE_MESSAGE_SIZE		16		//保存设置报文的大小

#define BOTE_MESSAGE_SIZE		16		//波特率报文的大小

#define START_MESSAGE_SIZE		9		//开启报文的大小

#define SPEED_MESSAGE_SIZE		28		//频率报文的大小

#define SYSTEM_SELECTON_SIZE	12		//选择系统报文大小

#define GET_MESSAGE_SIZE		11		//报文的报文大小

void GPS_Init(uint32_t BaudRate);
void GPS_SendMesssage(uint8_t* message, const uint8_t size);
void GPS_ConvMeg(void);


#endif
