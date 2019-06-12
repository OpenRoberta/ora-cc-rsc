#ifndef NEPOSENSORS
#define NEPOSENSORS

#define DEFAULT_MODE_COLOR      COL_COLOR
#define DEFAULT_MODE_GYRO       GYRO_ANG
#define DEFAULT_MODE_INFRARED   IR_PROX
#define DEFAULT_MODE_TOUCH      TOUCH_PRESS
#define DEFAULT_MODE_ULTRASONIC US_DIST_CM
#define DEFAULT_MODE_SOUND      NXT_SOUND_DB
#define DEFAULT_MODE_COMPASS    NXT_COMPASS
#define DEFAULT_MODE_IRSEEKER   IR_SEEK

inline int ReadSensorInMode (int port, int mode) {
    if (GetSensorName(port) != mode) {
        SetSensorMode(port, mode);
    }
    return readSensor(port);
}

#endif