#include "rgb-led.h"

static float easeInOutQuad(float t) { return t<.5 ? 2*t*t : -1+(4-2*t)*t; }

RGBLed::RGBLed() {
  this->start_color = {0, 0, 0};
  this->current_color = {0, 0, 0};
  this->dest_color = {0, 0, 0};
  this->start = 0;
  this->duration = 0;
}

void RGBLed::update(t_millis now) {

  float f = (float)(now - this->start) / this->duration;
  if( f < 0 ) {
    this->current_color = this->start_color;
  } // if 
  else
  if( f > 1.0 ) {
    this->current_color = this->dest_color;
  }
  else {
    f = easeInOutQuad(f);
    this->current_color.r = this->start_color.r + (this->dest_color.r - this->start_color.r) * f;    
    this->current_color.g = this->start_color.g + (this->dest_color.g - this->start_color.g) * f;    
    this->current_color.b = this->start_color.b + (this->dest_color.b - this->start_color.b) * f;    
  } // else
}

