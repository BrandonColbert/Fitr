#ifndef FITR_MANAGER_H
#define FITR_MANAGER_H

#include <iostream>
#include <iomanip>
#include <string>
#include <thread>

#include "Fitr.h"

#include "SerialConnection.h"
#include "SerialHandler.h"
#include "FitrPrint.h"

/*
std::ostream & operator<<(std::ostream &stream, Vector3 &angles) {
	stream << "Vector3(" << angles.x << ", " << angles.y << ", " << angles.z << ")";

	return stream;
}
*/

class FitrManager {
	private:
		std::string portNumber = "";
		int baudRate = FITR_BR;

		Hand *hand = nullptr;
		SerialConnection *serial = nullptr;
	public:
		FitrManager(int, char*[]);
		~FitrManager();
		void stop();
		Hand& getHand();
		void utilize(char, char*, int);
};

#endif