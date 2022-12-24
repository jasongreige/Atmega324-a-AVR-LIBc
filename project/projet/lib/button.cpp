#ifndef F_CPU
#define F_CPU 8000000
#endif

#include "button.h"

Button::Button(volatile uint8_t* direction, volatile uint8_t* pinIn, uint8_t pinInNumber)
{
    direction_ = direction;
    pinIn_ = pinIn;
    pinInNumber_ = pinInNumber;
}

void Button::setEIMSK(EIMSKModes EIMSKMode)
{
    EIMSKMode_ = EIMSKMode;
}

void Button::setEICRA(EICRAModes EICRAMode)
{
    EICRAMode_ = EICRAMode;
}

void Button::initialisation()
{
    // cli est une routine qui bloque toutes les interruptions.
    // Il serait bien mauvais d'être interrompu alors que
    // le microcontrôleur n'est pas prêt...
    cli ();
    // configurer et choisir les ports pour les entrées
    // et les sorties. DDRx... Initialisez bien vos variables
    
    //DDRA |= (1<<DDA0)|(1<<DDA1); Sorties commentées car elles sont spécifiques à l`utilisation de DEL;
    //DDRD &= ~(1<<DDD2); 

    *direction_ &= ~(1<<pinInNumber_);
    
    // cette procédure ajuste le registre EIMSK
    // de l’ATmega324PA pour permettre les interruptions externes
    EIMSK |= EIMSKMode_ ;
    // il faut sensibiliser les interruptions externes aux
    // changements de niveau du bouton-poussoir
    // en ajustant le registre EICRA
    EICRA |= EICRAMode_ ;
    // sei permet de recevoir à nouveau des interruptions.
    sei ();
}

bool Button::buttonPressed() {
    
    return (*pinIn_ & (1<<pinInNumber_));
}

bool Button::debounceButtonPressed()
{
    const uint8_t debounceTime = 10;   
    if (Button::buttonPressed()) {
        _delay_ms(debounceTime);
        if (Button::buttonPressed())
            return true;
    }
    return false;
}


