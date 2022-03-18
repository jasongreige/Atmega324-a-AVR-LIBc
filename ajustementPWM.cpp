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

void ajustementPwm (uint8_t pourcentage) {
  // mise à un des sorties OC1A et OC1B sur comparaison
  // réussie en mode PWM 8 bits, phase correcte
  // et valeur de TOP fixe à 0xFF (mode #1 de la table 16-5
  // page 130 de la description technique du ATmega324PA)
  OCR1A = 255 - (255*(pourcentage/100)) ;
  OCR1B = 255 - (255*(pourcentage/100)) ;
  // division d'horloge par 8 - implique une fréquence de PWM fixe
  TCCR1A |= (1 << COM1A1) | (1 << COM1A0) | (1 << COM1B1) | (1 << COM1B0) |(1 << WGM10);    //p.130
  TCCR1B |= (1 << CS11); 
  TCCR1C = 0;
}
             

int main()
{
  DDRD = 0x30;

  for (int i=0; i<5; i++){
    ajustementPwm(100-i*25)
    _delay_ms(2000)
  }
  return 0;
}