#ifndef NEPOSTRINGS
#define NEPOSTRINGS

#include "NEPOColors.h"

#define ToString std::to_string

namespace std {

    string to_string (string str) {
        return str;
    }

    string to_string (const char * str) {
        return str;
    }

    string to_string (bool b) {
            return b ? "true" : "false";
    }

    list<std::string> to_string (initializer_list<string> strings) {
        return strings;
    }

    string to_string (Color color) {
        return GetColorName(color);
    }


    list<string> to_string (list<double> numbers) {
        list<string> strings;
        for (double number : numbers) {
            strings.push_back(ToString(number));
        }
        return strings;
    }
}

#endif