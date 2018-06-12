#include "FitrManager.h"

using namespace std;
using namespace FitrPrint;
using namespace Transmit;

FitrManager::FitrManager(int argc, char *argv[]) : hand(new Hand(5)) {
	portNumber = argc < 2 ? "4" : argv[1];
	baudRate = argc < 3 ? FITR_BR : atoi(argv[2]);
	fitrMem = new FitrMemory(argc < 4 ? "FitrSharedMemory" : argv[3], hand->totalFingers);
}

FitrManager::~FitrManager() {
	stop();
	delete hand;
	delete serial;
	delete fitrMem;
}

Hand& FitrManager::getHand() {
	return *hand;
}

void FitrManager::start() {
	if(serial == nullptr) {
		lastSyncTime = 0LL;

		thread([&](FitrManager &manager) {
			std::stringstream port;
			port << "\\\\.\\COM";
			port << manager.portNumber;

			println("Attempting to connect through \"", port.str(), "\" at a baud rate of ", manager.baudRate);

			manager.serial = new SerialConnection(port.str(), manager.baudRate, 2000);
			SerialHandler().handleWith(*manager.serial, manager);
		}, std::ref(*this)).detach();
	}
}

void FitrManager::stop() {
	serial->disconnect();
}

void FitrManager::utilize(char code, char *data, int bytes) {
	auto fingerFlex = [&](int index) {
		decodeInt((*hand)[index].flex, data, bytes);
	};

	auto fingerRot = [&](int index) {
		decodeQuaternion((*hand)[index].rotation, data, bytes);
	};

	switch(code) {
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
		case Code::PALM_R:
			decodeQuaternion(hand->rotation, data, bytes);
			break;
		default:
			break;
	}

	if(code == Code::PALM_R) {
		fitrMem->sync(*hand);

		auto getTime = [&] {
			return chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now().time_since_epoch()).count();
		};

		long long time = getTime();

		if(lastSyncTime > 0) {
			double csr = 1.0 / ((double)(time - lastSyncTime) / 1000.0);

			if(csr > 0) {
				if(syncRate < 0) {
					syncRate = csr;
				} else {
					syncRate = (csr + syncRate) / 2.0;
				}
			}
		}

		if((int)syncRate < 0) {
			syncRate = 0;
			//println("Lagging, need to catch up!!!");
		} else {
			println("Sync Rate: ", (int)syncRate, "sps (syncs per second)");
		}

		lastSyncTime = getTime();
	}
}