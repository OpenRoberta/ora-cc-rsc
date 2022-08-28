/*
 * DC Motor Lib
 *
 * Copyright (C) 2021-2022 Christian Poulter
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef DCMOTOR_H
#define DCMOTOR_H

#include "MicroBitI2C.h"

enum class Direction { Forward, Backward, Stop };
enum class Motor { M1 = 0, M2 = 1, M3 = 2, M4 = 3 };

class DcMotor {

    public:
    DcMotor(uint8_t i2cAddress);

    void init();
    void release();
    void setPercent(double motor, double speed);
    void setPercent(double motor, Direction direction, double speed);
    void setRaw(Motor motor, Direction direction, uint16_t speed);
    void setSpeed(Motor motor, uint16_t speed);
    void setDirection(Motor motor, Direction direction);

    private:
    I2C i2c;
    uint8_t i2cAddress;

    void allOff();
    void setPin(uint8_t pin, bool value);
    void writeI2C(uint8_t reg, uint8_t value);
    void writeI2C(uint8_t startreg, uint8_t value1, uint8_t value2, uint8_t value3, uint8_t value4);

    static const uint8_t PINS[4][3];

    static const uint8_t PCA9685_REGISTER_PRESCALE = 0xFE;
    static const uint8_t PCA9685_REGISTER_MODE1 = 0x0;
    static const uint8_t PCA9685_REGISTER_FIRST_LED = 0x6;

    static const uint8_t PRESCALE_VALUE = 1;

    static const PinName SDA = I2C_SDA0;
    static const PinName SCL = I2C_SCL0;

    // Pre scale calculation: round(25MHz/(4096*freq)) - 1
    // good values for freq could be 1.2-5.0 kHz

};

#endif
