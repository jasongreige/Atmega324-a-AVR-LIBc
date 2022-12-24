
#include "pwm.h"

PWM::PWM()
{

    TCCR0A  |= (1 << COM0A1) | (1 << COM0B1) |(1 << WGM00);
    TCCR0B |= (1 << CS01);
    TCNT0 = 0;
    DDRB |= (1<<PB3)|(1<<PB4)|(1<<PB2)|(1<<PB5);
    
}

void PWM::avancer(uint8_t a, uint8_t b)
{
    PORTB &= ~(1 << PB5)&~(1 << PB2);
    OCR0A = a;
    OCR0B = b;
}

void PWM::reculer(uint8_t a, uint8_t b)
{
    PORTB |= (1 << PB5)|(1 << PB2);
    OCR0A = a;
    OCR0B = b;
}

void PWM::spinGauche(uint8_t a, uint8_t b)
{
    PORTB &= ~(1<<PB5);
    PORTB |= (1<<PB2);

    OCR0A = a;
    OCR0B = b;
}

void PWM::spinDroite(uint8_t a, uint8_t b)
{
    PORTB &= ~(1<<PB2);
    PORTB |= (1<<PB5);

    OCR0A = a;
    OCR0B = b;
}

void PWM::arreter()
{
    avancer(0,0);
}

void PWM::tournerDroite()
{
    arreter();
    _delay_ms(5);
    avancer(0,SPEED_LEFT_MOTOR+15);
    _delay_ms(2000);
    arreter();
}

void PWM::tournerGauche()
{
    arreter();
    _delay_ms(5);
    avancer(SPEED_RIGHT_MOTOR+15,0);
    _delay_ms(2000);
    arreter();
}

void PWM::avancerAvecLigne(uint8_t lecture)
{
    _delay_ms(5);

    LineCaptor lineCaptor;
    etatCapteur = lineCaptor.captLine();

    switch(etatCapteur)
    {
        case veryBigTurnLeft:
            avancer(SPEED_RIGHT_MOTOR-10,0);
            break;    
        case bigTurnLeft:
            compteur++;
            arreter();
            _delay_ms(500);
            avancer(SPEED_RIGHT_MOTOR+30, SPEED_LEFT_MOTOR+30);
            _delay_ms(500);
            avancer(SPEED_RIGHT_MOTOR+50, 0);
            _delay_ms(250);            
            break;
        case mediumTurnLeft:
            avancer(SPEED_RIGHT_MOTOR + 20,SPEED_LEFT_MOTOR);
            break;
        case smallTurnLeft:
            avancer(SPEED_RIGHT_MOTOR+10, SPEED_LEFT_MOTOR );
            break;
        case center:
            avancer(SPEED_RIGHT_MOTOR,SPEED_LEFT_MOTOR);
            break;
        case smallTurnRight:
            avancer(SPEED_RIGHT_MOTOR,SPEED_LEFT_MOTOR +10);
            break;
        case mediumTurnRight:
            avancer(SPEED_RIGHT_MOTOR ,SPEED_LEFT_MOTOR+20);
            break;
        case bigTurnRight:
            avancer(0, SPEED_LEFT_MOTOR-10);
            break;
        case veryBigTurnRight:
            avancer(0,SPEED_LEFT_MOTOR-10);
            break;
        case turn90Right:
            compteur++;
            arreter();
            _delay_ms(500);
            avancer(SPEED_RIGHT_MOTOR+30, SPEED_LEFT_MOTOR+30);
            _delay_ms(500);
            avancer(0, SPEED_LEFT_MOTOR+50);
            _delay_ms(200);
            break;
        case turn90Right2:
            compteur++;
            arreter();
            _delay_ms(500);
            avancer(SPEED_RIGHT_MOTOR+30, SPEED_LEFT_MOTOR+30);
            _delay_ms(500);
            avancer(0, SPEED_LEFT_MOTOR+50);
            _delay_ms(200);
            break;   
        case turn90Left2:
            compteur++;
            arreter();
            _delay_ms(500);
            avancer(SPEED_RIGHT_MOTOR+30, SPEED_LEFT_MOTOR+30);
            _delay_ms(500);
            avancer(SPEED_RIGHT_MOTOR+50, 0); 
            _delay_ms(250);
            break;
        case fullLine:
            compteur++;
            break;

    }
}


void PWM::avancerPartieS()
{
    _delay_ms(5);

    LineCaptor lineCaptor;
    etatCapteur = lineCaptor.captLine();

    switch(etatCapteur)
    {
        case veryBigTurnLeft:
            avancer(SPEED_RIGHT_MOTOR-10,0);
            _delay_ms(100);
            break;   
        case bigTurnLeft:
            avancer(SPEED_RIGHT_MOTOR-10,0);
            _delay_ms(100);
            break;
        case mediumTurnLeft:
            avancer(SPEED_RIGHT_MOTOR + 20,SPEED_LEFT_MOTOR);
            _delay_ms(100);
            break;
        case smallTurnLeft:
            avancer(SPEED_RIGHT_MOTOR+10, SPEED_LEFT_MOTOR );
            _delay_ms(100);
            break;
        case center:
            avancer(SPEED_RIGHT_MOTOR,SPEED_LEFT_MOTOR);
            _delay_ms(100);
            break;
        case smallTurnRight:
            avancer(SPEED_RIGHT_MOTOR,SPEED_LEFT_MOTOR +10);
            _delay_ms(100);
            break;
        case mediumTurnRight:
            avancer(SPEED_RIGHT_MOTOR ,SPEED_LEFT_MOTOR+20);
            _delay_ms(100);
            break;
        case bigTurnRight:
            avancer(0, SPEED_LEFT_MOTOR-10);
            _delay_ms(100);
            break;
        case veryBigTurnRight:
            avancer(0,SPEED_LEFT_MOTOR-10);
            _delay_ms(100);
            break;

    }
}



void PWM::avancerAvecLigneSansVirage(uint8_t lecture)
{
    _delay_ms(5);

    LineCaptor lineCaptor;
    etatCapteur = lineCaptor.captLine();

    switch(etatCapteur)
    {
        case veryBigTurnLeft:
            avancer(SPEED_RIGHT_MOTOR-10,50);
            break;    
        case bigTurnLeft:
            avancer(SPEED_RIGHT_MOTOR-10,50);
            break;
        case mediumTurnLeft:
            avancer(SPEED_RIGHT_MOTOR + 20,SPEED_LEFT_MOTOR);
            break;
        case smallTurnLeft:
            avancer(SPEED_RIGHT_MOTOR+10, SPEED_LEFT_MOTOR );
            break;
        case center:
            avancer(SPEED_RIGHT_MOTOR,SPEED_LEFT_MOTOR);
            break;
        case smallTurnRight:
            avancer(SPEED_RIGHT_MOTOR,SPEED_LEFT_MOTOR +30);
            break;
        case mediumTurnRight:
            avancer(SPEED_RIGHT_MOTOR ,SPEED_LEFT_MOTOR+30);
            break;
        case bigTurnRight:
            avancer(60, SPEED_LEFT_MOTOR-10);
            break;
        case veryBigTurnRight:
            avancer(60,SPEED_LEFT_MOTOR-10);
            break;
    }
}

void PWM::bondirEntreLignes()
{
    _delay_ms(5);

    LineCaptor lineCaptor;
    etatCapteur = lineCaptor.captLine();

    switch(etatCapteur)
    {
        case veryBigTurnLeft:
            countBondir++;
            avancer(0,SPEED_LEFT_MOTOR+10);
            break;    
        case bigTurnLeft:
            countBondir++;
            avancer(0 ,SPEED_LEFT_MOTOR+10);
            break;
        case mediumTurnLeft:
             countBondir++;
             avancer(0, SPEED_LEFT_MOTOR +10);
             break;
        case smallTurnLeft:
             countBondir++;
             avancer(0, SPEED_LEFT_MOTOR+10);
             break;
        case bigTurnRight:
            avancer(SPEED_RIGHT_MOTOR+10,0);
            break;
        case veryBigTurnRight:
            avancer(SPEED_RIGHT_MOTOR+10,0);
            break;
        case mediumTurnRight:
             avancer(SPEED_RIGHT_MOTOR+10, 0);
             break;
        case smallTurnRight:
             avancer(SPEED_RIGHT_MOTOR +10, 0);
             break;
        case turn90Right:
             avancer(SPEED_RIGHT_MOTOR +10,0);
             break;
        case turn90Right2:
             avancer(SPEED_RIGHT_MOTOR+10,0);
             break;
        case turn90Left:
             countBondir++;
             avancer(0,SPEED_LEFT_MOTOR +10);
             break;
        case turn90Left2:
             countBondir++;
             avancer(0,SPEED_LEFT_MOTOR+10 );
             break;
        case center:
             avancer(SPEED_RIGHT_MOTOR, SPEED_LEFT_MOTOR);
             break;

    }
}

void PWM::choisirChemin(uint8_t lecture)
{
    if (lecture >= 42 && lecture <= 65){
        avancer(0,150);//aller a droite
        _delay_ms(750);
    }    
    if (lecture >= 17 && lecture <= 21){
        avancer(150,0);//aller a gauche
        _delay_ms(750);
    }   
}

void PWM::reculerChemin(uint8_t lecture){
    if (lecture >= 42 && lecture <= 65){
        reculer(0,150);//aller a droite
        _delay_ms(600);
    }    
    if (lecture >= 17 && lecture <= 21){
        reculer(150,0);//aller a gauche
        _delay_ms(600);
    }  
}


void PWM::choisirPoteau(uint8_t lecture)
{
    if (lecture >= 17 && lecture <= 21){
        avancer(0,150);//aller a droite
        _delay_ms(600);
    }    
    if (lecture >= 42 && lecture <= 65){
        avancer(150,0);//aller a gauche
        _delay_ms(600);
    }   
}

void PWM::reculerPoteau(uint8_t lecture){
    if (lecture >= 17 && lecture <= 21){
        reculer(0,150);//aller a droite
        _delay_ms(600);
    }    
    if (lecture >= 42 && lecture <= 65){
        reculer(150,0);//aller a gauche
        _delay_ms(600);
    }  
}

void PWM::stationnement(){
    reculer(145,140);
    _delay_ms(1600);
    arreter();
    _delay_ms(1000);
    reculer(0, 150);
    _delay_ms(1100);
    arreter();
    _delay_ms(1000);
    reculer(137, 130);
    _delay_ms(1550);
    arreter();
}











