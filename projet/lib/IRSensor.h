#pragma once
#define F_CPU 8000000
#include <avr/io.h>
#include "can.h"
#include <math.h>
#include <util/delay.h>
#include "rs232.h"
#include <stdio.h>

class IRSensor{
    public:
        IRSensor();

        double readNumericValue();
        long readAndReturnValue();
    private:    
        Can can;
        RS232 rs232;
};
