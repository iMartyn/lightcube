#include <Arduino.h>

#include "libpixel.h"

static const int data_pin = 7;
#define PIN_HIGH() PORTE |= _BV(6)
#define PIN_LOW() PORTE &= ~_BV(6)

void
start_data(void)
{
  PIN_LOW();
  delayMicroseconds(50);
}

void
end_data(void)
{
  PIN_HIGH();
  delayMicroseconds(50);
}

static void
out_byte(uint8_t val)
{
  uint8_t i;
  
  for (i = 0; i < 8; i++) {
    cli();
    PIN_HIGH();
    if ((val & 0x80) == 0) {
      delayMicroseconds(5);
      PIN_LOW();
      delayMicroseconds(25);
    } else {
      delayMicroseconds(20);
      PIN_LOW();
      delayMicroseconds(25);
    }
    sei();
    val <<= 1;
  }
}

void
out_color(uint8_t r, uint8_t g, uint8_t b)
{
  out_byte(r);
  out_byte(b);
  out_byte(g);
  delayMicroseconds(20);
}

void
init_pixel(void)
{
  // initialize the digital pin as an output.
  pinMode(data_pin, OUTPUT);
  PIN_HIGH();  
}
