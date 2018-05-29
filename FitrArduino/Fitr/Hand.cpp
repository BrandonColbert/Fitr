#include "Hand.h"

Hand::Hand(int totalFingers) {
    fingers = new Finger[totalFingers];
}

Hand::~Hand() {
    delete[] fingers;
}

Finger& Hand::operator[](int finger) {
    if(finger < 0) {
        return fingers[0];
    } else if(finger < totalFingers) {
        return fingers[finger];
    } else {
        return fingers[totalFingers - 1];
    }
}
