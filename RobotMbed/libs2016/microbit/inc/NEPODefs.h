//NEPO definitions for calliope
// Display brightness is in interval of 0..255
// To scale that to 0..9 we use the following multipliers:
#define _SET_BRIGHTNESS_MULTIPLIER 28.34
#define _GET_BRIGHTNESS_MULTIPLIER 0.0353
// Light sensor returns values in interval 0..255
// To scale them to 0..100 we use this multiplier:
#define _GET_LIGHTLEVEL_MULTIPLIER 0.3922
#define _ITERATION_SLEEP_TIMEOUT 1

// math constants used in math blocks. Avoids inclusion of <math.h>

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
#include <list>
#include <array>

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
        for( uint16_t i = 0 ; i < sorted.size() / 2 ; i ++ ) {
            iterator++;
        }
        return (*iterator + *--iterator ) / 2;
    } else {
        for( uint16_t i = 0 ; i < sorted.size() / 2 ; i ++ ) {
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

template <typename T, long unsigned S>
std::array<T, S> _convertToArray(std::list<T> &list) {
  auto iterator = list.begin();
  std::array<T, S> result;
  int i = 0;
  for(auto iterator = list.begin(), i = 0; iterator != list.end(); ++iterator, ++i) {
    result[i] = *iterator;
  }
  return result;
}

template <typename T, long unsigned S>
std::array<T, S> _convertToArray(std::initializer_list<T> list) {
  auto iterator = list.begin();
  std::array<T, S> result;
  int i = 0;
  for(auto iterator = list.begin(), i = 0; iterator != list.end(); ++iterator, ++i) {
    result[i] = *iterator;
  }
  return result;
}
