/*
  RobertaFunctions.cpp - additional library for interfacing with Bot'n Roll ONE Arduino Compatible
  from www.botnroll.com that allows using some extra functions from https://www.open-roberta.org/
  Created by Evgeniya Ovchinnikova, August 30, 2016.
  Released into the public domain.
*/

#ifndef RobertaFunctions_h
#define RobertaFunctions_h

#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Arduino.h"

#ifdef ARDUINO_AVR_BOB3
#include "BOB3.h"
#endif

class RobertaFunctions
{
  public:
		int randomIntegerInRange(int val1, int val2);
		float randomFloat();
		double clamp(double val, double min, double max);
		bool isPrime(double number);
		bool isWhole(double val);
	    void createArray(double *arr, int len, double arrAssign[]);
	    void createArray(bool   *arr, int len, bool arrAssign[]);
	    void createArray(String *arr, int len, String arrAssign[]);
	    void createArray(int *arr, int len, int arrAssign[]);
#ifdef ARDUINO_AVR_BOB3
        void createArray(Bob3Color *arr, int len, Bob3Color arrAssign[]);  
#endif
		int arrFindFirst(int len, double arr[], double item);
		int arrFindFirst(int len,   bool arr[],   bool item);
		int arrFindFirst(int len, char **arr,   String item);
		int arrFindLast( int len, double arr[], double item);
		int arrFindLast( int len,   bool arr[],   bool item);
		int arrFindLast( int len, char **arr, String item);
		double arrSum(int len, double arr[]);
		double arrMin(int len, double arr[]);
		double arrMax(int len, double arr[]);
		double arrMean(int len, double arr[]);
		void arrInsertionSort(int len,  double *arr);
		double arrMedian(int len, double arr[]);
		double arrStandardDeviatioin(int len, double arr[]);
		double arrRand(int len, double arr[]);
		double arrMode(int len, double arr[]);
};
#endif
