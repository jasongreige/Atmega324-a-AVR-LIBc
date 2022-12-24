#define F_CPU 8000000
#include <avr/interrupt.h>
#include <avr/io.h>

class TimerProjet
{
public:

    void startTimer(double duration);
    void stop();

};

