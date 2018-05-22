#ifndef FLEX_SENSOR_H
#define FLEX_SENSOR_H

#include "Component.h"

class FlexSensor : public Component {
    private:
        float maxTransitionPercent;
        void transitionTo(float&, float);
    protected:
        float calculateFlexResistance();
    public:
        int pin;
        float inVolts, outVolts, resistor, fr, lowestFR, highestFR;

        FlexSensor(int, float, float);
        float getFlex();
        int getFlexInt();
};

#endif
