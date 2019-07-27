#ifndef NEPOSTRINGS
#define NEPOSTRINGS

#include "NEPOColors.h"



    std::string ToString (std::string str) {
        return str;
    }

    std::string ToString (const char * str) {
        return str;
    }

    std::string ToString (bool b) {
        return b ? "true" : "false";
    }


    std::string removeTrailingZeros (std::string str);

    std::string ToString (double d) {
        std::string str = std::to_string(d);
        return removeTrailingZeros(str);
    }

    std::string removeTrailingZeros (std::string str) {
        int lastCharacter = str.length() - 1;
        for (int i = lastCharacter; i > 0; i--) {
            char c = str[i];
            if (c == '.') {
                return str.substr(0, i);
            } else if (c != '0') {
                return str.substr(0, i + 1);
            }
        }
        return str;
    }

    std::string ToString (int n) {
         return std::to_string(n);
    }

    std::string ToString (unsigned long long n) { // used for the times
         return std::to_string(n);
    }

    std::string ToString (Color color) {
        return GetColorName(color);
    }


    template<typename T>
    std::string ToString (std::list<T> values) {
        /**
         * We cannot use stringstream because the executables becomes too large
         */
        std::string str = "[";
        int counter = 0;
        for (T value : values) {
            str += ToString(value);
            if (counter < values.size() - 1) {
                str += ", ";
            }
            counter++;
        }
        str += "]";
        return str;
    }


#endif