#ifndef F_CPU
#define F_CPU 8000000
#endif

#ifndef BUTTON_H
#define BUTTON_H

#include <avr/io.h> 
#include <util/delay.h>
#include <avr/interrupt.h>


enum EICRAModes 
{
    lowLevel_0 = (0<<ISC00)|(0<<ISC01),
    anyEdge_0 = (1<<ISC00)|(0<<ISC01),
    fallingEdge_0 = (0<<ISC00)|(1<<ISC01),
    risingEdge_0 = (1<<ISC00)|(1<<ISC01),

    lowLevel_1 = (0<<ISC10)|(0<<ISC11),
    anyEdge_1 = (1<<ISC10)|(0<<ISC11),
    fallingEdge_1 = (0<<ISC10)|(1<<ISC11),
    risingEdge_1 = (1<<ISC10)|(1<<ISC11),

    lowLevel_2 = (0<<ISC20)|(0<<ISC21),
    anyEdge_2 = (1<<ISC20)|(0<<ISC21),
    fallingEdge_2 = (0<<ISC20)|(1<<ISC21),
    risingEdge_2 = (1<<ISC20)|(1<<ISC21)
};

enum EIMSKModes
{
    int0 = (1 << INT0),
    int1 = (1 << INT1),
    int2 = (1 << INT2)
};

class Button {
public:

    EIMSKModes EIMSKMode_;
    EICRAModes EICRAMode_;
    volatile uint8_t* direction_;
    volatile uint8_t* pinIn_;
    uint8_t pinInNumber_;

    Button(volatile uint8_t* direction, volatile uint8_t* pinIn, uint8_t pinInNumber);

    void setEIMSK(EIMSKModes EIMSKMode);

    void setEICRA(EICRAModes EICRAMode);

    void initialisation();

    bool buttonPressed();
  
    bool debounceButtonPressed();
};

#endif /* DEL_H */
