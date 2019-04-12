/* mbed Microcontroller Library
 * Copyright (c) 2013 Nordic Semiconductor
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef MBED_PINNAMES_H
#define MBED_PINNAMES_H

#include "cmsis.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    PIN_INPUT,
    PIN_OUTPUT
} PinDirection;

#define PORT_SHIFT  3

typedef enum {
    p0  = 0,
    p1  = 1,
    p2  = 2,
    p3  = 3,
    p4  = 4,
    p5  = 5,
    p6  = 6,
    p7  = 7,
    p8  = 8,
    p9  = 9,
    p10 = 10,
    p11 = 11,
    p12 = 12,
    p13 = 13,
    p14 = 14,
    p15 = 15,
    p16 = 16,
    p17 = 17,
    p18 = 18,
    p19 = 19,
    p20 = 20,
    p21 = 21,
    p22 = 22,
    p23 = 23,
    p24 = 24,
    p25 = 25,
    p26 = 26,
    p27 = 27,
    p28 = 28,
    p29 = 29,
    p30 = 30,  
    
    //NORMAL PINS...
    P0_0  = p0,
    P0_1  = p1,
    P0_2  = p2,
    P0_3  = p3,
    P0_4  = p4,
    P0_5  = p5,
    P0_6  = p6,
    P0_7  = p7,
    
    P0_8  = p8,
    P0_9  = p9,
    P0_10 = p10,
    P0_11 = p11,
    P0_12 = p12,
    P0_13 = p13,
    P0_14 = p14,
    P0_15 = p15,
    
    P0_16 = p16,
    P0_17 = p17,
    P0_18 = p18,
    P0_19 = p19,
    P0_20 = p20,
    P0_21 = p21,
    P0_22 = p22,
    P0_23 = p23,
    
    P0_24 = p24,
    P0_25 = p25,
    P0_26 = p26,
    P0_27 = p27,
    P0_28 = p28,
    P0_29 = p29,
    P0_30 = p30,

    //PADS
    PAD1 = p0,
    PAD2 = p1,
    PAD3 = p2,
    PAD4 = p22,

    //LED MATRIX COLS
    COL1 = p4,
    COL2 = p5,
    COL3 = p6,
    COL4 = p7,
    COL5 = p8,
    COL6 = p9,
    COL7 = p10,
    COL8 = p11,
    COL9 = p12,

    //LED MATRIX ROWS
    ROW1 = p13,
    ROW2 = p14,
    ROW3 = p15,

    A_RX = p26,
    A_TX = p27,

    //BUTTON A and B
    BUTTON_A = p17,
    BUTTON_B = p16,

    //RGB LED (WS2812b)
    RGBLED = p18,

    //MOTOR DRIVER
    MOTOR_NSLEEP = p28,
    MOTOR_IN1 = p29,
    MOTOR_IN2 = p30,

    //MICROPHONE
    MIC = p3,

    //I2C
    SCL = p19,
    SDA = p20,

    MOSI = p9,
    MISO = p8,
    SCK = p7,

    // RX AND TX PINS
    TGT_TX = p24,
    TGT_RX = p25,

    //ACCEL INTERRUPT PIN (BMX055)
    ACCEL_INT = p21,


    // Not connected
    NC = (int)0xFFFFFFFF,

    RX_PIN_NUMBER = TGT_RX,
    TX_PIN_NUMBER = TGT_TX,
    CTS_PIN_NUMBER = 31, //unused  ** REQUIRES A PROPER FIX **
    RTS_PIN_NUMBER = 31, //unused 

    // mBed interface Pins
    USBTX = TX_PIN_NUMBER,
    USBRX = RX_PIN_NUMBER,

    LED1    = PAD1,
    LED2    = PAD2,
    LED3    = PAD3,
    LED4    = PAD4,

    //SDA (SERIAL DATA LINE)
    I2C_SDA0 = SDA,

    //SCL (SERIAL CLOCK LINE)
    I2C_SCL0 = SCL

} PinName;

typedef enum {
    PullNone = 0,
    PullDown = 1,
    PullUp = 3,
    PullDefault = PullUp
} PinMode;

#ifdef __cplusplus
}
#endif

#endif
