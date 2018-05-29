#include <iostream>

#include "../../FitrArduino/Fitr/Fitr.h"

#include "SerialConnection.h"
#include "SerialHandler.h"
#include "FitrPrint.h"

using namespace std;
using namespace FitrPrint;
using namespace Transmit;

ostream& operator<<(ostream &stream, Vector3 &angles) {
	stream << "Vector3(" << angles.x << ", " << angles.y << ", " << angles.z << ")";

	return stream;
}

void utilize(char code, char *data, int bytes) {
	switch(code) {
		case Code::INT:
			int v;
			decode(v, data, bytes);

			println("Reconstructed ", v, " from ", data);
			break;
		case Code::FLOAT:
			float v1;
			decode(v1, data, bytes);

			println("Reconstructed ", v, " from ", data);
			break;
		default:
			break;
	}
}

int main(int argc, char *argv[]) {
	stringstream port;
	port << "\\\\.\\COM";
	port << (argc < 2 ? "4" : argv[1]);

	int baudRate = argc < 3 ? FITR_BR : atoi(argv[2]);

	cout << "Attempting to connect through \"" << port.str() << "\" at a baud rate of " << baudRate << endl;

	SerialConnection serial(port.str(), baudRate, 2000);
	SerialHandler().handle(serial, utilize);

	return 0;
}
