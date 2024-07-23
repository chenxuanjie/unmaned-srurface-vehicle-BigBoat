#include "stm32f4xx.h"                  // Device header
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "FormatConversion.h"


/**
  * @brief  
  * @param  s: string to be split;
  * @param 	sub_strings: strings split from s.			 
  * @retval count of split string.
  */
int8_t SplitString(char* s, char** sub_strings) 
{
    char* sub_string;
    const char delimiter[2] = ",";
    int i = 0;
    
    sub_string = strtok(s, delimiter);

    while (sub_string != NULL) {
        int length = strlen(sub_string) + 1;
        sub_strings[i] = (char*)malloc(length);
        if (sub_strings[i] == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            return -1;
        }
        strcpy(sub_strings[i], sub_string);
        i++;
        sub_string = strtok(NULL, delimiter);
    }

    return i;
}
