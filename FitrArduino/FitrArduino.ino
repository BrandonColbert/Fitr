#include "Fitr/Fitr.h"
#include "FitrPrint.h"

#include "MPU6050_6Axis_MotionApps20.h"
#include "I2Cdev.h"
#include "Wire.h"

#include <SoftwareSerial.h>

//#define FITR_DEBUG
//#define FITR_BT

using namespace Transmit;
using namespace FitrPrint;

const int pins_fs_amount = 5;
const int pins_fs[] = {7, 6, 3, 2, 1};
const int pins_mpu_amount = 6;
const int pins_mpu[] = {5, 6, 7, 2, 3, 4};
const int pin_i2c_clock = 5;
const int pin_i2c_data = 4;

//ax, ay, az, gx, gy, gz
const int mpu_offsets[][pins_mpu_amount] = {
	{-3540,    35,  -26,  98,  10,  22}, //palm
	{-3859, -2449, 1207, 157,   4, -30}, //thumb
	{ 1405,   -73, 1277,  72,   0,   9}, //index
	{ 1394,   274,  872,  82, -13,  29}, //middle
	{-2386,  -414, 1606,  41,  64,  20}, //ring
	{-2124,  2999, 1319,  93, -33, -41}, //pinky
};

float flexionLows[pins_fs_amount];
float flexionHighs[pins_fs_amount];
float flexions[pins_fs_amount];
float lastFlexions[pins_fs_amount];
FitrQuaternion rotations[1 + pins_fs_amount];
bool enabledRotations[1 + pins_fs_amount] = {true, true, true, true, true, true};

uint8_t fifoBuffer[64];
uint16_t packetSize;
MPU6050 mpu;

SoftwareSerial hc05(10, 11);

List<char> transmitBuffer;
template<typename T>
void transmitSend(char code, List<char> &&data) {
	transmitBuffer.add(code);
	transmitBuffer.add(data.size());
	transmitBuffer.addAll(data);
	data.clear();
}

void transmitFlush() {
	char *d = transmitBuffer.array();

	#ifndef FITR_DEBUG
		#ifdef FITR_BT
			hc05.write(d, transmitBuffer.size());
		#else
			Serial.write(d, transmitBuffer.size());
		#endif
	#endif

	delete d;
	transmitBuffer.clear();
}

void setup() {
	#ifdef FITR_BT
		hc05.begin(FITR_BR);
	#else
		Serial.begin(FITR_BR);
	#endif

	for(int i = 0, pin = 0; i < pins_mpu_amount; i++) {
        pin = pins_mpu[i];
        pinMode(pin, OUTPUT);
        digitalWrite(pin, HIGH);
    }

	Wire.begin();
	TWBR = 4;//24;

	#ifndef FITR_DEBUG
		#ifdef FITR_BT
			while(!(hc05.available() > 0));
		#else
			while(!(Serial.available() > 0));
		#endif
	#endif

	for(int i = 0, pin = 0; i < pins_mpu_amount; i++) {
        pin = pins_mpu[i];
		for(int j = 0; j < pins_mpu_amount; j++) digitalWrite(pins_mpu[j], j == i ? LOW : HIGH);

		#ifdef FITR_DEBUG
			print("Initializing MPU #", i);
		#endif

		mpu.initialize();

		if(mpu.testConnection() && mpu.dmpInitialize() == 0) {
			#ifdef FITR_DEBUG
				print(" with offsets: ");

				for(int j = 0; j < pins_mpu_amount; j++) {
					print(j > 0 ? ", " : "", mpu_offsets[i][j]);
				}

				println("");
			#endif

			mpu.setXAccelOffset(mpu_offsets[i][0]);
			mpu.setYAccelOffset(mpu_offsets[i][1]);
			mpu.setZAccelOffset(mpu_offsets[i][2]);
			mpu.setXGyroOffset(mpu_offsets[i][3]);
			mpu.setYGyroOffset(mpu_offsets[i][4]);
			mpu.setZGyroOffset(mpu_offsets[i][5]);

			mpu.setDMPEnabled(true);
			packetSize = mpu.dmpGetFIFOPacketSize();
		} else {
			enabledRotations[i] = false;
		}

		#ifdef FITR_DEBUG
			if(!enabledRotations[i]) println("(Error)");
		#endif
    }

    #ifdef FITR_DEBUG
      println("Ready");
    #endif
}

void loop() {
    for(int i = 0; i < pins_mpu_amount; i++) {
		if(enabledRotations[i]) {
	        int pin = pins_mpu[i];
	        for(int j = 0; j < pins_mpu_amount; j++) digitalWrite(pins_mpu[j], j == i ? LOW : HIGH);

			uint16_t fifoCount = mpu.getFIFOCount();
			Quaternion mpuRotation;

			if(fifoCount >= 1024) {
				mpu.resetFIFO();
			} else if(fifoCount >= packetSize) {
				mpu.getFIFOBytes(fifoBuffer, packetSize);
				fifoCount -= packetSize;
				mpu.dmpGetQuaternion(&mpuRotation, fifoBuffer);

				rotations[i] = FitrQuaternion(mpuRotation.x, mpuRotation.y, mpuRotation.z, mpuRotation.w);
				//FitrQuaternion(-mpuRotation.y, mpuRotation.z, mpuRotation.x, -mpuRotation.w);
			}
		}
    }

	for(int i = 0, flow = 0, fhigh = 0, flex = 0; i < pins_fs_amount; i++) {
		if(enabledRotations[1 + i]) {
	        flow = flexionLows[i];
	        fhigh = flexionHighs[i];
	        flex = analogRead(pins_fs[i]);

			if(flow == 0 && fhigh == 0) flexionLows[i] = flexionHighs[i] = flex;
	        else if(flex < flow) flexionLows[i] = flex;
	        else if(flex > fhigh) flexionHighs[i] = flex;

			lastFlexions[i] = flexions[i];
			flexions[i] = 1.0f - (float)(flex - flow) / (float)(fhigh - flow);
			//if(lastFlexions[i] != flexions[i]) println("Flex: ", flexions[i], "(", flex, ")");
		}
	}

	for(int i = 0; i < pins_fs_amount; i++) {
		if(enabledRotations[1 + i]) {
			if(lastFlexions[i] != flexions[i]) transmitSend(Code::FINGER_1_F + i, encodeFloat(flexions[i]));
			transmitSend(Code::FINGER_1_R + i, encodeQuaternion(rotations[1 + i]));
		}
	}

	transmitSend(Code::PALM_R, encodeQuaternion(rotations[0]));
	transmitFlush();
}
