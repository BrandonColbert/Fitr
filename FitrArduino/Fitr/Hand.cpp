#include "Hand.h"

Hand::Hand() {}
Hand::Hand(int totalFingers) : fingers(new Finger[totalFingers]), totalFingers(totalFingers) {}

Hand::Hand(Hand &source) : fingers(new Finger[source.totalFingers]), totalFingers(source.totalFingers) {
    for(int i = 0; i < totalFingers; i++) {
        fingers[i].flex = source[i].flex;
        fingers[i].rotation = source[i].rotation;
    }
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
