#include "SerialConnection.h"

using namespace std;
using namespace FitrPrint;

SerialConnection::SerialConnection(string port, int baudRate, int waitTime) {
	_connected = false;

	pointOfConnect:
	handle = CreateFile(port.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if(handle == INVALID_HANDLE_VALUE) {
		if(GetLastError() == ERROR_FILE_NOT_FOUND) {
			int waitTime = 3;
			println("Port ", port, " unavailable, reattempting in ", waitTime, " seconds");
			Sleep(waitTime * 1000);
			goto pointOfConnect;
		} else {
			println("Unable to connect");
			exit(1);
		}
	} else {
		DCB serialParams = {0};

		if(GetCommState(handle, &serialParams)) {
			serialParams.BaudRate = baudRate;
			serialParams.ByteSize = 8;
			serialParams.StopBits = ONESTOPBIT;
			serialParams.Parity = NOPARITY;
			serialParams.fDtrControl = DTR_CONTROL_ENABLE;

			if (SetCommState(handle, &serialParams)) {
				_connected = true;
				PurgeComm(handle, PURGE_RXCLEAR | PURGE_TXCLEAR);
				Sleep(waitTime);

				println("Now connected!");
			}
			else {
				println("Unable to set serial parameters");
			}
		} else {
			println("Unable to acquire serial parameters");
		}
	}

}

SerialConnection::~SerialConnection() {
	disconnect();
}

void SerialConnection::simpleRead() {
	simpleRead(cout);
}

void SerialConnection::simpleRead(ostream &stream) {
	char buffer[256] = "";

	if(read(buffer, 255) > 0) {
		stream << buffer;
	}
}

bool SerialConnection::connected() {
	return _connected;
}

void SerialConnection::disconnect() {
	if(_connected) CloseHandle(handle);
	_connected = false;
}