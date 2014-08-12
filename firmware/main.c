#define F_CPU 1000000ul

#include <stdint.h>

#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/cpufunc.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include <stdbool.h>

static void
init_pwm(void)
{
  // Use OC1A rather than OC0B to avoid attiny45 hardware bug
  // Configure both timers for 8-bit fast PWM, clk/8 (500Hz)
  // A match value of zero glitches, so do not enable output channels yet
  TIMSK = 0;
  OCR0A = 0;
  TCCR0A = _BV(WGM01) | _BV(WGM00);
  TCCR0B = _BV(CS01);

  OCR1A = 0;
  OCR1B = 0;
  OCR1C = 0xff;
  TCCR1 = _BV(PWM1A) | _BV(COM1A1) | _BV(CS12);
  GTCCR = _BV(PWM1B) | _BV(COM1B1);

  // Enable the output pins
  DDRB |= _BV(4) | _BV(0) | _BV(1);
}

static void
init_serial(void)
{
  DDRB &= ~_BV(2);
  DDRB |= _BV(3);
  PORTB |= _BV(2) | _BV(3);
}

int
main()
{
  init_pwm();
  init_serial();
  asm volatile (
    "clr r16\n\t"
    "clr r17\n\t"
    "clr r18\n\t"
    "rjmp 8f\n\t"
    "0:\n\t"
    "ldi r19, 25\n\t"
    "1:\n\t"
    "dec r19\n\t"
    "breq 4f\n\t"
    // Wait for high level
    "9:\n\t"
    "sbis 0x16, 2\n\t" // PINB(2)
    " rjmp 9b\n\t"
    "lsl r16\n\t"
    "rol r17\n\t"
    "rol r18\n\t"
    "nop\n\t"
    "nop\n\t"
    // Read bit value
    "sbic 0x16, 2\n\t" // PINB(2)
    " ori r16, 1\n\t"
    // Wait for low level
    "sbis 0x16, 2\n\t"
    " rjmp 1b\n\t"
    "sbis 0x16, 2\n\t"
    " rjmp 1b\n\t"
    "sbis 0x16, 2\n\t"
    " rjmp 1b\n\t"
    "sbis 0x16, 2\n\t"
    " rjmp 1b\n\t"
    "sbis 0x16, 2\n\t"
    " rjmp 1b\n\t"
    "sbis 0x16, 2\n\t"
    " rjmp 1b\n\t"
    "sbis 0x16, 2\n\t"
    " rjmp 1b\n\t"
    "sbis 0x16, 2\n\t"
    " rjmp 1b\n\t"
    "sbis 0x16, 2\n\t"
    " rjmp 1b\n\t"
    "sbis 0x16, 2\n\t"
    " rjmp 1b\n\t"
    "sbis 0x16, 2\n\t"
    " rjmp 1b\n\t"
    "sbis 0x16, 2\n\t"
    " rjmp 1b\n\t"
    // Ignore resets when data register not full
    "rjmp 0b\n\t"
    // Register full
    "4:\n\t"
    "cbi 0x18, 3\n\t" // PORTB(3)
    // Wait for high level
    "5:\n\t"
    "sbis 0x16, 2\n\t" // PINB(2)
    " rjmp 5b\n\t"
    "nop\n\t"
    "nop\n\t"
    "nop\n\t"
    "nop\n\t"
    "sbi 0x18, 3\n\t" // PORTB(3)
    "sbis 0x16, 2\n\t" // PINB(2)
    " rjmp 4b\n\t" // Short bit
    // Long bit
    "nop\n\t"
    "nop\n\t"
    "nop\n\t"
    "nop\n\t"
    "nop\n\t"
    "nop\n\t"
    "sbis 0x16, 2\n\t"
    " rjmp 4b\n\t"
    "sbis 0x16, 2\n\t"
    " rjmp 4b\n\t"
    "sbis 0x16, 2\n\t"
    " rjmp 4b\n\t"
    "sbis 0x16, 2\n\t"
    " rjmp 4b\n\t"
    "sbis 0x16, 2\n\t"
    " rjmp 4b\n\t"
    "sbis 0x16, 2\n\t"
    " rjmp 4b\n\t"
    "sbis 0x16, 2\n\t"
    " rjmp 4b\n\t"
    "sbis 0x16, 2\n\t"
    " rjmp 4b\n\t"
    "sbis 0x16, 2\n\t"
    " rjmp 4b\n\t"
    "sbis 0x16, 2\n\t"
    " rjmp 4b\n\t"
    // Reset/latch
    "out 0x29, r16\n\t" // OCR0A
    "out 0x2e, r17\n\t" // OCR1A
    "out 0x2b, r18\n\t" // OCR1B
    "cpi r16, 0\n\t"
    "breq 7f\n\t"
    // Enable PWM output
    "ldi r20, 0x83\n\t" // COM0A1 | WGM01 | WGM00
    "out 0x2a, r20\n\t" // TCCR0A
    "rjmp 8f\n\t"
    "7:\n\t"
    // Disable PWM output to avoid glitches
    "ldi r20, 0x03\n\t" // WGM01 | WGM00
    "out 0x2a, r20\n\t" // TCCR0A
    "8:\n\t" // Wait for low level
    "sbic 0x16, 2\n\t" // PINB(2)
    " rjmp 8b\n\t"
    "rjmp 0b\n\t"
    );
  return 0;
}
