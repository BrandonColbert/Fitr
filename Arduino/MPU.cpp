#include "MPU.h"

MPU::MPU() {
    Serial.println("Initializing I2C devices...");
    imu.initialize();

    // verify connection
    Serial.println("Testing device connections...");
    Serial.println(imu.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
}

int* MPU::getRaw() {
    imu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    return new int[6] {
        (int)ax, (int)ay, (int)az,
        (int)gx, (int)gy, (int)gz
    };
}
