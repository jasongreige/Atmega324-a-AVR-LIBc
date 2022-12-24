
#include <avr/io.h>
#include "song.h"
#include <math.h>


Song::Song()
{
    TCCR2A = (1 << WGM21) | (1 << COM2A0);

    DDRD |= (1 << PD7)|(1<<PD6);
    TCNT2 = 0;
    
}

double Song::calculerFrequence(uint8_t note)
{
    double frequenceNote = 440.0*(pow(2.0, (note - 69) /12));

    return frequenceNote;
}

void Song::jouer(uint8_t note)
{
    TCCR2B = (1 << CS22) | (1 << CS21) | (1 << CS20);
    double frequence = calculerFrequence(note);
    OCR2A = (F_CPU / ((2 * 1024) * frequence)) - 1;
}

void Song::arreter()
{
    TCCR2B = 0;
}

