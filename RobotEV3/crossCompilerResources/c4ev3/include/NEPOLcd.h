#ifndef NEPOLCD
#define NEPOLCD

#include "NEPOColors.h"
#include "NEPOPictures.h"

#define LCD_COLOR_BLACK 1


void DrawString (std::string str, int x, int y) {
    SetCursorX(x);
    SetCursorY(y);
    LcdPrintf(LCD_COLOR_BLACK, "%s", str.c_str());
}

template<typename T>
void DrawString (std::list<T> strings, int x, int y) {
    SetCursorX(x);
    SetCursorY(y);
    int counter = 0;
    for (auto str : strings) {
        LcdPrintf(LCD_COLOR_BLACK, "%s ", ToString(str).c_str());
        counter++;
        if (counter < strings.size() - 1) {
            LcdPrintf(LCD_COLOR_BLACK, ",");
        }
    }
}



#endif
