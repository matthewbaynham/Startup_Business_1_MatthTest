/*
 *  ClsPrimeNumbers.h
 *  Hello World Table View
 *
 *  Created by Matthew Baynham on 1/5/11.
 *  Copyright 2011 Baynham Coding. All rights reserved.
 *
 */

#ifndef CLS_PRIME_NUMBERS_H
#define CLS_PRIME_NUMBERS_H

#include <vector>
#include <math.h>
#include "modMisc.h"

using namespace std;
using namespace modMisc;

class ClsPrimeNumbers 
{
public:
	ClsPrimeNumbers();
	~ClsPrimeNumbers();
	void extendDataSet(int);
	int commonDenominator(int, int);
	int iRandomPrime(int);
	
private:
	vector<int> vecPrimes;
	int iMaxPrime;
	int iMaxChecked;
};

#endif