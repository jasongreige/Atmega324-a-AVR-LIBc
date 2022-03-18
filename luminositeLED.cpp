/*
Noms des auteurs: Paul Yaacoub et Jason Greige
Description du programme: Ce programme allume la DEL d'une certaine couleur dépendemment de l'intensité de la source de lumiere qu'on expose à la photorésistance 
                          installée sur le breadboard du robot. Si la lumiere est basse (en cachant la photorésistance), la DEL prendra la couleur verte, si la lumiere
                          est à un bon niveau (lumiere ambiante), la DEL tournera à l'ambré, si la lumiere est trop forte (photorésistance sous une lampe de poche),
                          la DEL devient rouge.

                          Lumiere basse: < 220
                          Lumiere bon niveau: 220 à 245 exclu
                          Lumiere trop forte: > 245

Broches IN/OUT: La broche IN est A0 (DDRA &= ~(1<<DDA0)) et nos broches OUT sont B0 et B1 (DDRA |= (1<<DDB0)|(1<<DDB1)).
*/


#define F_CPU 8000000
#include <avr/io.h> 
#include <can.h>
#include <util/delay.h> 

can convertisseur;

enum EtatDel {ROUGE=0x02, VERTE = 0x01};

uint8_t luminositeLu;

uint8_t borne1 = 220;
uint8_t borne2 = 245;

void delAmbre(){
  uint8_t alternateur = VERTE;
  for (int i = 0; i<300; i++)
    alternateur = ~alternateur;
    PORTB = alternateur;
    _delay_ms(10);
}

int main(){

  DDRB |= (1<<DDB0)|(1<<DDB1);
  DDRA &= ~(1<<DDA0);

  while(true){
    luminositeLu = (convertisseur.lecture(PINA) >> 2);

    if (luminositeLu <= borne1)
      PORTB = VERTE;

    else if ((luminositeLu > borne1) && (luminositeLu < borne2)) 
      delAmbre();
    
    else
      PORTB = ROUGE;
    
  }
  
}