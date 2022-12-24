#ifndef F_CPU
#define F_CPU 8000000
#endif

#include "del.h"

Led::Led(volatile uint8_t* portOut, volatile uint8_t* direction, uint8_t firstPin, uint8_t secondPin)
{
    portOut_ = portOut;
    direction_ = direction;
    firstPin_ = firstPin;
    secondPin_ = secondPin;
    //DDRB |= ((1 << PINB0) | (1 << PINB1));
    *direction_ |= (1<<firstPin_)|(1<<secondPin_);
    
}

void Led::turnRed()
{
    *portOut_ |= (1 << firstPin_);
    *portOut_ &= ~(1 << secondPin_);
}

void Led::turnGreen()
{
    *portOut_ |= (1 << secondPin_);
    *portOut_ &= ~(1 << firstPin_);
}

void Led::turnOff()
{
    *portOut_ &= ~((1 << secondPin_) | (1 << firstPin_));
}


void Led::turnAmber(uint16_t nbOfTenMs) 
{
    for (uint16_t i = 0; i < nbOfTenMs; i++) {
        turnRed();
        _delay_ms(3);
        turnGreen();
        _delay_ms(7);
        turnOff();
    }
}

void Led::blinkRed()
{
    for (int i=0; i<8; i++){
        turnOff();
        _delay_ms(125);        
        turnRed();
        _delay_ms(125);
        turnOff();
        
    }        
}

void Led::blinkGreen()
{
    for (int i=0; i<8; i++){
        turnOff();
        _delay_ms(125);
        turnGreen();
        _delay_ms(125);
        turnOff();
        
    }    
}

void Led::blinkAmber()
{
    for (int i=0; i<8; i++){
        turnOff();
        _delay_ms(125);
        turnAmber(12);
        turnOff();
        
    }    
}

