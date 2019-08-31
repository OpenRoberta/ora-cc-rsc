/*  BSD-License:

Copyright (c) 2016 by Nils Springob, nicai-systems, Germany

All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.
  * Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.
  * Neither the name nicai-systems nor the names of its contributors may be
    used to endorse or promote products derived from this software without
    specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

/*! @file    analog.c
 *  @brief   Zuordnung der physikalischen Pins zu symbolischen Namen
 *  @author  Nils Springob (nils.springob@nicai-systems.com)
 *  @date    2015-02-25
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

#include "iodefs.h"
#include "analog.h"


#ifdef __cplusplus
extern "C" {
#endif

extern uint8_t _bob3_revision;
extern uint8_t _bob3_edition; // 0=THT 1=SMD
#define BOB3_EDITION_THT 0
#define BOB3_EDITION_SMD 1

extern uint8_t _arm_mode;

uint16_t analog_samples[2 * ANALOG_CNT];

uint16_t analog_random_seed;
//uint8_t analog_pos;
//uint8_t analog_flags;
volatile uint8_t analog_sample_id;
uint16_t analog_sum;
uint16_t analog_irq_temp;

// Interrupt
uint8_t analog_ui_ch;
uint8_t analog_ui_cnt;
uint8_t analog_first = 1;

#define AREF_1V1(x) (_BV(REFS1)|_BV(REFS0)|((x)))
#define AREF_AVCC(x) (_BV(REFS0)|((x)))
#define AREF_EXT(x)  ((x))
#define AMUX_TEMP 0x08
#define AMUX_1V1  0x0e
#define AMUX_0V   0x0f

void analog_init() {
  analog_first = 1;
  analog_ui_ch = 3;
#if defined(DIDR0)
//  DIDR0 = 0xff;
#endif

  ADCSRA = _BV(ADPS2)  // prescale faktor = 64 ADC laeuft
         //| _BV(ADPS1)  // mit 8 MHz / 64 = 125 kHz
	 | _BV(ADPS0)  // mit 8 MHz / 32 = 250 kHz
         | _BV(ADEN)   // ADC an
//         | _BV(ADATE)  // auto trigger
         | _BV(ADIE)   // enable interrupt
         | _BV(ADSC);  // Beginne mit der Konvertierung

  DDRC |= _BV(2) | _BV(3) | _BV(4) | _BV(5);
  DDRB |= _BV(0);

  PORTC |= _BV(0) | _BV(1); // PullUp!
  
  activate_output_bit(IO_ARM_1R);
  activate_output_bit(IO_ARM_1C);
  activate_output_bit(IO_ARM_2R);
  activate_output_bit(IO_ARM_2C);
  activate_output_bit(IO_EN_IR);
  
  if (_bob3_revision==103) {
    deactivate_output_bit(IO_HEAD_S);
  }
  
  if (SREG&_BV(7)) {
    // measure all once!
    analog_wait_update();
  } else {
    // precharge Key and Voltage input
    analog_samples[ANALOG_VOLT] = analog_samples[ANALOG_VOLT+ANALOG_CNT] = (ADC_BANDGAP_CHANNEL_VOLTAGE*1024)/5;
  }
}

uint8_t analog_enable(uint8_t enable) {
  uint8_t result = (ADCSRA & _BV(ADIE))?0xff:0x00;
  if (enable) {
    ADCSRA = _BV(ADPS2)  // prescale faktor = 64 ADC laeuft
           | _BV(ADPS0)  // mit 8 MHz / 32 = 250 kHz
           | _BV(ADEN)   // ADC an
           | _BV(ADIE)   // enable interrupt
           | _BV(ADSC);  // Beginne mit der Konvertierung
  } else {
    ADCSRA &= ~_BV(ADIE);
    ADCSRA &= ~_BV(ADEN);
  }
  return result;
}  

uint16_t analog_getValue(uint8_t idx) {
  //ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  uint16_t retval = 0;
  {
    uint8_t bak = SREG;
    cli();
    retval = analog_samples[idx];
    SREG = bak;
  }
  return retval;
}


uint16_t analog_getValueExt(uint8_t idx, uint8_t active) {
  if (active==0) {
    //ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    uint16_t retval;
    {
      uint8_t bak = SREG;
      cli();
      retval = analog_samples[idx];
      SREG = bak;
    }
    return retval;

  } else if (active==1) {
    idx += ANALOG_CNT;
    //ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    uint16_t retval;
    {
      uint8_t bak = SREG;
      cli();
      retval = analog_samples[idx];
      SREG = bak;
    }
    return retval;

  } else if (active==2) {
    uint8_t idx2 = idx+ANALOG_CNT;
    int16_t val = 0;
    //ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    if (idx==ANALOG_IR) {
      uint8_t bak = SREG;
      cli();
      val = analog_samples[idx2]; // ON
      uint16_t val2 = analog_samples[idx]; // OFF
      val -= val2;
      if (_bob3_edition==BOB3_EDITION_SMD) {
        val = (val>10)?(val-10):0;
        val /= 2U;
      }
      val2 /= 32U;
      val = (val>val2)?(val-val2):0;
      SREG = bak;
    } else if (idx<8) {
      // inverted signal: 1023 == no touch
      uint8_t bak = SREG;
      cli();
      val = analog_samples[idx];
      val -= analog_samples[idx2];
      SREG = bak;
    } else {
      uint8_t bak = SREG;
      cli();
      val = analog_samples[idx2];
      val -= analog_samples[idx];
      SREG = bak;
    }
    return (val<0)?0:val;
  }
  return 0;
}

uint16_t analog_getRandomSeed() {
  //random_r(unsigned long *  __ctx);
  //ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  uint16_t retval;
  {
    uint8_t bak = SREG;
    cli();
    retval = analog_random_seed;
    SREG = bak;
  }
  return retval;
}


void analog_wait_update() {
  uint8_t id = analog_sample_id;
  while (id == analog_sample_id) {
  }
}


uint8_t analog_has_update() {
  static uint8_t id = 0;
  if (id == analog_sample_id) {
    return 0;
  }
  id = analog_sample_id;
  return 1;
}

#define KEY_THRESHOLD 50
#define KEY_COUNT 5

static uint8_t updateKey (uint8_t state, uint8_t value) {
  if (value>KEY_THRESHOLD) {
    if (state&0x01) {
      return 1;
      //return (state>1)?(state-2):1;
    } else {
      return (state>=(2*KEY_COUNT-2))?1:(state+2);
    }
  } else {
    if (state&0x01) {
      return (state>(2*KEY_COUNT-2))?0:(state+2);
    } else {
      return 0;
      //return (state>1)?(state-2):0;
    }    
  }
}



void analog_irq_hook() __attribute__((weak));
void analog_irq_hook() {
}

void _key_update_state() __attribute__((weak));
void _key_update_state() {
}

void _key_update_event() __attribute__((weak));
void _key_update_event() {
}

#if F_CPU==8000000
#define STD0 0
#define STD2 2
#define STD3 3
#define STD4 4
#define STD5 5
#define STD6 6

#elif F_CPU==15000000
#define STD0 0
#define STD2 2
#define STD3 3
#define STD4 4
#define STD5 5
#define STD6 6

#elif F_CPU==20000000
#define STD0 0
#define STD2 3
#define STD3 4
#define STD4 5
#define STD5 7
#define STD6 8

#else
#error "unsupported clock speed!"
#endif


// **** IR Sensor ****
static inline void analog_ai_ir(uint16_t value) {
  if (analog_ui_cnt==0) {
    ADMUX = AREF_1V1(IOG_ANALOG_BIT_SENS_IR);
  } else if (analog_ui_cnt==6) {
    ADMUX = AREF_1V1(IOG_ANALOG_BIT_SENS_IR);
    PORTC &= ~_BV(IOG_ANALOG_BIT_SENS_IR);
    DDRC |= _BV(IOG_ANALOG_BIT_SENS_IR);
    DDRC &= ~_BV(IOG_ANALOG_BIT_SENS_IR);
    PORTC |= _BV(IOG_ANALOG_BIT_SENS_IR);
  } else if (analog_ui_cnt==8) {
    analog_sum = value;
  } else if (analog_ui_cnt==9) {
    analog_sum += value;
  } else if (analog_ui_cnt==10) {
    analog_sum += value;
  } else if (analog_ui_cnt==11) {
    analog_sum += value;
    analog_irq_temp = analog_sum/4;
    activate_output_bit(IO_EN_IR);
    set_output_bit(IO_EN_IR);
  } else if (analog_ui_cnt==14) {
    analog_sum = value;
  } else if (analog_ui_cnt==15) {
    analog_sum += value;
  } else if (analog_ui_cnt==16) {
    analog_sum += value;
  } else if (analog_ui_cnt==17) {
    analog_sum += value;
    analog_samples[ANALOG_IR+ANALOG_CNT] = analog_sum/4;
    analog_samples[ANALOG_IR] = analog_irq_temp;
    clear_output_bit(IO_EN_IR);
    analog_ui_cnt = 0;
    return;
  }
  analog_ui_cnt++;
}


// **** Temperature Sensor ****
static inline void analog_ai_temp(uint16_t value) {
  if (analog_ui_cnt==0) {
    ADMUX = AREF_1V1(AMUX_TEMP);
  } else if (analog_ui_cnt==4) {
    analog_sum = value;
  } else if (analog_ui_cnt==5) {
    analog_sum += value;
  } else if (analog_ui_cnt==6) {
    analog_sum += value;
  } else if (analog_ui_cnt==7) {
    analog_sum += value;
    value = analog_sum;
    analog_samples[ANALOG_TEMP] = value;
    value += 7*analog_samples[ANALOG_CNT+ANALOG_TEMP] + 4;
    value /= 8;
    analog_samples[ANALOG_CNT+ANALOG_TEMP] = value;
    analog_ui_cnt = 0;
    return;
  }
  analog_ui_cnt++;
}


// **** Supply Voltage Sensor ****
static inline void analog_ai_volt(uint16_t value) {
  if (analog_ui_cnt==0) {
    ADMUX = AREF_AVCC(AMUX_1V1);
  } else if (analog_ui_cnt==2) {
    analog_samples[ANALOG_VOLT] = value;
    analog_ui_cnt = 0;
    return;
  }
  analog_ui_cnt++;
}


      
#define CASE_ADC_KEY(CASE, ANALOG_CH, IO_EN_BIT, AREF, APORT) \
    case CASE+STD0: \
      ADMUX = AREF(APORT); \
      PORTC &= ~_BV(APORT);\
      DDRC |= _BV(APORT);\
      DDRC &= ~_BV(APORT);\
      PORTC |= _BV(APORT);\
      break; \
    case CASE+STD2: \
      analog_sum = value; \
      set_output_bit(IO_EN_BIT); \
      break; \
    case CASE+STD5: \
      analog_sum = (analog_sum>value)?(analog_sum-value):0; \
      analog_samples[ANALOG_CH+ANALOG_CNT]=analog_sum; \
      clear_output_bit(IO_EN_BIT); \
      analog_samples[ANALOG_CH]=updateKey(analog_samples[ANALOG_CH], analog_sum/4); \
      break;

   
// **** Supply Voltage Sensor ****
static inline void analog_ai_key_v102(uint16_t value) {
  switch (analog_ui_cnt) {
        CASE_ADC_KEY(0*STD6, ANALOG_L0,  IO_ARM_1R, AREF_AVCC, IOG_ANALOG_BIT_ARM_U)
        CASE_ADC_KEY(1*STD6, ANALOG_L1,  IO_ARM_1C, AREF_AVCC, IOG_ANALOG_BIT_ARM_U)
        CASE_ADC_KEY(2*STD6, ANALOG_L2,  IO_ARM_1C, AREF_AVCC, IOG_ANALOG_BIT_ARM_L)
        CASE_ADC_KEY(3*STD6, ANALOG_L3,  IO_ARM_1R, AREF_AVCC, IOG_ANALOG_BIT_ARM_L)
        CASE_ADC_KEY(4*STD6, ANALOG_R0,  IO_ARM_2R, AREF_AVCC, IOG_ANALOG_BIT_ARM_U)
        CASE_ADC_KEY(5*STD6, ANALOG_R1,  IO_ARM_2C, AREF_AVCC, IOG_ANALOG_BIT_ARM_U)
        CASE_ADC_KEY(6*STD6, ANALOG_R2,  IO_ARM_2C, AREF_AVCC, IOG_ANALOG_BIT_ARM_L)
        CASE_ADC_KEY(7*STD6, ANALOG_R3,  IO_ARM_2R, AREF_AVCC, IOG_ANALOG_BIT_ARM_L)
  }    
  analog_ui_cnt++;
  if (analog_ui_cnt >= 8*STD6) {
    analog_ui_cnt = 0;
  }
} 

// **** Supply Voltage Sensor ****
static inline void analog_ai_key_v103(uint16_t value) {
  switch (analog_ui_cnt) {
        CASE_ADC_KEY(0*STD6, ANALOG_L0,  IO_ARM2_TA, AREF_AVCC, IOG_ANALOG_BIT_ARM2_S1)
        CASE_ADC_KEY(1*STD6, ANALOG_L1,  IO_ARM2_TB, AREF_AVCC, IOG_ANALOG_BIT_ARM2_S1)
        CASE_ADC_KEY(2*STD6, ANALOG_L2,  IO_ARM2_TB, AREF_AVCC, IOG_ANALOG_BIT_ARM2_S1)
        CASE_ADC_KEY(3*STD6, ANALOG_L3,  IO_ARM2_TC, AREF_AVCC, IOG_ANALOG_BIT_ARM2_S1)
        CASE_ADC_KEY(4*STD6, ANALOG_R0,  IO_ARM2_TA, AREF_AVCC, IOG_ANALOG_BIT_ARM2_S2)
        CASE_ADC_KEY(5*STD6, ANALOG_R1,  IO_ARM2_TB, AREF_AVCC, IOG_ANALOG_BIT_ARM2_S2)
        CASE_ADC_KEY(6*STD6, ANALOG_R2,  IO_ARM2_TB, AREF_AVCC, IOG_ANALOG_BIT_ARM2_S2)
        CASE_ADC_KEY(7*STD6, ANALOG_R3,  IO_ARM2_TC, AREF_AVCC, IOG_ANALOG_BIT_ARM2_S2)
  }  
  analog_ui_cnt++;
  if (analog_ui_cnt >= 8*STD6) {
    analog_ui_cnt = 0;
  }
}      

// **** Supply Voltage Sensor ****
static inline void analog_ai_key_detect(uint16_t value) {
  switch (analog_ui_cnt) {
        case 0:
          // set all arm output pins to vcc level
          set_output_bit(IO_ARM2_TA);
          set_output_bit(IO_ARM2_TB);
          set_output_bit(IO_ARM2_TC);
          activate_output_bit(IO_ARM2_TA);
          activate_output_bit(IO_ARM2_TB);
          activate_output_bit(IO_ARM2_TC);
          // enable pull-ups on arm input pins
          deactivate_output_bit(IO_ARM2_S1);
          deactivate_output_bit(IO_ARM2_S2);
          set_output_bit(IO_ARM2_S1);
          set_output_bit(IO_ARM2_S2);
          ADMUX = AREF_AVCC(IOG_ANALOG_BIT_ARM2_S1);
          break;

        case 2*STD6:
          analog_sum = value;
          break;
        case 2*STD6+1:
          analog_sum += value;
          break;
        case 2*STD6+2:
          analog_sum += value;
          break;
        case 2*STD6+3:
          analog_sum += value;
          break;
        case 2*STD6+4:
          analog_sum /= 2;
          analog_sum += 15*analog_samples[ANALOG_L0]+7;
          analog_sum /= 16;
          break;
        case 2*STD6+5:
          analog_samples[ANALOG_L0] = analog_sum;
          ADMUX = AREF_AVCC(IOG_ANALOG_BIT_ARM2_S2);
          break;

        case 4*STD6:
          analog_sum = value;
          break;
        case 4*STD6+1:  
          analog_sum += value;
          break;
        case 4*STD6+2:
          analog_sum += value;
          break;
        case 4*STD6+3:
          analog_sum += value;
          break;
        case 4*STD6+4:
          analog_sum /= 2;
          analog_sum += 15*analog_samples[ANALOG_R0]+7;
          analog_sum /= 16;
          break;
        case 4*STD6+5:
          analog_samples[ANALOG_R0] = analog_sum;
          break;
  }  
  analog_ui_cnt++;
  if (analog_ui_cnt >= 8*STD6) {
    analog_ui_cnt = 0;
  }
}



ISR(ADC_vect) {
  uint16_t value = ADC;
  sei();
  switch (analog_ui_ch) {
    // Vcc Voltage Ref
    case 0: analog_ai_key_v102(value); break;
    case 1: analog_ai_key_v103(value); break;
    case 2: analog_ai_key_detect(value); break;
    
    // 1.1V Voltage Ref
    case 3: analog_ai_ir(value); break;
    case 4: analog_ai_temp(value); break;
    
    // Vcc Voltage Ref
    case 5: analog_ai_volt(value); break;
  }
  
  if (analog_ui_cnt==0) {
    if (analog_ui_ch<3) analog_ui_ch=3;
    else if (analog_ui_ch<5) analog_ui_ch++;
    else if (_bob3_revision==102) analog_ui_ch=0;
    else if (_arm_mode==1) analog_ui_ch=1;
    else if (_arm_mode==2) analog_ui_ch=2;
    else analog_ui_ch=0;
    
    if (analog_ui_ch<3) {
      analog_irq_hook();
      analog_sample_id++;
      analog_random_seed<<=1;
      if (analog_first) {
        analog_first = 0;
        analog_samples[ANALOG_TEMP+ANALOG_CNT] = analog_samples[ANALOG_TEMP];
      }
    }
  }
  
  analog_random_seed += value;
  ADCSRA |= _BV(ADSC);
}


#ifdef __cplusplus
} // extern "C"
#endif


