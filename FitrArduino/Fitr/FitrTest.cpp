#include <iostream>
#include <iomanip>

#include "Fitr.h"

using namespace std;
using namespace Transmit;

ostream& operator<<(ostream &stream, Vector3 &angles) {
	return stream << "Vector3(" << angles.x << ", " << angles.y << ", " << angles.z << ")";
}

ostream& operator<<(ostream &stream, Quaternion &angles) {
	return stream << "Quaternion(" << angles.x << ", " << angles.y << ", " << angles.z << ", " << angles.w << ")";
}

ostream& operator<<(ostream &stream, AccelGyro &angles) {
	return stream << "AccelGyro(" << angles.ax << ", " << angles.ay << ", " << angles.az << ", " << angles.gx << ", " << angles.gy << ", " << angles.gz  << ")";
}

int main() {
	float value = 20734.52474f;
	List<char> data = encodeFloat(value);

	float re;
	char *arr = data.array();
	decodeFloat(re, arr, data.size());
	delete arr;
	data.clear();

	cout << "Reconstructed " << setprecision(12) << re << ", original was " << value << endl;

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

void example6() {
	Hand *hand = new Hand(5);

	cout << "Created hands" << endl;

	(*hand)[0].flex = 1.2f;
	(*hand)[1].flex = 0.1f;
	(*hand)[2].flex = 100.0f;
	(*hand)[3].flex = 38.323f;
	(*hand)[4].flex = 21017.28f;

	cout << "Set hand flexes" << endl;

	for(int i = 0; i < hand->totalFingers; i++) {
		cout << "Finger(" << i << "): " << (*hand)[i].flex << endl;
	}

	cout << "Finished reading hand flex values" << endl;
}

void example7() {
	List<int> list1(19, 1, 83, 29);

	cout << "\tList 1 size: " << list1.size() << endl;
	cout << "\tList 1 is:" << endl;

	for(int i = 0; i < list1.size(); i++) {
		cout << list1[i] << endl;
	}

	List<int> list2(-219, 18, 90, 232, -12);

	cout << "\tList 2 size: " << list2.size() << endl;
	cout << "\tList 2 is:" << endl;

	for(int i = 0; i < list2.size(); i++) {
		cout << list2[i] << endl;
	}

	list1.addAll(list2);
	list1.addAll(List<int>(10000, -1234, 919, 812, -9));

	cout << "\tAdded list 2 and some other numbers to list 1 " << endl;
	cout << "\tList 1 size: " << list1.size() << endl;
	cout << "\tList 1 is:" << endl;

	for(int i = 0; i < list1.size(); i++) {
		cout << "At " << i << ": " << list1[i] << endl;
	}

	int start1 = 6, end1 = 11;
	cout << "\tGetting indices " << start1 << " to " << end1 << " from list 1" << endl;
	int *arr1 = list1.subset(start1, end1).array();
	for(int i = 0; i < 1 + end1 - start1; i++) {
		cout << "At " << i << ": " << arr1[i] << endl;
	}

	delete[] arr1;
}

void example8() {
	AccelGyro a1(1.2f, -13.f, 239.32f, 239234.2130f, 12.283f, -128.1f);
	List<char> data1 = encode(a1);

	AccelGyro rea1;
	char *arr1 = data1.array();
	decode(rea1, arr1, data1.size());

	data1.clear();
	delete[] arr1;

	cout << "Transported as " << a1 << " and ended up as " << rea1 << endl;

	Vector3 a2(1.2f, -13.f, 239.32f);
	List<char> data2 = encode(a2);

	Vector3 rea2;
	char *arr2 = data2.array();
	decode(rea2, arr2, data2.size());

	data2.clear();
	delete[] arr2;

	cout << "Transported as " << a2 << " and ended up as " << rea2 << endl;

	Quaternion a3(1.2f, -13.f, 239.32f, -12.12f);
	List<char> data3 = encode(a3);

	Quaternion rea3;
	char *arr3 = data3.array();
	decode(rea3, arr3, data3.size());

	data3.clear();
	delete[] arr3;

	cout << "Transported as " << a3 << " and ended up as " << rea3 << endl;
}

*/
