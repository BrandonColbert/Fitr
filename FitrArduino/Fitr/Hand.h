#ifndef FITR_HAND_H
#define FITR_HAND_H

#include "FitrQuaternion.h"

struct Finger {
    public:
        float flex = 0.0f;
        FitrQuaternion rotation;
};

class Hand {
    private:
        Finger *fingers;
    public:
        int totalFingers;
		FitrQuaternion rotation;

        Hand();
        Hand(int);
        Hand(Hand&);
        ~Hand();
        Finger& operator[](int);
};

#endif
