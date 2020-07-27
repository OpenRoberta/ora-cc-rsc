#ifndef NEPOLOG
#define NEPOLOG

#include "ev3_sensors/ev3_sensors.private.h"

#define LOG_DELAY 2000

void * loggingLoop (void * arguments);

void startLoggingThread (uint8_t motorPorts) {
    static uint8_t _motorPorts = 0;
    _motorPorts = motorPorts;
    pthread_t threadId;
    pthread_create(&threadId, NULL, loggingLoop, &_motorPorts);
}

void logMotors (uint8_t motorPorts);
void logSensors ();

void * loggingLoop (void * arguments) {
    uint8_t motorPorts = *((uint8_t *) arguments);
    while (1) {
        logMotors(motorPorts);
        logSensors();
        Wait(LOG_DELAY);
    }
}

bool isMotorPortUsed (uint8_t usedMotorPorts, uint8_t motorPort);
void logMotor(char portName, uint8_t port);

void logMotors (uint8_t usedMotorPorts) {
    char motorPortName[] = { 'A', 'B', 'C', 'D' };
    uint8_t motorPorts[] = { OUT_A, OUT_B, OUT_C, OUT_D };
    for (int i = 0; i < NUM_OUTPUTS; i++) {
        uint8_t port = motorPorts[i];
        if (isMotorPortUsed(usedMotorPorts, port)) {
            logMotor(motorPortName[i], port);
        }
    }
}

bool isMotorPortUsed (uint8_t usedMotorPorts, uint8_t motorPort) {
    return usedMotorPorts & motorPort;
}

void logMotor(char portName, uint8_t port) {
    int tachoCount;
    OutputGetTachoCount(port, &tachoCount);
    TermPrintf("%c: %d", portName, tachoCount);
}

void logSensor (int port);

void logSensors () {
    for (int i = 0; i < NUM_INPUTS; i++) {
        logSensor(i);
    }
}

std::string readSensorAsString (int port, SensorHandler * sensor);

void logSensor (int port) {
    SensorHandler * sensor = GetSensor(port);
    if (sensor  == NULL) {
        return;
    }

    std::string value = readSensorAsString(port, sensor);
    TermPrintf("%d: %s", port + 1, value.c_str());
}

std::string readEV3TouchSensorAsString (int port) {
    return ToString(ReadEV3TouchSensor(port));
}

std::string readEV3GyroSensorAsString (int port) {
    return "angle: " + ToString(ReadEV3GyroSensorAngle(port, EV3GyroInterleavedAngle)) +
        " rate: " + ToString(ReadEV3GyroSensorRate(port, EV3GyroInterleavedRate));
}

std::string readEV3ColorSensorAsString (int port) {
    int mode = EV3Color->currentSensorMode[port];
    switch (mode) {
        case EV3_COLOR_SENSOR_REFLECT_MODE:
            return ToString(ReadEV3ColorSensorReflectedLight(port));
        case EV3_COLOR_SENSOR_AMBIENT_MODE:
            return ToString(ReadEV3ColorSensorAmbientLight(port));
        case EV3_COLOR_SENSOR_COLOR_MODE:
            return ToString(ReadEV3ColorSensorColor(port));
        case EV3_COLOR_SENSOR_RAW_COLOR_MODE:
            return ToString(NEPOReadEV3ColorSensorRGB(port));
        default:
            return "???";   
    }
}

std::string readEV3IrSensorAsString (int port) {
    int mode = EV3Ir->currentSensorMode[port];
    switch (mode) {
        case EV3_IR_SENSOR_PROXIMITY_MODE:
            return ToString(ReadEV3IrSensorProximity(port)); 
        case EV3_IR_SENSOR_SEEK_MODE:
            return ToString(_ReadIRSeekAllChannels(port));
        default:
            return "???";   
    }
}


std::string readEV3UltrasonicSensorAsString (int port) {
    int mode = EV3Ultrasonic->currentSensorMode[port];
    switch (mode) {
        case EV3_ULTRASONIC_SENSOR_DISTANCE_MM_MODE:
            return ToString(ReadEV3UltrasonicSensorDistance(port, EV3_ULTRASONIC_CM));
        case EV3_ULTRASONIC_SENSOR_LISTEN_MODE:
            return ToString(ReadEV3UltrasonicSensorListen(port));
        default:
            return "???";   
    }
}


std::string readHTCompassSensorAsString (int port) {
    return "angle: " + ToString(ReadHTCompassSensor(port, HTCompassAngle)) + 
        " comp: " + ToString(ReadHTCompassSensor(port, HTCompassCompass));
}

std::string readHTIrSensorAsString (int port) {
    int mode = HTIrV2->currentSensorMode[port];
    return ToString(ReadHTIrV2Sensor(port, (HTIrV2ReadingMode) mode));
}

std::string readNXTSoundSensorAsString (int port) {
    return ToString(ReadNXTSoundSensor(port, NXT_SOUND_DB));
}

std::string readHTColorSensorV2AsString (int port) {
    int mode = HTColorV2->currentSensorMode[port];
    switch (mode) {
        case HT_COLOR_SENSOR_V2_DEFAULT_MODE:
            return "color id: " + ToString(ReadHTColorSensorV2(port)) +
                " rgb: " + ToString(NEPOReadHTColorSensorV2RGB(port)) +
                " light: " + ToString(NEPOReadHTColorSensorV2Light(port));
        case HT_COLOR_SENSOR_V2_PASSIVE_MODE:
            return "ambient light: " + ToString(NEPOReadHTColorSensorV2AmbientLight(port));
        default:
            return "???";   
    }
}

std::string readSensorAsString (int port, SensorHandler * sensor) {
    if (sensor == EV3Touch) {
        return readEV3TouchSensorAsString(port);
    } else if (sensor == EV3Gyro) {
        return readEV3GyroSensorAsString(port);
    } else if (sensor == EV3Color) {
        return readEV3ColorSensorAsString(port);
    } else if (sensor == EV3Ir) {
        return readEV3IrSensorAsString(port);
    } else if (sensor == EV3Ultrasonic) {
        return readEV3UltrasonicSensorAsString(port);
    } else if (sensor == HTCompass) {
        return readHTCompassSensorAsString(port);
    } else if (sensor == HTIrV2) {
        return readHTIrSensorAsString(port);
    } else if (sensor == NXTSound) {
        return readNXTSoundSensorAsString(port);
    } else if (sensor == HTColorV2) {
        return readHTColorSensorV2AsString(port);
    }

    return "Not implemented";
}


#endif
