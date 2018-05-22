#ifndef COMPONENT_H
#define COMPONENT_H

#include <Arduino.h>

class Component {
    private:
        char *name;
    protected:
        template<typename T> inline void print(T t) { Serial.print(t); }
        template<typename T> inline void println(T t) { Serial.println(t); }
    public:
        Component();
        virtual ~Component() = default;
        void setName(const char*);
        char* getName();
};

#endif
