#ifndef BOARD_H
#define BOARD_H

#include "List.h"
#include "Component.h"

#include "Wire.h"

class Board {
    private:
        List<Component*> components;
    public:
        Board();
        void addComponent(const char*, Component*);
        Component& getComponent(const char*);
        Component& getComponent(int);
        void removeComponent(const char*);
        List<Component*> getComponents();
};

#endif
