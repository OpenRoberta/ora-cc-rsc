/*
  BnrRescue.cpp - Library for interfacing with Bot'n Roll CoSpace Rescue Module
  Created by José Cruz, November 19, 2014.
  Released into the public domain.
*/

#include "Wire.h"
#include "BnrRescue.h"
///////////////////////////////////////////////////////////////////////
//Private Routines
///////////////////////////////////////////////////////////////////////
void BnrRescue::i2cConnect(byte moduleAddress)
{
    _moduleAddress = moduleAddress;
	Wire.begin();                           // join i2c bus (address optional for master)
}

void BnrRescue::i2cSendData(byte command, byte buffer[], byte numBytes)
{
    Wire.beginTransmission(_moduleAddress); // transmit to device #0x10
    Wire.write(command);                    // sends one byte
    for (int k=0;k<numBytes;k++)
    {
       Wire.write(buffer[k]);               // sends one byte
    }
    Wire.endTransmission();                 // stop transmitting
}

void BnrRescue::request2Bytes(byte command, byte *byte1,byte *byte2)
{
  byte data[2]={0,0};
  int i=0;
  Wire.beginTransmission(_moduleAddress);	// transmit to device
  Wire.write(command);                          // sends the command
  Wire.endTransmission();                       // stop transmitting
  delay(1);                                     // This delay is very important!
  Wire.requestFrom((int)_moduleAddress,(int)2); // requests 2 bytes
  while (Wire.available())
  {
    data[i]=Wire.read();
    i++;
  }
  *byte1=data[0];
  *byte2=data[1];
}

void BnrRescue::request3Bytes(byte command, byte *byte1,byte *byte2,byte *byte3)
{
  byte data[3]={0,0,0};
  int i=0;
  Wire.beginTransmission(_moduleAddress);	// transmit to device
  Wire.write(command);                          // sends the command
  Wire.endTransmission();                       // stop transmitting
  delay(1);                                     // This delay is very important!
  Wire.requestFrom((int)_moduleAddress,(int)3); // requests 3 bytes
  while (Wire.available())
  {
    data[i]=Wire.read();
    i++;
  }
  *byte1=data[0];
  *byte2=data[1];
  *byte3=data[2];
}

///////////////////////////////////////////////////////////////////////
//setup routines
///////////////////////////////////////////////////////////////////////
void BnrRescue::setModuleAddress(byte newAddress)
{
  byte buffer[]={newAddress, KEY1, newAddress, KEY2};
  i2cSendData(COMMAND_ADDRESS_CFG, buffer,sizeof(buffer));
  _moduleAddress=newAddress;
  delay(2);
}
void BnrRescue::setSonarStatus(byte status)
{
  byte buffer[]={status, KEY1, status, KEY2};
  i2cSendData(COMMAND_SONAR_CFG, buffer,sizeof(buffer));
  delay(2);
}
void BnrRescue::setRgbStatus(byte status)
{
  byte buffer[]={status, KEY1, status, KEY2};
  i2cSendData(COMMAND_RGB_CFG, buffer,sizeof(buffer));
  delay(2);
}

///////////////////////////////////////////////////////////////////////
//Data reading routines
///////////////////////////////////////////////////////////////////////
void BnrRescue::readFirmware(byte *firm1,byte *firm2)
{
    byte value[2]={0};
    request2Bytes(COMMAND_FIRMWARE, &value[0], &value[1]);
   *firm1=value[0];
   *firm2=value[1];
}
void BnrRescue::readSonars(byte *dist1, byte *dist2, byte *dist3)
{
    byte value[3]={0};
    request3Bytes(COMMAND_SONARS, &value[0], &value[1], &value[2]);
   *dist1=value[0];
   *dist2=value[1];
   *dist3=value[2];
}
void BnrRescue::readRgbL(byte *red, byte *green, byte *blue)
{
    byte value[3]={0};
    request3Bytes(COMMAND_RGBL, &value[0], &value[1], &value[2]);
   *red=value[0];
   *green=value[1];
   *blue=value[2];
}
void BnrRescue::readRgbR(byte *red, byte *green, byte *blue)
{
    byte value[3]={0};
    request3Bytes(COMMAND_RGBR, &value[0], &value[1], &value[2]);
   *red=value[0];
   *green=value[1];
   *blue=value[2];
}
