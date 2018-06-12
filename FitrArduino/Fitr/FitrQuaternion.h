#ifndef FITRQUATERNION_H
#define FITRQUATERNION_H

class FitrQuaternion {
    public:
        float x, y, z, w;
        FitrQuaternion() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
        FitrQuaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
        FitrQuaternion(const FitrQuaternion &other) : x(other.x), y(other.y), z(other.z), w(other.w) {}
        FitrQuaternion(const FitrQuaternion &&other) : x(other.x), y(other.y), z(other.z), w(other.w) {}

        FitrQuaternion& operator=(FitrQuaternion other) {
            this->x = other.x;
            this->y = other.y;
            this->z = other.z;
            this->w = other.w;

            return *this;
        }
};

#endif
