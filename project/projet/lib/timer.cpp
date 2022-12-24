#include "timer.h"

//*****Attention duree est en secondes*****
void TimerProjet::startTimer(double duration) {
    TCNT1 = 0;
    
    uint16_t temps = duration * (F_CPU / 1024.0);
    OCR1A = temps;

    TCCR1A = (1 << WGM12);

    TCCR1B = (1 << CS12) | (1 << CS10);

    TCCR1C = 0;
    
    TIMSK1 |= (1 << OCIE1A);
}


void TimerProjet::stop() {
    TCCR1B &= ~((1 << CS12) | (1 << CS10));
    TIMSK1 &= ~(1 << OCIE1A);
}

