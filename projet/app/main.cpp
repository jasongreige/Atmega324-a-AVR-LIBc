/*
* Description du programme: Le robot doit compléter un parcours en suivant une ligne noire à l'aide d'un capteur. Le  parcours comporte 3 parties: la partie A, la partie B et la partie S. La partie A consiste à détecter des poteaux  placés aléatoirement sur la partie B du parcours. Si le poteau est proche du robot, celui-ci émet un son aigu. S'il est placé loin du poteau, celui-ci émet un son grave. À chaque détection de poteau, le robot écrit dans sa mémoire la position à laquelle il a détecté le poteau puisque cette information sera utiliée dans la partie B, suite à un reset complet du robot une fois la partie A terminée. La fin de la partie A se fait lorsque le robot détecte une ligne noire horizontale. La partie B consiste à choisir un chemin de telle sorte que le robot ne fonce pas sur les poteaux présent sur le parcours. Arrivé au milieu du chemin, s'il n'y avait que 2 poteaux placés sur le parcours, le robot recule de 1,5 pouce, s'il y en avait 3, le robot se tourne à 15 degrés vers le deuxième poteau. Pour la partie S, le robot doit quitter son suivi de ligne une fois qu'il entre dans un couloir pour bondir entre ce couloir. Une fois avoir traversé tout le couloir en bondissant, il reprend son suivi de ligne jusqu'à ce qu'il ne détecte plus de ligne noire. Il devra ensuite de cela effectuer un stationnement entre les 4 points jaune placés à la fin de la partie S. Il faut préciser que l'on peut spécifier la partie que l'on souhaite faire exécuter au robot en manipulant les boutons du robot. Pour naviguer entre les modes, on utilise le bouton poussoir présent sur le Bread Board. La DEL change de couleur selon le mode présentemment actif: le vert pour la partie A, le rouge pour la partie B et l'ambré pour la partie S. Si l'on veut sélectionner le mode, il suffit d'appuyer sur le bouton blanc du robot. La DEL va alors clignoter 4 fois pendant 2 secondes avec la couleur du mode sélectionné. 

* Identifications matérielles: PORT A: - On utilise le port A0 en entrée et le VCC et GND du port A pour le capteur de distance
                                       - On utilise le port A2 à A7 en entrée pour brancher le capteur de suivi de  ligne. (Le VCC et GND du capteur de ligne sont branchés sur le PORTC) 
                               PORT B: - On utilise le port B0 et B1 en sortie pour manipuler la DEL
                                       - On utilise le port B2 à B5 en sortie pour manipuler nos moteurs (PWM)
                               PORT C: - On utilise le VCC et GND du port C pour le capteur de ligne
                               PORT D: - On utilise le port D2 en entrée pour le bouton blanc sur le robot
                                       - On utilise le port D3 en entrée pour le bouton-poussoir sur le bread board
                                       - On utilise le port D6 et D7 en sortie pour manipuler le piézo (PWM)    
                                       - On utilise le VCC et GND du port D pour le bouton-poussoir sur le bread-board    
    
* Table des états:  
+------------------+----+----+------------+         
| Etat present     | D2 | Etat suivant    |
+------------------+----+----+------------+
| POINT_A          | 0  | POINT_A         |
| POINT_A          | 1  | POINT_A_SELECTED| 
+------------------+----+-----------------+
| POINT_B          | 0  | POINT_B         |
| POINT_B          | 1  | POINT_B_SELECTED|
+------------------+----+-----------------+
| POINT_S          | 0  | POINT_S         | 
| POINT_S          | 1  | POINT_S_SELECTED| 
+------------------+----+-----------------+
| POINT_A_SELECTED | 0  | POINT_A         |
| POINT_A_SELECTED | 1  | POINT_A         | 
+------------------+----+-----------------+
| POINT_B_SELECTED | 0  | POINT_S_SELECTED|
| POINT_B_SELECTED | 1  | POINT_S_SELECTED|
+------------------+----+-----------------+
| POINT_S_SELECTED | 0  | POINT_A         | 
| POINT_S_SELECTED | 1  | POINT_A         | 
+------------------+----+-----------------+


+------------------+----+----+------------+
| Etat present     | D3 | Etat suivant    |
+------------------+----+----+------------+
| POINT_A          | 0  | POINT_A         |
| POINT_A          | 1  | POINT_B         | 
+------------------+----+-----------------+
| POINT_B          | 0  | POINT_B         |
| POINT_B          | 1  | POINT_S         |
+------------------+----+-----------------+
| POINT_S          | 0  | POINT_S         | 
| POINT_S          | 1  | POINT_A         | 
+------------------+----+-----------------+
*/


#include "button.h"
#include "debug.h"
#include "del.h"
#include "can.h"
#include "lineCaptor.h"
#include "memoire_24.h"
#include "pwm.h"
#include "rs232.h"
#include "song.h"
#include "timer.h"
#include "IRSensor.h"

volatile bool gMinuterieExpiree = 0;

Button buttonBreadBoard = Button(&DDRD, &PIND, 3);  

volatile bool appuiD3 = false;

Button buttonRobot = Button(&DDRD, &PIND, 2);       

volatile bool appuiD2 = false;

ISR ( INT0_vect ) { 
  _delay_ms ( 30 );
  if (buttonRobot.buttonPressed()) {
    appuiD2 = true;
  }
  else{
    appuiD2 = false;
  }
  EIFR |= (1 << INTF0) ;
}

ISR ( INT1_vect ) {
  _delay_ms ( 30 );
  if (!(PIND & (1<<PIND3))){
    appuiD3 = true;
  }
  else{         
    appuiD3 = false;
  }
  EIFR |= (1 << INTF0) ;
}

ISR (TIMER1_COMPA_vect){
  gMinuterieExpiree = 1;
}




int main(){
    buttonBreadBoard.setEIMSK(int1);
    buttonBreadBoard.setEICRA(anyEdge_1);
    buttonRobot.setEIMSK(int0);
    buttonRobot.setEICRA(anyEdge_0);

    buttonBreadBoard.initialisation();
    buttonRobot.initialisation();

    Led led(&PORTB,&DDRB, 0, 1);  
    PWM pwm;
    IRSensor sensor;
    LineCaptor lineCaptor;
    Song song;
    Memoire24CXXX memoire;
    RS232 rs232;
    TimerProjet timer;

    uint8_t adresse = 0x00;
    uint8_t premiereLecture;
    uint8_t deuxiemeLecture;
    uint8_t troisiemeLecture;

    uint8_t distanceValue=0;
    uint16_t countForB = 0;
    uint16_t countForS = 0;

    enum State{POINT_A, POINT_B, POINT_S, POINT_A_SELECTED, POINT_B_SELECTED, POINT_S_SELECTED};
    State state = POINT_A;

    while (true){
        switch (state){

            case POINT_A:
                led.turnGreen();
                if (appuiD3){
                    while(appuiD3){;}
                    state = POINT_B;
                }
                if (appuiD2){
                    while(appuiD2){;}
                    led.blinkGreen();
                    state = POINT_A_SELECTED;
                }
                break;

            case POINT_B:
                led.turnRed();
                if (appuiD3){
                    while(appuiD3){;}
                    state = POINT_S;
                }
                if (appuiD2){
                    while(appuiD2){;}
                    led.blinkRed();
                    state = POINT_B_SELECTED;
                }
                break;

            case POINT_S:
                while (!appuiD3 && !appuiD2)
                    led.turnAmber(1);
                if (appuiD3){
                    while(appuiD3){;}
                    state = POINT_A;
                }
                if (appuiD2){
                    while(appuiD2){;}
                    led.blinkAmber();
                    state = POINT_S_SELECTED;
                }
                break;

            case POINT_A_SELECTED:
                memoire.ecriture(adresse, 0);
                adresse++;  
                memoire.ecriture(adresse, 0);
                adresse++; 
                memoire.ecriture(adresse, 0);
                adresse++;                
                
                pwm.avancer(135, 130); 
                _delay_ms(1700);     
  
      
                adresse= 0x00; 
                pwm.etatCapteur = 0b00100; 
                while (pwm.etatCapteur!= 0b11111){                
                    pwm.avancerAvecLigne(0); 
                    distanceValue = sensor.readAndReturnValue();
                    rs232.transmissionSimpleUART(distanceValue);
                    if (distanceValue >= 42 && distanceValue <= 65){
                        _delay_ms(30);
                        if (distanceValue>=42 && distanceValue <=65){
                            memoire.ecriture(adresse, distanceValue);
                            adresse++;    
                            _delay_ms(5);
                            pwm.arreter();
                            song.jouer(81);
                            _delay_ms(1000);
                            song.arreter();
                            pwm.avancer(135, 130);
                            _delay_ms(1000);
                        }
                    }
                    else if (distanceValue >=17 && distanceValue <= 21){
                        distanceValue = sensor.readAndReturnValue();
                        distanceValue = sensor.readAndReturnValue();
                        if(distanceValue >=17 && distanceValue <= 21){
                            memoire.ecriture(adresse, distanceValue);
                            adresse++;
                            _delay_ms(5);                        
                            pwm.arreter();
                            song.jouer(45);
                            _delay_ms(1000);
                            song.arreter();
                            pwm.avancer(135, 130);
                            _delay_ms(1000);
                        }
                    }
                    else{;}   
                }
                pwm.arreter();
                _delay_ms(10000);
                state = POINT_A;
                break;

            case POINT_B_SELECTED:
                adresse = 0x00;
                memoire.lecture(adresse, &premiereLecture);
                adresse++;
                memoire.lecture(adresse, &deuxiemeLecture);
                adresse++;
                memoire.lecture(adresse, &troisiemeLecture);
                
                rs232.transmissionSimpleUART(premiereLecture);
                rs232.transmissionSimpleUART(deuxiemeLecture);
                rs232.transmissionSimpleUART(troisiemeLecture);

                pwm.avancer(SPEED_RIGHT_MOTOR, SPEED_LEFT_MOTOR);
                _delay_ms(1000);
                while (pwm.compteur!=3){
                    pwm.avancerAvecLigne(0);
                }
                if (troisiemeLecture == 0){
                    pwm.choisirChemin(deuxiemeLecture);
                }
                else{
                    pwm.choisirChemin(troisiemeLecture);                   
                } 
                
                pwm.etatCapteur = 0b00100;
  
                while(countForB != 2500){
                    countForB++; 
                    pwm.avancerAvecLigneSansVirage(0);        
                }
                pwm.arreter();
                _delay_ms(1000);   
                
                
                if (troisiemeLecture == 0){
                    pwm.reculer(215, 200);
                    _delay_ms(600);
                    pwm.arreter();
                    _delay_ms(1000);
                    pwm.compteur=0;

                    while(pwm.compteur!= 1){
                        pwm.avancerAvecLigne(0);
                    }
                   
                    pwm.choisirChemin(premiereLecture);
                }
                else {
                    pwm.choisirPoteau(deuxiemeLecture);
                    pwm.arreter();
                    _delay_ms(1000);
                    pwm.reculerPoteau(deuxiemeLecture);
                    pwm.arreter();
                    _delay_ms(1000);
                    pwm.compteur = 0;

                    while (pwm.compteur != 1){
                        pwm.avancerAvecLigne(0);
                    }    
                   
                    pwm.choisirChemin(premiereLecture);
                }
                countForB=0;   

                pwm.etatCapteur = 0b00100;
                countForB = 0;
                while (countForB !=1650){
                    countForB++;
                    pwm.avancerAvecLigneSansVirage(0);
                }
                
                /*pwm.arreter();
                _delay_ms(1000);*/

                pwm.compteur = 0;
                pwm.etatCapteur = lineCaptor.captLine();
                while (pwm.compteur != 3) { 
                    pwm.avancerAvecLigne(0);
                }
                pwm.arreter();                  
                _delay_ms(1000);                
                
                state = POINT_S_SELECTED;
                break;

            case POINT_S_SELECTED:
                led.turnAmber(100);
                pwm.avancer(SPEED_RIGHT_MOTOR+20, SPEED_LEFT_MOTOR+20);
                _delay_ms(800);
                countForS = 0;
                while (countForS != 900){
                    countForS++;  
                    pwm.avancerAvecLigneSansVirage(0);  
                }
                pwm.arreter();
                _delay_ms(500);
                
                while (pwm.etatCapteur != 0b10000){
                    pwm.avancer(30, SPEED_LEFT_MOTOR);
                    pwm.etatCapteur = lineCaptor.captLine();
                }
        
                countForS = 0;
                pwm.countBondir = 0;
                while (true){
                    pwm.bondirEntreLignes();
                    if (pwm.etatCapteur == 0b00000){
                        gMinuterieExpiree = false;
                        timer.startTimer(1.3);
                        pwm.etatCapteur = lineCaptor.captLine();
                        while (gMinuterieExpiree == false && pwm.etatCapteur == 0b00000){
                            pwm.bondirEntreLignes();
                        }
                        if (gMinuterieExpiree == true){
                            timer.stop();
                            break;
                        }
                    }
                }
                pwm.arreter();
                _delay_ms(1000);
                
                pwm.etatCapteur = lineCaptor.captLine();
                while (pwm.etatCapteur != 0b10000){
                    pwm.avancer(50, SPEED_LEFT_MOTOR);
                    pwm.etatCapteur = lineCaptor.captLine();
                }
                 
                pwm.etatCapteur = lineCaptor.captLine();
                while (true){
                    pwm.avancerAvecLigneSansVirage(0);
                    if (pwm.etatCapteur == 0b00000){
                        gMinuterieExpiree = false;
                        timer.startTimer(1);                        
                        while(gMinuterieExpiree == false && pwm.etatCapteur == 0b00000){
                            pwm.avancerAvecLigneSansVirage(0);
                        }
                        if (gMinuterieExpiree == true){
                            timer.stop();                             
                            break;
                        }
                    }     
                }
                pwm.arreter();
                _delay_ms(1000);
                pwm.stationnement();
                _delay_ms(10000);
                state = POINT_A;
                break;       
        }    
    }
    return 0;
}
