#include "FitrManager.h"

using namespace std;
using namespace FitrPrint;
using namespace Transmit;

FitrManager::FitrManager(int argc, char *argv[]) : hand(new Hand(5)) {
	portNumber = argc < 2 ? "4" : argv[1];
	baudRate = argc < 3 ? FITR_BR : atoi(argv[2]);

	thread([&](FitrManager &manager) {
		std::stringstream port;
		port << "\\\\.\\COM";
		port << manager.portNumber;

		println("Attempting to connect through \"", port.str(), "\" at a baud rate of ", manager.baudRate);

		manager.serial = new SerialConnection(port.str(), manager.baudRate, 2000);
		SerialHandler().handleWith(*manager.serial, manager);
	}, std::ref(*this)).detach();
}

FitrManager::~FitrManager() {
	stop();
	delete hand;
	delete serial;
}

Hand& FitrManager::getHand() {
	return *hand;
}

void FitrManager::stop() {
	serial->disconnect();
}

ostream& operator<<(ostream &stream, Quaternion &angles) {
	return stream << "Quaternion(" << angles.x << ", " << angles.y << ", " << angles.z << ", " << angles.w << ")";
}

void FitrManager::utilize(char code, char *data, int bytes) {
	auto fingerFlex = [&](int index) {
		float v;
		decodeFloat(v, data, bytes);
		(*hand)[index].flex = v;
	};

	auto fingerRot = [&](int index) {
		Quaternion rot;
		decodeQuaternion(rot, data, bytes);
		(*hand)[index].rotation = rot;
	};

	switch(code) {
		case Code::INT:
			{
				int v;
				decodeInt(v, data, bytes);
				println("Reconstructed ", v, " from ", data);
			}
			break;
		case Code::FLOAT:
			{
				float v;
				decodeFloat(v, data, bytes);
				println("Reconstructed ", v, " from ", data);
			}
			break;
		case Code::FINGER_1_F: fingerFlex(0); break;
		case Code::FINGER_2_F: fingerFlex(1); break;
		case Code::FINGER_3_F: fingerFlex(2); break;
		case Code::FINGER_4_F: fingerFlex(3); break;
		case Code::FINGER_5_F: fingerFlex(4); break;
		case Code::FINGER_1_R: fingerRot(0); break;
		case Code::FINGER_2_R: fingerRot(1); break;
		case Code::FINGER_3_R: fingerRot(2); break;
		case Code::FINGER_4_R: fingerRot(3); break;
		case Code::FINGER_5_R: fingerRot(4); break;
		default:
			break;
	}

	auto showFlex = [&](int n) {
		int max = 25;

		int size = n / 4 > max ? max : n / 4;

		for (int i = 0; i < size; i++) {
			print("-");
		}

		for (int i = max + 10 - size; i >= 0; i--) {
			print(" ");
		}
	};

	//showFlex((int)(*hand)[0].flex);
	//showFlex((int)(*hand)[1].flex);
	//showFlex((int)(*hand)[2].flex);
	//showFlex((int)(*hand)[3].flex);
	//showFlex((int)(*hand)[4].flex);

	//cout << endl;

	//println("Flex(", (int)code, "): ", (*hand)[1].flex);

	//println("Code: ", (int)code, " and thumb rotation is ", (*hand)[0].rotation);
}