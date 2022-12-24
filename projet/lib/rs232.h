#pragma once
#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h> 
#include "memoire_24.h"

class RS232 
{
public:
   RS232();

   void initialisation();

   void transmissionSimpleUART(uint8_t donnee);

   void transmissionFromMemory(Memoire24CXXX memoire, const uint16_t adresseDebut, const uint16_t adresseFin);

    uint8_t reception();

};
