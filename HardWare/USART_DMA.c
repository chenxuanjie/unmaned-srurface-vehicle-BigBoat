#include "stm32f4xx.h"                  // Device header
#include "USART_DMA.h"
#include "USART1.h"
#include "string.h"
#include "stdlib.h"
#include "DMA2.h"

static UsartList Usart;
static void USART_DMA_IRQHandler(void);
char string[USART_DMA_DATA_MAXSIZE];

void USART_DMA_Init(uint32_t BaudRate)
{

	USART1_Config(BaudRate);
	USART1_SetIRQHandler(USART_DMA_IRQHandler);
	DMA2_USART1_Init();
	DMA2_USART1_Start(USART1_RX_Stream);		/* restart to transmit data */	
}

/**
  * @brief  Get string from PC
  * @param  
  * @retval 
  */
int8_t USART_DMA_GetInfo(Info* InfoStructure)
{
	char *num_s = NULL;
	char temp_string[USART_DMA_DATA_MAXSIZE] = "";

	sprintf(temp_string, "en:%d\r\n", Usart.SendEN);
	/* Recieve Data *******************/
	if (Usart.SendEN != RESET)
	{	
		/* Get string from Pc string recieve buf */
		strncpy(temp_string, Datas2, sizeof(Datas2));
		/* update variable "yaw" when aceept right */
		if ((num_s = strstr(temp_string, "yaw=")) != NULL)
		{
			num_s += 4;				
			InfoStructure->yaw = atoi(num_s);
		}		
	}

	
	return 0;
}

/**
  * @brief  
  * @param  n: num that controled by Usart.
		You must update buf after call this funciton.
  * @param  c: specifies the char to tell what num would be calculated.
  *     @arg NULL: don't need char to tell.
* @retval 
  */
int8_t USART_DMA_SetNum(int32_t *n, int c)
{
	char *num_s = NULL;
	int32_t LastNum = 0;
	static char temp_string[USART_DMA_DATA_MAXSIZE] = "";	
	
	LastNum = *n;
	if ((strchr(Datas2, c) != NULL || NULL == c) && Usart.SendEN != RESET) /* if sign is matching */
	{
		/* Recieve Data ************************************/
		strncpy(temp_string, Datas2, sizeof(Datas2));

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
		else if (strchr(Datas2, '?') != NULL)
		{
			printf("num %c: %d.\tlastNum:%d\r\n", c, *n, LastNum);
			return 0;
		}
		else
			printf("Error! Please check your inpput. Maybe there are no sign could be detected.\r\n");
		printf("num %c: %d.\tlastNum:%d\r\n", c, *n, LastNum);	
	}

	return 0;
}

int8_t USART_DMA_SetNum_f(float *n, int c)
{
	char *num_s = NULL;
	float LastNum = 0;
	static char temp_string[USART_DMA_DATA_MAXSIZE] = "";	
	
	LastNum = *n;
	if ((strchr(Datas2, c) != NULL || NULL == c) && Usart.SendEN != RESET) /* if sign is matching */
	{
		/* Recieve Data ************************************/
		strncpy(temp_string, Datas2, sizeof(Datas2));

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
		else if (strchr(Datas2, '?') != NULL)
		{
			printf("num %c: %f.\tlastNum:%f\r\n", c, *n, LastNum);
			return 0;
		}
		else
			printf("Error! Please check your inpput. Maybe there are no sign could be detected.\r\n");
		printf("num %c: %f.\tlastNum:%f\r\n", c, *n, LastNum);	
	}

	return 0;
}

/**
  * @brief  
		You must update buf after call this funciton.
  * @param  
  * @retval 
  */
int8_t USART_DMA_SetNum2(UsartNumber UNumStructure[])
{
	const char* sign[4] = {"=", "+", "-", "?"};
	char *name_s = NULL, *num_s = NULL;
	char temp_string[USART_DMA_DATA_MAXSIZE] = "";
	int32_t key = 0, rightSign = 0;
	
	/* Recieve Data *******************/
	if (Usart.SendEN != RESET)
	{	
		strncpy(temp_string, Datas2, sizeof(Datas2));
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
			USART_DMA_UpdateBuf();
			return -1;
		}
		/* show all data */
		if (strchr(temp_string, '?') != NULL)
		{
			printf("\r\n");
			for (int j = 0; j < UNUM_MAXNUM; j ++)
				printf("%s: %d.\tlastNum:%d\r\n", UNumStructure[j].Name, UNumStructure[j].Num, UNumStructure[j].LastNum[j]);
			printf("\r\n");
			USART_DMA_UpdateBuf();
			return 0;
		}

		/* identify the specific num */
		for (int i = 0; i < UNUM_MAXNUM; i ++)
		{		
			if (!strcmp(UNumStructure[i].Name, name_s))		/* if equal */
			{
				key = i;
				UNumStructure[i].LastNum[i] = UNumStructure[i].Num;
				break;
			}
			if (i == UNUM_MAXNUM - 1 && strcmp(UNumStructure[i].Name, name_s) != 0)
			{
				printf("Error! Please check your variable's name\r\n");
				USART_DMA_UpdateBuf();
				return -1;
			}
		}
		if (!strcmp(sign[rightSign], "="))
		{			
			UNumStructure[key].Num = strtof(num_s, NULL);
		}
		else if (!strcmp(sign[rightSign], "+"))
		{
			UNumStructure[key].Num = UNumStructure[key].LastNum[key] + strtof(num_s, NULL);
		}
		else if (!strcmp(sign[rightSign], "-"))
		{
			UNumStructure[key].Num = UNumStructure[key].LastNum[key] - strtof(num_s, NULL);
		}
		else	/* input number directly*/
		{
			name_s = UNumStructure[0].Name;
			num_s = temp_string;
			
			UNumStructure[0].Num = strtof(num_s, NULL);
		}
		printf("%s: %d.\tlastNum:%d\r\n",UNumStructure[key].Name, UNumStructure[key].Num, UNumStructure[key].LastNum[key]);
	}
	else
		return -1;
	return 0;
}

int8_t USART_DMA_SetNum2_f(UsartNumber UNumStructure[])
{
	const char* sign[4] = {"=", "+", "-", "?"};
	char *name_s = NULL, *num_s = NULL;
	char temp_string[USART_DMA_DATA_MAXSIZE] = "";
	int32_t key = 0, rightSign = 0;
	
	/* Recieve Data *******************/
	if (Usart.SendEN != RESET)
	{	
		strncpy(temp_string, Datas2, sizeof(Datas2));
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
			USART_DMA_UpdateBuf();
			return -1;
		}
		/* show all data */
		if (strchr(temp_string, '?') != NULL)
		{
			printf("\r\n");
			for (int j = 0; j < UNUM_MAXNUM; j ++)
				printf("%s: %f.\tlastNum:%f\r\n", UNumStructure[j].Name, UNumStructure[j].Num_f,  UNumStructure[j].LastNum_f[j]);
			printf("\r\n");
			USART_DMA_UpdateBuf();
			return 0;
		}

		/* identify the specific num */
		for (int i = 0; i < UNUM_MAXNUM; i ++)
		{		
			if (!strcmp(UNumStructure[i].Name, name_s))		/* if equal */
			{
				key = i;
				UNumStructure[i].LastNum_f[i] = UNumStructure[i].Num_f;
				break;
			}
			if (i == UNUM_MAXNUM - 1 && strcmp(UNumStructure[i].Name, name_s) != 0)
			{
				printf("Error! Please check your variable's name\r\n");
				USART_DMA_UpdateBuf();
				return -1;
			}
		}
		if (!strcmp(sign[rightSign], "="))
		{			
			UNumStructure[key].Num_f = strtof(num_s, NULL);
		}
		else if (!strcmp(sign[rightSign], "+"))
		{
			UNumStructure[key].Num_f = UNumStructure[key].LastNum_f[key] + strtof(num_s, NULL);
		}
		else if (!strcmp(sign[rightSign], "-"))
		{
			UNumStructure[key].Num_f = UNumStructure[key].LastNum_f[key] - strtof(num_s, NULL);
		}
		else	/* input number directly*/
		{
			name_s = UNumStructure[0].Name;
			num_s = temp_string;
			
			UNumStructure[0].Num_f = strtof(num_s, NULL);
		}
		printf("%s: %f.\tlastNum:%f\r\n",UNumStructure[key].Name, UNumStructure[key].Num_f, UNumStructure[key].LastNum_f[key]);
	}
	else
		return -1;
	return 0;
}

/* Delay about 50ms is better for data reception */
void USART_DMA_UpdateBuf(void)
{
	if (Usart.SendEN != RESET)
	{
		/* waitting for next reception */
		Usart.SendEN = RESET;
		
		memset(Datas2, 0, sizeof(Datas2));		/* clear array */
		DMA2_USART1_Start(USART1_RX_Stream);		/* restart to transmit data */
	
	}
}

void USART_DMA_GetBuf(char *buf)
{
	strncpy(buf, Datas2, sizeof(*buf));
}

void USART_DMA_SendString(char *buf)
{
	Usart_SendString(USART1, buf);
}

static void USART_DMA_IRQHandler(void)
{	
	uint8_t clear = 0;
	if (USART_GetITStatus(USART1, USART_IT_IDLE) == SET)	
	{
		clear = USART1->DR;		/* clear IDLE flag */
		Usart.DatasLength = sizeof(Datas2) - DMA_GetCurrDataCounter(USART1_RX_Stream);
		/* Usart will be free */
		Usart.SendEN = SET;

		if (strcmp(Datas2, "")) 		/*if not equal */
		{
			strncpy(string, Datas2, USART_DMA_DATA_MAXSIZE);
//			memset(Datas2, 0, sizeof(Datas2));
//			DMA2_USART1_Start(USART1_RX_Stream);
		}
		USART_ClearITPendingBit(USART1,USART_IT_IDLE);//?????
	}
}
