#include "SerialConnection.h"

using namespace std;

SerialConnection::SerialConnection(string port, int baudRate, int waitTime) {
	_connected = false;
	handle = CreateFile(port.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if(handle == INVALID_HANDLE_VALUE) {
		if(GetLastError() == ERROR_FILE_NOT_FOUND) {
			cout << "Port " << port << "unavailable" << endl;
		}

		cout << "Unable to connect" << endl;
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

				cout << "Now connected!" << endl;
			}
			else {
				cout << "Unable to set serial parameters" << endl;
			}
		} else {
			cout << "Unable to acquire serial parameters" << endl;
		}
	}

}

SerialConnection::~SerialConnection() {
	if(_connected) {
		_connected = false;
		CloseHandle(handle);
	}
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
