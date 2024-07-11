#ifndef __BLUETOOTH_H
#define __BLUETOOTH_H

#define BNUM_MAXNUM		2
/* maximum size of recieving  data */
#define BLUETOOTH_DATA_MAXSIZE	100
#define BLUETOOTH_NAME_MAXSIZE	10

/*Bluetooth State Pin definition */
#define BULETOOTH_STATE_RCC		RCC_AHB1Periph_GPIOD
#define BULETOOTH_STATE_GPIO	 GPIOA
#define BULETOOTH_STATE_PIN		GPIO_Pin_10
#define BULETOOTH_RXD_PIN		GPIO_Pin_9
#define BULETOOTH_TXD_PIN		GPIO_Pin_8
/*Bluetooth State Pin definition */
#define BULETOOTH_EN_RCC		RCC_AHB1Periph_GPIOD
#define BULETOOTH_EN_GPIO		 GPIOD
#define BULETOOTH_EN_PIN		GPIO_Pin_11

typedef struct {
	uint8_t IsRunning;
	uint8_t SendEN;
	uint8_t UseState;
	uint8_t UseEN;
	uint32_t DatasLength;	/* the length of Datas[BLUETOOTH_DATA_MAXSIZE] */
}BluetoothList;

typedef struct {
	char Name[BLUETOOTH_NAME_MAXSIZE];
	int32_t Num;
	float Num_f;
	int32_t LastNum[BNUM_MAXNUM];
	float LastNum_f[BNUM_MAXNUM];
}BluetoothNumber;
	
void Bluetooth_Init(uint8_t State, uint8_t EN);
int8_t Bluetooth_SetNum(int32_t *n, int c);
int8_t Bluetooth_SetNum_f(float *n, int c);
int8_t Bluetooth_SetNum2(BluetoothNumber BNumStructure[]);
int8_t Bluetooth_SetNum2_f(BluetoothNumber BNumStructure[]);
void Bluetooth_UpdateBuf(void);
void Bluetooth_GetBuf(char *buf);

#endif
