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

/**
  * This file contains functions used to maintain compatability and portability.
  * It also contains constants that are used elsewhere in the DAL.
  */

#ifndef MICROBIT_COMPAT_H
#define MICROBIT_COMPAT_H

#include "mbed.h"
#include "MicroBitConfig.h"
#include <array>

#define PI 3.14159265359

/**
  * Determines the smallest of the two numbers
  *
  * @param a the first number
  *
  * @param b the second number
  *
  * @return The smallest of the two given values.
  */
inline int min(int a, int b)
{
    return (a < b ? a : b);
}

/**
  * Determines the largest of the two numbers
  *
  * @param a the first number
  *
  * @param b the second number
  *
  * @return The larger of the two given values.
  */
inline int max(int a, int b)
{
    return (a > b ? a : b);
}

/**
  * Sets a given area of memory to zero.
  *
  * @param a the pointer to the beginning of the memory to clear
  *
  * @param b the number of bytes to clear.
  */
inline void *memclr(void *a, size_t b)
{
    return memset(a,0,b);
}

/**
  * Determines if the given character is a printable ASCII/UTF8 decimal digit (0..9).
  *
  * @param c the character to check
  *
  * @return true if the character is a digit, false otherwise.
  */
inline bool isdigit(char c)
{
    return (c > 47 && c < 58);
}

/**
  * Performs an in buffer reverse of a given char array.
  *
  * @param s the string to reverse.
  *
  * @return MICROBIT_OK, or MICROBIT_INVALID_PARAMETER.
  */
int string_reverse(char *s);

/**
  * Converts a given integer into a string representation.
  *
  * @param n The number to convert.
  *
  * @param s A pointer to the buffer where the resulting string will be stored.
  *
  * @return MICROBIT_OK, or MICROBIT_INVALID_PARAMETER.
  */
int itoa(int n, char *s);

/**
  * Converts a given unsigned long into a string representation.
  *
  * @param n The number to convert.
  *
  * @param s A pointer to the buffer where the resulting string will be stored.
  *
  * @return MICROBIT_OK, or MICROBIT_INVALID_PARAMETER.
  */
int ultoa(unsigned long n, char *s);


/**
  * Converts a given integer into a string representation.
  *
  * @param n The number to convert.
  *
  * @param s A pointer to the buffer where the resulting string will be stored.
  *
  * @return MICROBIT_OK, or MICROBIT_INVALID_PARAMETER.
  */
int dtoa(double n, char *s);


/**
  * Find the first occurrence of a given element in array.
  *
  * @param e The searched element.
  *
  * @param a The array in which we look for the element.
  *
  * @return index of the element in location, it the element is not found it returns -1.
  */
template<class T, std::size_t SIZE>
int findFirstOccurrenceOfElementInArray(T e, std::array<T, SIZE>& a) {
  int i = 0;
  for (T& aa : a) {
    if (aa == e) {
      return i;
    }
    i++;
  }
  return -1;
}

/**
  * Find the last occurrence of a given element in array.
  *
  * @param e The searched element.
  *#include <cmath>
  * @param a The array in which we look for the element.
  *
  * @return index of the element in location, it the element is not found it returns -1.
  */
template<class T, std::size_t SIZE>
int findLastOccurrenceOfElementInArray(T e, std::array<T, SIZE>& a) {
  int ix = -1;
  int i = 0;
  for (T& aa : a) {
    if (aa == e) {
      ix = i;
    }
    i++;
  }
  return ix;
}

/**
  * Find the last occurrence of a given element in array.
  *
  * @param e The searched element.
  *
  * @param a The array in which we look for the element.
  *
  * @return index of the element in location, it the element is not found it returns -1.
  */
template<class T, std::size_t SIZE>
float sum(std::array<T, SIZE>& a) {
  float sum = 0.0;
  for(auto& num : a)
      sum += num;
  return sum;
}

/**
  * Find the last occurrence of a given element in array.
  *
  * @param e The searched element.
  *
  * @param a The array in which we look for the element.
  *
  * @return index of the element in location, it the element is not found it returns -1.
  */
template<class T, std::size_t SIZE>
float average(std::array<T, SIZE>& a) {
  return sum(a) / a.size();
}


/**
  * Find the last occurrence of a given element in array.
  *
  * @param e The searched element.
  *
  * @param a The array in which we look for the element.
  *
  * @return index of the element in location, it the element is not found it returns -1.
  */
template<class T, std::size_t SIZE>
float median(std::array<T, SIZE>& a) {
  int middle;
  int size = a.size();
  float average, median;
  middle = size / 2.0;
  if (size % 2) {
    median = (a[middle] + a[middle + 1]) / 2.0;
  } else {
    median = a[middle + 0] / 1.0;
  }
  return median;
}

/**
  * Find the last occurrence of a given element in array.
  *
  * @param e The searched element.
  *
  * @param a The array in which we look for the element.
  *
  * @return index of the element in location, it the element is not found it returns -1.
  */
template<class T, std::size_t SIZE>
float standardDeviation(std::array<T, SIZE>& a) {
  float mean, std = 0.0;

  mean = average(a);
  for(auto& num : a)
      std += pow(num - mean, 2);
  return sqrt(std / a.size());
}

/**
  * Find the last occurrence of a given element in array.
  *
  * @param e The searched element.
  *
  * @param a The array in which we look for the element.
  *
  * @return index of the element in location, it the element is not found it returns -1.
  */
template<class T, std::size_t SIZE>
float min(std::array<T, SIZE>& a) {
  T min_item = a[0];

  for(auto& num : a)
      min_item = min(min_item, num);
  return min_item;
}

/**
  * Find the last occurrence of a given element in array.
  *
  * @param e The searched element.
  *
  * @param a The array in which we look for the element.
  *
  * @return index of the element in location, it the element is not found it returns -1.
  */
template<class T, std::size_t SIZE>
float max(std::array<T, SIZE>& a) {
  T max_item = a[0];

  for(auto& num : a)
      max_item = max(max_item, num);
  return max_item;
}

/**
  * Find the last occurrence of a given element in array.
  *
  * @param e The searched element.
  *
  * @param a The array in which we look for the element.
  *
  * @return index of the element in location, it the element is not found it returns -1.
  */
template<class T, std::size_t SIZE>
float randomElement(std::array<T, SIZE>& a) {
  int ix = rand() % a.size();
  return a[ix];
}

#endif
