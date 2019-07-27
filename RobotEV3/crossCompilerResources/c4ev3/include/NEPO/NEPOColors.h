#ifndef NEPOCOLORS
#define NEPOCOLORS

/* Colors not already defined in c4ev3 library */
#ifndef INPUT_BROWNCOLOR
#define INPUT_BROWNCOLOR 7
#endif

#ifndef INPUT_ORANGECOLOR
#define INPUT_ORANGECOLOR 8
#endif

#ifndef INPUT_LIMECOLOR
#define INPUT_LIMECOLOR 9
#endif

#ifndef INPUT_VIOLETCOLOR
#define INPUT_VIOLETCOLOR 10
#endif

#ifndef INPUT_CRIMSONCOLOR
#define INPUT_CRIMSONCOLOR 11
#endif

#ifndef INPUT_PURPLECOLOR
#define INPUT_PURPLECOLOR 12
#endif

std::string GetColorName (Color color) {
	switch(color) {
	    case None:
	        return "None";
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
	    case Orange:
	      return "Orange";
	    case Lime:
	      return "Lime";
	    case Violet:
	      return "Violet";
	    case Crimson:
	      return "Crimson";
	    case Purple:
	      return "Purple";
	    case Grey:
	    default:
	      return "Grey";
  }
}

Color GetColorFromC4ev4Int (int c4ev3Color) {
    switch(c4ev3Color) {
	    case 0:
	        return None;
	    case 1:
	      return Black;
	    case 2:
	      return Blue;
	    case 3:
	      return Green;
	    case 4:
	      return Yellow;
	    case 5:
	      return Red;
	    case 6:
	      return White;
	    case 7:
	      return Brown;
	    case 8:
	      return Orange;
	    case 9:
	      return Lime;
	    case 10:
	      return Violet;
	    case 11:
	      return Crimson;
	    case 12:
	      return Purple;
	    case 13:
	    case 14:
	    case 15:
	    case 16:
	    case 17:
	      return White;
	    default:
	      return Grey;
  }
}

#endif
