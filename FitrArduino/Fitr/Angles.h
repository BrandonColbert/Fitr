#ifndef ANGLES_H
#define ANGLES_H

class Angles {
    public:
        virtual ~Angles() {}

        virtual Angles& operator=(Angles&) {
            return *this;
        }
};

class Quaternion : public Angles {
    public:
        float x, y, z, w;
        Quaternion() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
        Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

        Quaternion& operator=(Quaternion &other) {
            this->x = other.x;
            this->y = other.y;
            this->z = other.z;
            this->w = other.w;

            return *this;
        }
};

class Vector3 : public Angles {
    public:
        float x, y, z;
        Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
        Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

        Vector3& operator=(Vector3 &other) {
            this->x = other.x;
            this->y = other.y;
            this->z = other.z;

            return *this;
        }
};

class AccelGyro : public Angles {
    public:
        float ax, ay, az, gx, gy, gz;
        AccelGyro() : ax(0.0f), ay(0.0f), az(0.0f), gx(0.0f), gy(0.0f), gz(0.0f) {}
        AccelGyro(float ax, float ay, float az, float gx, float gy, float gz) : ax(ax), ay(ay), az(az), gx(gx), gy(gy), gz(gz) {}

        AccelGyro& operator=(AccelGyro &other) {
            this->ax = other.ax;
            this->ay = other.ay;
            this->az = other.az;
            this->gx = other.gx;
            this->gy = other.gy;
            this->gz = other.gz;

            return *this;
        }
};

#endif
