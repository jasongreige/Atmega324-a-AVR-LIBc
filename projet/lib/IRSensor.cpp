#include "IRSensor.h"

IRSensor::IRSensor() {};

double IRSensor::readNumericValue(){
    
    double numericValue = can.get8Bits(can.lecture(PA0));
    return numericValue;
}


long IRSensor::readAndReturnValue(){
  _delay_ms(50);

  long data = 0;
  for (int i=0; i<50; i++) 
    data += readNumericValue();
  data = data/50;
  return data;
}


