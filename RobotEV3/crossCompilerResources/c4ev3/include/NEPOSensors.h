#ifndef NEPOSENSORS
#define NEPOSENSORS

#include "ev3_timer.h"
#include "NEPOLists.h"

#define DEFAULT_MODE_COLOR      COL_COLOR
#define DEFAULT_MODE_GYRO       GYRO_ANG
#define DEFAULT_MODE_INFRARED   IR_PROX
#define DEFAULT_MODE_TOUCH      TOUCH_PRESS
#define DEFAULT_MODE_ULTRASONIC US_DIST_CM
#define DEFAULT_MODE_SOUND      NXT_SOUND_DB
#define DEFAULT_MODE_COMPASS    NXT_COMPASS_COMPASS
#define DEFAULT_MODE_IRSEEKER   NXT_IR_SEEKER_DC

#define TIMERS_COUNT 5

inline void switchToMode(int port, int mode) {
    if (GetSensorName(port) != mode) {
        SetSensorMode(port, mode);
    }
}

inline int ReadSensorInMode (int port, int mode) {
    switchToMode(port, mode);
    return readSensor(port);
}

inline std::list<double> ReadColorSensorRGB (int port) {
    int rgb = ReadSensorInMode(port, COL_RGB);
    std::list<double> values;
    _setListElementByIndex(values, 0, GetRFromRGB(rgb));
    _setListElementByIndex(values, 1, GetGFromRGB(rgb));
    _setListElementByIndex(values, 2, GetBFromRGB(rgb));
    return values;
}

inline Color ReadColorSensor (int port) {
    int c4ev3 = ReadSensorInMode(port, COL_COLOR);
    return GetColorFromC4ev4Int(c4ev3);
}


inline std::list<double> _ReadIRSeekAllChannels (int port) {
    switchToMode(port, IR_SEEK);
    std::list<double> valuesInList;
    int * values = ReadIRSeekAllChannels(port);
    for (int i = 0; i < IR_CHANNELS; i++) {
        int measurement = values[i * 2];
        int raw = values[(i * 2) + 1];
        _setListElementByIndex(valuesInList, i * 2, measurement == -1 ? HUGE_VAL : measurement);
        _setListElementByIndex(valuesInList, (i * 2) + 1, raw);
    }
    return valuesInList;
}



unsigned long long NEPOTimers[TIMERS_COUNT];


inline void ResetTimer (int stopwatch) {
    NEPOTimers[stopwatch] = TimerGetMS();
}

inline unsigned long long GetTimerValue (int stopwatch) {
    return TimerGetMS() - NEPOTimers[stopwatch];
};

#endif
