/*
Noms des auteurs: Paul Yaacoub et Jason Greige
Description du programme: Ce programme affiche différentes couleurs de DEL selon le bouton cliqué et < quel moment il est cliqué. Le systeme commence avec une sortie 
                          de DEL rouge. Peu importe le boutton cliqué apres cela, il change < la couleur verte. Si la DEL est verte et que l'on appuie sur D2, la DEL
                          devient rouge. Si on appuie sur D3 < la place, la DEL s'éteint. Si la DEL est éteinte et que l'on appuie sur D2, la DEL devient verte, si on
                          appuie sur D3 lorsqu'elle est éteinte, elle devient rouge.
Broches IN/OUT: Nos broches IN sont D2 et D3 (DDRD &= ~(1<<DDD2)|~(1<<DDD3)) et nos broches OUT sont A0 et A1 (DDRA |= (1<<DDA0)|(1<<DDA1))

Table des états:
+--------------+----+----+--------------+----+----+
| Etat present | D3 | D2 | Etat suivant | A1 | A0 |
+--------------+----+----+--------------+----+----+
| Init         | 0  | 0  | Init         | 1  | 0  |
| Init         | 0  | 1  | ONECLICK     | 1  | 0  |
| Init         | 1  | 0  | ONECLICK     | 1  | 0  |
| Init         | 1  | 1  | Init         | 1  | 0  |
+--------------+----+----+--------------+----+----+
| ONECLICK     | 0  | 0  | ONECLICK     | 1  | 0  |
| ONECLICK     | 0  | 1  | Init         | 1  | 0  |
| ONECLICK     | 1  | 0  | TWOCLICK     | 1  | 0  |
| ONECLICK     | 1  | 1  | Init         | 1  | 0  |
+--------------+----+----+--------------+----+----+
| TWOCLICK     | 0  | 0  | TWOCLICK     | 0  | 0  |
| TWOCLICK     | 0  | 1  | ONECLICK     | 0  | 0  |
| TWOCLICK     | 1  | 0  | Init         | 0  | 0  |
| TWOCLICK     | 1  | 1  | Init         | 0  | 0  |
+--------------+----+----+--------------+----+----+
*/




#define F_CPU 8000000
#include <avr/interrupt.h>
#include <avr/io.h> 
#include <util/delay.h> 

volatile uint8_t minuterieExpiree;
volatile uint8_t boutonPoussoir; 

enum EtatDel {ETEINTE=0x00, ROUGE=0x02, VERT=0x01};

void initialisation ( void ) {
  // cli est une routine qui bloque toutes les interruptions.
  // Il serait bien mauvais d'être interrompu alors que
  // le microcontrôleur n'est pas prêt...
  cli ();
  // configurer et choisir les ports pour les entrées
  // et les sorties. DDRx... Initialisez bien vos variables
  DDRA |= (1<<DDA0)|(1<<DDA1);
  DDRD &= ~(1<<DDD2)|~(1<<DDD3);
  // cette procédure ajuste le registre EIMSK
  // de l’ATmega324PA pour permettre les interruptions externes
  EIMSK |= (1 << INT0)|(1 << INT1) ; 
  // il faut sensibiliser les interruptions externes aux
  // changements de niveau du bouton-poussoir
  // en ajustant le registre EICRA
  EICRA |= (1 << ISC01) | (1 << ISC00);                   //p.67
  // sei permet de recevoir à nouveau des interruptions.
  sei ();
}

ISR (TIMER1_COMPA_vect) {
  minuterieExpiree = 1;
}

ISR (INT0_vect) {
  // anti-rebond
  boutonPoussoir = 1;
  _delay_ms(30);
  EIFR |= (1 << INTF0) ;
}



//16 bits donc va jusqu'a 65535 (65536-1)
//8 000 000 / 1024 = 7812
void partirMinuterie ( uint16_t duree ) {
  minuterieExpiree = 0;
  // mode CTC du timer 1 avec horloge divisée par 1024
  // interruption après la durée spécifiée
  TCNT1 = 0 ;
  OCR1A = duree;
  TCCR1A |= (1 << COM1A1) | (1 << COM1A0);            //p.128
  TCCR1B |= (1 << CS12) | (0 << CS11) | (1 << CS10);  //p.131
  TCCR1C = 0;                                         //p.131
  TIMSK1 |= (1 << OCIE1A) ;                           //p.134
}


int main()
{

  initialisation();
  cli();
  PORTA = ETEINTE;
  _delay_ms(10000);
  PORTA=ROUGE;
  _delay_ms(100);
  PORTA = ETEINTE;
  sei();
  partirMinuterie(7812);
  do {
    // attendre qu'une des deux variables soit modifiée
    // par une ou l'autre des interruptions.
  } while ( minuterieExpiree == 0 &&   boutonPoussoir == 0 );
    // Une interruption      s'est produite. Arrêter toute
    // forme d'interruption. Une seule réponse suffit.
    cli ();
    // Verifier la réponse
    if (boutonPoussoir==1){
      while (boutonPoussoir==1){
        PORTA = VERT;
      }
    }
    else if (minuterieExpiree==1)
      while (minuterieExpiree==1){
        PORTA = ROUGE;
      }
  return 0;
}