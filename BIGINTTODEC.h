#pragma once
#pragma once
#ifndef BIGINTTODEC_H
#define BIGINTTODEC_H

#include <iostream>
#include <sstream>
#include <bitset>
//#include "BigInt.h"
using namespace std;

class Conversion {
public:
    Conversion() {}

    void fromHex(const string& hex) {
        for (char c : hex) {
            int value;
            if (c >= '0' && c <= '9') {
                value = c - '0';
            }
            else if (c >= 'a' && c <= 'f') {
                value = c - 'a' + 10;
            }
            else if (c >= 'A' && c <= 'F') {
                value = c - 'A' + 10;
            }
            else {
                cerr << "Invalid hex character: " << c << endl;
                return;
            }

            multiplyBy16();
            add(value);
        }
    }

    string toString() const {
        stringstream ss;
        ss << dec;

        int i = size - 1;
        // Find the first non-zero digit
        while (i >= 0 && digits[i] == 0) {
            --i;
        }

        // Output digits
        if (i >= 0) {
            ss << digits[i--];
            while (i >= 0) {
                ss << digits[i--];
            }
        }
        else {
            // If the number is zero, output a single '0'
            ss << '0';
        }

        return ss.str();
    }

    string toBinary() const {
        stringstream ss;
        ss << dec;

        int i = size - 1;
        // Find the first non-zero digit
        while (i >= 0 && digits[i] == 0) {
            --i;
        }

        // Output digits in binary format
        if (i >= 0) {
            ss << bitset<4>(digits[i--]);  // Assuming each digit corresponds to 4 bits
            while (i >= 0) {
                ss << bitset<4>(digits[i--]);  // Assuming each digit corresponds to 4 bits
            }
        }
        else {
            // If the number is zero, output a single '0'
            ss << '0';
        }

        return ss.str();
    }

private:
    static const int size = 100;  // Adjust the size as needed
    int digits[size] = { 0 };

    void multiplyBy16() {
        for (int i = 0; i < size; ++i) {
            digits[i] *= 16;
        }

        normalize();
    }

    void add(int value) {
        digits[0] += value;

        normalize();
    }

    void normalize() {
        for (int i = 0; i < size - 1; ++i) {
            if (digits[i] >= 10) {
                digits[i + 1] += digits[i] / 10;
                digits[i] %= 10;
            }
        }
    }
};


#endif TODEC_H