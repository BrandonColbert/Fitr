#include "Component.h"

Component::Component() {
    setName("Unknown");
}

void Component::setName(const char *name) {
    this->name = const_cast<char*>(name);
}

char* Component::getName() {
    return name;
}
