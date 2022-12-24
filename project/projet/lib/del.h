#ifndef F_CPU
#define F_CPU 8000000
#endif

#ifndef LED_H
#define LED_H

#include <avr/io.h> 
#include <util/delay.h>

class Led {
public:

    volatile uint8_t* portOut_;
    volatile uint8_t* direction_;
    uint8_t firstPin_;
    uint8_t secondPin_;

    Led(volatile uint8_t* portOut, volatile uint8_t* direction, uint8_t firstPin, uint8_t secondPin);
    
    void turnRed();
    
    void turnGreen();

    void turnOff();
    
    void turnAmber(uint16_t nbOfTenMs);

    void blinkRed();
    void blinkGreen();
    void blinkAmber();
};

#endif /* LED_H */
