#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h> 
#include "memoire_24.h"
#include "rs232.h"

RS232::RS232() 
{
    initialisation();
}

void RS232::initialisation()
{
    // 2400 bauds. Nous vous donnons la valeur des deux
   // premiers registres pour vous éviter des complications.
   UBRR0H = 0;
   UBRR0L = 0xCF;
   // permettre la réception et la transmission par le UART0
   UCSR0A |= (1 << UDRE0); 
   UCSR0B |= (1 << TXEN0) | (1 << RXEN0);
   // Format des trames: 8 bits, 1 stop bits, sans parité
   UCSR0C |= (0 << UCSZ02) | (1 << UCSZ01) | (1 << UCSZ00);
}

void RS232::transmissionSimpleUART(uint8_t donnee)
{
   while(!(UCSR0A & (1 << UDRE0))){}
   UDR0 = donnee;
   
}

void RS232::transmissionFromMemory(Memoire24CXXX memoire, const uint16_t adresseDebut, const uint16_t adresseFin)
{
   uint8_t valeurLecture;
   for (uint16_t adresse = adresseDebut; valeurLecture != adresseFin; adresse++)
   {
      memoire.lecture(adresse, &valeurLecture);
      transmissionSimpleUART(valeurLecture);
   }
}

uint8_t RS232::reception()
{
    while(!(UCSR0A & (1 << RXC0)));
    return UDR0;
}

