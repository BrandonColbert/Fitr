#ifndef SERIAL_HANDLER_H
#define SERIAL_HANDLER_H

#include <sstream>
#include <iostream>
#include <ostream>
#include <fstream>
#include <string>

#include "../../FitrArduino/Fitr/Fitr.h"

#include "SerialConnection.h"
#include "FitrPrint.h"

class SerialHandler {
	public:
		SerialHandler() {}
		void handle(SerialConnection, void (*)(char, char*, int));
};

#endif