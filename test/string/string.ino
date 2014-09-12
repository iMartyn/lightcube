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
  int n;
  uint8_t row;
  static unsigned long last_tick;
  unsigned long now;
  uint8_t val;
  uint8_t col;
  uint8_t bright;

  start_data();
  for (n = 0; n < 64; n++) {
      col = n >> 3;
      row = n & 7;
      if (col & 1)
	row = 7 - row;
      val = ((col + phase) + (row << 3) - (row * 2)) & 63;
      row = val >> 3;
      col = val & 7;
      bright = (7 - col) << 5;
      if (row == 0) {
	  out_color(bright, 0, 0);
      } else if (row == 2) {
	  out_color(0, bright, 0);
      } else if (row == 4) {
	  out_color(0, 0, bright);
      } else {
	  out_color(0, 0, 0);
      }
  }
  now = millis();
  if (1||(last_tick == 0) || (now - last_tick >= 1000)) {
      last_tick = now;
      phase = (phase + 1) & 63;
  }
  end_data();
}
