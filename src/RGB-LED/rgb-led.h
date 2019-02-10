#ifndef RGB_LED_H
#define RGB_LED_H

#include <Arduino.h>

#define RGB_MODE 1
#define HSV_MODE 2

typedef unsigned long t_millis;

typedef struct {
  byte r;
  byte g;
  byte b;
} RGB;

class RGBLed {

 public:
  uint8_t  state;
  t_millis start;     // start time
  t_millis duration;  // stop - start
  RGB current_color;  //
  RGB start_color;    //
  RGB dest_color;     //

  RGBLed();
  void update(t_millis t);
  //Color( uint32_t c );
  //Color( uint8_t mode, uint16_t a, uint16_t b, uint16_t c );
  //void hsv( uint16_t h, uint16_t s, uint16_t v);
};

#endif