#ifndef SERIAL_HANDLER_H
#define SERIAL_HANDLER_H

#include <sstream>
#include <iostream>
#include <ostream>
#include <fstream>
#include <string>

#include "Fitr.h"

#include "SerialConnection.h"
#include "FitrPrint.h"

class FitrManager;

class SerialHandler {
	public:
		SerialHandler() {}
		void handle(SerialConnection&);
		void handleWith(SerialConnection&, FitrManager&);
};

#include "FitrManager.h"

#endif