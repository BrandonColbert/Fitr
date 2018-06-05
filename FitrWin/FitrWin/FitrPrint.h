#ifndef FITR_PRINT_H
#define FITR_PRINT_H

#include <iostream>
#include <ostream>

namespace FitrPrint {
	template<typename T>
	void print(T t) {
		std::cout << t;
		//logFile << t;
	}

	template<typename T, typename ...Ts>
	void print(T t, Ts ...ts) {
		print(t);
		print(ts...);
	}

	/*
	void print() {
		print("");
	}
	*/

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

	/*
	void println() {
		print("\n");
	}
	*/
}

#endif