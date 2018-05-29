#ifndef HAND_H
#define HAND_H

#include "Angles.h"

struct Finger {
    float flex;
    Angles angles;
};

class Hand {
    private:
        Finger *fingers;
    public:
        int totalFingers;
        Angles rotation;

        Hand(int);
        ~Hand();
        Finger& operator[](int);
};

#endif
