#define F_CPU 8000000
#define PWM_H
#include <avr/io.h> 
#include <util/delay.h>
#include "lineCaptor.h"

#define SPEED_RIGHT_MOTOR 100
#define SPEED_LEFT_MOTOR 100    

class PWM
{
public:
    PWM();
    void avancer(uint8_t a, uint8_t b);
    void reculer(uint8_t a, uint8_t b);
    void arreter();
    void tournerDroite();
    void tournerGauche();
    
    void avancerAvecLigne(uint8_t lecture);
    void avancerPartieS();    

    void choisirChemin(uint8_t lecture);
    void reculerChemin(uint8_t lecture);

    void choisirPoteau(uint8_t lecture);
    void reculerPoteau(uint8_t lecture);

    void spinGauche(uint8_t a, uint8_t b);
    void spinDroite(uint8_t a, uint8_t b);

    void avancerAvecLigneSansVirage(uint8_t lecture);

    void bondirEntreLignes();

    void stationnement();
    
    uint8_t etatCapteur;
    uint8_t compteur=0;
    uint8_t countBondir = 0;
};
