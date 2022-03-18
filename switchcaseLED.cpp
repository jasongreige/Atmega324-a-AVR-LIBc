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
#include <avr/io.h> 
#include <util/delay.h> 

bool appuiD2()
{
  const uint8_t debounceTime = 10;
  //PD2 = position de la pin D2
  //PIND = registre qui contient 8 bit dont tu connais pas la composition (xxxx xxxx)
  // 1<<PD2 = shifter vers la gauche la valeur 1 de PD2 fois (0000 0100)
  // PIND & (1<<PD2) = xxxx xxxx & 0000 0100 = 0000 0x00
  if (PIND & (1<<PIND2)){
    _delay_ms(debounceTime);
    if (PIND & (1<<PIND2)){
      return true;
    }
  }
  return false;
}

bool appuiD3(){
  const uint8_t debounceTime = 10;
  if (PIND&(1<<PIND3)){
    _delay_ms(debounceTime);
    if (PIND&(1<<PIND3)){
      return true;
    }
  }
  return false;
}


int main()
{
  enum EtatDel {ETEINTE=0x00, ROUGE=0x02, VERT=0x01};
  enum class EtatSysteme {INIT, ONECLICK, TWOCLICK};
  bool condition;

  EtatSysteme etatSysteme = EtatSysteme::INIT;

  DDRA |= (1<<DDA0)|(1<<DDA1);
  DDRD &= ~(1<<DDD2)|~(1<<DDD3);
while (true)
  {
    while(appuiD2());
    while(appuiD3());
    condition = true;

    switch (etatSysteme)
    {
      case EtatSysteme::INIT:
            PORTA = ROUGE;
            while (!appuiD2() && condition){
                if (appuiD3()){
                    etatSysteme = EtatSysteme::ONECLICK;
                    condition = false;
                }
            }
            if (appuiD2() && condition)
                etatSysteme = EtatSysteme::ONECLICK;
        break;
      case EtatSysteme::ONECLICK:
            PORTA = VERT;
            while (!appuiD2() && condition){
                if (appuiD3()){
                    etatSysteme = EtatSysteme::TWOCLICK;
                    condition = false;
                }
            }
            if (appuiD2() && condition)
                etatSysteme = EtatSysteme::INIT;
        break;
      case EtatSysteme::TWOCLICK:
            PORTA = ETEINTE;
            while (!appuiD2() && condition){
                if (appuiD3()){
                    etatSysteme = EtatSysteme::INIT;
                    condition = false;
                }
            }
            if (appuiD2() && condition)
                etatSysteme = EtatSysteme::ONECLICK;
        break;
    }
  }
  return 0;
}