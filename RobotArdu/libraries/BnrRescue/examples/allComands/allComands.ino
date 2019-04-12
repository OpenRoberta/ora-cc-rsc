/*
Bot'n Roll Rescue Module for CoSpace (www.botnroll.com)
 Bot'n Roll Rescue Module for CoSpace is an I2C device designed for Bot'n Roll ONE A robot and CoSpace Rescue.
 The module provides new RGB values, from both color sensors, every 30ms.
 The module provides new distance values in cm, from the 3 sonars, every 60ms.

Purpose:
 This example lists all available routines to interface with Bot'n Roll Rescue Module for CoSpace.
 - this example was developed using Arduino 1.0.6 open-source Arduino environment
 - this example is compatible with Bot'n Roll Rescue Module for CoSpace firmware version 1.0 or higher. Visit http://www.botnroll.com/onea/ to check if your version is updated.
 - this example is not intended to be uploaded to Bot'n Roll ONE A or arduino board.

 This example was created by José Cruz (www.botnroll.com)
 on 19 January 2015
 
 Routines defined at BnrRescue.h:
        //setup routines
        void i2cConnect(byte moduleAddress);    //Enable I2C communication
        void setModuleAddress(byte newAddress); //Change I2C Address
        void setSonarStatus(byte status);       //Enable/Disable Sonar scanning
        void setRgbStatus(byte status);         //Enable/Disable RGB scanning

        //reading routines
        void readFirmware(byte*,byte*);         //Read module firmware version
        void readSonars(byte*,byte*,byte*);     //Read the 3 sonars distance in cm -> (new values every 60ms)
        void readRgbL(byte*,byte*,byte*);       //Read Left RGB sensor -> (new values every 30ms)
        void readRgbR(byte*,byte*,byte*);       //Read Right RGB sensor -> (new values every 30ms)
 
 This example was created by José Cruz (www.botnroll.com)
 on 19 January 2015
 
 This example is in the public domain.
 http://www.botnroll.com
*/
 
#include <Wire.h>        // required by BnrRescue.cpp
#include <BnrRescue.h>   // Bot'n Roll CoSpace Rescue Module library

//constants definitions
#define MODULE_ADDRESS 0x2C        //default factory address

BnrRescue brm;         // declaration of object variable to control Bot'n Roll Rescue Module

byte firmware[2]={0,0};
byte sonarL=0, sonarC=0, sonarR=0;
byte rgbL[3]={0,0,0};
byte rgbR[3]={0,0,0};

void setup()
{
   //setup routines
    brm.i2cConnect(MODULE_ADDRESS);    //Enable I2C communication
    brm.setModuleAddress(0x2C);        //Change I2C Address
    brm.setSonarStatus(ENABLE);   //Enable/Disable Sonar scanning
    brm.setRgbStatus(ENABLE);     //Enable/Disable RGB scanning
    
    //reading routines
    brm.readFirmware(&firmware[0],&firmware[1]);    //Read module firmware version
    brm.readSonars(&sonarL,&sonarC,&sonarR);       //Read the 3 sonars distance in cm
    brm.readRgbL(&rgbL[0],&rgbL[1],&rgbL[2]);      //Read Left RGB sensor
    brm.readRgbR(&rgbR[0],&rgbR[1],&rgbR[2]);      //Read Right RGB sensor
}
void loop()
{
}
