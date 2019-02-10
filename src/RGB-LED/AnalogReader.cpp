#include "Arduino.h"
#include "AnalogReader.h"

AnalogReader::AnalogReader(float weight, int pin)
{
  this->pin = pin;
  if(weight > 1.0)
    this->weight = weight/100.0;
  else
    this->weight = weight;  
}

void AnalogReader::update(void) {
 float raw = analogRead(this->pin);
 this->current += (this->weight*(raw - this->current));
}

int AnalogReader::get(void) {
  return this->current;
}

byte AnalogReader::get_byte(void) {
  return (byte)map(this->current, 0, 1023, 0, 255);
}

byte AnalogReader::update_and_get_byte(void) {
  this->update();
  return 200 - (byte)map(this->current, 0, 1023, 0, 200);
}
