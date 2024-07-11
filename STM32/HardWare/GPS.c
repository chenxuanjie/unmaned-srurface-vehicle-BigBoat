#include "stm32f4xx.h" 
#include "GPS.h" 
#include "USART2.h"

//保存配置指令，每次发过指令都要保存
uint8_t save_message[SAVE_MESSAGE_SIZE] 	= {0xf1, 0xd9, 0x06, 0x09, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2f, 0x00, 0x00, 0x00, 0x46, 0xb7};

//波特率报文
uint8_t bote_115200[BOTE_MESSAGE_SIZE]		= {0xf1, 0xd9, 0x06, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc2, 0x01, 0x00, 0xd1, 0xe0};

//冷启动，启动时间长，但是最干净的启动。
uint8_t cool_start[START_MESSAGE_SIZE]		= {0xf1, 0xd9, 0x06, 0x40, 0x01, 0x00, 0x01, 0x48, 0x22};
uint8_t hot_start[START_MESSAGE_SIZE]		= {0xf1, 0xd9, 0x06, 0x40, 0x01, 0x00, 0x02, 0x49, 0x23};

//频率设置报文
uint8_t speed_1Hz[SPEED_MESSAGE_SIZE]		= {0xf1, 0xd9, 0x06, 0x42, 0x14, 0x00, 0x00, 0x01, 0x38, 0x35, 0xe8, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xb5, 0x6b};
uint8_t speed_5Hz[SPEED_MESSAGE_SIZE]		= {0xf1, 0xd9, 0x06, 0x42, 0x14, 0x00, 0x00, 0x05, 0x38, 0x00, 0xc8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x61, 0x05};

//导航系统选择
uint8_t gps_message[SYSTEM_SELECTON_SIZE]		= {0xf1, 0xd9, 0x06, 0x0c, 0x04, 0x00, 0x01, 0x00, 0x00, 0x00, 0x17, 0xa0};
uint8_t gps_bd_message[SYSTEM_SELECTON_SIZE]	= {0xf1, 0xd9, 0x06, 0x0c, 0x04, 0x00, 0x05, 0x00, 0x00, 0x00, 0x1b, 0xb0};

//报文选择
uint8_t open_gaa_message[GET_MESSAGE_SIZE]   = {0xf1, 0xd9, 0x06, 0x01, 0x03, 0x00, 0xf0, 0x00, 0x01, 0xfb, 0x10};
uint8_t close_gaa_message[GET_MESSAGE_SIZE]  = {0xf1, 0xd9, 0x06, 0x01, 0x03, 0x00, 0xf0, 0x00, 0x00, 0xfa, 0x0f};
uint8_t open_rmc_message[GET_MESSAGE_SIZE]  = {0xf1, 0xd9, 0x06, 0x01, 0x03, 0x00, 0xf0, 0x05, 0x01, 0x00, 0x1A};
uint8_t close_rmc_message[GET_MESSAGE_SIZE]  = {0xf1, 0xd9, 0x06, 0x01, 0x03, 0x00, 0xf0, 0x05, 0x00, 0xff, 0x19};


void GPS_Init(uint32_t BaudRate)
{
	USART2_Config(BaudRate);		// usart2 init
		
	GPS_SendMesssage(gps_bd_message,SYSTEM_SELECTON_SIZE);
	
	GPS_SendMesssage(speed_1Hz,SPEED_MESSAGE_SIZE);
	
	GPS_SendMesssage(open_gaa_message,GET_MESSAGE_SIZE);

	GPS_SendMesssage(open_rmc_message,GET_MESSAGE_SIZE);

	printf("start_end\r\n");
}

//发信息到gps
void GPS_SendMesssage(uint8_t* message, const uint8_t size)
{
	int i;
	for(i = 0; i < size; i++)
	{
		Usart2_SendChar(message[i]);
	}
	
	// it must be send
	for(i = 0; i < SAVE_MESSAGE_SIZE; i++)
	{
		Usart2_SendChar(save_message[i]);
	}	
}
