#ifndef NEPOCOLORS
#define NEPOCOLORS

std::string GetColorName (Color color) {
	switch(color) {
	    case ColorBlack:
	      return "Black";
	    case ColorBlue:
	      return "Blue";
	    case ColorGreen:
	      return "Green";
	    case ColorYellow:
	      return "Yellow";
	    case ColorRed:
	      return "Red";
	    case ColorWhite:
	      return "White";
	    case ColorBrown:
	      return "Brown";
	    case ColorNone:
	    default:
	      return "None";
  }
}

Color mapHTColorIdToColor(int colorId) {
    switch(colorId) {
		case 0:
            return ColorBlack;
        case 1:
            return ColorRed;
        case 2:
        case 3:
            return ColorBlue;
        case 4:
            return ColorGreen;
        case 5:
        case 6:
            return ColorYellow;
        case 7:
        case 8:
        case 9:
        case 10:
            return ColorRed;
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
            return ColorWhite;
        default:
            return ColorNone;
	}
}

#endif
