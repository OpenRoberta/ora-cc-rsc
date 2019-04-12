/*
  BnrRescue.h - Library for interfacing with Bot'n Roll Rescue Module for CoSpace
  Created by José Cruz, November 19, 2014.
  Released into the public domain.
*/

#ifndef BnrRescue_h
#define BnrRescue_h

#include "Arduino.h"

#define KEY1 0xAA // key used in critical commands
#define KEY2 0x55 // key used in critical commands
#define OFF 0
#define ON  1
#define DISABLE 0
#define ENABLE  1

 /*User Commands*/
#define COMMAND_FIRMWARE	0xFE //Read firmware version
#define COMMAND_RGBL		0xCF //Read RGB sensor1
#define COMMAND_RGBR            0xCE //Read RGB sensor2
#define COMMAND_SONARS		0xCC //Read sonar sensors
#define COMMAND_ADDRESS_CFG     0xF0 //Change I2C slave address
#define COMMAND_SONAR_CFG       0xEF //Turn ON/OFF sonars
#define COMMAND_RGB_CFG         0xEE //Turn ON/OFF RGB color sensors

class BnrRescue
{
    public:
        //setup routines
        void i2cConnect(byte moduleAddress);    //Enable I2C communication
        void setModuleAddress(byte newAddress); //Change I2C Address
        void setSonarStatus(byte status);   //Enable/Disable Sonar scanning
        void setRgbStatus(byte status);     //Enable/Disable RGB scanning

        //reading routines
        void readFirmware(byte*,byte*);
        void readSonars(byte*,byte*,byte*);
        void readRgbL(byte*,byte*,byte*);
        void readRgbR(byte*,byte*,byte*);

    private:
        void i2cSendData(byte command, byte buffer[], byte numBytes);
        void request2Bytes(byte,byte*,byte*);
        void request3Bytes(byte,byte*,byte*,byte*);

        byte _moduleAddress;
};

#endif


