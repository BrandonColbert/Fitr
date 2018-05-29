#include <iostream>

#include "Fitr.h"

using namespace std;
using namespace Transmit;

ostream& operator<<(ostream &stream, Vector3 &angles) {
	return stream << "Vector3(" << angles.x << ", " << angles.y << ", " << angles.z << ")";
}

ostream& operator<<(ostream &stream, Quaternion &angles) {
	return stream << "Quaternion(" << angles.x << ", " << angles.y << ", " << angles.z << ", " << angles.w << ")";
}

int main() {
	while(true) {
		int value = -21903;
		List<char> data = encode(value);

		//ENCODE SEEMINGLY DESTROYS ITSELF OVER A CERTAIN AMOUNT OF USES AND BECOMES INCONSISTENT

		cout << "~Hello World Test~" << endl;
		cout << (int)Code::INT << endl;
		for(int i = 0; i < data.size(); i++) {
	    	cout << (int)data[i] << endl;
		}
	}

    return 0;
}
/*
void example() {
    float accuracy = 1.0f;

    float trueInitial = 14 / accuracy;
    int initial = trueInitial * accuracy;

    cout << "Initial: " << trueInitial << endl << endl;

    cout << "Float Size: " << sizeof(float) << endl;

    char b1 = initial >> 0;
    char b2 = initial >> 8;
    char b3 = initial >> 16;
    char b4 = initial >> 24;

    cout << (int)b1 << " as " << b1 << endl;
    cout << (int)b2 << " as " << b2 << endl;
    cout << (int)b3 << " as " << b3 << endl;
    cout << (int)b4 << " as " << b4 << endl;

    int end = 0;

    unsigned char c1 = b1;
    unsigned char c2 = b2;
    unsigned char c3 = b3;
    unsigned char c4 = b4;

    end = c4;
    end = end << 8 | c3;
    end = end << 8 | c2;
    end = end << 8 | c1;

    float trueEnd = end / accuracy;

    cout << endl << "Final: " << trueEnd << endl << "--------------" << endl;
}

void example2() {
    double num = 1.345621;
    char *data = encode(num);

    float re = 0;
    decodeNumber(re, data);

    cout << "Transported as " << num << " and ended up as " << re << endl;
}

void example3() {
	Quaternion angles(0.21f, -1.34f, 10.4f, -12.0f);
    char *data = encode(angles);

    Quaternion reAngles;
    decode(reAngles, data);

    cout << "Transported as " << angles << " and ended up as " << reAngles << endl;
}

void example4() {
	List<int> numbers(1, 2, 3, 4, 5);

	cout << "It is" << endl;

	for(int i = 0; i < numbers.size(); i++) {
		cout << numbers[i] << endl;
	}

	numbers.clear();

	cout << "Now it is" << endl;

	numbers.add(6, 7, 8, 9, 10);

	for(int i = 0; i < numbers.size(); i++) {
		cout << numbers[i] << endl;
	}

	cout << "The array is" << endl;

	int *arr = numbers.array();

	for(int i = 0; i < numbers.size(); i++) {
		cout << arr[i] << endl;
	}
}

void example5() {
	int value = 338;
	List<char> data = encode(value);

	cout << (int)Code::INT << endl;
	for(int i = 0; i < data.size(); i++) {
		cout << "\t" << (int)data[i] << endl;
	}

	int remake;
	int size = data[0];
	data.remove(0);
	decode(remake, data.array(), size);

	cout << remake << endl;
}

*/
