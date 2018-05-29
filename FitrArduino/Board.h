#ifndef BOARD_H
#define BOARD_H

#include "Fitr/List.h"
#include "Component.h"

#include "Wire.h"

class Board {
    private:
        List<Component*> components;
    public:
        Board();
        Component& addComponent(const char*, Component*);
        Component& getComponent(const char*);
        Component& getComponent(int);
        void removeComponent(const char*);
        List<Component*> getComponents();
};

#endif
