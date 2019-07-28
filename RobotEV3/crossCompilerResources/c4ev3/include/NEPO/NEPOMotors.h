#ifndef NEPOMOTORS
#define NEPOMOTORS

#define SPEED_MAX 100

inline int Speed(int speed) {
    return std::max(-SPEED_MAX, std::min(speed, SPEED_MAX));
}

/**
 * RotateMotor of c4ev3, called with a negative angle, will behave as if the angle was positive.
 * This function will run the motor in reverse if the angle is negative.
*/
inline void RotateMotorForAngleWithTurn(int outputs, float speed, float angle, float turn) {
    // If the rotate block is placed right after a motor block, the rotation won't be executed,
    // since the wheels are already spinning and the encoder values change
    Off(outputs);
    Wait(100);

    if (angle < 0) {
        speed *= -1;
        angle *= -1;
    }
    RotateMotorEx(outputs, speed, angle, turn, true, true);
}

inline void RotateMotorForAngle(int outputs, float speed, float angle) {
    RotateMotorForAngleWithTurn(outputs, speed, angle, 0);
}


inline void SteerDrive(int portLeft, int portRight, float speedLeft, float speedRight) {
    OnFwdReg(portLeft, speedLeft);
    OnFwdReg(portRight, speedRight);
}

inline void StopSteer (int portLeft, int portRight) {
    Off(portLeft);
    Off(portRight);
}


inline float computeOuterCircumferenceForSteerDrive (float speedLeft, float speedRight, float distance);
inline float computeAngleForSteerDrive (float outerCircumference);
inline void WaitSteerDriveToReachAngle(int portRight, int portLeft, float speedLeft, float speedRight, int angle);

inline void SteerDriveForDistance(int portLeft, int portRight, float speedLeft, float speedRight, float distance) {
    float outerCircumference = computeOuterCircumferenceForSteerDrive(speedLeft, speedRight, distance);
    float angle = computeAngleForSteerDrive(outerCircumference);
    SteerDrive(portLeft, portRight, speedLeft, speedRight);
    WaitSteerDriveToReachAngle(portLeft, portRight, speedLeft, speedRight, angle);
    StopSteer(portLeft, portRight);
}

inline float computeOuterCircumferenceForSteerDrive (float speedLeft, float speedRight, float distance) {
    if (speedLeft + speedRight == 0) {
        return distance;
    } else if (abs(speedLeft) > abs(speedRight)) {
        return speedLeft / (speedRight + speedLeft) * 2.0 * distance;
    } else {
        return speedRight / (speedRight + speedLeft) * 2.0 * distance;
    }
}

inline float computeAngleForSteerDrive (float outerCircumference) {
    return abs(outerCircumference * 360.0 / (M_PI * WHEEL_DIAMETER));
}

inline void WaitMotorToReachAngle(int motorToCheck, float speedToCheck, int angle);

inline void WaitSteerDriveToReachAngle(int portLeft, int portRight, float speedLeft, float speedRight, int angle) {
    if (abs(speedLeft) < abs(speedRight)) {
        WaitMotorToReachAngle(portRight, speedRight, angle);
    } else {
        WaitMotorToReachAngle(portLeft, speedLeft, angle);
    }
}

inline void WaitMotorToReachAngle(int motorPort, float speed, int angle) {
    int start = MotorRotationCount(motorPort);
    int targetEnd = start + (speed > 0 ? angle : -angle);
    if (speed > 0) {
        while(MotorRotationCount(motorPort) < targetEnd) {
            Wait(1);
        }
    } else {
        while(MotorRotationCount(motorPort) > targetEnd) {
            Wait(1);
        }
    }
}

#endif
