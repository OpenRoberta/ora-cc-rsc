/*
The MIT License (MIT)

Copyright (c) 2016 British Broadcasting Corporation.
This software is provided by Lancaster University by arrangement with the BBC.

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

#ifndef MICROBIT_COLOR_H
#define MICROBIT_COLOR_H

#include "RefCounted.h"


struct ColorData : RefCounted
{
    uint8_t red;     // red channel
    uint8_t green;    // green channel
    uint8_t blue;    // blue channel
    uint8_t white;    // brighteness
};

/**
  * Class definition for a MicroBitColor.
  *
  * An MicroBitColor is a simple representation of a color in RGB and white .
  * n.b. This is a mutable, managed type.
  */
class MicroBitColor
{
    ColorData *ptr;     // Pointer to payload data


    /**
      * Internal constructor which provides sanity checking and initialises class properties.
      *
      * @param red channel of the color
      *
      * @param green channel of the color
      *
      * @param blue channel of the color
      *
      * @white brighteness of the color.
      */
    void init(const uint8_t red, const uint8_t green, const uint8_t blue, const uint8_t white);

    /**
      * Internal constructor which defaults to the Empty Color instance variable
      */
    void init_empty();

    public:
    static MicroBitColor EmptyColor;    // Shared representation of a null color.

    /**
      * Get current ptr, do not decr() it, and set the current instance to empty color.
      *
      * This is to be used by specialized runtimes which pass ColorData around.
      */
    ColorData *leakData();

    /**
      * Constructor.
      * Create an color from a specially prepared constant array, with no copying. Will call ptr->incr().
      *
      * @param ptr The literal - first two bytes should be 0xff, then red, 0, green, 0, blue, 0, white. The literal has to be 4-byte aligned.
      *
      * @code
      * static const uint8_t color[] __attribute__ ((aligned (4))) = { 0xff, 0xff, 10, 0, 5, 0, 100, 0, 115, };
      * MicroBitColor i((ColorData*)(void*)color);
      * @endcode
      */
    MicroBitColor(ColorData *ptr);

    /**
      * Default Constructor.
      * Creates a new reference to the empty MicroBitColor color
      *
      * @code
      * MicroBitColor c(); //an empty color instance
      * @endcode
      */
    MicroBitColor();


    /**
      * Copy Constructor.
      * Add ourselves as a reference to an existing MicroBitColor.
      *
      * @param color The MicroBitColor to reference.
      *
      * @code
      * MicroBitColor c(0,1,0,1,0);
      * MicroBitColor c2(c); //points to c
      * @endcode
      */
    MicroBitColor(const MicroBitColor &color);


    /**
      * Constructor.
      * Create a representation of a color, based on a given RGBW.
      *
      * @param red channel of the color
      *
      * @param green channel of the color
      *
      * @param blue channel of the color
      *
      * @white brighteness of the color.
      *
      * @code
      * MicroBitColor c(10,5,9,255);
      * @endcode
      */
    MicroBitColor(const uint8_t red, const uint8_t green, const uint8_t blue, const uint8_t white);


    /**
      * Destructor.
      *
      * Removes buffer resources held by the instance.
      */
    ~MicroBitColor();

    /**
      * Copy assign operation.
      *
      * Called when one MicroBitColor is assigned the value of another using the '=' operator.
      *
      * Decrement our reference count and free up the buffer as necessary.
      *
      * Then, update our buffer to refer to that of the supplied MicroBitColor,
      * and increase its reference count.
      *
      * @param s The MicroBitColor to reference.
      *
      * @code
      * MicroBitColor c(10,5,15,100);
      * MicroBitColor c1();
      * c1 = c; // i1 now references i
      * @endcode
      */
    MicroBitColor& operator = (const MicroBitColor& c);


    /**
      * Equality operation.
      *
      * Called when one MicroBitColor is tested to be equal to another using the '==' operator.
      *
      * @param i The MicroBitColor to test ourselves against.
      *
      * @return true if this MicroBitColor is identical to the one supplied, false otherwise.
      *
      * @code
      * MicroBitDisplay display;
      * MicroBitColor c();
      * MicroBitColor c1();
      *
      * if(c == c1) //will be true
      *     display.scroll("true");
      * @endcode
      */
    bool operator== (const MicroBitColor& c);

    /**
      * Gets the red channel of this color.
      *
      * @return The red channel of this color.
      *
      * @code
      * MicroBitColor c(10,5,100,115);
      * c.getRed(); // equals 10...
      * @endcode
      */
    uint8_t getRed() const
    {
        return ptr->red;
    }

    /**
      * Gets the green channel of this color.
      *
      * @return The green channel of this color.
      *
      * @code
      * MicroBitColor c(10,5,100,115);
      * c.getGreen(); // equals 5...
      * @endcode
      */
    uint8_t getGreen() const
    {
        return ptr->green;
    }

    /**
      * Gets the blue channel of this color.
      *
      * @return The blue channel of this color.
      *
      * @code
      * MicroBitColor c(10,5,100,115);
      * c.getBlue(); // equals 100...
      * @endcode
      */
    uint8_t getBlue() const
    {
        return ptr->blue;
    }

    /**
      * Gets the brighteness of this color.
      *
      * @return The brighteness of this color.
      *
      * @code
      * MicroBitColor c(10,5,100,115);
      * i.getWhite(); // equals 115...
      * @endcode
      */
    uint8_t getWhite() const
    {
        return ptr->white;
    }

};

#endif
