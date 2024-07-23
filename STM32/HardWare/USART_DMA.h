#ifndef __USART_DMA_H
#define __USART_DMA_H

#define UNUM_MAXNUM		4
/* maximum size of recieving  data */
#define USART_DMA_DATA_MAXSIZE	100
#define USART_DMA_NAME_MAXSIZE	10

typedef struct {
	uint8_t IsRunning;
	uint8_t SendEN;
	uint32_t DatasLength;	/* the length of Datas[USART_DMA_DATA_MAXSIZE] */
}UsartList;

typedef struct {
	char Name[USART_DMA_NAME_MAXSIZE];
	int32_t Num;
	float Num_f;
	int32_t LastNum[UNUM_MAXNUM];
	float LastNum_f[UNUM_MAXNUM];	
}UsartNumber;

typedef struct {
	int32_t LMainMotor_PWM;
	int32_t RMainMotor_PWM;
	int32_t LSideMotor_PWM;
	int32_t RSideMotor_PWM;
}MessageFromNXInit;
	
extern char string[USART_DMA_DATA_MAXSIZE];

void USART_DMA_Init(uint32_t BaudRate);
int8_t USART_DMA_SetNum(int32_t *n, int c);
int8_t USART_DMA_SetNum2(UsartNumber BNumStructure[]);
int8_t USART_DMA_SetNum_f(float *n, int c);
int8_t USART_DMA_SetNum2_f(UsartNumber BNumStructure[]);
int8_t USART_DMA_GetStringFromNX(char* s);
void USART_DMA_UpdateBuf(void);
void USART_DMA_GetBuf(char *buf);
void USART_DMA_SendString(char *buf);

#endif
