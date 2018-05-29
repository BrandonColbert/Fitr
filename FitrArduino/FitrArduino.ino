#include "BoardModules.h"

#include "Fitr/Fitr.h"

using namespace Transmit;

Board board;

void setup() {
	Serial.begin(FITR_BR);
	/*
	board.addComponent("IMU Palm", new MPU());
	board.addComponent("Flex Thumb", new FlexSensor(1, 5, 2000));
	board.addComponent("Flex Index", new FlexSensor(2, 5, 2000));
	board.addComponent("Flex Middle", new FlexSensor(3, 5, 2000));
	*/

	while(!(Serial.available() > 0));
}

void loop() {
	float value = 1.3f;

	List<char> data = encode(value);
	char *d = data.array();

	Serial.write(Code::FLOAT);
	Serial.write(data.size());
	Serial.write(d, data.size());

	delete d;
	data.clear();
 /*
	for(int i = 0; i < data.size(); i++) {
    	Serial.println((int)data[i]);
	}
 */

	/*
	int value = -21903;//Vector3 angle(1.0f, 2.0f, 5.0f);
	List<char> data = encode(value);

	Serial.write(Code::INT);
	for(int i = 0; i < data.size(); i++) {
    	Serial.write(data[i]);
		Serial.flush();
	}
	*/

	//Serial.println("Hello world!");

	//Serial.write("\n");
	//Serial.flush();

	/*
   Vector3 remake;
   Transmit::decode(remake, data);
   Serial.print("Vector3(");
   Serial.print(remake.x);
   Serial.print(", ");
   Serial.print(remake.y);
   Serial.print(", ");
   Serial.print(remake.z);
   Serial.print(")");
   Serial.print(" from ");
   Serial.print((char*)data);
   Serial.print(" or ");
   Serial.print(data[0]);
   Serial.println("");
   */

   //Serial.println("Hello World");

   //Serial.print("(Begin) ");
   //Serial.write(data);
   //Serial.println(" (End)\n");

	//printAll();

	//delay(1000);
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

void printDetailed(FlexSensor &flexSensor) {
	int currentFlex = flexSensor.getFlex();

	if(currentFlex != lastFlex) {
		Serial.print("Flex: ");
		Serial.print(currentFlex);
		Serial.print("%");
		Serial.print(" (delta of ");
		Serial.print(currentFlex - lastFlex);
		Serial.print(") at ");
		Serial.print(flexSensor.fr);
		Serial.print(" with low of ");
		Serial.print(flexSensor.lowestFR);
		Serial.print(" and high of ");
		Serial.print(flexSensor.highestFR);
		Serial.println();

		lastFlex = currentFlex;
	}
}
