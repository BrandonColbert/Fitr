#ifndef FITR_PRINT_H
#define FITR_PRINT_H

#include <Arduino.h>

namespace FitrPrint {
	template<typename T>
	void print(T t) {
		Serial.print(t);
	}

	template<typename T, typename ...Ts>
	void print(T t, Ts ...ts) {
		print(t);
		print(ts...);
	}

	template<typename T>
	void println(T t) {
		print(t);
		print("\n");
	}

	template<typename T, typename ...Ts>
	void println(T t, Ts ...ts) {
		print(t);
		print(ts...);
		print("\n");
	}
}

#endif
