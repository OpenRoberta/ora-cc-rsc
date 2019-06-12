#ifndef NEPOMATH
#define NEPOMATH

#include <math.h>

#define GOLDEN_RATIO 1.61803398875

inline float MathMin(float firstValue, float secondValue) {
    return firstValue < secondValue ? firstValue : secondValue;
}

inline float MathMax(float firstValue, float secondValue) {
    return firstValue > secondValue ? firstValue : secondValue;
}

#endif