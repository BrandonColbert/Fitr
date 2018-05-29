#include "SerialHandler.h"

using namespace std;
using namespace Transmit;
using namespace FitrPrint;

void SerialHandler::handle(SerialConnection serial, void (*utilize)(char, char*, int)) {
	bool ensure = false;

	char transmitCode = Code::NONE;
	int amount = -1;
	List<char> data;

	while (serial.connected()) {
		char buffer[1] = "";

		if (serial.read(buffer, 1) > 0) {
			if (ensure) {
				if (transmitCode == Code::NONE) {
					transmitCode = buffer[0];
				}
				else if (amount == -1) {
					amount = buffer[0];

					println("Found code ", (int)transmitCode, " with size of ", amount, " bytes");
				}
				else if (amount > 0) {
					amount--;

					data.add(buffer[0]);

					if (amount == 0) {
						for (int i = 0; i < data.size(); i++) {
							println("\t-> ", (int)data[i]);
						}

						char *dataArray = data.array();
						utilize(transmitCode, dataArray, data.size());
						delete dataArray;

						transmitCode = Code::NONE;
						amount = -1;
						data.clear();
					}
				}
			}
		}
		else if (!ensure) {
			ensure = true;
			cout << "Waking up the device" << endl;

			char wake[] = TC_START;
			if (serial.write(wake, strlen(wake)) > 0) {
				cout << "Reading..." << endl;
			}
			else {
				cout << "Device not responding?" << endl;
				exit(1);
			}
		}
	}
}