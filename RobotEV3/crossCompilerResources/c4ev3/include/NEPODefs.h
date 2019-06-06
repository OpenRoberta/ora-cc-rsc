#include <math.h>

#define SPEED_MAX 100

inline float MathMin(float firstValue, float secondValue) {
  if (firstValue < secondValue){
    return firstValue;
  }
  else{
    return secondValue;
  }
}

inline float MathMax(float firstValue, float secondValue) {
  if (firstValue > secondValue){
    return firstValue;
  }
  else{
    return secondValue;
  }
}

inline int Speed(float speed){
    return MathMax(-SPEED_MAX, MathMin(speed, SPEED_MAX));
}

