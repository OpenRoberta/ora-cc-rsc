#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include "clock.h"

volatile uint16_t clock_us; // [0..999]
volatile uint32_t clock_sys_ms;

#define CLOCK_US_INC 256

void _clock_handle_interrupt_isr() {
  clock_us += CLOCK_US_INC;
  if (clock_us>999) {clock_us-=1000; clock_sys_ms++;}
}


void clock_sleep_until_ms(uint32_t ms) {
  ms--;
  while(1) {
    cli();
    uint32_t dt = ms-clock_sys_ms;
    if (dt&0x80000000UL) break;
    sleep_enable();
    sei();
    sleep_cpu();
    sleep_disable();
  }
  sei();  
}


void clock_sleep_ms(uint32_t ms) {
  clock_sleep_until_ms(ms + clock_get_sys_ms());
}


uint16_t clock_get_us() {
  uint16_t res;
  {
    uint8_t bak = SREG;
    cli();
    res = clock_us;
    SREG = bak;
  }
  return res;
}


uint32_t clock_get_sys_ms() {
  uint32_t res;
  {
    uint8_t bak = SREG;
    cli();
    res = clock_sys_ms;
    SREG = bak;
  }
  return res;
}
    

    
#ifdef ARDUINO

unsigned long millis() { return clock_get_sys_ms(); }
unsigned long micros() { return clock_get_sys_us(); }
void delay(unsigned long ms) { clock_sleep_ms(ms); }
void _time_suspend() {}
void _time_resume() {}

#endif

