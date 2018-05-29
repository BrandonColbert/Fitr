#include "Board.h"

Board::Board() {
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif
}

Component& Board::addComponent(const char *name, Component *component) {
    component->setName(name);
    components.add(component);

    return *component;
}

Component& Board::getComponent(const char *name) {
    for(int i = 0; i < components.size(); i++) {
        if(components[i]->getName() == name) {
            return *components[i];
        }
    }
}

Component& Board::getComponent(int index) {
    return *components[index];
}

void Board::removeComponent(const char *name) {
    for(int i = 0; i < components.size(); i++) {
        if(components[i]->getName() == name) {
            components.remove(i);
            return;
        }
    }
}

List<Component*> Board::getComponents() {
    return components;
}
