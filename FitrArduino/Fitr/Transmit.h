#ifndef TRANSMIT_H
#define TRANSMIT_H

#include "Angles.h"
#include "List.h"

#define FITR_BR 9600
#define TC_START "TC_START"

namespace Transmit {
    namespace Code {
        const char NONE = 0,
                   INT = 1,
                   FLOAT = 2;
    }

    template<typename Number>
    static int getDecimals(Number n) {
        int p = 1, a = 0;

        while((int)(n * p) != n * p) {
            p *= 10;
            a++;
        }

        return a;
    }

    static int getAccuracy(int decimals) {
        int a = 1;

        while(decimals > 0) {
            a *= 10;
            decimals--;
        }

        return a;
    }

    static List<char> encode(int info) {
        List<char> list;

        for(int i = 0; i < 4; i++) {
            list.add(info >> i * 8);
        }

        return list;
    }

    static void decode(int &dest, char *data, int length) {
        for(int i = length - 1; i >= 0; i--) {
            dest = dest << 8 | (unsigned char)data[i];
        }
    }

    static List<char> encode(float info) {
        List<char> list;

        int decimals = getDecimals(info);
        int representation = (int)(info * getAccuracy(decimals));

        for(int i = 0; i < 5; i++) {
            list.add(i == 0 ? (char)decimals : (char)(representation >> (i - 1) * 8));
        }

        return list;
    }

    static void decode(float &dest, char *data, int length) {
        int accuracy = getAccuracy((unsigned char)data[0]);
        int info = 0;

        for(int i = length - 1; i > 0; i--) {
            info = info << 8 | (unsigned char)data[i];
        }

        dest = (float)info / accuracy;
    }
}

#endif

/*
    char* encode(float info) {
        int decimals = getDecimals(info),
            accuracy = getAccuracy(decimals),
            numberSize = sizeof(Number),
            bits = (int)(info * accuracy);

        char *data = new char[numberSize + 1];

        for(int i = 0; i < numberSize + 1; i++) {
            data[i] = i == 0 ? (char)decimals : (char)(bits >> (i - 1) * 8);
        }

        return data;
    }

    template<typename Number>
    void decodeNumber(Number &dest, char *data) {
        int numberSize = sizeof(Number),
            accuracy = getAccuracy((unsigned char)data[0]),
            info = 0;

        for(int i = numberSize; i > 0; i--) {
            info = info << 8 | (unsigned char)data[i];
        }

        dest = (Number)info / accuracy;
    }

    char* encode(Vector3 &info) {
        int size = sizeof(float) + 1;

        char *data = new char[size * 3];

        for(int i = 0; i < 3; i++) {
            char *p = nullptr;

            switch(i) {
                case 0:
                    p = encodeNumber(info.x);
                    break;
                case 1:
                    p = encodeNumber(info.y);
                    break;
                case 2:
                    p = encodeNumber(info.z);
                    break;
            }

            for(int j = 0; j < size; j++) {
                data[i * size + j] = p[j];
            }
        }

        return data;
    }

    void decode(Vector3 &dest, char *data) {
        const int size = sizeof(float) + 1;

        for(int i = 0; i < 3; i++) {
            float *p = nullptr;

            switch(i) {
                case 0:
                    p = &dest.x;
                    break;
                case 1:
                    p = &dest.y;
                    break;
                case 2:
                    p = &dest.z;
                    break;
            }

            char pd[size];

            for(int j = 0; j < size; j++) {
                pd[j] = data[i * size + j];
            }

            decodeNumber<float>(*p, pd);
        }
    }

    char* encode(Quaternion &info) {
        int size = sizeof(float) + 1;

        char *data = new char[size * 4];

        for(int i = 0; i < 4; i++) {
            char *p = nullptr;

            switch(i) {
                case 0:
                    p = encodeNumber(info.x);
                    break;
                case 1:
                    p = encodeNumber(info.y);
                    break;
                case 2:
                    p = encodeNumber(info.z);
                    break;
                case 3:
                    p = encodeNumber(info.w);
                    break;
            }

            for(int j = 0; j < size; j++) {
                data[i * size + j] = p[j];
            }
        }

        return data;
    }

    void decode(Quaternion &dest, char *data) {
        const int size = sizeof(float) + 1;

        for(int i = 0; i < 4; i++) {
            float *p = nullptr;

            switch(i) {
                case 0:
                    p = &dest.x;
                    break;
                case 1:
                    p = &dest.y;
                    break;
                case 2:
                    p = &dest.z;
                    break;
                case 3:
                    p = &dest.w;
                    break;
            }

            char pd[size];

            for(int j = 0; j < size; j++) {
                pd[j] = data[i * size + j];
            }

            decodeNumber<float>(*p, pd);
        }
    }

    char* encode(AccelGyro &info) {
        const int size = sizeof(float) + 1;

        char *data = new char[size * 6];

        for(int i = 0; i < 6; i++) {
            char *p = nullptr;

            switch(i) {
                case 0:
                    p = encodeNumber(info.ax);
                    break;
                case 1:
                    p = encodeNumber(info.ay);
                    break;
                case 2:
                    p = encodeNumber(info.az);
                    break;
                case 3:
                    p = encodeNumber(info.gx);
                    break;
                case 4:
                    p = encodeNumber(info.gy);
                    break;
                case 5:
                    p = encodeNumber(info.gz);
                    break;
            }

            for(int j = 0; j < size; j++) {
                data[i * size + j] = p[j];
            }
        }

        return data;
    }

    void decode(AccelGyro &dest, char *data) {
        const int size = sizeof(float) + 1;

        for(int i = 0; i < 4; i++) {
            float *p = nullptr;

            switch(i) {
                case 0:
                    p = &dest.ax;
                    break;
                case 1:
                    p = &dest.ay;
                    break;
                case 2:
                    p = &dest.az;
                    break;
                case 3:
                    p = &dest.gx;
                    break;
                case 4:
                    p = &dest.gy;
                    break;
                case 5:
                    p = &dest.gz;
                    break;
            }

            char pd[size];

            for(int j = 0; j < size; j++) {
                pd[j] = data[i * size + j];
            }

            decodeNumber<float>(*p, pd);
        }
    }

    template<typename T, typename ...Ts>
    char* encode(T &info, Ts &...extra) {
        List<char*> data;

        data.add(encode(info));
        data.add(encode(extra...));

        return data;
    }
    */
