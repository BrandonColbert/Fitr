#ifndef ultrasonicsensor_h
#define ultrasonicsensor_h

#include <Arduino.h>

class UltraSonicSensor {
  private:
    int echo, trig;
  public:
    UltraSonicSensor(int, int);
    float duration();
    static float toCentimeters(int);
    static float toInches(int);
    static const char *requirements();
};

#endif