#ifndef FITR_HAND_H
#define FITR_HAND_H

#include "FitrQuaternion.h"

struct Finger {
    public:
        int flex = 0;
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
