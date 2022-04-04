/*
 *  ClsPrimeNumbers.cpp
 *  Hello World Table View
 *
 *  Created by Matthew Baynham on 1/5/11.
 *  Copyright 2011 Baynham Coding. All rights reserved.
 *
 */

#ifndef CLS_PRIME_NUMBERS_CPP
#define CLS_PRIME_NUMBERS_CPP

#include <vector>
#include <math.h>
#include "modMisc.h"
#include "ClsPrimeNumbers.h"

using namespace std;
using namespace modMisc;

ClsPrimeNumbers::ClsPrimeNumbers() {
	vecPrimes.clear();
	
	//load the first primes until 1000
	vecPrimes.push_back(2);//1
	vecPrimes.push_back(3);//2
	vecPrimes.push_back(5);//3
	vecPrimes.push_back(7);//4
	vecPrimes.push_back(11);//5
	vecPrimes.push_back(13);//6
	vecPrimes.push_back(17);//7
	vecPrimes.push_back(19);//8
	vecPrimes.push_back(23);//9
	vecPrimes.push_back(29);//10
	vecPrimes.push_back(31);//11
	vecPrimes.push_back(37);//12
	vecPrimes.push_back(41);//13
	vecPrimes.push_back(43);//14
	vecPrimes.push_back(47);//15
	vecPrimes.push_back(53);//16
	vecPrimes.push_back(59);//17
	vecPrimes.push_back(61);//18
	vecPrimes.push_back(67);//19
	vecPrimes.push_back(71);//20
	vecPrimes.push_back(73);//21
	vecPrimes.push_back(79);//22
	vecPrimes.push_back(83);//23
	vecPrimes.push_back(89);//24
	vecPrimes.push_back(97);//25
	vecPrimes.push_back(101);//26
	vecPrimes.push_back(103);//27
	vecPrimes.push_back(107);//28
	vecPrimes.push_back(109);
	vecPrimes.push_back(113);
	vecPrimes.push_back(127);
	vecPrimes.push_back(131);
	vecPrimes.push_back(137);
	vecPrimes.push_back(139);
	vecPrimes.push_back(149);
	vecPrimes.push_back(151);
	vecPrimes.push_back(157);
	vecPrimes.push_back(163);
	vecPrimes.push_back(167);
	vecPrimes.push_back(173);
	vecPrimes.push_back(179);
	vecPrimes.push_back(181);
	vecPrimes.push_back(191);
	vecPrimes.push_back(193);
	vecPrimes.push_back(197);
	vecPrimes.push_back(199);
	vecPrimes.push_back(211);
	vecPrimes.push_back(223);
	vecPrimes.push_back(227);
	vecPrimes.push_back(229);
	vecPrimes.push_back(233);
	vecPrimes.push_back(239);
	vecPrimes.push_back(241);
	vecPrimes.push_back(251);
	vecPrimes.push_back(257);
	vecPrimes.push_back(263);
	vecPrimes.push_back(269);
	vecPrimes.push_back(271);
	vecPrimes.push_back(277);
	vecPrimes.push_back(281);
	vecPrimes.push_back(283);
	vecPrimes.push_back(293);
	vecPrimes.push_back(307);
	vecPrimes.push_back(311);
	vecPrimes.push_back(313);
	vecPrimes.push_back(317);
	vecPrimes.push_back(331);
	vecPrimes.push_back(337);
	vecPrimes.push_back(347);
	vecPrimes.push_back(349);
	vecPrimes.push_back(353);
	vecPrimes.push_back(359);
	vecPrimes.push_back(367);
	vecPrimes.push_back(373);
	vecPrimes.push_back(379);
	vecPrimes.push_back(383);
	vecPrimes.push_back(389);
	vecPrimes.push_back(397);
	vecPrimes.push_back(401);
	vecPrimes.push_back(409);
	vecPrimes.push_back(419);
	vecPrimes.push_back(421);
	vecPrimes.push_back(431);
	vecPrimes.push_back(433);
	vecPrimes.push_back(439);
	vecPrimes.push_back(443);
	vecPrimes.push_back(449);
	vecPrimes.push_back(457);
	vecPrimes.push_back(461);
	vecPrimes.push_back(463);
	vecPrimes.push_back(467);
	vecPrimes.push_back(479);
	vecPrimes.push_back(487);
	vecPrimes.push_back(491);
	vecPrimes.push_back(499);
	vecPrimes.push_back(503);
	vecPrimes.push_back(509);
	vecPrimes.push_back(521);
	vecPrimes.push_back(523);
	vecPrimes.push_back(541);
	vecPrimes.push_back(547);
	vecPrimes.push_back(557);
	vecPrimes.push_back(563);
	vecPrimes.push_back(569);
	vecPrimes.push_back(571);
	vecPrimes.push_back(577);
	vecPrimes.push_back(587);
	vecPrimes.push_back(593);
	vecPrimes.push_back(599);
	vecPrimes.push_back(601);
	vecPrimes.push_back(607);
	vecPrimes.push_back(613);
	vecPrimes.push_back(617);
	vecPrimes.push_back(619);
	vecPrimes.push_back(631);
	vecPrimes.push_back(641);
	vecPrimes.push_back(643);
	vecPrimes.push_back(647);
	vecPrimes.push_back(653);
	vecPrimes.push_back(659);
	vecPrimes.push_back(661);
	vecPrimes.push_back(673);
	vecPrimes.push_back(677);
	vecPrimes.push_back(683);
	vecPrimes.push_back(691);
	vecPrimes.push_back(701);
	vecPrimes.push_back(709);
	vecPrimes.push_back(719);
	vecPrimes.push_back(727);
	vecPrimes.push_back(733);
	vecPrimes.push_back(739);
	vecPrimes.push_back(743);
	vecPrimes.push_back(751);
	vecPrimes.push_back(757);
	vecPrimes.push_back(761);
	vecPrimes.push_back(769);
	vecPrimes.push_back(773);
	vecPrimes.push_back(787);
	vecPrimes.push_back(797);
	vecPrimes.push_back(809);
	vecPrimes.push_back(811);
	vecPrimes.push_back(821);
	vecPrimes.push_back(823);
	vecPrimes.push_back(827);
	vecPrimes.push_back(829);
	vecPrimes.push_back(839);
	vecPrimes.push_back(853);
	vecPrimes.push_back(857);
	vecPrimes.push_back(859);
	vecPrimes.push_back(863);
	vecPrimes.push_back(877);
	vecPrimes.push_back(881);
	vecPrimes.push_back(883);
	vecPrimes.push_back(887);
	vecPrimes.push_back(907);
	vecPrimes.push_back(911);
	vecPrimes.push_back(919);
	vecPrimes.push_back(929);
	vecPrimes.push_back(937);
	vecPrimes.push_back(941);
	vecPrimes.push_back(947);
	vecPrimes.push_back(953);
	vecPrimes.push_back(967);
	vecPrimes.push_back(971);
	vecPrimes.push_back(977);
	vecPrimes.push_back(983);
	vecPrimes.push_back(991);
	vecPrimes.push_back(997);
	
	iMaxPrime = 997;
	iMaxChecked = 1000;
};

ClsPrimeNumbers::~ClsPrimeNumbers() {
	vecPrimes.clear();
};

void ClsPrimeNumbers::extendDataSet(int iFigure) {
	//Calculate all the prime numbers from the existing dataset until iFigure
	vector<int>::iterator itPrime;
	int iCount;
	bool bIsPrime;
	
	for (iCount = iMaxPrime +1; iCount <= iFigure; iCount++) {
/*	
		bIsPrime = true;
		
		for (itPrime = vecPrimes.begin(); itPrime != vecPrimes.end(); itPrime++) {
			if (fmod(iCount, *itPrime) == 0) {
				bIsPrime = false;
			};
		};
*/		
		bIsPrime = true;
		itPrime = vecPrimes.begin();
		do {
			if (fmod((float)iCount, (float)*itPrime) == 0) {
				bIsPrime = false;
			};
			itPrime++;
		} while (itPrime != vecPrimes.end() or bIsPrime != false);
		
		if (bIsPrime == true) {
			vecPrimes.push_back(iCount);
			
			iMaxPrime = iCount;
		};
	};
	
	iMaxChecked = iFigure;
};

int ClsPrimeNumbers::commonDenominator(int iFigureA, int iFigureB) {
	vector<int>::iterator itPrime;
	int iDenominater;
	
	if (iFigureA == 0 or iFigureB == 0) {
		iDenominater = 1;
	} else {
		iDenominater = 1;
		
		for (itPrime = vecPrimes.begin(); itPrime != vecPrimes.end(); itPrime++) {
			if (fmod((float)iFigureA, (float)*itPrime) == 0 and fmod((float)iFigureB, (float)*itPrime) == 0) {
				iDenominater = iDenominater * (*itPrime);
			};
		};
		
		if (iDenominater != 1) {
			iDenominater = iDenominater * commonDenominator(iFigureA / iDenominater, iFigureB / iDenominater);
		};
	};

	return iDenominater;
};

int ClsPrimeNumbers::iRandomPrime(int iMax) {
	int iCountMax;
	bool bIsMax;
	int iItem;
	int iResult;
	
	iCountMax = 0;
	
	do {
		iCountMax++;
		
		if (iCountMax > vecPrimes.size()) {
			bIsMax = true;
		} else if (vecPrimes[iCountMax] < iMax) {
			bIsMax = false;
		} else {
			bIsMax = true;
		};
	} while (bIsMax == false);
	
	if (iCountMax < vecPrimes.size()) {
		iItem = modMisc::iRandomNumber(0, iCountMax - 1);
	} else {
		iItem = modMisc::iRandomNumber(0, vecPrimes.size() - 1);
	};

	iResult = vecPrimes[iItem];

	return iResult;
	
};


#endif
