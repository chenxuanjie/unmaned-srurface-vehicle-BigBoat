#ifndef __FORMATCONVERSION_H
#define __FORMATCONVERSION_H

#include "stm32f4xx.h"


#define SUB_STRING_MAX_SIZE		10
#define SUB_STRING_SIZE			5

int8_t SplitString(char* s, char** sub_strings);

#endif
