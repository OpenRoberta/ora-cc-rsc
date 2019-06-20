#ifndef NEPOSTRINGS
#define NEPOSTRINGS

#include "NEPOColors.h"

#define ToString std::to_string

namespace std {

    std::string to_string (std::string str) {
        return str;
    }

    std::string to_string (const char * str) {
        return str;
    }

    std::string to_string (bool b) {
        return b ? "true" : "false";
    }

    std::list<std::string> to_string (std::initializer_list<std::string> strings) {
        return strings;
    }

    std::string to_string (Color color) {
        return GetColorName(color);
    }
}

#endif
