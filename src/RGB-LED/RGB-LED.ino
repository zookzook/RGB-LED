
#include <Adafruit_NeoPixel.h>
#include "rgb-led.h"
#include "AnalogReader.h"

// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1


#if defined(__AVR_ATmega328P__)
#define PIN 3
#endif
#if defined (__AVR_ATtiny85__)
#define PIN 0
#define MODE_PIN 1
#endif

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 1

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_RGB + NEO_KHZ800);
RGBLed led;

#if defined(__AVR_ATmega328P__)
AnalogReader red(0.1, A0);
AnalogReader green(0.1, A1);
AnalogReader blue(0.1, A2);
#endif
#if defined (__AVR_ATtiny85__)
AnalogReader red(0.1, A1);
AnalogReader green(0.1, A2);
AnalogReader blue(0.1, A3);
#endif 

void read_color(RGB* d) {
  d->r = red.update_and_get_byte();
  d->g = green.update_and_get_byte();
  d->b = blue.update_and_get_byte();  
}

void setup() {

  #if defined(__AVR_ATmega328P__)
    Serial.begin(115200);
  #endif

  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code

  init_timer();
  pixels.begin(); // This initializes the NeoPixel library.

  t_millis t = millis() + 10;

  led.start = t;
  led.duration = 1000;
  led.start_color = {0, 0, 0};
  led.current_color = {0, 0, 0};
  led.state = 0;
  read_color(&led.dest_color);

  pinMode(MODE_PIN, INPUT);
  digitalWrite(MODE_PIN, HIGH);
}

void init_timer() {
  noInterrupts();                         // disable all interrupts
  
  #if defined(__AVR_ATmega328P__)
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;

  OCR1A = 1563/3;             // compare match register 100µs

  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= ((1 << CS12) | (1 << CS10));    // 1024 prescaler 
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
  #endif
  interrupts();             // enable all interrupts

  #if defined (__AVR_ATtiny85__)
  TCCR1 |= (1 << CTC1);     // clear timer on compare match
  TCCR1 |= (1 << CS13) | (1 << CS11) | (1 << CS10);  // 1024 prescaler 
  OCR1C = 782/4;            // compare match value 
  TIMSK |= (1 << OCIE1A);   // enable compare match interrupt
  #endif
}



ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{
  if(PINB & B00000010) {
    t_millis t = millis();  
    led.update(t);    
  }

  
  RGB* rgb = &led.current_color;

  pixels.setPixelColor(0, pixels.Color(rgb->r, rgb->g,rgb->b));
  pixels.show(); // This sends the updated pixel color to the hardware.

}

void update_animation(t_millis t) {

  red.update();
  green.update();
  blue.update();

  if(led.start + led.duration < t ) {

    led.start = t;
    led.duration = 500;

    if(led.state == 0) {
      led.state = 1;
      read_color(&led.start_color);
      led.current_color = led.start_color;
      led.dest_color = {0, 0, 0};
    }
    else {
      led.state = 0;
      read_color(&led.dest_color);
      led.start_color = {0, 0, 0};
      led.current_color = {0, 0, 0};
    } // else
  } // if 
}

void loop() {

  if(PINB & B00000010) {  
    t_millis t = millis();
    update_animation(t);
  }
  else {
      read_color(&led.current_color);
  }

}
