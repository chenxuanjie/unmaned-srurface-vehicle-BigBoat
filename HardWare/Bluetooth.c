#include "stm32f4xx.h" 
#include <stdio.h>
#include "Bluetooth.h"
#include "USART3.h"
#include "string.h"
#include "stdlib.h"
#include "DMA1.h"
#include "LED.h"


uint8_t len;

static BluetoothList Bluetooth;
//static BluetoothNumber BNUM = {"num", 0};
static uint8_t IsConnected(void);
static void Bluetooth_IRQHandler(void);

/**
* @brief  
* @param  State: Specify whether to use State Pin.
*     @arg SET: Use State Pin.
*     @arg RESET: Do not use State Pin.
* @param  EN: Specify whether to use EN Pin.
*     @arg SET: Use EN Pin.
*     @arg RESET: Do not use EN Pin.
* @retval None
*/
void Bluetooth_Init(uint8_t State, uint8_t EN)
{
	USART3_Config(9600);
	USART3_SetHandler(Bluetooth_IRQHandler);
	DMA1_Init();
	DMA1_Start(USART3_RX_Stream);		/* restart to transmit data */	
	/* Use State Pin */
	if ((Bluetooth.UseState = State) != RESET)
	{
		RCC_AHB1PeriphClockCmd(BULETOOTH_STATE_RCC, ENABLE);
		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Pin = BULETOOTH_STATE_PIN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(BULETOOTH_STATE_GPIO, &GPIO_InitStructure);		
	}	
	/* Use EN Pin */
	if ((Bluetooth.UseEN = EN) != RESET)
	{
		RCC_AHB1PeriphClockCmd(BULETOOTH_EN_RCC, ENABLE);

		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Pin = BULETOOTH_EN_PIN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(BULETOOTH_EN_GPIO, &GPIO_InitStructure);		
	}
	
	/* connected successful */
	if (Bluetooth.UseState != RESET && IsConnected())
	{
		printf("--Link Succeed!--\r\n");
	}
}

//int8_t Bluetooth_Test(void)
//{
//	printf("Please input +1 in your edit area,\
//		you will recieve message.\r\n");
//	while (Bluetooth.SendEN != SET);	/* wait message */
//	Bluetooth.SendEN = RESET;
//	if (-1 != Bluetooth_SetNum2())
//	{
//		BNUM.Num = strtod(Datas, NULL);
//		printf("%s: %d\r\n", BNUM.Name, BUM.Num);
//	}

//	return 0;
//}

/**
  * @brief  
  * @param  n: num that controled by bluetooth.
		You must update buf after call this funciton.
  * @param  c: specifies the char to tell what num would be calculated.
  *     @arg NULL: don't need char to tell.
* @retval 
  */
int8_t Bluetooth_SetNum(int32_t *n, int c)
{
	char *num_s = NULL;
	int32_t LastNum = 0;
	static char temp_string[BLUETOOTH_DATA_MAXSIZE] = "";	
	
	LastNum = *n;
	if ((strchr(Datas, c) != NULL || NULL == c) && Bluetooth.SendEN != RESET) /* if sign is matching */
	{
		/* Recieve Data ************************************/
		strncpy(temp_string, Datas, sizeof(Datas));

		/* Calculate Data ******************/
		if ((num_s = strstr(temp_string, "=")) != NULL)
		{
			num_s += 1;	/* num after sign '=' */				
			*n = strtod(num_s, NULL);
		}
		else if ((num_s = strstr(temp_string, "+")) != NULL)
		{
			num_s += 1;
			*n = LastNum + strtod(num_s, NULL);
		}
		else if ((num_s = strstr(temp_string, "-")) != NULL)
		{
			num_s += 1;
			*n = LastNum - strtod(num_s, NULL);
		}
		else if (strchr(Datas, '?') != NULL)
		{
			printf("num %c = %d.\tlastNum=%d\r\n", c, *n, LastNum);
			return 0;
		}
		else
			printf("Error! Please check your inpput. Maybe there are no sign could be detected.\r\n");
		printf("num %c = %d,\tlastNum=%d\r\n", c, *n, LastNum);	
	}
	
	return 0;
}

int8_t Bluetooth_SetNum_f(float *n, int c)
{
	char *num_s = NULL;
	float LastNum = 0;
	static char temp_string[BLUETOOTH_DATA_MAXSIZE] = "";	
	
	LastNum = *n;
	if ((strchr(Datas, c) != NULL || NULL == c) && Bluetooth.SendEN != RESET) /* if sign is matching */
	{
		/* Recieve Data ************************************/
		strncpy(temp_string, Datas, sizeof(Datas));

		/* Calculate Data ******************/
		if ((num_s = strstr(temp_string, "=")) != NULL)
		{
			num_s += 1;	/* num after sign '=' */				
			*n = strtof(num_s, NULL);
		}
		else if ((num_s = strstr(temp_string, "+")) != NULL)
		{
			num_s += 1;
			*n = LastNum + strtof(num_s, NULL);
		}
		else if ((num_s = strstr(temp_string, "-")) != NULL)
		{
			num_s += 1;
			*n = LastNum - strtof(num_s, NULL);
		}
		else if (strchr(Datas, '?') != NULL)
		{
			printf("num %c = %f.\tlastNum=%f\r\n", c, *n, LastNum);
			return 0;
		}
		else
			printf("Error! Please check your inpput. Maybe there are no sign could be detected.\r\n");
		printf("num %c= %f,\tlastNum=%f\r\n", c, *n, LastNum);	
	}

	return 0;
}

/**
  * @brief  
		You must update buf after call this funciton.
  * @param  
  * @retval 
  */
int8_t Bluetooth_SetNum2(BluetoothNumber BNumStructure[])
{
	const char* sign[4] = {"=", "+", "-", "?"};
	char *name_s = NULL, *num_s = NULL;
	char temp_string[BLUETOOTH_DATA_MAXSIZE] = "";
	int32_t key = 0, rightSign = 0;
	
	/* Recieve Data *******************/
	if (Bluetooth.SendEN != RESET)
	{	
		strncpy(temp_string, Datas, sizeof(Datas));
		/* Calculate Data ******************/
		for (int i = 0; i < 4; i ++)
		{		
			if (strstr(temp_string, sign[i]) != NULL)
			{
				rightSign = i;
				num_s = strstr(temp_string, sign[i]) + 1;
				/* 用了strtok后，如果temp_string改变，name_s也会改变 */
				name_s = strtok(temp_string, sign[i]);		/* strtok change string */
				break;
			}
		}
		/* not sign is in array sign[] */
		if (NULL == num_s)
		{
			printf("Error! Please check your inpput. Maybe there are no sign could be detected.\r\n");
			Bluetooth_UpdateBuf();
			return -1;
		}
		/* show all data */
		if (strchr(temp_string, '?') != NULL)
		{
			printf("\r\n");
			for (int j = 0; j < BNUM_MAXNUM; j ++)
				printf("%s = %d,\tlastNum = %d\r\n", BNumStructure[j].Name, BNumStructure[j].Num, BNumStructure[j].LastNum[j]);
			printf("\r\n");
			Bluetooth_UpdateBuf();
			return 0;
		}

		/* identify the specific num */
		for (int i = 0; i < BNUM_MAXNUM; i ++)
		{		
			if (!strcmp(BNumStructure[i].Name, name_s))		/* if equal */
			{
				key = i;
				BNumStructure[i].LastNum[i] = BNumStructure[i].Num;
				break;
			}
			if (i == BNUM_MAXNUM - 1 && strcmp(BNumStructure[i].Name, name_s) != 0)
			{
				printf("Error! Please check your variable's name\r\n");
				Bluetooth_UpdateBuf();
				return -1;
			}
		}
		if (!strcmp(sign[rightSign], "="))
		{			
			BNumStructure[key].Num = strtof(num_s, NULL);
		}
		else if (!strcmp(sign[rightSign], "+"))
		{
			BNumStructure[key].Num = BNumStructure[key].LastNum[key] + strtof(num_s, NULL);
		}
		else if (!strcmp(sign[rightSign], "-"))
		{
			BNumStructure[key].Num = BNumStructure[key].LastNum[key] - strtof(num_s, NULL);
		}
		else	/* input number directly*/
		{
			name_s = BNumStructure[0].Name;
			num_s = temp_string;
			
			BNumStructure[0].Num = strtof(num_s, NULL);
		}
		printf("%s = %d,\tlastNum = %d\r\n",BNumStructure[key].Name, BNumStructure[key].Num, BNumStructure[key].LastNum[key]);
	}
	else
		return -1;
	return 0;
}

int8_t Bluetooth_SetNum2_f(BluetoothNumber BNumStructure[])
{
	const char* sign[4] = {"=", "+", "-", "?"};
	char *name_s = NULL, *num_s = NULL;
	char temp_string[BLUETOOTH_DATA_MAXSIZE] = "";
	int32_t key = 0, rightSign = 0;
	
	/* Recieve Data *******************/
	if (Bluetooth.SendEN != RESET)
	{	
		strncpy(temp_string, Datas, sizeof(Datas));
		/* Calculate Data ******************/
		for (int i = 0; i < 4; i ++)
		{		
			if (strstr(temp_string, sign[i]) != NULL)
			{
				rightSign = i;
				num_s = strstr(temp_string, sign[i]) + 1;
				/* 用了strtok后，如果temp_string改变，name_s也会改变 */
				name_s = strtok(temp_string, sign[i]);		/* strtok change string */
				break;
			}
		}
		/* not sign is in array sign[] */
		if (NULL == num_s)
		{
			printf("Error! Please check your inpput. Maybe there are no sign could be detected.\r\n");
			Bluetooth_UpdateBuf();
			return -1;
		}
		/* show all data */
		if (strchr(temp_string, '?') != NULL)
		{
			printf("\r\n");
			for (int j = 0; j < BNUM_MAXNUM; j ++)
				printf("%s: %f.\tlastNum:%f\r\n", BNumStructure[j].Name, BNumStructure[j].Num_f,  BNumStructure[j].LastNum_f[j]);
			printf("\r\n");
			Bluetooth_UpdateBuf();
			return 0;
		}

		/* identify the specific num */
		for (int i = 0; i < BNUM_MAXNUM; i ++)
		{		
			if (!strcmp(BNumStructure[i].Name, name_s))		/* if equal */
			{
				key = i;
				BNumStructure[i].LastNum_f[i] = BNumStructure[i].Num_f;
				break;
			}
			if (i == BNUM_MAXNUM - 1 && strcmp(BNumStructure[i].Name, name_s) != 0)
			{
				printf("Error! Please check your variable's name\r\n");
				Bluetooth_UpdateBuf();
				return -1;
			}
		}
		if (!strcmp(sign[rightSign], "="))
		{			
			BNumStructure[key].Num_f = strtof(num_s, NULL);
		}
		else if (!strcmp(sign[rightSign], "+"))
		{
			BNumStructure[key].Num_f = BNumStructure[key].LastNum_f[key] + strtof(num_s, NULL);
		}
		else if (!strcmp(sign[rightSign], "-"))
		{
			BNumStructure[key].Num_f = BNumStructure[key].LastNum_f[key] - strtof(num_s, NULL);
		}
		else	/* input number directly*/
		{
			name_s = BNumStructure[0].Name;
			num_s = temp_string;
			
			BNumStructure[0].Num_f = strtof(num_s, NULL);
		}
		printf("%s: %f.\tlastNum:%f\r\n",BNumStructure[key].Name, BNumStructure[key].Num_f, BNumStructure[key].LastNum_f[key]);
	}
	else
		return -1;
	return 0;
}

/* Delay about 50ms is better for data reception */
void Bluetooth_UpdateBuf(void)
{
	if (Bluetooth.SendEN != RESET)
	{
		/* waitting for next reception */
		Bluetooth.SendEN = RESET;
		
		memset(Datas, 0, sizeof(Datas));		/* clear array */
		DMA1_Start(USART3_RX_Stream);		/* restart to transmit data */
	
	}
}

void Bluetooth_GetBuf(char *buf)
{
	strncpy(buf, Datas, sizeof(*buf));
}

/* only can be used when BLUETOOTH_STATE has ben defined */
static uint8_t IsConnected(void)
{
	if (GPIO_ReadInputDataBit(BULETOOTH_STATE_GPIO, BULETOOTH_STATE_PIN) == SET)
		return SET;
	else
		return RESET;
}
/*********************************************************/

static void Bluetooth_IRQHandler(void)
{
	uint8_t clear = 0;
	if (USART_GetITStatus(USART3, USART_IT_IDLE) == SET)	
	{
		clear = USART3->DR;		/* clear IDLE flag */
		Bluetooth.DatasLength = sizeof(Datas) - DMA_GetCurrDataCounter(USART3_RX_Stream);
		/* Bluetooth will be free */
		Bluetooth.SendEN = SET;

		USART_ClearITPendingBit(USART3,USART_IT_IDLE);//?????
	}
}
