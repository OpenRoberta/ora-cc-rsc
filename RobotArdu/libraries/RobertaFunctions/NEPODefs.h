#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef M_E
#define M_E 2.718281828459045
#endif

#ifndef M_GOLDEN_RATIO
#define M_GOLDEN_RATIO 1.61803398875
#endif

#ifndef M_SQRT2
#define M_SQRT2 1.41421356237
#endif

#ifndef M_SQRT1_2
#define M_SQRT1_2 0.707106781187
#endif

#ifndef M_INFINITY
#define M_INFINITY 0x7f800000
#endif

#ifndef RCHANNEL(color)
#define RCHANNEL(color) ((color >> 11) * 255 / 31)
#endif

#ifndef GCHANNEL(color)
#define GCHANNEL(color) ( ((color << 5) >> 10 ) * 255 / 63)
#endif

#ifndef BCHANNEL(color)
#define BCHANNEL(color) ( ((color << 11) >> 11 ) * 255 / 31)
#endif

#ifndef _SENSEBOX_INCLUDES
#include <ArduinoSTL.h>
#endif

#include <list>
#include <math.h>

inline unsigned RGB(unsigned r, unsigned g, unsigned b) {
    return (((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3));
}

inline double absD(double d) {
  return d < 0.0 ? -d : d;
}

inline bool isWholeD(double d) {
  return d == floor(d);
}

inline bool isPrimeD(double d) {
  if (!isWholeD(d)) {
    return false;
  }
  int n = (int) d;
  if ( n < 2 ) { return false; }
  if ( n == 2 ) { return true; }
  if ( n % 2 == 0 ) { return false; }
  for ( int i = 3, s = (int)(sqrt( d )+1); i <= s; i += 2 ) {
    if ( n % i == 0 ) { return false; }
  }
  return true;
}

inline int _randomIntegerInRange(int val1, int val2){
  int min = fmin(val1, val2);
  int max = fmax(val1, val2) + 1;
  return min + (rand()%(min - max));
}

inline float _randomFloat(){
  return (float)rand()/(float)RAND_MAX;
}

template <typename T>
std::list<T> &_createListRepeat(unsigned count, T e)
{
  std::list<T> &l = *(new std::list<T>);
  for (unsigned i = 0; i< count; i++) {
      l.push_back(e);
  }
  return l;
}

template <typename T>
T _getListElementByIndex(std::list<T> &list, unsigned index) {
    auto iterator = list.begin();
    advance(iterator, index);
    return (*iterator);
}

template <typename T>
T _getAndRemoveListElementByIndex(std::list<T> &list, unsigned index) {
    auto iterator = list.begin();
    advance(iterator, index);
    T value = (*iterator);
    list.erase(iterator);
    return value;
}

template <typename T>
void _removeListElementByIndex(std::list<T> &list, unsigned index) {
    _getAndRemoveListElementByIndex(list, index);
}

/*
 * The only known situation where the cast of P to T would be needed is for int to double
 * in other cases T and P will be the same type. If only one template parameter is used
 * then the match void setListElementByIndex(std::list<double>, int, int) would not be possible
 */

template <typename T, typename P>
void _setListElementByIndex(std::list<T> &list, unsigned index, P value) {
    if (index < list.size()) {
        auto iterator = list.begin();
        advance(iterator, index);
        (*iterator) = (T) (value);
    } else {
        list.push_back((T) (value));
    }
}

template <typename T, typename P>
void _insertListElementBeforeIndex(std::list<T> &list, unsigned index, P value) {
    auto iterator = list.begin();
    advance(iterator, index);
    list.insert(iterator, (T) (value));
}

template <typename T, typename P>
int _getFirstOccuranceOfElement(std::list<T> &list, P value) {
    int i = 0;
    auto iterator = list.begin();
    for(i = 0, iterator = list.begin(); iterator != list.end(); ++iterator, ++i) {
        if ((P) (*iterator) == value) {
            return i;
        }
    }
    return -1;
}

template <typename T, typename P>
int _getLastOccuranceOfElement(std::list<T> &list, P value) {
    int i = 0;
    auto iterator = list.rbegin();
    for(i = 0, iterator = list.rbegin(); iterator != list.rend(); ++iterator, ++i) {
        if ((P) (*iterator) == value) {
            return list.size() - i - 1;
        }
    }
    return -1;
}

template <typename T>
std::list<T> &_getSubList(std::list<T> &list, int startIndex, int endIndex) {
    auto beginIterator = list.begin();
    advance(beginIterator, startIndex);
    auto endIterator = list.begin();
    advance(endIterator, endIndex + 1);
    return *(new std::list<T>(beginIterator, endIterator));
}

double _getListSum(std::list<double> &list) {
    double result = 0;
    for(auto iterator = list.begin(); iterator != list.end(); ++iterator) {
        result += *iterator;
    }
    return result;
}

double _getListMin(std::list<double> &list) {
    double result = *(list.begin());
    for(auto iterator = list.begin(); iterator != list.end(); ++iterator) {
        if (result > *iterator) {
            result = *iterator;
        }
    }
    return result;
}

double _getListMax(std::list<double> &list) {
    double result = *(list.begin());
    for(auto iterator = list.begin(); iterator != list.end(); ++iterator) {
        if (result < *iterator) {
            result = *iterator;
        }
    }
    return result;
}

double _getListMedian(std::list<double> &list) {
    std::list<double> sorted(list);
    sorted.sort();
    auto iterator = sorted.begin();
    if( sorted.size() % 2 == 0 ) {
        for( int i = 0 ; i < sorted.size() / 2 ; i ++ ) {
            iterator++;
        }
        return (*iterator + *--iterator ) / 2;
    } else {
        for( int i = 0 ; i < sorted.size() / 2 ; i ++ ) {
            iterator++;
        }
        return *iterator;
    }
}

double _getListAverage(std::list<double> &list) {
    double sum = _getListSum(list);
    return sum / list.size();
}

double _getListStandardDeviation(std::list<double> &list) {
    double mean = _getListSum(list) / list.size();
    double sum = 0;
    for(auto iterator = list.begin(); iterator != list.end(); ++iterator) {
        sum += (*iterator - mean)*(*iterator - mean);
    }
    sum /= list.size() - 1;
    return sqrt(sum);
}

template <typename T,typename U>
void assertNepo(bool test, String text, T left, String op, U right) {
    if (!test) {
        Serial.println("Assertion failed: " + text + " " + left + " " + op + " " + right);
        Serial.flush();
    }
}
