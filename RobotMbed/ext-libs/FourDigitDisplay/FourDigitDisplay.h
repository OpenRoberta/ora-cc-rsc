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

#ifndef FOUR_DIGIT_DISPLAY_H
#define FOUR_DIGIT_DISPLAY_H

#include "MicroBit.h"

// Based on the TM1637 chip
class FourDigitDisplay
{
  public:
    FourDigitDisplay(PinName, PinName);
    void digit(int address, int number);
    void show(int number, int fromPosition, bool showColon, bool inDecimal = true);
    void display();
    void clear();
    bool getColon();
    void setColon(bool showColon);
    int getBrightness();
    void setBrightness(int brightness);

  private:
    DigitalOut clkPin;
    DigitalOut dataPin;
    int brightness;
    bool showColon;
    int buffer[4];

    void writeByte(int wrData);
    void start();
    void stop();
    int coding(int digit);
};

#endif