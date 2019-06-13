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

char * getColorName (int color) {
	switch(color) {
	    case 0:
	        return "Transparent";
	    case 1:
	      return "Black";
	    case 2:
	      return "Blue";
	    case 3:
	      return "Green";
	    case 4:
	      return "Yellow";
	    case 5:
	      return "Red";
	    case 6:
	      return "White";
	    case 7:
	      return "Brown";
	    case 8:
	      return "Orange";
	    case 9:
	      return "Lime";
	    case 10:
	      return "Violet";
	    case 11:
	      return "Crimson";
	    case 12:
	      return "Purple";
	    case 13:
	    case 14:
	    case 15:
	    case 16:
	    case 17:
	      return "White";
	    default:
	      return "Grey";
  }
}

#endif
