#ifndef NEPOSENSORS
#define NEPOSENSORS

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

inline void NEPOSetAllSensors(SensorHandler * port1, SensorHandler * port2, SensorHandler * port3, SensorHandler * port4){
    DrawString("Initializing sensors", 1, 3);
    SetAllSensors(port1, port2, port3, port4);
    LcdClean();
}

inline void NEPOResetEV3GyroSensor(int port) {
    LcdClean();
    DrawString("Resetting Gyro " + ToString(port + 1), 3, 3);
    ResetEV3GyroSensor(port);
    LcdClean();
}

inline double colorComponentValueToPercentage (double value) {
    return ((value * 100.0) / 1023.0);
}

inline std::list<double> NEPOReadEV3ColorSensorRGB (int port) {
    RGB rgb = ReadEV3ColorSensorRGB(port);
    std::list<double> values;
    _setListElementByIndex(values, 0, colorComponentValueToPercentage(rgb.red));
    _setListElementByIndex(values, 1, colorComponentValueToPercentage(rgb.green));
    _setListElementByIndex(values, 2, colorComponentValueToPercentage(rgb.blue));
    return values;
}

inline std::list<double> _ReadIRSeekAllChannels (int port) {
    EV3IrSeekResult res = ReadEV3IrSensorSeek(port);
    std::list<double> valuesInList;
    for (int i = 0; i < EV3_IR_CHANNELS; i++) {
        int direction = res.directions[i];
        int distance = res.distances[i];
        if (distance == 128) { // no beacon for this channel
            valuesInList.push_back(0);
            valuesInList.push_back(HUGE_VAL);
        } else {
            valuesInList.push_back(direction);
            valuesInList.push_back(distance);
        }
    }
    return valuesInList;
}

inline Color NEPOReadHTColorSensorV2 (int port) {
    int colorId = ReadHTColorSensorV2(port);
    return mapHTColorIdToColor(colorId);
}

inline std::list<double> NEPOReadHTColorSensorV2RGB (int port) {
    RGBA rgba = ReadHTColorSensorV2RGBA(port, HTColorSensorDefaultMode);
    std::list<double> values;
    _setListElementByIndex(values, 0, colorComponentValueToPercentage(rgba.red));
    _setListElementByIndex(values, 1, colorComponentValueToPercentage(rgba.green));
    _setListElementByIndex(values, 2, colorComponentValueToPercentage(rgba.blue));
    return values;
}

inline double NEPOReadHTColorSensorV2Light (int port) {
    RGBA rgba = ReadHTColorSensorV2RGBA(port, HTColorSensorDefaultMode);
    return colorComponentValueToPercentage(rgba.white);
}

inline double NEPOReadHTColorSensorV2AmbientLight (int port) {
    RGBA rgba = ReadHTColorSensorV2RGBA(port, HTColorSensorPassiveMode);
    double light = (rgba.white * 100.0) / 38200.0;
    return std::max(light, 100.0);
}


unsigned long long NEPOTimers[TIMERS_COUNT];

inline void ResetTimer (int stopwatch) {
    NEPOTimers[stopwatch] = TimerGetMS();
}

inline unsigned long long GetTimerValue (int stopwatch) {
    return TimerGetMS() - NEPOTimers[stopwatch];
};

#endif
