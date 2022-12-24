#include "lineCaptor.h"

LineCaptor::LineCaptor(){}

uint8_t LineCaptor::captLine()
{
    return ((PINA & 0b01111100)>>2);
}
