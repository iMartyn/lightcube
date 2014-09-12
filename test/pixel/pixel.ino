/*
  Test a single pixel
 */

#include "libpixel.h"

// the setup routine runs once when you press reset:
void setup() {                
  Serial.begin(9600);
  init_pixel();
}

static uint8_t phase;

// the loop routine runs over and over again forever:
void loop() {
  uint8_t r, g, b;
  r = 0;
  g = 0;
  b = 0;
  start_data();
  if (phase == 0) {
    r = 0x08;
    phase++;
  } else if (phase == 1) {
    g = 0x08;
    phase++;
  } else {
    b = 0x08;
    phase = 0;
  }
  out_color(r,g,b);
  end_data();
  delay(200);
}
