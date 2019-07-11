#ifndef NEPOLCD
#define NEPOLCD

#include "NEPOColors.h"
#include "NEPOPictures.h"

#define LCD_COLOR_BLACK 1
#define DEFAULT_FONT_CHARACTER_HEIGHT 10
#define DEFAULT_FONT_CHARACTER_WIDTH 8

void SetCursorColumn (int column) {
    SetCursorX(column * DEFAULT_FONT_CHARACTER_WIDTH);
}

void SetCursorRow (int row) {
    SetCursorY(row * DEFAULT_FONT_CHARACTER_HEIGHT);
}

void DrawString (std::string str, int column, int row) {
    SetCursorColumn(column);
    SetCursorRow(row);
    LcdPrintf(LCD_COLOR_BLACK, "%s", str.c_str());
}

template<typename T>
void DrawString (std::list<T> strings, int column, int row) {
    SetCursorColumn(column);
    SetCursorRow(row);
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
