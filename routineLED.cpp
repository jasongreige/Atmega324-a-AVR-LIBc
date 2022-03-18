/*
Noms des auteurs: Paul Yaacoub et Jason Greige
Description du programme: Ce programme utilise le boutton installé sur le breadboard. Lorsque le boutton est appuyé, il y a un compteur qui incrémente tant que le boutton est appuyé.
                          Ce compteur incrémente de 10 à chaque seconde. Lorsque le boutton est relaché, la DEL allume vert pendant 1/2 secondes. La DEL s'éteint ensuite pendant 2 secondes.
                          Ensuite, la DEL clignote (compteur / 2) fois au rythme de 2 fois par seconde. Finalement, la DEL tourne au vert pour 1 seconde et elle s'éteint. Si jamais le boutton
                          est appuyé pour 12 secondes consécutives, la DEL tourne au vert automatiquement, sans avoir à relacher le boutton sur le breadboard. La DEL va donc ensuite clignoter 
                          rouge 60 fois, allumer vert pour 1 seconde et s'éteindre. 
Broches IN/OUT: Notre broche IN est D2 (DDRD &= ~(1<<DDD2)) et nos broches OUT sont B0 et B1 (DDRA |= (1<<DDB0)|(1<<DDB1))

Table des états:
+--------------+----+--------------+--------------------------------------------+
| État présent | D2 | État suivant | OUTPUT                                     |
|              |    |              |                                            |
+--------------+----+--------------+--------------------------------------------+
| INIT         | 0  | INIT         | ÉTEINTE                                    |
| INIT         | 1  | CLICK        |                                            |
+--------------+----+--------------+--------------------------------------------+
| CLICK        | 0  | RELEASE      | ÉTEINTE                                    |
| CLICK        | 1  |              |                                            |
+--------------+----+--------------+--------------------------------------------+
| RELEASE      | 0  | INIT         | 1- Green(0.5sec)                           |
| RELEASE      | 1  | INIT         | 2- Red ((compteur/2) fois, 2 fois par sec) |
|              |    |              | 3- Green (1sec)                            |
+--------------+----+--------------+--------------------------------------------+
*/


#define F_CPU 8000000
#include <avr/io.h> 
#include <util/delay.h> 

bool appuiD2()
{
  const uint8_t debounceTime = 10;
  if (!(PIND & (1<<PIND2))){
    _delay_ms(debounceTime);
    if (!(PIND & (1<<PIND2))){
      return true;
    }
  }
  return false;
}


int main()
{
  enum EtatDel {ETEINTE=0x00, ROUGE=0x02, VERTE = 0x01};
  enum class EtatSysteme {INIT, CLICK, RELEASE};

  int compteur = 0;
  bool condition = false;

  EtatSysteme etatSysteme = EtatSysteme::INIT;

  DDRB |= (1<<DDB0)|(1<<DDB1);
  DDRD &= ~(1<<DDD2);
  while (true) {
    switch (etatSysteme) {
      case EtatSysteme::INIT:
        PORTB = ETEINTE;
        compteur = 0;
        if (appuiD2())
          etatSysteme = EtatSysteme::CLICK;
        break;

      case EtatSysteme::CLICK:
        PORTB = ETEINTE;
        if (appuiD2()){
          condition = true;
          while (appuiD2() && (compteur < 120)){
            compteur++;
            _delay_ms(100);
            
          }
        }
        if ((!appuiD2() && condition) | (compteur==120))   
          etatSysteme = EtatSysteme::RELEASE;
        break;

      case EtatSysteme::RELEASE:
        PORTB = VERTE;
        _delay_ms(500);
        PORTB = ETEINTE;
        _delay_ms(2000);

        for (int i=0; i<(compteur/2); i++) {
          PORTB = ROUGE;
          _delay_ms(250);
          PORTB = ETEINTE;
          _delay_ms(250);
        }

        PORTB = VERTE;
        _delay_ms(1000);
        etatSysteme = EtatSysteme::INIT;
        break;
    }
  }
  return 0;
}