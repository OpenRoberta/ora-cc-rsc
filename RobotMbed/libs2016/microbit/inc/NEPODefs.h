//NEPO definitions for calliope
// Display brightness is in interval of 0..255
// To scale that to 0..9 we use the following multipliers:
#define _SET_BRIGHTNESS_MULTIPLIER 28.34
#define _GET_BRIGHTNESS_MULTIPLIER 0.0353
// Light sensor returns values in interval 0..255
// To scale them to 0..100 we use this multiplier:
#define _GET_LIGHTLEVEL_MULTIPLIER 0.3922
#define _ITERATION_SLEEP_TIMEOUT 1

// math constants used in math blocks. Avoids inclusion of <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef M_E
#define M_E 2.718281828459045
#endif

#ifndef M_GOLDEN_RATIO
#define M_GOLDEN_RATIO 1.61803398875
#endif

#ifndef M_SQRT2
#define M_SQRT2 1.41421356237
#endif

#ifndef M_SQRT1_2
#define M_SQRT1_2 0.707106781187
#endif

#ifndef M_INFINITY
#define M_INFINITY 0x7f800000
#endif

#ifndef _CB_20_ADDR
#define _CB_20_ADDR (0x20 << 1)
#endif

#ifndef _CB_21_ADDR
#define _CB_21_ADDR (0x21 << 1)
#endif

#define assertNepo(test, msg, left, op, right) ({ \
    if (test == false) { \
        ManagedString _assertMsg = ManagedString("Assertion failed: ") + ManagedString(msg) \
        + ManagedString(" ") + ManagedString(left) + ManagedString(" ") + ManagedString(op) \
        + ManagedString(" ") + ManagedString(right) + ManagedString("\r\n"); \
        _uBit.serial.setTxBufferSize(_assertMsg.length()); \
        _uBit.serial.send(_assertMsg); \
        _uBit.sleep(_ITERATION_SLEEP_TIMEOUT); \
    } \
})

#include <list>
#include <array>
#include <limits.h>

template<typename T>
T _getListElementByIndex(std::list<T> &list, unsigned index) {
    auto iterator = list.begin();
    advance(iterator, index);
    return (*iterator);
}

template<typename T>
T _getAndRemoveListElementByIndex(std::list<T> &list, unsigned index) {
    auto iterator = list.begin();
    advance(iterator, index);
    T value = (*iterator);
    list.erase(iterator);
    return value;
}

template<typename T>
void _removeListElementByIndex(std::list<T> &list, unsigned index) {
    _getAndRemoveListElementByIndex(list, index);
}

/*
 * The only known situation where the cast of P to T would be needed is for int to double
 * in other cases T and P will be the same type. If only one template parameter is used
 * then the match void setListElementByIndex(std::list<double>, int, int) would not be possible
 */

template<typename T>
std::list<T> &_createListRepeat(unsigned count, T e) {
    std::list<T> &l = *(new std::list<T>);
    for (unsigned i = 0; i < count; i++) {
        l.push_back(e);
    }
    return l;
}

template<typename T, typename P>
void _setListElementByIndex(std::list<T> &list, unsigned index, P value) {
    if (index < list.size()) {
        auto iterator = list.begin();
        advance(iterator, index);
        (*iterator) = (T) (value);
    } else {
        list.push_back((T) (value));
    }
}

template<typename T, typename P>
void _insertListElementBeforeIndex(std::list<T> &list, unsigned index,
        P value) {
    auto iterator = list.begin();
    advance(iterator, index);
    list.insert(iterator, (T) (value));
}

template<typename T, typename P>
int _getFirstOccuranceOfElement(std::list<T> &list, P value) {
    int i = 0;
    auto iterator = list.begin();
    for (i = 0, iterator = list.begin(); iterator != list.end();
            ++iterator, ++i) {
        if ((P) (*iterator) == value) {
            return i;
        }
    }
    return -1;
}

template<typename T, typename P>
int _getLastOccuranceOfElement(std::list<T> &list, P value) {
    int i = 0;
    auto iterator = list.rbegin();
    for (i = 0, iterator = list.rbegin(); iterator != list.rend();
            ++iterator, ++i) {
        if ((P) (*iterator) == value) {
            return list.size() - i - 1;
        }
    }
    return -1;
}

template<typename T>
std::list<T> &_getSubList(std::list<T> &list, int startIndex, int endIndex) {
    auto beginIterator = list.begin();
    advance(beginIterator, startIndex);
    auto endIterator = list.begin();
    advance(endIterator, endIndex + 1);
    return *(new std::list<T>(beginIterator, endIterator));
}

double _getListSum(std::list<double> &list) {
    double result = 0;
    for (auto iterator = list.begin(); iterator != list.end(); ++iterator) {
        result += *iterator;
    }
    return result;
}

double _getListMin(std::list<double> &list) {
    double result = *(list.begin());
    for (auto iterator = list.begin(); iterator != list.end(); ++iterator) {
        if (result > *iterator) {
            result = *iterator;
        }
    }
    return result;
}

double _getListMax(std::list<double> &list) {
    double result = *(list.begin());
    for (auto iterator = list.begin(); iterator != list.end(); ++iterator) {
        if (result < *iterator) {
            result = *iterator;
        }
    }
    return result;
}

double _getListMedian(std::list<double> &list) {
    std::list<double> sorted(list);
    sorted.sort();
    auto iterator = sorted.begin();
    if (sorted.size() % 2 == 0) {
        for (uint16_t i = 0; i < sorted.size() / 2; i++) {
            iterator++;
        }
        return (*iterator + *--iterator) / 2;
    } else {
        for (uint16_t i = 0; i < sorted.size() / 2; i++) {
            iterator++;
        }
        return *iterator;
    }
}

double _getListAverage(std::list<double> &list) {
    double sum = _getListSum(list);
    return sum / list.size();
}

double _getListStandardDeviation(std::list<double> &list) {
    double mean = _getListSum(list) / list.size();
    double sum = 0;
    for (auto iterator = list.begin(); iterator != list.end(); ++iterator) {
        sum += (*iterator - mean) * (*iterator - mean);
    }
    sum /= list.size() - 1;
    return sqrt(sum);
}

template<typename T, long unsigned S>
std::array<T, S> _convertToArray(std::list<T> &list) {
    auto iterator = list.begin();
    std::array<T, S> result;
    int i = 0;
    for (auto iterator = list.begin(), i = 0; iterator != list.end();
            ++iterator, ++i) {
        result[i] = *iterator;
    }
    return result;
}

template<typename T, long unsigned S>
std::array<T, S> _convertToArray(std::initializer_list<T> list) {
    auto iterator = list.begin();
    std::array<T, S> result;
    int i = 0;
    for (auto iterator = list.begin(), i = 0; iterator != list.end();
            ++iterator, ++i) {
        result[i] = *iterator;
    }
    return result;
}

void _cbStop(char buf[5], MicroBitI2C *i2c) {
    for (uint8_t i = 0; i < 5; i++) {
        buf[i] = 0;
    }
    // stop both motors
    i2c->write(_CB_20_ADDR, buf, 5);
}

void _cbInit(char buf[5], MicroBitI2C *i2c, MicroBit *uBit) {
    for (uint8_t i = 0; i < 5; i++) {
        buf[i] = 0;
    }
    // stop both motors
    _cbStop(buf, i2c);
    // turn of both front leds
    i2c->write(_CB_21_ADDR, buf, 2);
    // turn of all rgb leds
    buf[0] = 0x01;
    i2c->write(_CB_21_ADDR, buf, 5);
    uBit->sleep(_ITERATION_SLEEP_TIMEOUT);
}

int _getDistance(MicroBitColor current, MicroBitColor match) {
    uint8_t redDist;
    uint8_t greenDist;
    uint8_t blueDist;

    redDist = current.getRed() - match.getRed();
    greenDist = current.getGreen() - match.getGreen();
    blueDist = current.getBlue() - match.getBlue();

    return (redDist * redDist) + (greenDist * greenDist) + (blueDist * blueDist);
}

uint8_t _findNearestColor(MicroBitColor current) {
    MicroBitColor map[] = { MicroBitColor(0, 0, 0, 255), MicroBitColor(0, 153,
            0, 255), MicroBitColor(255, 0, 0, 255), MicroBitColor(255, 255, 102,
            255), MicroBitColor(51, 102, 255, 255), MicroBitColor(0, 204, 204,
            255), MicroBitColor(102, 0, 204, 255), MicroBitColor(255, 255, 255,
            255) };
    int shortestDistance = INT_MAX;
    uint8_t index = -1;

    for (uint8_t i = 0; i < 8; i++) {
        MicroBitColor match;
        int distance;

        match = map[i];
        distance = _getDistance(current, match);

        if (distance < shortestDistance) {
            index = i;
            shortestDistance = distance;
        }
    }
    return index;
}
void _cbSetRGBLed(char buf[5], MicroBitI2C *i2c, uint8_t port,
        uint8_t colorIndex) {
    uint8_t length = 2;
    uint8_t color = 0x80 | colorIndex;
    bool allLeds = false;
    if (port == 5) { // all leds
        length = 5;
        port = 1;
        buf[2] = color;
        buf[3] = color;
        buf[4] = color;
    }
    buf[0] = port;
    buf[1] = color;
    i2c->write(_CB_21_ADDR, buf, length);
}

void _cbSetRGBLed(char buf[5], MicroBitI2C *i2c, uint8_t port,
        MicroBitColor color) {
    _cbSetRGBLed(buf, i2c, port, _findNearestColor(color));
}

void _cbSetLed(char buf[5], MicroBitI2C *i2c, uint8_t &ledState, uint8_t port,
        bool on) {
    buf[0] = 0;
    if (on) {
        ledState |= port;
    } else {
        ledState &= (0xFF - port);
    }
    buf[1] = ledState;
    i2c->write(_CB_21_ADDR, buf, 2);
}

void _cbSetMotors(char buf[5], MicroBitI2C *i2c, int speedLeft,
        int speedRight) {
    buf[0] = 0;
    if (speedLeft < 0) {
        speedLeft *= -1;
        buf[1] = 0x01;
    } else {
        buf[1] = 0x00;
    }
    if (speedLeft > 100) {
        speedLeft = 100;
    }
    buf[2] = speedLeft * 2.55;
    if (speedRight < 0) {
        speedRight *= -1;
        buf[3] = 0x01;
    } else {
        buf[3] = 0x00;
    }
    if (speedRight > 100) {
        speedRight = 100;
    }
    buf[4] = speedRight * 2.55;
    i2c->write(_CB_20_ADDR, buf, 5);
}

void _cbSetMotor(char buf[5], MicroBitI2C *i2c, uint8_t motor, int speed) {
    buf[0] = motor;
    buf[1] = 0;
    if (speed < 0) {
        speed *= -1;
        buf[1] = 0x01;
    }
    if (speed > 100) {
        speed = 100;
    }
    buf[2] = speed * 2.55;
    i2c->write(_CB_20_ADDR, buf, 3);
}

uint16_t _cbGetSampleUltrasonic(char buf[5], MicroBitI2C *i2c) {
    i2c->read(_CB_21_ADDR, buf, 3);
    return (((buf[1] * 256) + buf[2]) * 0.1);
}

bool _cbGetSampleInfrared(char buf[5], MicroBitI2C *i2c, uint8_t sensor) {
    i2c->read(_CB_21_ADDR, buf, 1);
    return ((buf[0] &= sensor) == 0 ? true : false);
}

ManagedString _castColorToString(MicroBitColor color) {
    return (ManagedString("RGBW(") + color.getRed() + ManagedString(", ")
            + color.getGreen() + ManagedString(", ") + color.getBlue()
            + ManagedString(", ") + color.getWhite() + ManagedString(")"));
}

void _cbSetServo(char buf[5], MicroBitI2C *i2c, char servo, int position) {
    buf[0] = servo;
    buf[1] = min(max(position, 0), 180);
    i2c->write(_CB_20_ADDR, buf, 2);
}
