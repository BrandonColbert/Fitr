#ifndef FITR_QUATERNION_H
#define FITR_QUATERNION_H

class Quaternion {
    public:
        float x, y, z, w;
        Quaternion() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
        Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

        Quaternion& operator=(Quaternion &other) {
            this->x = other.x;
            this->y = other.y;
            this->z = other.z;
            this->w = other.w;

            return *this;
        }
};

#endif
