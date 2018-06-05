#ifndef FITR_TRANSMIT_H
#define FITR_TRANSMIT_H

#include "Quaternion.h"
#include "List.h"

#define FITR_BR 115200
//#define FITR_BR 9600
#define TC_START "TC_START"

#define FITR_FAST_INTS
//#define FITR_PRECISE_FLOATS

namespace Transmit {
    namespace Code {
        const char NONE = 0,
                   INT = 1,
                   FLOAT = 2,
                   FINGER_1_F = 3,
                   FINGER_2_F = 4,
                   FINGER_3_F = 5,
                   FINGER_4_F = 6,
                   FINGER_5_F = 7,
                   FINGER_1_R = 8,
                   FINGER_2_R = 9,
                   FINGER_3_R = 10,
                   FINGER_4_R = 11,
                   FINGER_5_R = 12,
				   PALM_R = 13;
    }

    //Transmit: Integer
    static List<char> encodeInt(int info) {
        List<char> list;

        for(int i = 0; i < 4; i++) {
            char d = info >> i * 8;

#ifdef FITR_FAST_INTS
            if(d == 0) {
                return list;
            }
#endif

            list.add(d);
        }

        return list;
    }

    static void decodeInt(int &dest, char *data, int length) {
#ifdef FITR_FAST_INTS
        if(length == 0) {
            dest = 0;
        } else {
            for(int i = 3; i >= 0; i--) {
                dest = dest << 8 | (i > (length - 1) ? 0 : (unsigned char)data[i]);
            }
        }
#else
        for(int i = length - 1; i >= 0; i--) {
            dest = dest << 8 | (unsigned char)data[i];
        }
#endif
    }

    //Transmit: Float
    static List<char> encodeFloat(float info) {
#ifdef FITR_PRECISE_FLOATS
        List<char> list;

        if(info != info) {
            info = 0;
        }

        int whole = (int)info;
        int fraction = (int)((double)(info - whole) * 10000.0);

        list.addAll(encodeInt(whole));
        list.addAll(encodeInt(fraction));

        return list;
#else
        return encodeInt((int)((double)info * 100.0));
#endif
    }

    static void decodeFloat(float &dest, char *data, int length) {
#ifdef FITR_PRECISE_FLOATS
        List<char> list;

        for(int i = 0; i < length; i++) {
            list.add(data[i]);
        }

        int ss = length / 2;

		char *s1 = list.subset(ss * 0, ss * 1 - 1).array();
        char *s2 = list.subset(ss * 1, ss * 2 - 1).array();

        int whole;
        int fraction;

        decodeInt(whole, s1, ss);
        decodeInt(fraction, s2, ss);

        delete[] s1;
        delete[] s2;

        dest = (float)((double)whole + (double)fraction / 10000.0);
#else
        int v;
        decodeInt(v, data, length);
        dest = (float)((double)v / 100.0);
#endif
    }

    //Transmit: Quaternion
    static List<char> encodeQuaternion(Quaternion &info) {
        List<char> list;

        list.addAll(encodeFloat(info.x));
        list.addAll(encodeFloat(info.y));
        list.addAll(encodeFloat(info.z));
        list.addAll(encodeFloat(info.w));

        return list;
    }

    static void decodeQuaternion(Quaternion &dest, char *data, int length) {
        List<char> list;

        for(int i = 0; i < length; i++) {
            list.add(data[i]);
        }

        int ss = length / 4;

        char *s1 = list.subset(ss * 0, ss * 1 - 1).array();
        char *s2 = list.subset(ss * 1, ss * 2 - 1).array();
        char *s3 = list.subset(ss * 2, ss * 3 - 1).array();
        char *s4 = list.subset(ss * 3, ss * 4 - 1).array();

        decodeFloat(dest.x, s1, ss);
        decodeFloat(dest.y, s2, ss);
        decodeFloat(dest.z, s3, ss);
        decodeFloat(dest.w, s4, ss);

        delete[] s1;
        delete[] s2;
        delete[] s3;
        delete[] s4;
    }
}

#endif
