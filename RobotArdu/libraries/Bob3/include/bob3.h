#ifndef _BOB3_H_
#define _BOB3_H_

#include <avr/io.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include <utils.h>
#include <stdlib.h>

#ifndef _CLAMP
#define _CLAMP(x, lower, upper) (( (x) > (upper) ) ? (upper) : ( (x) < (lower) ? (lower) : (x) ))
#endif

inline unsigned RGB(unsigned r, unsigned g, unsigned b) {
    return ((r & 0xF0) << 4) | (g & 0xF0) | ((b & 0xF0) >> 4);
}

inline double absD(double d) {
    return d < 0.0 ? -d : d;
}

inline bool isWholeD(double d) {
    return d == floor(d);
}

inline bool isPrimeD(double d) {
    if (!isWholeD(d)) {
        return false;
    }
    int n = (int) d;
    if (n < 2) {
        return false;
    }
    if (n == 2) {
        return true;
    }
    if (n % 2 == 0) {
        return false;
    }
    for (int i = 3, s = (int) (sqrt(d) + 1); i <= s; i += 2) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}

inline int _randomIntegerInRange(int val1, int val2) {
    int min = fmin(val1, val2);
    int max = fmax(val1, val2) + 1;
    return min + (rand() % (min - max));
}

inline float _randomFloat() {
    return (float) rand() / (float) RAND_MAX;
}

void setup();
void loop();

enum {
  ARM_NONE,
  ARM_TOP,
  ARM_MID_TOP,
  ARM_MID_BOT,
  ARM_BOT
};

enum {
  ARMS_OFF      = 0,
  ARMS_TOUCH    = 1,
  ARMS_DETECTOR = 2
};


enum {
  OFF            = 0x000,
  WHITE          = 0xfff,
  ON             = 0xfff,
  RED            = 0xf00,
  GREEN          = 0x0f0,
  BLUE           = 0x00f,
  YELLOW         = 0xff0,
  CYAN           = 0x0ff,
  FUCHSIA        = 0xf0f,
  
  ORANGE         = 0xf80,
  KABARED        = 0xd42,
  PURPLE         = 0xf08,
  VIOLET         = 0x63a,
  AQUAMARINE     = 0x7fd,
  BROWN          = 0xa33,
  CORAL          = 0xf75,
  CORNFLOWERBLUE = 0x69e,
  STEELBLUE      = 0x48a,
  ROYALBLUE      = 0x46e,
  FORESTGREEN    = 0x282,
  SEAGREEN       = 0x5f9,
  COLERED        = 0x100,
  UNICORN        = 0x609
};


enum {
  ARM_1  = 1,
  ARM_2  = 2,
  EYE_1  = 1,
  EYE_2  = 2,
  LED_3 = 3,
  LED_4 = 4
};

// linker optional:
int16_t _Bob3_receiveMessage(uint8_t carrier, uint16_t timeout);
void _Bob3_transmitMessage(uint8_t carrier, uint8_t code);


class Bob3 {
  public:
    static void init();
    static void setLed(uint8_t id, uint16_t color);
    static uint16_t getLed(uint8_t id);
    static inline void setEyes(uint16_t eye1, uint16_t eye2) {setLed(1, eye1); setLed(2, eye2);}
    static inline void setWhiteLeds(uint16_t wled1, uint16_t wled2) {setLed(3, wled1); setLed(4, wled2);}
    static inline void setLeds(uint16_t eye1, uint16_t eye2, uint16_t wled1, uint16_t wled2) {setLed(1, eye1); setLed(2, eye2); setLed(3, wled1); setLed(4, wled2);}
    static uint16_t getIRSensor();
    static uint16_t getIRLight();
    static void enableIRSensor(bool enable);
    static uint8_t getArm(uint8_t id);
    static void enableArms(uint8_t enable);
    static uint16_t getTemperature();
    static uint16_t getMillivolt();
    static uint8_t getID();
    static inline int16_t receiveMessage(uint8_t carrier, uint16_t timeout) {return _Bob3_receiveMessage(carrier, timeout);}
    static inline void transmitMessage(uint8_t carrier, uint8_t message) {_Bob3_transmitMessage(carrier, message);}
    static inline int16_t receiveMessage(uint16_t timeout) {return _Bob3_receiveMessage(0, timeout);}
    static inline void transmitMessage(uint8_t message) {_Bob3_transmitMessage(0, message);}

    // compatibility:
    static inline int16_t receiveIRCode(uint8_t carrier, uint16_t timeout) {return _Bob3_receiveMessage(carrier, timeout);}
    static inline void transmitIRCode(uint8_t carrier, uint8_t message) {_Bob3_transmitMessage(carrier, message);}
    static inline int16_t receiveIRCode(uint16_t timeout) {return _Bob3_receiveMessage(0, timeout);}
    static inline void transmitIRCode(uint8_t message) {_Bob3_transmitMessage(0, message);}
    
};



extern Bob3 bob3;

//#ifndef ARDUINO
inline static 
void delay(uint16_t ms) {while (ms--) _delay_ms(1);}
//#endif

inline static 
void delay32(uint32_t ms) {while (ms--) _delay_ms(1);}

inline static uint16_t rgb(uint8_t r, uint8_t g, uint8_t b) __attribute__((const));

inline static 
uint16_t rgb(uint8_t r, uint8_t g, uint8_t b) {return ((uint16_t)r<<8)|(g<<4)|b;}

uint16_t mixColor(uint16_t color1, uint16_t color2, uint8_t w1, uint8_t w2);

uint16_t hsv(int16_t h, uint8_t s, uint8_t v);

uint16_t hsvx(uint8_t h, uint8_t s, uint8_t v);

uint16_t randomNumber(uint16_t lo, uint16_t hi);

uint16_t randomBits(uint8_t zeros, uint8_t ones);

uint32_t random32();


static inline 
uint8_t random8() {
  return random32() & 0xff;
}


static inline 
uint16_t random16() {
  return random32() & 0xffff;
}

static inline 
void remember(int16_t value) {eeprom_update_word ((uint16_t *)(E2END-1), ~value);}

static inline 
int16_t recall() {return ~eeprom_read_word ((const uint16_t *)(E2END-1));}

#endif
