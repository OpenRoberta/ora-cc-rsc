#ifndef NEPOLCD
#define NEPOLCD

#include "NEPOColors.h"
#include "NEPOPictures.h"

#define LCD_COLOR_BLACK 1


void DrawString (String str, int x, int y) {
    SetCursorX(x);
    SetCursorY(y);
    LcdPrintf(LCD_COLOR_BLACK, "%s", str.c_str());
}

void DrawString (std::list<String> strings, int x, int y) {
    SetCursorX(x);
    SetCursorY(y);
    int counter = 0;
    for (auto str : strings) {
        LcdPrintf(LCD_COLOR_BLACK, "%s ", str.c_str());
        counter++;
        if (counter < strings.size() - 1) {
            LcdPrintf(LCD_COLOR_BLACK, ",");
        }
    }
}

void DrawNumber (double n, int x, int y) {
    LcdTextf(LCD_COLOR_BLACK, x, y, "%f", n);
}

void DrawNumber (std::list<double> numbers, int x, int y) {
    std::list<std::string> numberStrings;
    for (double n : numbers) {
        numberStrings.push_back(std::to_string(n));
    }
    DrawString(numberStrings, x, y);
}

const char * getBoolAsString (bool b) {
    return b ? "true" : "false";
}

void DrawBool (bool b, int x, int y) {
    LcdTextf(LCD_COLOR_BLACK, x, y, getBoolAsString(b));
}

void DrawBool (std::list<bool> booleans, int x, int y) {
    std::list<std::string> boolStrings;
    for (bool b: booleans) {
        boolStrings.push_back(getBoolAsString(b));
    }
    DrawString(boolStrings, x, y);
}

void DrawColorName (unsigned int color, int x, int y) {
    LcdTextf(LCD_COLOR_BLACK, x, y, GetColorName(color).c_str());
}

void DrawColorName (std::list<unsigned int> colors, int x, int y) {
    std::list<std::string> colorNames;
    for (unsigned int color : colors) {
        colorNames.push_back(GetColorName(color).c_str());
    }
    DrawString(colorNames, x, y);
}

#endif
