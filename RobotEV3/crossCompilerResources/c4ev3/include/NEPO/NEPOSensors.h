#ifndef NEPOSENSORS
#define NEPOSENSORS

#include "NEPOLists.h"
#include "ev3_sensors/ev3_sensors.private.h"

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
    ResetEV3GyroSensor(port, EV3GyroSoftwareOffset);
    LcdClean();
}

inline double colorComponentValueToPercentage (double value) {
    return ((value * 100.0) / 1023.0);
}

inline double colorComponentValueTo255 (double value) {
    return ((value * 255.0) / 1023.0);
}

/*
 * NEPOReadEV3ColorSensorColor, NEPOReadEV3ColorSensorReflectedLight and
 * NEPOReadEV3ColorSensorAmbientLight are wrapped so that we can set
 * EV3Color->currentSensorMode[port], which doesn't get set in c4ev3, but we
 * need it in readEV3ColorSensorAsString
 */

inline Color NEPOReadEV3ColorSensorColor (int port) {
    EV3Color->currentSensorMode[port] = EV3_COLOR_SENSOR_COLOR_MODE;
    return ReadEV3ColorSensorColor(port);
}

inline int NEPOReadEV3ColorSensorReflectedLight (int port) {
    EV3Color->currentSensorMode[port] = EV3_COLOR_SENSOR_REFLECT_MODE;
    return ReadEV3ColorSensorReflectedLight(port);
}

inline int NEPOReadEV3ColorSensorAmbientLight (int port) {
    EV3Color->currentSensorMode[port] = EV3_COLOR_SENSOR_AMBIENT_MODE;
    return ReadEV3ColorSensorAmbientLight(port);
}

inline std::list<double> NEPOReadEV3ColorSensorRGB (int port) {
    EV3Color->currentSensorMode[port] = EV3_COLOR_SENSOR_RAW_COLOR_MODE;
    RGB rgb;
    ReadEV3ColorSensorColorRGB(port, &rgb);
    std::list<double> values;
    _setListElementByIndex(values, 0, colorComponentValueTo255(rgb.red));
    _setListElementByIndex(values, 1, colorComponentValueTo255(rgb.green));
    _setListElementByIndex(values, 2, colorComponentValueTo255(rgb.blue));
    return values;
}

// TODO: Rename with NEPO prefix to follow convention
inline std::list<double> _ReadIRSeekAllChannels (int port) {
    EV3IrSeekResult res;
    ReadEV3IrSensorSeek(port, &res);
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
    RGBA rgba;
    ReadHTColorSensorV2RGBA(port, HTColorSensorDefaultMode, &rgba);
    std::list<double> values;
    _setListElementByIndex(values, 0, colorComponentValueTo255(rgba.red));
    _setListElementByIndex(values, 1, colorComponentValueTo255(rgba.green));
    _setListElementByIndex(values, 2, colorComponentValueTo255(rgba.blue));
    return values;
}

inline double NEPOReadHTColorSensorV2Light (int port) {
    RGBA rgba;
    ReadHTColorSensorV2RGBA(port, HTColorSensorDefaultMode, &rgba);
    return colorComponentValueToPercentage(rgba.white);
}

inline double NEPOReadHTColorSensorV2AmbientLight (int port) {
    RGBA rgba;
    ReadHTColorSensorV2RGBA(port, HTColorSensorPassiveMode, &rgba);
    double light = (rgba.white * 100.0) / 38200.0;
    return std::min(light, 100.0);
}

inline double NEPOReadEV3GyroAngle (int port) {
    return ReadEV3GyroSensorAngle(port, EV3GyroInterleavedAngle);
}

inline double NEPOReadEV3GyroRate (int port) {
    return ReadEV3GyroSensorRate(port, EV3GyroInterleavedRate);
}

unsigned long long NEPOTimers[TIMERS_COUNT];

inline void ResetTimer (int stopwatch) {
    NEPOTimers[stopwatch] = TimerGetMS();
}

inline unsigned long long GetTimerValue (int stopwatch) {
    return TimerGetMS() - NEPOTimers[stopwatch];
};

#endif
