#pragma once
#define F_CPU 8000000
#include <avr/io.h>


class LineCaptor 
{
    public:
        LineCaptor();
        uint8_t captLine();
};

enum lineCaptorState 
{
    veryBigTurnRight  = 0b10000,
    bigTurnRight      = 0b11000,
    mediumTurnRight   = 0b01000,
    smallTurnRight    = 0b01100, 
    center            = 0b00100,
    smallTurnLeft     = 0b00110,
    mediumTurnLeft    = 0b00010,
    bigTurnLeft       = 0b00011,
    veryBigTurnLeft   = 0b00001,
    fullLine          = 0b11111,
    noLine            = 0b00000,
    turn90Left        = 0b00111,
    turn90Left2       = 0b01111,
    turn90Right       = 0b11100,
    turn90Right2      = 0b11110,
    threeCenter       = 0b01110
};
