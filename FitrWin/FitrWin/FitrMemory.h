#ifndef FITR_MEMORY_H
#define FITR_MEMORY_H

#include <Windows.h>
#include <cstdio>
#include <string>
#include "Fitr.h"
#include "FitrPrint.h"

using namespace FitrPrint;

class FitrMemory {
	private:
		std::string mapName;
		HANDLE map;
		PVOID sharedMemory;
		unsigned int handSize;
	public:
		FitrMemory(std::string name, int fingers) {
			handSize = (1 + fingers) * 4 + fingers;

			println("Creating shareable memory pool named ", name, " of size ", handSize);

			mapName = name;
			map = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, handSize, name.c_str());
			sharedMemory = MapViewOfFile(map, FILE_MAP_ALL_ACCESS, 0, 0, handSize * sizeof(float));

			if(map == NULL || sharedMemory == NULL) {
				println("It didn't work!");

				CloseHandle(map);
				throw std::exception("Unable to create memory map");
			}
		}

		~FitrMemory() {
			mapName = "";
			CloseHandle(map);
			delete sharedMemory;
			handSize = 0;
		}

		void sync(Hand &hand) {
			float *data = new float[handSize];

			int index = 0;

			auto cf = [&](float f) {
				data[index] = f;
				index++;
			};

			auto cq = [&](Quaternion q) {
				cf(q.x);
				cf(q.y);
				cf(q.z);
				cf(q.w);
			};

			cq(hand.rotation);

			for(int i = 0; i < hand.totalFingers; i++) {
				cf(hand[i].flex);
				cq(hand[i].rotation);
			}

			CopyMemory(sharedMemory, data, handSize * sizeof(float));
			delete data;

			/*
			println("Block");
			for(int i = 0; i < (int)handSize; i++) println("\t", i, ": ", ((float*)sharedMemory)[i]);
			println("");
			*/
		}
};

#endif