#ifndef MPU_H
#define MPU_H

#include "Component.h"

#include "I2Cdev.h"
#include "MPU6050.h"

class MPU : public Component {
    private:
        int16_t ax, ay, az, gx, gy, gz;
        MPU6050 imu;
    public:
        MPU();
        int* getRaw();
};

#endif
