/*
Bot'n Roll Rescue Module for CoSpace (www.botnroll.com)
 Bot'n Roll Rescue Module for CoSpace is an I2C device designed for Bot'n Roll ONE A robot and CoSpace Rescue.
 The module provides new RGB values, from both color sensors, every 30ms.
 The module provides new distance values in cm, from the 3 sonars, every 60ms.

Purpose:
 This example reads the RGB color sensors and sonars printing the values on Serial Monitor.
 - this example was developed using Arduino 1.0.6 open-source Arduino environment
 - this example is compatible with Bot'n Roll Rescue Module for CoSpace firmware version 1.0 or higher. Visit http://www.botnroll.com/onea/ to check if your version is updated.
 
 This example was created by José Cruz (www.botnroll.com)
 on 19 January 2015
 
 This code example is in the public domain. 
 http://www.botnroll.com
*/
 
#include <Wire.h>            // required by BnrRescue.cpp
#include <BnrRescue.h>       // Bot'n Roll CoSpace Rescue Module library

//constants definitions
#define MODULE_ADDRESS 0x2C  //default factory address

BnrRescue brm;               // declaration of object variable to control Bot'n Roll Rescue Module

byte firmware[2]={0,0};
byte sonarL=0, sonarC=0, sonarR=0;
byte rgbL[3]={0,0,0};
byte rgbR[3]={0,0,0};

void setup()
{
    Serial.begin(57600);              //Set baud rate to 57600 for printing values using XBee
    brm.i2cConnect(MODULE_ADDRESS);   //Enable I2C communication
    brm.setModuleAddress(0x2C);       //Change I2C Address
    brm.setSonarStatus(ENABLE);       //Enable/Disable Sonar scanning
    brm.setRgbStatus(ENABLE);         //Enable/Disable RGB scanning
    brm.readFirmware(&firmware[0],&firmware[1]);    //Read module firmware version
    Serial.print("Firmware Version: ");Serial.print(firmware[0],DEC); Serial.print("."); Serial.println(firmware[1],DEC);
}
void loop()
{
    brm.readRgbL(&rgbL[0],&rgbL[1],&rgbL[2]);      //Read Left RGB sensor
    Serial.print("RgbL: ");Serial.print(rgbL[0],DEC); Serial.print("  "); Serial.print(rgbL[1],DEC); Serial.print("  "); Serial.println(rgbL[2],DEC);    
    brm.readRgbR(&rgbR[0],&rgbR[1],&rgbR[2]);      //Read Right RGB sensor
    Serial.print("RgbR: ");Serial.print(rgbR[0],DEC); Serial.print("  "); Serial.print(rgbR[1],DEC); Serial.print("  "); Serial.println(rgbR[2],DEC);
    delay(30); //New RGB values every 30ms

    brm.readRgbL(&rgbL[0],&rgbL[1],&rgbL[2]);      //Read Left RGB sensor
    Serial.print("RgbL: ");Serial.print(rgbL[0],DEC); Serial.print("  "); Serial.print(rgbL[1],DEC); Serial.print("  "); Serial.println(rgbL[2],DEC);    
    brm.readRgbR(&rgbR[0],&rgbR[1],&rgbR[2]);      //Read Right RGB sensor
    Serial.print("RgbR: ");Serial.print(rgbR[0],DEC); Serial.print("  "); Serial.print(rgbR[1],DEC); Serial.print("  "); Serial.print(rgbR[2],DEC);
    brm.readSonars(&sonarL,&sonarC,&sonarR);       //Read the 3 sonars distance in cm
    Serial.print(" Sonars: ");Serial.print(sonarL,DEC); Serial.print("  "); Serial.print(sonarC,DEC); Serial.print("  "); Serial.println(sonarR,DEC);    
    delay(30); //New Sonar values every 60ms
}
