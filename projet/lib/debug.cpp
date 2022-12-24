#ifndef F_CPU
#define F_CPU 8000000
#endif
#include "debug.h"
#include <stdio.h>

RS232 rs232;

void debug_funct(char str[], uint8_t num)
{
    char table[50] = {};
    for (uint8_t i=0; str[i]!=0;i++)
        rs232.transmissionSimpleUART(str[i]);  
    for (uint8_t j=0; table[j]!=0; j++)    
        rs232.transmissionSimpleUART(table[j]);     
}



