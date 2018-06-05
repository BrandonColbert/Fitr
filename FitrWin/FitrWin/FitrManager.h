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
#include "FitrMemory.h"

class FitrManager {
	private:
		long long lastSyncTime = 0LL;
		double syncRate = 0;

		std::string portNumber = "";
		int baudRate = FITR_BR;

		Hand *hand = nullptr;
		SerialConnection *serial = nullptr;
		FitrMemory *fitrMem = nullptr;
	public:
		FitrManager(int, char*[]);
		~FitrManager();
		void start();
		void stop();
		Hand& getHand();
		void utilize(char, char*, int);
};

#endif