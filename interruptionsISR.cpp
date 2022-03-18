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

volatile bool appuiD2 = false;
volatile bool appuiD3 = true;

enum EtatDel {ETEINTE=0x00, ROUGE=0x02, VERT=0x01};
enum class EtatSysteme {INIT, ONECLICK, TWOCLICK};

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
  EICRA |= (0 << ISC01) | (1 << ISC00);
  // sei permet de recevoir à nouveau des interruptions.
  sei ();
}

volatile EtatSysteme etatSysteme = EtatSysteme::INIT; // selon le nom de votre variable
// placer le bon type de signal d'interruption
// à prendre en charge en argument
ISR (INT0_vect) {  //vec
  // laisser un délai avant de confirmer la réponse du
  // bouton-poussoir: environ 30 ms (anti-rebond)
  _delay_ms ( 30 );
  // se souvenir ici si le bouton est pressé ou relâché
  // changements d'état tels que ceux de la
  // semaine précédente
  if (PIND & (1<<PIND2))
      appuiD2 = true;
    
  else
    appuiD2 = false;

  // Voir la note plus bas pour comprendre cette instruction et son rôle
  EIFR |= (1 << INTF0) ;
}

ISR (INT1_vect) {  //vec
  // laisser un délai avant de confirmer la réponse du
  // bouton-poussoir: environ 30 ms (anti-rebond)
  _delay_ms ( 30 );
  // se souvenir ici si le bouton est pressé ou relâché
  // changements d'état tels que ceux de la
  // semaine précédente
  if (PIND & (1<<PIND3))
      appuiD3 = true;
    
  else
    appuiD3 = false;
  
  // Voir la note plus bas pour comprendre cette instruction et son rôle
  EIFR |= (1 << INTF1) ;
}


int main()
{
  bool condition;

  initialisation();

while (true)
  {
    while(appuiD2);
    while(appuiD3);
    condition = true;

    switch (etatSysteme)
    {
      case EtatSysteme::INIT:
        PORTA = ROUGE;
        while (!appuiD2 && condition){
          if (appuiD3){
            etatSysteme = EtatSysteme::ONECLICK;
            condition = false;
          }
        }
          if ((appuiD2) && condition)
            etatSysteme = EtatSysteme::ONECLICK;
        break;
      case EtatSysteme::ONECLICK:
        PORTA = VERT;
        while ((!appuiD2) && condition){
          if (appuiD3){
            etatSysteme = EtatSysteme::TWOCLICK;
            condition = false;
          }
        }
          if (appuiD2 && condition)
            etatSysteme = EtatSysteme::INIT;
        break;
      case EtatSysteme::TWOCLICK:
        PORTA = ETEINTE;
        while (!appuiD2 && condition){
          if (appuiD3){
            etatSysteme = EtatSysteme::INIT;
            condition = false;
          }
        }
          if ((appuiD2) && condition)
            etatSysteme = EtatSysteme::ONECLICK;
        break;
    }
  }
  return 0;
}