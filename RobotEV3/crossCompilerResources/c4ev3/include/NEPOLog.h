#ifndef NEPOLOG
#define NEPOLOG

#define LOG_DELAY 2000

void * loggingLoop (void * arguments);

void startLoggingThread (uint8_t motorPorts) {
    pthread_t threadId;
    pthread_create(&threadId, NULL, loggingLoop, &motorPorts);
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

std::string readSensorAsString (int port, int modeName);

void logSensor (int port) {
    int modeName = GetSensorName(port);
    if (modeName == NO_SEN) {
        return;
    }
    std::string value = readSensorAsString(port, modeName);
    TermPrintf("%d: %s", port + 1, value.c_str());
}

std::string readSensorAsString (int port, int modeName) {
    // TODO: ultrasonic presence prints 0 and 1 instead of false and true
    switch (modeName) {
        case COL_COLOR:
            return ToString(ReadColorSensor(port));
        case COL_RGB:
            return ToString(ReadColorSensorRGB(port));
        case IR_SEEK:
            return ToString(_ReadIRSeekAllChannels(port));
        default:
            return ToString(readSensor(port));
    }
}

#endif