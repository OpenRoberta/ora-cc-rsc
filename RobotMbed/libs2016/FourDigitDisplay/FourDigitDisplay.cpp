//  Author:Frankie.Chu
//  Date:9 April,2012
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
//  Modified record:
//    Author: Patrick Werner <boon.werner@gmail.com>
//    Date: 20.04.2018
//    Changes: Compatability with Calliope mini
/*******************************************************************************/

#include "FourDigitDisplay.h"

static int DigitEncoding[] = {0x3f, 0x06, 0x5b, 0x4f,
                              0x66, 0x6d, 0x7d, 0x07,
                              0x7f, 0x6f, 0x77, 0x7c,
                              0x39, 0x5e, 0x79, 0x71}; //0~9,A,b,C,d,E,F

FourDigitDisplay::FourDigitDisplay(PinName clkPin, PinName dataPin) : clkPin(clkPin), dataPin(dataPin), brightness(2), showColon(true)
{
    this->clear();
}

void FourDigitDisplay::writeByte(int wrData)
{
    for (int i = 0; i < 8; ++i) //sent 8bit data
    {
        this->clkPin = 0;
        if (wrData & 0x01)
            this->dataPin = 1; //LSB first
        else
            this->dataPin = 0;
        wrData >>= 1;
        this->clkPin = 1;
    }
    this->clkPin = 0; //wait for the ACK
    this->dataPin = 1;
    this->clkPin = 1;
}

// Start to send signal to TM1637
void FourDigitDisplay::start()
{
    this->clkPin = 1;
    this->dataPin = 1;
    this->dataPin = 0;
    this->clkPin = 0;
}

// Terminate the signal transmission
void FourDigitDisplay::stop()
{
    this->clkPin = 0;
    this->dataPin = 0;
    this->clkPin = 1;
    this->dataPin = 1;
}

// Encode the colon into the digit
int FourDigitDisplay::coding(int digit)
{
    int colonData = (this->showColon) ? 0x80 : 0;
    return ((digit == 0x7f) ? 0x00 : DigitEncoding[digit]) + colonData;
}

// Display a number on the given digit address
void FourDigitDisplay::digit(int address, int number)
{
    number = this->coding(number);
    this->start();
    this->writeByte(0x44);
    this->stop();
    this->start();
    this->writeByte(address | 0xc0);
    this->writeByte(number);
    this->stop();
    this->start();
    this->writeByte(0x88 + this->brightness);
    this->stop();
}

// Clear the display
void FourDigitDisplay::clear()
{
    this->showColon = false;
    this->buffer[0] = 0x7f;
    this->buffer[1] = 0x7f;
    this->buffer[2] = 0x7f;
    this->buffer[3] = 0x7f;
    this->display();
}

bool FourDigitDisplay::getColon()
{
    return this->showColon;
}

void FourDigitDisplay::setColon(bool showColon)
{
    this->showColon = showColon;
}

int FourDigitDisplay::getBrightness()
{
    return this->brightness;
}

void FourDigitDisplay::setBrightness(int brightness)
{
    this->brightness = max(0, min(7, brightness));
}

void FourDigitDisplay::show(int number, int fromPosition, bool showColon, bool inDecimal)
{
    this->showColon = showColon;

    // Avoid illegal cases
    number = max(0, min(9999, number));
    fromPosition = max(0, min(3, fromPosition));

    if (number < 10)
    { // Single digit
        this->buffer[fromPosition] = number;
    }
    else
    { // Multiple digits
        int nDigits = 2; // Minimum 2 digits
        if (number >= 100)
        {
            nDigits = 3;
        }
        if (number >= 1000)
        {
            nDigits = 4;
        }

        // Start saving into buffer from the least significant digit
        int position = fromPosition + nDigits - 1;
        while (number > 0)
        {
            this->buffer[position--] = number % (inDecimal ? 10 : 16);
            number = number / (inDecimal ? 10 : 16);
        }
    }

    // Display the result
    this->display();
}

// Display the current buffer contents
void FourDigitDisplay::display()
{
    for (int i = 0; i < 4; ++i)
    {
        this->digit(i, this->buffer[i]);
    }
}