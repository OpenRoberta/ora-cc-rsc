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

#include "DcMotor.h"

// Pins: PWM   dir1   dir2
const uint8_t DcMotor::PINS[4][3] = {
    {     8,    10,    9},
    {    13,    11,   12},
    {     2,     4,    3},
    {     7,     5,    6},
};

DcMotor::DcMotor(uint8_t i2cAddress) : i2c(SDA, SCL) {
    this -> i2cAddress = (i2cAddress << 1);
}

void DcMotor::init() {
    writeI2C(PCA9685_REGISTER_MODE1, 0x00);
    writeI2C(PCA9685_REGISTER_MODE1, 0x10);
    writeI2C(PCA9685_REGISTER_PRESCALE, PRESCALE_VALUE);
    writeI2C(PCA9685_REGISTER_MODE1, 0x00);

    wait_ms(5);

    writeI2C(PCA9685_REGISTER_MODE1, 0xa1);

    allOff();
}

void DcMotor::allOff() {
	for (uint8_t i = 0; i < 16; i++) writeI2C(i, 0, 0, 0x10, 0);
}

void DcMotor::release() {
	allOff();
    writeI2C(PCA9685_REGISTER_MODE1, 0x08);
}

void DcMotor::setPercent(double motor, double speed) {
    if (speed > 0) {
    	setPercent(motor, Direction::Forward, abs(speed));

    } else if (speed < 0) {
    	setPercent(motor, Direction::Backward, abs(speed));

    } else {
    	setPercent(motor, Direction::Stop, 0);
    }
}

void DcMotor::setPercent(double motorDouble, Direction direction, double speed) {
	int8_t motorInt = motorDouble - 1;
	if (motorInt < 0 || motorInt > 3) return;
	Motor motor = static_cast<Motor>(motorInt);

	uint16_t rawSpeed = (direction == Direction::Stop) ? 0 : (fmin(fmax(speed, -100), 100) * 41);

	setRaw(motor, direction, rawSpeed);
}

void DcMotor::setRaw(Motor motor, Direction direction, uint16_t speed) {
    setSpeed(motor, speed);
    setDirection(motor, direction);
}

void DcMotor::setSpeed(Motor motor, uint16_t speed) {
    uint8_t pin = PINS[static_cast<int>(motor)][0];

    if (speed == 0)
        writeI2C(pin, 0, 0, 0x10, 0);                       // full OFF

    else if (speed > 4095)
        writeI2C(pin, 0x10, 0, 0, 0);                       // full ON

    else
        writeI2C(pin, 0, 0, speed >> 8, speed & 0xFF);      // pwm ON

}

void DcMotor::setDirection(Motor motor, Direction direction) {
    uint8_t pin1 = PINS[static_cast<int>(motor)][1];
    uint8_t pin2 = PINS[static_cast<int>(motor)][2];

    switch(direction) {
        case Direction::Forward:
            setPin(pin2, false);
            setPin(pin1, true);
            break;

        case Direction::Backward:
            setPin(pin1, false);
            setPin(pin2, true);
            break;

        case Direction::Stop:
            setPin(pin1, false);
            setPin(pin2, false);
            break;
    }
}

// internal sets
void DcMotor::setPin(uint8_t pin, bool value) {
    if (value)
        writeI2C(pin, 0x10, 0, 0, 0);                       // ON
    else
        writeI2C(pin, 0, 0, 0x10, 0);                       // OFF
}

// I2C read / write
void DcMotor::writeI2C(uint8_t reg, uint8_t value) {
    char buffer[2];
    buffer[0] = reg;
    buffer[1] = value;

    i2c.write(i2cAddress, buffer, 2);
}

void DcMotor::writeI2C(uint8_t basereg, uint8_t value1, uint8_t value2, uint8_t value3, uint8_t value4) {
    char buffer[5];
    buffer[0] = PCA9685_REGISTER_FIRST_LED + 4 * basereg;
    buffer[1] = value2;
    buffer[2] = value1;
    buffer[3] = value4;
    buffer[4] = value3;

    i2c.write(i2cAddress, buffer, 5);
}
