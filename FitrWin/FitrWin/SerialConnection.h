#ifndef SERIALCONNECTION_H
#define SERIALCONNECTION_H

#include <windows.h>
#include <string>
#include <iostream>

class SerialConnection {
	private:
		volatile bool _connected;
		HANDLE handle;
		COMSTAT status;
		DWORD errors;
	public:
		SerialConnection(std::string, int, int);
		~SerialConnection();
		void simpleRead();
		void simpleRead(std::ostream&);
		bool connected();
		void disconnect();

		template<typename T>
		int read(T *buffer, unsigned int maxQueueSize) {
			ClearCommError(handle, &errors, &status);

			DWORD amountRead = 0;
			unsigned int queueSize = status.cbInQue;

			if(queueSize > 0) {
				const unsigned int toReadAmount = queueSize > maxQueueSize ? maxQueueSize : queueSize;

				if(!ReadFile(handle, buffer, toReadAmount, &amountRead, NULL)) {
					amountRead = 0;
				}
			}

			buffer[amountRead] = 0;

			return amountRead;
		}

		template<typename T>
		int write(T *buffer, unsigned int maxQueueSize) {
			DWORD amount;

			if(!WriteFile(handle, buffer, maxQueueSize, &amount, 0)) {
				amount = 0;
				ClearCommError(handle, &errors, &status);
			}

			return amount;
		}
};

#endif
