#include "FlexSensor.h"

FlexSensor::FlexSensor(int pin, float inVolts, float resistor) {
	this->pin = pin;
	this->inVolts = inVolts;
	this->resistor = resistor;
	lowestFR = highestFR = fr = -resistor;//lowestFR = -2195.0f;
	maxTransitionPercent = 100.0f;
}

float FlexSensor::calculateFlexResistance() {
	outVolts = analogRead(pin);

	int nfr = resistor / (inVolts / outVolts - 1.0f);

	//println(outVolts);

	fr = nfr == 0 ? fr : nfr;

	return fr;
}

void FlexSensor::transitionTo(float &original, float target) {
	float transitionPercent = (target - original) / original;

	if(abs(transitionPercent) < maxTransitionPercent / 100.0f) {
		original = target;
	} else {
		print("Transitioning from ");
		print(original);
		print(" to ");
		print(target);
		print(" is too high of a change at ");
		print(transitionPercent * 100.0f);
		println("%");
	}
}

float FlexSensor::getFlex() {
	calculateFlexResistance();

	if(fr > lowestFR) {
		transitionTo(lowestFR, fr);
	} else if(fr < highestFR) {
		transitionTo(highestFR, fr);
	}

	return (lowestFR - fr) / (lowestFR - highestFR) * 100.0f;
}

int FlexSensor::getFlexInt() {
	return (int)getFlex();
}
