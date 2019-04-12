/***************************************************
  This is a library for the SHT31 Digital Humidity & Temp Sht31

  Designed specifically to work with the SHT31 Digital Sht31 from Adafruit
  ----> https://www.adafruit.com/products/2857

  These displays use I2C to communicate, 2 pins are required to
  interface
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include "Sht31.h"
#include "mbed.h"

Sht31::Sht31(PinName sda, PinName scl) : _i2c(sda, scl) {
    _i2caddr = (0x44 << 1);
    reset();
    readStatus();
}

float Sht31::readTemperature(void) {
    if (! readTempHum()) return NAN;
    return temp;
}

float Sht31::readHumidity(void) {
    if (! readTempHum()) return NAN;
    return humidity;
}

void Sht31::reset(void) {
    writeCommand(SHT31_SOFTRESET);
    wait_ms(10);
}

uint16_t Sht31::readStatus(void) {
    writeCommand(SHT31_READSTATUS);
    char val[1];
    _i2c.read(_i2caddr, val, 1);
    uint16_t stat = val[0];
    stat <<= 8;
    _i2c.read(_i2caddr, val, 1);
    stat |= val[0];
    // printf("0x%X\r\n", stat);
    return stat;
}

void Sht31::writeCommand(uint16_t cmd) {
    char buf[2];
    buf[0] = (cmd >> 8);
    buf[1] = (cmd & 0xFF);
    _i2c.write(_i2caddr, buf, 2);
}

bool Sht31::readTempHum(void) {
    char readbuffer[6];

    writeCommand(SHT31_MEAS_HIGHREP);

    wait_ms(500);
    _i2c.read(_i2caddr, readbuffer, 6);
    
    uint16_t ST, SRH;
    ST = readbuffer[0];
    ST <<= 8;
    ST |= readbuffer[1];

    if (readbuffer[2] != crc8((uint8_t *) readbuffer, 2)) {
        return false;
    }

    SRH = readbuffer[3];
    SRH <<= 8;
    SRH |= readbuffer[4];

    if (readbuffer[5] != crc8((uint8_t *) readbuffer+3, 2)) {
        return false;
    }

    double stemp = ST;
    stemp *= 175;
    stemp /= 0xffff;
    stemp = -45 + stemp;
    temp = stemp;

    double shum = SRH;
    shum *= 100;
    shum /= 0xFFFF;

    humidity = shum;

    return true;
}

uint8_t Sht31::crc8(const uint8_t *data, int len) {
    const uint8_t POLYNOMIAL(0x31);
    uint8_t crc(0xFF);

    for ( int j = len; j; --j ) {
        crc ^= *data++;

        for ( int i = 8; i; --i ) {
            crc = (crc & 0x80) ? (crc << 1) ^ POLYNOMIAL : (crc << 1);
        }
    }
    return crc;
}
