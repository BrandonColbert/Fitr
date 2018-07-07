#include "Fitr/Fitr.h"
#include "FitrPrint.h"

#include "MPU6050_6Axis_MotionApps20.h"
#include "I2Cdev.h"
#include "Wire.h"

//#define FITR_DEBUG

using namespace Transmit;
using namespace FitrPrint;

const int pins_fs_amount = 5;
const int pins_fs[] = {7, 6, 3, 2, 1};
const int pins_mpu_amount = 6;
const int pins_mpu[] = {2, 3, 4, 5, 6, 7};
const int pin_i2c_clock = 5;
const int pin_i2c_data = 4;

int flexionLows[pins_fs_amount];
int flexionHighs[pins_fs_amount];
int flexions[pins_fs_amount];
FitrQuaternion rotations[1 + pins_fs_amount];

Quaternion mpuRotation;
volatile bool mpuInterrupt = false;
bool dmpReady = false;
uint8_t mpuIntStatus, devStatus, fifoBuffer[64];
uint16_t packetSize, fifoCount;
MPU6050 mpu;

template<typename T>
void transmitSend(char code, List<char> &&data) {
	char *d = data.array();

    #ifndef FITR_DEBUG
    	Serial.write(code);
    	Serial.write(data.size());
    	Serial.write(d, data.size());
    #endif

	delete d;
	data.clear();
}

void onDataReady() {
	mpuInterrupt = true;
}

void setup() {
	Serial.begin(FITR_BR);

    for(int i = 0, pin = 0; i < pins_mpu_amount; i++) {
        pin = pins_mpu[i];
        pinMode(pin, OUTPUT);
        digitalWrite(pin, LOW);
    }

    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
		Wire.begin();
		TWBR = 24;
	#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
		Fastwire::setup(400, true);
	#endif

	while(!(Serial.available() > 0));

    //println("Initializing MPUs");
	mpu.initialize();
    mpu.testConnection();//println(mpu.testConnection() ? "MPUs connected" : "MPUs can't connect");

	devStatus = mpu.dmpInitialize();

	mpu.setXGyroOffset(220);
	mpu.setYGyroOffset(76);
	mpu.setZGyroOffset(-85);
	mpu.setZAccelOffset(1788);

	if(devStatus == 0) {
		mpu.setDMPEnabled(true);
		attachInterrupt(0, onDataReady, RISING);
		mpuIntStatus = mpu.getIntStatus();
		dmpReady = true;
		packetSize = mpu.dmpGetFIFOPacketSize();
	} else {
		//println("DMP Initialization failed (code ", devStatus, ")");
	}
}

void loop() {
    for(int i = 0; i < pins_mpu_amount; i++) {
        int pin = pins_mpu[i];
        for(int j = 0; j < pins_mpu_amount; j++) digitalWrite(pins_mpu[j], j == i ? LOW : HIGH);

        if(!dmpReady) return;
		while(!mpuInterrupt && fifoCount < packetSize) break;

		mpuInterrupt = false;
		mpuIntStatus = mpu.getIntStatus();
		fifoCount = mpu.getFIFOCount();

		if(fifoCount == 1024) {
			mpu.resetFIFO();
		} else {
			while(fifoCount < packetSize) fifoCount = mpu.getFIFOCount();
			mpu.getFIFOBytes(fifoBuffer, packetSize);
			fifoCount -= packetSize;
			mpu.dmpGetQuaternion(&mpuRotation, fifoBuffer);
            // x  y  z  w ->  a  b  c  d at i0
            // w  x  z  y <-  d  a  c  b at i19 from last
            //-y  z  x -w <- -d  c  b  -a at i23 from last

			rotations[i] = FitrQuaternion(-mpuRotation.y, mpuRotation.z, mpuRotation.x, -mpuRotation.w); //FitrQuaternion(mpuRotation.x, mpuRotation.y, mpuRotation.z, mpuRotation.w);
			//println("quat\t", mpuRotation.w, "\t", mpuRotation.x, "\t", mpuRotation.y, "\t", mpuRotation.z);
		}
    }
/*
	for(int i = 0; i < pins_mpu_amount; i++) {
		print(i, "->", rotations[i].w, ":", rotations[i].x, ":", rotations[i].y, ":", rotations[i].z, i < pins_mpu_amount - 1 ? "____" : "");
	}
    println("");
*/

	//print("Flex: ");
	for(int i = 0, flow = 0, fhigh = 0, flex = 0; i < pins_fs_amount; i++) {
        flow = flexionLows[i];
        fhigh = flexionHighs[i];
        flex = analogRead(pins_fs[i]);

		if(flow == 0 && fhigh == 0) flexionLows[i] = flexionHighs[i] = flex;
        else if(flex < flow) flexionLows[i] = flex;
        else if(flex > fhigh) flexionHighs[i] = flex;

		flexions[i] = (int)((float)(flex - flow) / (float)(fhigh - flow) * 100.0f);
		//print(i == 0 ? "" : ", ", flexions[i], "(", flex, ")");
	}
	//println("");

	transmitSend(Code::FINGER_1_F, encodeInt(flexions[0]));
	transmitSend(Code::FINGER_1_R, encodeQuaternion(rotations[1]));
	transmitSend(Code::FINGER_2_F, encodeInt(flexions[1]));
	transmitSend(Code::FINGER_2_R, encodeQuaternion(rotations[2]));
	transmitSend(Code::FINGER_3_F, encodeInt(flexions[2]));
	transmitSend(Code::FINGER_3_R, encodeQuaternion(rotations[3]));
	transmitSend(Code::FINGER_4_F, encodeInt(flexions[3]));
	transmitSend(Code::FINGER_4_R, encodeQuaternion(rotations[4]));
	transmitSend(Code::FINGER_5_F, encodeInt(flexions[4]));
	transmitSend(Code::FINGER_5_R, encodeQuaternion(rotations[5]));
	transmitSend(Code::PALM_R, encodeQuaternion(rotations[0]));
}
