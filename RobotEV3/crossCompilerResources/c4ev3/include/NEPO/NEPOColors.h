#ifndef NEPOCOLORS
#define NEPOCOLORS

std::string GetColorName (Color color) {
	switch(color) {
	    case Black:
	      return "Black";
	    case Blue:
	      return "Blue";
	    case Green:
	      return "Green";
	    case Yellow:
	      return "Yellow";
	    case Red:
	      return "Red";
	    case White:
	      return "White";
	    case Brown:
	      return "Brown";
	    case None:
	    default:
	      return "None";
  }
}

Color mapHTColorIdToColor(int colorId) {
    switch(colorId) {
		case 0:
            return Black;
        case 1:
        case 2:
        case 3:
            return Blue;
        case 4:
            return Green;
        case 5:
        case 6:
            return Yellow;
        case 7:
            return Red;
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
            return White;
        default:
            return None;
	}
}

#endif
