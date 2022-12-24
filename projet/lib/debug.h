#pragma once

#ifndef F_CPU
#define F_CPU 8000000
#endif

#include "rs232.h"// ICI ON DOIT DEFINE DEBUG AVEC LE MAKE

#ifdef DEBUG
#define DEBUG_PRINT(str, num) debug_funct(str, num)

#else

#define DEBUG_PRINT(str, num) do {} while (0) //code mort

#endif /* DEL_H */

void debug_funct(char str[], uint8_t num);
