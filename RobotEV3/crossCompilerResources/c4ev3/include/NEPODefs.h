#include <math.h>

#define TEXT_COLOR_WHITE 0
#define TEXT_COLOR_BLACK 1

#define GOLDEN_RATIO 1.61803398875

inline float MathMin(float firstValue, float secondValue) {
    return firstValue < secondValue ? firstValue : secondValue;
}

inline float MathMax(float firstValue, float secondValue) {
    return firstValue > secondValue ? firstValue : secondValue;
}

#include "NEPOMotors.h"
#include "NEPOSensors.h"

