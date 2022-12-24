#define F_CPU 8000000
#include <avr/io.h>


class Song
{
public:

    Song();

    double calculerFrequence(uint8_t note);
    void jouer(uint8_t note);
    void arreter();

    
};
