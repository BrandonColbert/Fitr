#ifndef FITR_HAND_H
#define FITR_HAND_H

#include "Quaternion.h"

struct Finger {
    public:
        float flex = 0.0f;
        Quaternion rotation;
};

class Hand {
    private:
        Finger *fingers;
    public:
        int totalFingers;
		Quaternion rotation;

        Hand();
        Hand(int);
        Hand(Hand&);
        ~Hand();
        Finger& operator[](int);
};

#endif
