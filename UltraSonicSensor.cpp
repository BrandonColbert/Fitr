#include "UltraSonicSensor.h"

UltraSonicSensor::UltraSonicSensor(int echoPin, int trigPin) {
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
  echo = echoPin;
  trig = trigPin;
}

float UltraSonicSensor::duration() {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  return pulseIn(echo, HIGH);
}

float UltraSonicSensor::toCentimeters(int duration) {
  return duration * 0.017f;
}

float UltraSonicSensor::toInches(int duration) {
  return duration * 0.00665f;
}

const char *UltraSonicSensor::requirements() {
	return "5V, Digital Pin x2, Ground";
}