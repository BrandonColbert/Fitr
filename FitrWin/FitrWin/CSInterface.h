#include "FitrManager.h"

#define DllExport __declspec(dllexport)

static FitrManager *manager = nullptr;

extern "C" {
	DllExport void Connect() {
		char *p[1] = {};
		manager = new FitrManager(0, p);
	}

	DllExport bool IsConnected() {
		return manager != nullptr;
	}

	DllExport void Disconnect() {
		if (manager != nullptr) {
			manager->stop();
		}
	}

	DllExport Hand* GetHand() {
		return &manager->getHand();
	}

	DllExport Finger* GetFinger(Hand *hand, int finger) {
		return &(*hand)[finger];
	}

	DllExport float GetFingerFlex(Finger *finger) {
		return finger->flex;
	}

	DllExport Quaternion* GetFingerRotation(Finger *finger) {
		return &finger->rotation;
	}

	DllExport float GetQuaternionX(Quaternion *rotation) {
		return rotation->x;
	}

	DllExport float GetQuaternionY(Quaternion *rotation) {
		return rotation->y;
	}

	DllExport float GetQuaternionZ(Quaternion *rotation) {
		return rotation->z;
	}

	DllExport float GetQuaternionW(Quaternion *rotation) {
		return rotation->w;
	}
}