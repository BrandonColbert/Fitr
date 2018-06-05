#include "BoardModules.h"

#include "Fitr/Fitr.h"

using namespace Transmit;

Board board;

template<typename T>
void transmitSend(char code, List<char> &&data) {
	char *d = data.array();

	Serial.write(code);
	Serial.write(data.size());
	Serial.write(d, data.size());

	delete d;
	data.clear();
}

void setup() {
	Serial.begin(FITR_BR);
	//board.addComponent("IMU Palm", new MPU());
	board.addComponent("ThumbFlex", new FlexSensor(5, 5, 2000));
	board.addComponent("IndexFlex", new FlexSensor(4, 5, 2000));
	board.addComponent("MiddleFlex", new FlexSensor(3, 5, 2000));
	board.addComponent("RingFlex", new FlexSensor(2, 5, 2000));
	board.addComponent("PinkyFlex", new FlexSensor(1, 5, 2000));

	while(!(Serial.available() > 0));
}

void loop() {
	Quaternion exampleRotation(0.0f, 0.0f, 0.0f, 1.0f);

	transmitSend(Code::FINGER_1_F, encodeFloat(((FlexSensor*)&board.getComponent(0))->getFlex()));
	transmitSend(Code::FINGER_1_R, encodeQuaternion(exampleRotation));
	transmitSend(Code::FINGER_2_F, encodeFloat(((FlexSensor*)&board.getComponent(1))->getFlex()));
	transmitSend(Code::FINGER_2_R, encodeQuaternion(exampleRotation));
	transmitSend(Code::FINGER_3_F, encodeFloat(((FlexSensor*)&board.getComponent(2))->getFlex()));
	transmitSend(Code::FINGER_3_R, encodeQuaternion(exampleRotation));
	transmitSend(Code::FINGER_4_F, encodeFloat(((FlexSensor*)&board.getComponent(3))->getFlex()));
	transmitSend(Code::FINGER_4_R, encodeQuaternion(exampleRotation));
	transmitSend(Code::FINGER_5_F, encodeFloat(((FlexSensor*)&board.getComponent(4))->getFlex()));
	transmitSend(Code::FINGER_5_R, encodeQuaternion(exampleRotation));
	transmitSend(Code::PALM_R, encodeQuaternion(exampleRotation));
}

void printAll() {
	bool first = true;

	for(int i = 0; i < board.getComponents().size(); i++) {

		Component *component = board.getComponents()[i];

		if(!first) {
			Serial.print(" | ");
		}

		Serial.print(component->getName());
		Serial.print("[");

		if(strstr(component->getName(), "Flex") != NULL) {
			FlexSensor *flexSensor = (FlexSensor*)component;
			Serial.print(flexSensor->getFlex());
			Serial.print("%");
		} else if(strstr(component->getName(), "IMU") != NULL) {
			MPU *mpu = (MPU*)component;
			int *values = mpu->getRaw();

			Serial.print(values[0]);
			for(int i = 1; i < 6; i++) {
				Serial.print(", ");
				Serial.print(values[i]);
			}
		}

		Serial.print("]");
		first = false;
	}

	Serial.println();
}

int lastFlex = 12345;

void printDetailed(FlexSensor *flexSensor) {
	int currentFlex = flexSensor->getFlex();

	if(currentFlex != lastFlex) {
		Serial.print(flexSensor->getName());
		Serial.print(": ");
		Serial.print(currentFlex);
		Serial.print("%");
		Serial.print(" (delta of ");
		Serial.print(-(currentFlex - lastFlex));
		Serial.print(") at ");
		Serial.print(-flexSensor->fr);
		Serial.print(" with low of ");
		Serial.print(-flexSensor->lowestFR);
		Serial.print(" and high of ");
		Serial.print(-flexSensor->highestFR);
		Serial.println();

		lastFlex = currentFlex;
	}
}

void printGraph(FlexSensor *flexSensor) {
	Serial.print(flexSensor->getName());
	Serial.print(": ");
	int bars = flexSensor->getFlex();
	for(int i = 0; i < bars; i++) {
		Serial.print("-");
	}
	Serial.println("");
}
