#ifndef NEPOLCD
#define NEPOLCD

#include "NEPOColors.h"

#define TEXT_COLOR_WHITE 0
#define TEXT_COLOR_BLACK 1

inline void LcdTextNum (float n, int x, int y) {
	LcdTextf(TEXT_COLOR_BLACK, x, y, "%f", n);
}

inline void LcdTextString (char * str, int x, int y) {
	// TODO: Handle text overflow (use TermPrintf)
	LcdTextf(TEXT_COLOR_BLACK, x, y, "%s", str);
}

inline void LcdTextBool (bool b, int x, int y) {
	LcdTextf(TEXT_COLOR_BLACK, x, y, b ? "true" : "false");
}

inline void LcdTextColor (int color, int x, int y) {
	LcdTextf(TEXT_COLOR_BLACK, x, y, getColorName(color));
}


#define EYESOPEN "/home/root/lms2012/prjs/BrkProg_SAVE/NEPO-EYESOPEN.bmp"
#define OLDGLASSES "/home/root/lms2012/prjs/BrkProg_SAVE/NEPO-EYESOPEN.bmp"


#endif
