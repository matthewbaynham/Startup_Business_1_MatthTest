/*
 *  modMisc.cpp
 *  Command Line C++ App
 *
 *  Created by Matthew Baynham on 02/07/2010.
 *  Copyright 2010 Baynham Coding. All rights reserved.
 *
 */

#include <string>;
#include <cstddef>//std array rapper
#include <vector>;
#include <cctype>
#include <stdlib.h>;
#include <time.h>
#include <sstream>
#include <math.h>
#include <sqlite3.h>
#include <algorithm>
#include <string>
#include "modMisc.h"

#ifndef MOD_MISC_CPP
#define MOD_MISC_CPP

using namespace std;

namespace modMisc {
	int addingNumbers(int x, int y) {
		return x + y;
	};

	bool isSubStringInString(string sMaster, string sSubString){
		std::string::size_type idx;

		idx = sMaster.find(sSubString);

		if (idx == std::string::npos){
			return false;
		}
		else {
			return true;
		}
	};

	std::string decodeText(string sText, vector<strFloatValue> fValues){
		std::string sTemp;
		std::string::size_type iPos;
		std::vector<strFloatValue>::reverse_iterator rit;
		int iCounter;
		std::string sCounter;
		std::stringstream ssSubString;
		std::string sSubString;
		std::string sValue;
		std::ostringstream oValue;
		
		sTemp = sText;
		
		iCounter = 0;
		
		for (rit=fValues.rbegin(); rit < fValues.rend();rit++){
			ssSubString.str("");
			ssSubString << "<" << iCounter << ">";
			sSubString = ssSubString.str();
			
			iPos = sTemp.find(sSubString);
			
			oValue.str("");
			oValue << fValues[iCounter].fValue;
			sValue = oValue.str();
			
			
			if (iPos == std::string::npos){
				//error
			}
			else{
				sTemp = sTemp.replace(iPos, sSubString.length(), sValue);
			}
			
			sTemp = replaceAll(sTemp, "+ -", " - ");
			sTemp = replaceAll(sTemp, "+-", " - ");
			sTemp = replaceAll(sTemp, "    ", " ");
			sTemp = replaceAll(sTemp, "   ", " ");
			sTemp = replaceAll(sTemp, "  ", " ");
			
			iCounter++;
		};
		
		sTemp = sTemp;
		

	
	
		sCounter.clear();
//		ssSubString.clear();
		sSubString.clear();
		sValue.clear();
//		oValue.clear();
		
		return sTemp;
	};
	
	std::string replaceAll(std::string sMaster, std::string sBefore, std::string sAfter){
		std::string sTemp;
		std::string::size_type iPos;

		sTemp = sMaster;
		
		iPos = sTemp.find(sBefore);
		
		while (iPos != std::string::npos) {
			sTemp = sTemp.replace(iPos, sBefore.length(), sAfter);
			iPos = sTemp.find(sBefore);
		};
		
		return sTemp;
	}
	
	std::string currentDateTime(){
		std::time_t raw_time;
		struct tm * timeinfo;
		std::string sResult;
		char buffer[80];
		
		time (&raw_time);
		timeinfo = localtime (&raw_time);
		
		sResult = strftime(buffer, 80, "%Y%m%d %H%M%S", timeinfo);
		sResult = (string) buffer;
		
		return sResult;
	}

	
	long iRandomNumber(int iMin, int iMax)
	{
		try{
			int iResult;
			int iRand = rand();
			time_t seconds;
			
			if (iMin == iMax) {
				iResult = iMax;
			} else {
				do {
					if (iMin > iMax) {
						int iSwap;
						
						iSwap = iMin;
						iMin = iMax;
						iMax= iSwap;
					};
					
					iRand = rand();

					seconds = time (NULL);
					
					iResult = iMin + fabs(fmod((iRand * (int)seconds + (int)seconds + iRand), (iMax - iMin + 1)));
				} while (not (iResult >= iMin and iResult <= iMax));
			};
			
			return iResult;
		}
		catch(...){
			return 0;
		}
	};
	
	long iRandomNumberNonZero(int iMin, int iMax)
	{
		try{
			if (iMin == iMax) {
				return iMax;
			} else {
				int x = 0;

				if (iMin > iMax) {
					int iSwap;
					
					iSwap = iMin;
					iMin = iMax;
					iMax= iSwap;
				};
				
				
				while (x == 0){
					x = iRandomNumber(iMin, iMax);
				};
				
				return x;
			};
		}
		catch(...){
			return 0;
		}
	};
	
	strTrigNumbers friendlyTrigNumbers(int iSize){
		strTrigNumbers oTrigNo;
		int iSelection;
		
		switch(iSize){
			case 1:
				iSelection = iRandomNumber(1, 4); 
			case 2:
				iSelection = iRandomNumber(3, 8);
			case 3:
				iSelection = iRandomNumber(6, 12);
			case 4:
				iSelection = iRandomNumber(9, 20);
			case 5:
				iSelection = iRandomNumber(13, 126);
			default:
				iSelection = iRandomNumber(1, 126); 
		};
		
		switch(iSelection){
			case 1:	
				oTrigNo.fAdjacent = 3;	
				oTrigNo.fOpposite = 4;	
				oTrigNo.fHypotenuse = 5;	
				break;
			case 2:	
				oTrigNo.fAdjacent = 4;	
				oTrigNo.fOpposite = 3;	
				oTrigNo.fHypotenuse = 5;	
				break;
			case 3:	
				oTrigNo.fAdjacent = 6;	
				oTrigNo.fOpposite = 8;	
				oTrigNo.fHypotenuse = 10;	
				break;
			case 4:	
				oTrigNo.fAdjacent = 8;	
				oTrigNo.fOpposite = 6;	
				oTrigNo.fHypotenuse = 10;	
				break;
			case 5:	
				oTrigNo.fAdjacent = 5;	
				oTrigNo.fOpposite = 12;	
				oTrigNo.fHypotenuse = 13;	
				break;
			case 6:	
				oTrigNo.fAdjacent = 12;	
				oTrigNo.fOpposite = 5;	
				oTrigNo.fHypotenuse = 13;	
				break;
			case 7:	
				oTrigNo.fAdjacent = 9;	
				oTrigNo.fOpposite = 12;	
				oTrigNo.fHypotenuse = 15;	
				break;
			case 8:	
				oTrigNo.fAdjacent = 12;	
				oTrigNo.fOpposite = 9;	
				oTrigNo.fHypotenuse = 15;	
				break;
			case 9:	
				oTrigNo.fAdjacent = 8;	
				oTrigNo.fOpposite = 15;	
				oTrigNo.fHypotenuse = 17;	
				break;
			case 10:	
				oTrigNo.fAdjacent = 15;	
				oTrigNo.fOpposite = 8;	
				oTrigNo.fHypotenuse = 17;	
				break;
			case 11:	
				oTrigNo.fAdjacent = 12;	
				oTrigNo.fOpposite = 16;	
				oTrigNo.fHypotenuse = 20;	
				break;
			case 12:	
				oTrigNo.fAdjacent = 16;	
				oTrigNo.fOpposite = 12;	
				oTrigNo.fHypotenuse = 20;	
				break;
			case 13:	
				oTrigNo.fAdjacent = 7;	
				oTrigNo.fOpposite = 24;	
				oTrigNo.fHypotenuse = 25;	
				break;
			case 14:	
				oTrigNo.fAdjacent = 15;	
				oTrigNo.fOpposite = 20;	
				oTrigNo.fHypotenuse = 25;	
				break;
			case 15:	
				oTrigNo.fAdjacent = 20;	
				oTrigNo.fOpposite = 15;	
				oTrigNo.fHypotenuse = 25;	
				break;
			case 16:	
				oTrigNo.fAdjacent = 24;	
				oTrigNo.fOpposite = 7;	
				oTrigNo.fHypotenuse = 25;	
				break;
			case 17:	
				oTrigNo.fAdjacent = 10;	
				oTrigNo.fOpposite = 24;	
				oTrigNo.fHypotenuse = 26;	
				break;
			case 18:	
				oTrigNo.fAdjacent = 24;	
				oTrigNo.fOpposite = 10;	
				oTrigNo.fHypotenuse = 26;	
				break;
			case 19:	
				oTrigNo.fAdjacent = 20;	
				oTrigNo.fOpposite = 21;	
				oTrigNo.fHypotenuse = 29;	
				break;
			case 20:	
				oTrigNo.fAdjacent = 21;	
				oTrigNo.fOpposite = 20;	
				oTrigNo.fHypotenuse = 29;	
				break;
			case 21:	
				oTrigNo.fAdjacent = 18;	
				oTrigNo.fOpposite = 24;	
				oTrigNo.fHypotenuse = 30;	
				break;
			case 22:	
				oTrigNo.fAdjacent = 24;	
				oTrigNo.fOpposite = 18;	
				oTrigNo.fHypotenuse = 30;	
				break;
			case 23:	
				oTrigNo.fAdjacent = 16;	
				oTrigNo.fOpposite = 30;	
				oTrigNo.fHypotenuse = 34;	
				break;
			case 24:	
				oTrigNo.fAdjacent = 30;	
				oTrigNo.fOpposite = 16;	
				oTrigNo.fHypotenuse = 34;	
				break;
			case 25:	
				oTrigNo.fAdjacent = 21;	
				oTrigNo.fOpposite = 28;	
				oTrigNo.fHypotenuse = 35;	
				break;
			case 26:	
				oTrigNo.fAdjacent = 28;	
				oTrigNo.fOpposite = 21;	
				oTrigNo.fHypotenuse = 35;	
				break;
			case 27:	
				oTrigNo.fAdjacent = 12;	
				oTrigNo.fOpposite = 35;	
				oTrigNo.fHypotenuse = 37;	
				break;
			case 28:	
				oTrigNo.fAdjacent = 35;	
				oTrigNo.fOpposite = 12;	
				oTrigNo.fHypotenuse = 37;	
				break;
			case 29:	
				oTrigNo.fAdjacent = 15;	
				oTrigNo.fOpposite = 36;	
				oTrigNo.fHypotenuse = 39;	
				break;
			case 30:	
				oTrigNo.fAdjacent = 36;	
				oTrigNo.fOpposite = 15;	
				oTrigNo.fHypotenuse = 39;	
				break;
			case 31:	
				oTrigNo.fAdjacent = 24;	
				oTrigNo.fOpposite = 32;	
				oTrigNo.fHypotenuse = 40;	
				break;
			case 32:	
				oTrigNo.fAdjacent = 32;	
				oTrigNo.fOpposite = 24;	
				oTrigNo.fHypotenuse = 40;	
				break;
			case 33:	
				oTrigNo.fAdjacent = 9;	
				oTrigNo.fOpposite = 40;	
				oTrigNo.fHypotenuse = 41;	
				break;
			case 34:	
				oTrigNo.fAdjacent = 40;	
				oTrigNo.fOpposite = 9;	
				oTrigNo.fHypotenuse = 41;	
				break;
			case 35:	
				oTrigNo.fAdjacent = 27;	
				oTrigNo.fOpposite = 36;	
				oTrigNo.fHypotenuse = 45;	
				break;
			case 36:	
				oTrigNo.fAdjacent = 36;	
				oTrigNo.fOpposite = 27;	
				oTrigNo.fHypotenuse = 45;	
				break;
			case 37:	
				oTrigNo.fAdjacent = 14;	
				oTrigNo.fOpposite = 48;	
				oTrigNo.fHypotenuse = 50;	
				break;
			case 38:	
				oTrigNo.fAdjacent = 30;	
				oTrigNo.fOpposite = 40;	
				oTrigNo.fHypotenuse = 50;	
				break;
			case 39:	
				oTrigNo.fAdjacent = 40;	
				oTrigNo.fOpposite = 30;	
				oTrigNo.fHypotenuse = 50;	
				break;
			case 40:	
				oTrigNo.fAdjacent = 48;	
				oTrigNo.fOpposite = 14;	
				oTrigNo.fHypotenuse = 50;	
				break;
			case 41:	
				oTrigNo.fAdjacent = 24;	
				oTrigNo.fOpposite = 45;	
				oTrigNo.fHypotenuse = 51;	
				break;
			case 42:	
				oTrigNo.fAdjacent = 45;	
				oTrigNo.fOpposite = 24;	
				oTrigNo.fHypotenuse = 51;	
				break;
			case 43:	
				oTrigNo.fAdjacent = 20;	
				oTrigNo.fOpposite = 48;	
				oTrigNo.fHypotenuse = 52;	
				break;
			case 44:	
				oTrigNo.fAdjacent = 48;	
				oTrigNo.fOpposite = 20;	
				oTrigNo.fHypotenuse = 52;	
				break;
			case 45:	
				oTrigNo.fAdjacent = 28;	
				oTrigNo.fOpposite = 45;	
				oTrigNo.fHypotenuse = 53;	
				break;
			case 46:	
				oTrigNo.fAdjacent = 45;	
				oTrigNo.fOpposite = 28;	
				oTrigNo.fHypotenuse = 53;	
				break;
			case 47:	
				oTrigNo.fAdjacent = 33;	
				oTrigNo.fOpposite = 44;	
				oTrigNo.fHypotenuse = 55;	
				break;
			case 48:	
				oTrigNo.fAdjacent = 44;	
				oTrigNo.fOpposite = 33;	
				oTrigNo.fHypotenuse = 55;	
				break;
			case 49:	
				oTrigNo.fAdjacent = 40;	
				oTrigNo.fOpposite = 42;	
				oTrigNo.fHypotenuse = 58;	
				break;
			case 50:	
				oTrigNo.fAdjacent = 42;	
				oTrigNo.fOpposite = 40;	
				oTrigNo.fHypotenuse = 58;	
				break;
			case 51:	
				oTrigNo.fAdjacent = 36;	
				oTrigNo.fOpposite = 48;	
				oTrigNo.fHypotenuse = 60;	
				break;
			case 52:	
				oTrigNo.fAdjacent = 48;	
				oTrigNo.fOpposite = 36;	
				oTrigNo.fHypotenuse = 60;	
				break;
			case 53:	
				oTrigNo.fAdjacent = 11;	
				oTrigNo.fOpposite = 60;	
				oTrigNo.fHypotenuse = 61;	
				break;
			case 54:	
				oTrigNo.fAdjacent = 60;	
				oTrigNo.fOpposite = 11;	
				oTrigNo.fHypotenuse = 61;	
				break;
			case 55:	
				oTrigNo.fAdjacent = 16;	
				oTrigNo.fOpposite = 63;	
				oTrigNo.fHypotenuse = 65;	
				break;
			case 56:	
				oTrigNo.fAdjacent = 25;	
				oTrigNo.fOpposite = 60;	
				oTrigNo.fHypotenuse = 65;	
				break;
			case 57:	
				oTrigNo.fAdjacent = 33;	
				oTrigNo.fOpposite = 56;	
				oTrigNo.fHypotenuse = 65;	
				break;
			case 58:	
				oTrigNo.fAdjacent = 39;	
				oTrigNo.fOpposite = 52;	
				oTrigNo.fHypotenuse = 65;	
				break;
			case 59:	
				oTrigNo.fAdjacent = 52;	
				oTrigNo.fOpposite = 39;	
				oTrigNo.fHypotenuse = 65;	
				break;
			case 60:	
				oTrigNo.fAdjacent = 56;	
				oTrigNo.fOpposite = 33;	
				oTrigNo.fHypotenuse = 65;	
				break;
			case 61:	
				oTrigNo.fAdjacent = 60;	
				oTrigNo.fOpposite = 25;	
				oTrigNo.fHypotenuse = 65;	
				break;
			case 62:	
				oTrigNo.fAdjacent = 63;	
				oTrigNo.fOpposite = 16;	
				oTrigNo.fHypotenuse = 65;	
				break;
			case 63:	
				oTrigNo.fAdjacent = 32;	
				oTrigNo.fOpposite = 60;	
				oTrigNo.fHypotenuse = 68;	
				break;
			case 64:	
				oTrigNo.fAdjacent = 60;	
				oTrigNo.fOpposite = 32;	
				oTrigNo.fHypotenuse = 68;	
				break;
			case 65:	
				oTrigNo.fAdjacent = 42;	
				oTrigNo.fOpposite = 56;	
				oTrigNo.fHypotenuse = 70;	
				break;
			case 66:	
				oTrigNo.fAdjacent = 56;	
				oTrigNo.fOpposite = 42;	
				oTrigNo.fHypotenuse = 70;	
				break;
			case 67:	
				oTrigNo.fAdjacent = 48;	
				oTrigNo.fOpposite = 55;	
				oTrigNo.fHypotenuse = 73;	
				break;
			case 68:	
				oTrigNo.fAdjacent = 55;	
				oTrigNo.fOpposite = 48;	
				oTrigNo.fHypotenuse = 73;	
				break;
			case 69:	
				oTrigNo.fAdjacent = 24;	
				oTrigNo.fOpposite = 70;	
				oTrigNo.fHypotenuse = 74;	
				break;
			case 70:	
				oTrigNo.fAdjacent = 70;	
				oTrigNo.fOpposite = 24;	
				oTrigNo.fHypotenuse = 74;	
				break;
			case 71:	
				oTrigNo.fAdjacent = 21;	
				oTrigNo.fOpposite = 72;	
				oTrigNo.fHypotenuse = 75;	
				break;
			case 72:	
				oTrigNo.fAdjacent = 45;	
				oTrigNo.fOpposite = 60;	
				oTrigNo.fHypotenuse = 75;	
				break;
			case 73:	
				oTrigNo.fAdjacent = 60;	
				oTrigNo.fOpposite = 45;	
				oTrigNo.fHypotenuse = 75;	
				break;
			case 74:	
				oTrigNo.fAdjacent = 72;	
				oTrigNo.fOpposite = 21;	
				oTrigNo.fHypotenuse = 75;	
				break;
			case 75:	
				oTrigNo.fAdjacent = 30;	
				oTrigNo.fOpposite = 72;	
				oTrigNo.fHypotenuse = 78;	
				break;
			case 76:	
				oTrigNo.fAdjacent = 72;	
				oTrigNo.fOpposite = 30;	
				oTrigNo.fHypotenuse = 78;	
				break;
			case 77:	
				oTrigNo.fAdjacent = 48;	
				oTrigNo.fOpposite = 64;	
				oTrigNo.fHypotenuse = 80;	
				break;
			case 78:	
				oTrigNo.fAdjacent = 64;	
				oTrigNo.fOpposite = 48;	
				oTrigNo.fHypotenuse = 80;	
				break;
			case 79:	
				oTrigNo.fAdjacent = 18;	
				oTrigNo.fOpposite = 80;	
				oTrigNo.fHypotenuse = 82;	
				break;
			case 80:	
				oTrigNo.fAdjacent = 80;	
				oTrigNo.fOpposite = 18;	
				oTrigNo.fHypotenuse = 82;	
				break;
			case 81:	
				oTrigNo.fAdjacent = 13;	
				oTrigNo.fOpposite = 84;	
				oTrigNo.fHypotenuse = 85;	
				break;
			case 82:	
				oTrigNo.fAdjacent = 36;	
				oTrigNo.fOpposite = 77;	
				oTrigNo.fHypotenuse = 85;	
				break;
			case 83:	
				oTrigNo.fAdjacent = 40;	
				oTrigNo.fOpposite = 75;	
				oTrigNo.fHypotenuse = 85;	
				break;
			case 84:	
				oTrigNo.fAdjacent = 51;	
				oTrigNo.fOpposite = 68;	
				oTrigNo.fHypotenuse = 85;	
				break;
			case 85:	
				oTrigNo.fAdjacent = 68;	
				oTrigNo.fOpposite = 51;	
				oTrigNo.fHypotenuse = 85;	
				break;
			case 86:	
				oTrigNo.fAdjacent = 75;	
				oTrigNo.fOpposite = 40;	
				oTrigNo.fHypotenuse = 85;	
				break;
			case 87:	
				oTrigNo.fAdjacent = 77;	
				oTrigNo.fOpposite = 36;	
				oTrigNo.fHypotenuse = 85;	
				break;
			case 88:	
				oTrigNo.fAdjacent = 84;	
				oTrigNo.fOpposite = 13;	
				oTrigNo.fHypotenuse = 85;	
				break;
			case 89:	
				oTrigNo.fAdjacent = 60;	
				oTrigNo.fOpposite = 63;	
				oTrigNo.fHypotenuse = 87;	
				break;
			case 90:	
				oTrigNo.fAdjacent = 63;	
				oTrigNo.fOpposite = 60;	
				oTrigNo.fHypotenuse = 87;	
				break;
			case 91:	
				oTrigNo.fAdjacent = 39;	
				oTrigNo.fOpposite = 80;	
				oTrigNo.fHypotenuse = 89;	
				break;
			case 92:	
				oTrigNo.fAdjacent = 80;	
				oTrigNo.fOpposite = 39;	
				oTrigNo.fHypotenuse = 89;	
				break;
			case 93:	
				oTrigNo.fAdjacent = 54;	
				oTrigNo.fOpposite = 72;	
				oTrigNo.fHypotenuse = 90;	
				break;
			case 94:	
				oTrigNo.fAdjacent = 72;	
				oTrigNo.fOpposite = 54;	
				oTrigNo.fHypotenuse = 90;	
				break;
			case 95:	
				oTrigNo.fAdjacent = 35;	
				oTrigNo.fOpposite = 84;	
				oTrigNo.fHypotenuse = 91;	
				break;
			case 96:	
				oTrigNo.fAdjacent = 84;	
				oTrigNo.fOpposite = 35;	
				oTrigNo.fHypotenuse = 91;	
				break;
			case 97:	
				oTrigNo.fAdjacent = 57;	
				oTrigNo.fOpposite = 76;	
				oTrigNo.fHypotenuse = 95;	
				break;
			case 98:	
				oTrigNo.fAdjacent = 76;	
				oTrigNo.fOpposite = 57;	
				oTrigNo.fHypotenuse = 95;	
				break;
			case 99:	
				oTrigNo.fAdjacent = 65;	
				oTrigNo.fOpposite = 72;	
				oTrigNo.fHypotenuse = 97;	
				break;
			case 100:	
				oTrigNo.fAdjacent = 72;	
				oTrigNo.fOpposite = 65;	
				oTrigNo.fHypotenuse = 97;	
				break;
			case 101:	
				oTrigNo.fAdjacent = 28;	
				oTrigNo.fOpposite = 96;	
				oTrigNo.fHypotenuse = 100;	
				break;
			case 102:	
				oTrigNo.fAdjacent = 60;	
				oTrigNo.fOpposite = 80;	
				oTrigNo.fHypotenuse = 100;	
				break;
			case 103:	
				oTrigNo.fAdjacent = 80;	
				oTrigNo.fOpposite = 60;	
				oTrigNo.fHypotenuse = 100;
				break;
			case 104:	
				oTrigNo.fAdjacent = 96;	
				oTrigNo.fOpposite = 28;	
				oTrigNo.fHypotenuse = 100;
				break;
			case 105:	
				oTrigNo.fAdjacent = 20;	
				oTrigNo.fOpposite = 99;	
				oTrigNo.fHypotenuse = 101;	
				break;
			case 106:	
				oTrigNo.fAdjacent = 99;	
				oTrigNo.fOpposite = 20;	
				oTrigNo.fHypotenuse = 101;	
				break;
			case 107:	
				oTrigNo.fAdjacent = 48;	
				oTrigNo.fOpposite = 90;	
				oTrigNo.fHypotenuse = 102;	
				break;
			case 108:	
				oTrigNo.fAdjacent = 90;	
				oTrigNo.fOpposite = 48;	
				oTrigNo.fHypotenuse = 102;	
				break;
			case 109:	
				oTrigNo.fAdjacent = 40;	
				oTrigNo.fOpposite = 96;	
				oTrigNo.fHypotenuse = 104;	
				break;
			case 110:	
				oTrigNo.fAdjacent = 96;	
				oTrigNo.fOpposite = 40;	
				oTrigNo.fHypotenuse = 104;	
				break;
			case 111:	
				oTrigNo.fAdjacent = 63;	
				oTrigNo.fOpposite = 84;	
				oTrigNo.fHypotenuse = 105;	
				break;
			case 112:	
				oTrigNo.fAdjacent = 84;	
				oTrigNo.fOpposite = 63;	
				oTrigNo.fHypotenuse = 105;	
				break;
			case 113:	
				oTrigNo.fAdjacent = 56;	
				oTrigNo.fOpposite = 90;	
				oTrigNo.fHypotenuse = 106;	
				break;
			case 114:	
				oTrigNo.fAdjacent = 90;	
				oTrigNo.fOpposite = 56;	
				oTrigNo.fHypotenuse = 106;	
				break;
			case 115:	
				oTrigNo.fAdjacent = 60;	
				oTrigNo.fOpposite = 91;	
				oTrigNo.fHypotenuse = 109;	
				break;
			case 116:	
				oTrigNo.fAdjacent = 91;	
				oTrigNo.fOpposite = 60;	
				oTrigNo.fHypotenuse = 109;	
				break;
			case 117:	
				oTrigNo.fAdjacent = 66;	
				oTrigNo.fOpposite = 88;	
				oTrigNo.fHypotenuse = 110;	
				break;
			case 118:	
				oTrigNo.fAdjacent = 88;	
				oTrigNo.fOpposite = 66;	
				oTrigNo.fHypotenuse = 110;	
				break;
			case 119:	
				oTrigNo.fAdjacent = 69;	
				oTrigNo.fOpposite = 92;	
				oTrigNo.fHypotenuse = 115;	
				break;
			case 120:	
				oTrigNo.fAdjacent = 92;	
				oTrigNo.fOpposite = 69;	
				oTrigNo.fHypotenuse = 115;	
				break;
			case 121:	
				oTrigNo.fAdjacent = 80;	
				oTrigNo.fOpposite = 84;	
				oTrigNo.fHypotenuse = 116;	
				break;
			case 122:	
				oTrigNo.fAdjacent = 84;	
				oTrigNo.fOpposite = 80;	
				oTrigNo.fHypotenuse = 116;	
				break;
			case 123:	
				oTrigNo.fAdjacent = 72;	
				oTrigNo.fOpposite = 96;	
				oTrigNo.fHypotenuse = 120;	
				break;
			case 124:	
				oTrigNo.fAdjacent = 96;	
				oTrigNo.fOpposite = 72;	
				oTrigNo.fHypotenuse = 120;	
				break;
			case 125:	
				oTrigNo.fAdjacent = 75;	
				oTrigNo.fOpposite = 100;	
				oTrigNo.fHypotenuse = 125;	
				break;
			case 126:	
				oTrigNo.fAdjacent = 100;	
				oTrigNo.fOpposite = 75;	
				oTrigNo.fHypotenuse = 125;	
				break;
		};
		
		if (oTrigNo.fHypotenuse != 0 ){
			oTrigNo.fAngle = sin(oTrigNo.fOpposite / oTrigNo.fHypotenuse);
		};
		
		return oTrigNo;
	}; //friendlyTrigNumbers
	
	
	bool stringEquals(string sStringA, string sStringB, bool bCaseIndependant, bool bTrim, bool bExcludeNonLettersAndNumbers){
		std::string sTempA;
		std::string sTempB;
		bool bIsDone;
		
		sTempA = sStringA;
		sTempB = sStringB;
		
		if (bCaseIndependant){
			std::transform(sTempA.begin(), sTempA.end(), sTempA.begin(), ::toupper);
			std::transform(sTempB.begin(), sTempB.end(), sTempB.begin(), ::toupper);
		}; //if case independant

		if(bTrim){
			bIsDone = false;
			while(not bIsDone){
				if(sTempA.length()==0) {
					bIsDone = true;
				}else{
					if(sTempA.substr(0, 1) == " "){
						sTempA = sTempA.substr(1, sTempA.length() - 1);
					}else{
						bIsDone = true;
					};
				};
			};//while
			
			bIsDone = false;
			while(not bIsDone){
				if(sTempA.length()==0) {
					bIsDone = true;
				}else{
					if(sTempA.substr(sTempA.length()-1, 1) == " "){
						sTempA = sTempA.substr(0, sTempA.length() - 1);
					}else{
						bIsDone = true;
					};
				};
			};//while
			
			bIsDone = false;
			while(not bIsDone){
				if(sTempB.length()==0) {
					bIsDone = true;
				}else{
					if(sTempB.substr(0, 1) == " "){
						sTempB = sTempB.substr(1, sTempB.length() - 1);
					}
					else{
						bIsDone = true;
					};
				};
			};//while
			
			bIsDone = false;
			while(not bIsDone){
				if(sTempB.length()==0) {
					bIsDone = true;
				}else{
					if(sTempB.substr(sTempB.length()-1, 1) == " "){
						sTempB = sTempB.substr(0, sTempB.length() - 1);
					}else{
						bIsDone = true;
					};
				};
			};//while
			
			
		};// if bTrim
		
		
		sTempA.clear();
		sTempB.clear();

		if (sTempA == sTempB) {
			return true;
		}else{
			return false;
		};
	};//stringEquals
						   
	bool Sqlite_TableDB(){
		return true;
	};
	
	bool Sqlite_TableExists(sqlite3 *db , std::string sTableName){
/*
		SQLiteCommand cmd = new SQLiteCommand(db);
		cmd.CommandText = "SELECT name FROM sqlite_master WHERE name='" + sTableName + "'";
		SQLiteDataReader rdr;
		rdr = cmd.ExecuteReader();
		if (rdr.HasRows)
			return true;
		else
			return false;
*/		
		return true;
	};

	bool Sqlite_Populated(){
		return true;
	};

	bool FileExists(string sFullPath){
		return true;
	};

	bool MultipleChoiceAnswerFound(vector<strStringAnswer> vec, std::string sAns) {
		vector<strStringAnswer>::iterator itAns;
		bool bIsFound;
		
		bIsFound = false;

		for (itAns=vec.begin(); itAns != vec.end(); itAns++) {
			if (itAns->sValue.compare(sAns.c_str()) == 0) {
				bIsFound = true;
			};
		};
		
		return bIsFound;
	};
	
	std::string numberInWords(int iFigure) {
		std::string sDigitLast;
		std::string sDigitTeens;
		std::string sDigitTens;
		std::string sResults;

		
		
		if (iFigure >= 0 and iFigure < 10) {
			switch ((int)fmod(iFigure, 10)) {
				case 0:
					sDigitLast = "Zero";
					break;
				case 1:
					sDigitLast = "One";
					break;
				case 2:
					sDigitLast = "Two";
					break;
				case 3:
					sDigitLast = "Three";
					break;
				case 4:
					sDigitLast = "Four";
					break;
				case 5:
					sDigitLast = "Five";
					break;
				case 6:
					sDigitLast = "Six";
					break;
				case 7:
					sDigitLast = "Severn";
					break;
				case 8:
					sDigitLast = "Eight";
					break;
				case 9:
					sDigitLast = "Nine";
					break;
				default:
					sDigitLast = "";
			};
			
			sResults = sDigitLast;
		} else if (iFigure > 9 and iFigure <20) {
			switch (iFigure) {
				case 10:
					sDigitTeens = "Ten";
					break;
				case 11:
					sDigitTeens = "Eleven";
					break;
				case 12:
					sDigitTeens = "Twelve";
					break;
				case 13:
					sDigitTeens = "Thirteen";
					break;
				case 14:
					sDigitTeens = "Forteen";
					break;
				case 15:
					sDigitTeens = "Fifteen";
					break;
				case 16:
					sDigitTeens = "Sixteen";
					break;
				case 17:
					sDigitTeens = "Seveteen";
					break;
				case 18:
					sDigitTeens = "Eighteen";
					break;
				case 19:
					sDigitTeens = "Ninteen";
					break;
				default:
					sDigitTeens = "";
			};
			
			sResults = sDigitTeens;
		} else if (iFigure >= 20 and iFigure < 100) {
			
			switch ((int)fmod(iFigure / 10, 10)) {
				case 2:
					sDigitTens = "Twenty";
					break;
				case 3:
					sDigitTens = "Thrirty";
					break;
				case 4:
					sDigitTens = "Forty";
					break;
				case 5:
					sDigitTens = "Fifty";
					break;
				case 6:
					sDigitTens = "Sixty";
					break;
				case 7:
					sDigitTens = "Seventy";
					break;
				case 8:
					sDigitTens = "Eighty";
					break;
				case 9:
					sDigitTens = "Ninety";
					break;
				default:
					sDigitTens = "";
			}
			
			switch ((int)fmod(iFigure, 10)) {
				case 1:
					sDigitLast = "One";
					break;
				case 2:
					sDigitLast = "Two";
					break;
				case 3:
					sDigitLast = "Three";
					break;
				case 4:
					sDigitLast = "Four";
					break;
				case 5:
					sDigitLast = "Five";
					break;
				case 6:
					sDigitLast = "Six";
					break;
				case 7:
					sDigitLast = "Severn";
					break;
				case 8:
					sDigitLast = "Eight";
					break;
				case 9:
					sDigitLast = "Nine";
					break;
				default:
					sDigitLast = "";
			};
		};
		
				
		sDigitLast.clear();
		sDigitTeens.clear();
		sDigitTens.clear();
//		sResults;

		return sResults;
	};

	
	std::string numberInWordsIst(int iFigure) {
		std::string sDigitLast;
		std::string sDigitTeens;
		std::string sDigitTens;
		std::string sResults;
		
		
		
		if (iFigure >= 0 and iFigure < 10) {
			switch ((int)fmod(iFigure, 10)) {
				case 0:
					sDigitLast = "Zero";
					break;
				case 1:
					sDigitLast = "First";
					break;
				case 2:
					sDigitLast = "Second";
					break;
				case 3:
					sDigitLast = "Third";
					break;
				case 4:
					sDigitLast = "Forth";
					break;
				case 5:
					sDigitLast = "Fifth";
					break;
				case 6:
					sDigitLast = "Sixth";
					break;
				case 7:
					sDigitLast = "Severnth";
					break;
				case 8:
					sDigitLast = "Eight";
					break;
				case 9:
					sDigitLast = "Nineth";
					break;
				default:
					sDigitLast = "";
			};
			
			sResults = sDigitLast;
		} else if (iFigure > 9 and iFigure <20) {
			switch (iFigure) {
				case 10:
					sDigitTeens = "Ten";
					break;
				case 11:
					sDigitTeens = "Eleven";
					break;
				case 12:
					sDigitTeens = "Twelve";
					break;
				case 13:
					sDigitTeens = "Thirteen";
					break;
				case 14:
					sDigitTeens = "Forteen";
					break;
				case 15:
					sDigitTeens = "Fifteen";
					break;
				case 16:
					sDigitTeens = "Sixteen";
					break;
				case 17:
					sDigitTeens = "Seveteen";
					break;
				case 18:
					sDigitTeens = "Eighteen";
					break;
				case 19:
					sDigitTeens = "Ninteen";
					break;
				default:
					sDigitTeens = "";
			};
			
			sResults = sDigitTeens;
		} else if (iFigure >= 20 and iFigure < 100) {
			
			switch ((int)fmod(iFigure / 10, 10)) {
				case 2:
					sDigitTens = "Twenty";
					break;
				case 3:
					sDigitTens = "Thrirty";
					break;
				case 4:
					sDigitTens = "Forty";
					break;
				case 5:
					sDigitTens = "Fifty";
					break;
				case 6:
					sDigitTens = "Sixty";
					break;
				case 7:
					sDigitTens = "Seventy";
					break;
				case 8:
					sDigitTens = "Eighty";
					break;
				case 9:
					sDigitTens = "Ninety";
					break;
				default:
					sDigitTens = "";
			}
			
			switch ((int)fmod(iFigure, 10)) {
				case 1:
					sDigitLast = "One";
					break;
				case 2:
					sDigitLast = "Two";
					break;
				case 3:
					sDigitLast = "Three";
					break;
				case 4:
					sDigitLast = "Four";
					break;
				case 5:
					sDigitLast = "Five";
					break;
				case 6:
					sDigitLast = "Six";
					break;
				case 7:
					sDigitLast = "Severn";
					break;
				case 8:
					sDigitLast = "Eight";
					break;
				case 9:
					sDigitLast = "Nine";
					break;
				default:
					sDigitLast = "";
			};
		};
		
		
		sDigitLast.clear();
		sDigitTeens.clear();
		sDigitTens.clear();
//		sResults;

		return sResults;
	};

	std::string symbolsInWords(std::string) {
	
	};

	std::string convertSecondsIntoTimeString(int iTotalSeconds) {
		int iSeconds;
		int iMinutes;
		int iHours;
		int iRemainingSeconds;
		
		iHours = iTotalSeconds / 3600;
		
		iRemainingSeconds = iTotalSeconds - (iHours * 3600);
		
		iMinutes = iRemainingSeconds / 60;
		
		iSeconds = iRemainingSeconds - (iMinutes * 60);

		std::stringstream ssTime;
		std::string sTime;
		
		ssTime << iHours << ":" << iMinutes << ":" << iSeconds;

		sTime = ssTime.str();
		
		return sTime;
	};

	std::string convertEnumText_QuestionType(enumQuestionType eQuestionType) {
		switch  (eQuestionType){
			case eQuesAddingNumbers:
				return "eQuesAddingNumbers";
				break;
			case eQuesAddingNumbersEasy:
				return "eQuesAddingNumbersEasy";
				break;
			case eQuesAddingNumbersMedium:
				return "eQuesAddingNumbersMedium";
				break;
			case eQuesAddingNumbersHard:
				return "eQuesAddingNumbersHard";
				break;
			case eQuesSubtractingNumbers:
				return "eQuesSubtractingNumbers";
				break;
			case eQuesSubtractingNumbersEasy:
				return "eQuesSubtractingNumbersEasy";
				break;
			case eQuesSubtractingNumbersMedium:
				return "eQuesSubtractingNumbersMedium";
				break;
			case eQuesSubtractingNumbersHard:
				return "eQuesSubtractingNumbersHard";
				break;
			case eQuesMultiplingNumbers:
				return "eQuesMultiplingNumbers";
				break;
			case eQuesMultiplingNumbersEasy:
				return "eQuesMultiplingNumbersEasy";
				break;
			case eQuesMultiplingNumbersMedium:
				return "eQuesMultiplingNumbersMedium";
				break;
			case eQuesMultiplingNumbersHard:
				return "eQuesMultiplingNumbersHard";
				break;
			case eQuesDividingNumbers:
				return "eQuesDividingNumbers";
				break;
			case eQuesDividingNumbersEasy:
				return "eQuesDividingNumbersEasy";
				break;
			case eQuesDividingNumbersMedium:
				return "eQuesDividingNumbersMedium";
				break;
			case eQuesDividingNumbersHard:
				return "eQuesDividingNumbersHard";
				break;
//			case eQuesDifferentiation:
//				return "eQuesDifferentiation";
//				break;
//			case eQuesDifferentiation2BasePoly:
//				return "eQuesDifferentiation2BasePoly";
//				break;
			case eQuesDifferentiation3BasePoly:
				return "eQuesDifferentiation3BasePoly";
				break;
			case eQuesDifferentiationExpFn:
				return "eQuesDifferentiationExpFn";
				break;
			case eQuesDifferentiationTrigFn:
				return "eQuesDifferentiationTrigFn";
				break;
			case eQuesDifferentiationChainRule:
				return "eQuesDifferentiationChainRule";
				break;
			case eQuesDifferentiationEverything:
				return "eQuesDifferentiationEverything";
				break;
			case eQuesTrigFromHypAdjCalcOpp:
				return "eQuesTrigFromHypAdjCalcOpp";
				break;
			case eQuesTrigFromHypOppCalcAdj:
				return "eQuesTrigFromHypOppCalcAdj";
				break;
			case eQuesTrigFromOppAdjCalcHyp:
				return "eQuesTrigFromOppAdjCalcHyp";
				break;

			case eQuesSquare:
				return "eQuesSquare";
				break;
			case eQuesSquare1Digit:
				return "eQuesSquare1Digit";
				break;
			case eQuesSquare2Digit:
				return "eQuesSquare2Digit";
				break;
			case eQuesSquare3Digit:
				return "eQuesSquare3Digit";
				break;
				
			case eQuesCubed:
				return "eQuesCubed";
				break;
			case eQuesCubed1Digit:
				return "eQuesCubed1Digit";
				break;
			case eQuesCubed2Digit:
				return "eQuesCubed2Digit";
				break;
			case eQuesCubed3Digit:
				return "eQuesCubed3Digit";
				break;
				
			case eQuesSquareRoot:
				return "eQuesSquareRoot";
				break;
			case eQuesSquareRoot1Digit:
				return "eQuesSquareRoot1Digit";
				break;
			case eQuesSquareRoot2Digit:
				return "eQuesSquareRoot2Digit";
				break;
			case eQuesSquareRoot3Digit:
				return "eQuesSquareRoot3Digit";
				break;
				
			case eQuesCubedRoot:
				return "eQuesCubedRoot";
				break;
			case eQuesCubedRoot1Digit:
				return "eQuesCubedRoot1Digit";
				break;
			case eQuesCubedRoot2Digit:
				return "eQuesCubedRoot2Digit";
				break;
			case eQuesCubedRoot3Digit:
				return "eQuesCubedRoot3Digit";
				break;
				
			case eQuesArithmetic:
				return "eQuesArithmetic";
				break;
			case eQuesArithmeticEasy:
				return "eQuesArithmeticEasy";
				break;
			case eQuesArithmeticMedium:
				return "eQuesArithmeticMedium";
				break;
			case eQuesArithmeticHard:
				return "eQuesArithmeticHard";
				break;
			case eQuesPolynomial:
				return "eQuesPolynomial";
				break;
			case eQuesPolynomialEasy:
				return "eQuesPolynomialEasy";
				break;
			case eQuesPolynomialMedium:
				return "eQuesPolynomialMedium";
				break;
			case eQuesPolynomialHard:
				return "eQuesPolynomialHard";
				break;

			case eQuesParallogram:
				return "eQuesParallogram";
				break;
			case eQuesParallogramVeryEasy:
				return "eQuesParallogramVeryEasy";
				break;
			case eQuesParallogramEasy:
				return "eQuesParallogramEasy";
				break;
			case eQuesParallogramMedium:
				return "eQuesParallogramMedium";
				break;
			case eQuesParallogramHard:
				return "eQuesParallogramHard";
				break;
			case eQuesParallogramVeryHard:
				return "eQuesParallogramVeryHard";
				break;
				
			case eQuesPythagorean:
				return "eQuesPythagorean";
				break;
			case eQuesPythagoreanVeryEasy:
				return "eQuesPythagoreanVeryEasy";
				break;
			case eQuesPythagoreanEasy:
				return "eQuesPythagoreanEasy";
				break;
			case eQuesPythagoreanMedium:
				return "eQuesPythagoreanMedium";
				break;
			case eQuesPythagoreanHard:
				return "eQuesPythagoreanHard";
				break;
			case eQuesPythagoreanVeryHard:
				return "eQuesPythagoreanVeryHard";
				break;
				
			case eQuesTrigonometry:
				return "eQuesTrigonometry";
				break;
			case eQuesTrigonometryVeryEasy:
				return "eQuesTrigonometryVeryEasy";
				break;
			case eQuesTrigonometryEasy:
				return "eQuesTrigonometryEasy";
				break;
			case eQuesTrigonometryMedium:
				return "eQuesTrigonometryMedium";
				break;
			case eQuesTrigonometryHard:
				return "eQuesTrigonometryHard";
				break;
			case eQuesTrigonometryVeryHard:
				return "eQuesTrigonometryVeryHard";
				break;
				
			case eQuesCylinder:
				return "eQuesCylinder";
				break;
			case eQuesCylinderVeryEasy:
				return "eQuesCylinderVeryEasy";
				break;
			case eQuesCylinderEasy:
				return "eQuesCylinderEasy";
				break;
			case eQuesCylinderMedium:
				return "eQuesCylinderMedium";
				break;
			case eQuesCylinderHard:
				return "eQuesCylinderHard";
				break;
			case eQuesCylinderVeryHard:
				return "eQuesCylinderVeryHard";
				break;
				
			case eQuesAverageMean:
				return "eQuesAverageMean";
				break;
			case eQuesAverageMeanVeryEasy:
				return "eQuesAverageMeanVeryEasy";
				break;
			case eQuesAverageMeanEasy:
				return "eQuesAverageMeanEasy";
				break;
			case eQuesAverageMeanMedium:
				return "eQuesAverageMeanMedium";
				break;
			case eQuesAverageMeanHard:
				return "eQuesAverageMeanHard";
				break;
			case eQuesAverageMeanVeryHard:
				return "eQuesAverageMeanVeryHard";
				break;
				
			case eQuesAverageMedian:
				return "eQuesAverageMedian";
				break;
			case eQuesAverageMedianVeryEasy:
				return "eQuesAverageMedianVeryEasy";
				break;
			case eQuesAverageMedianEasy:
				return "eQuesAverageMedianEasy";
				break;
			case eQuesAverageMedianMedium:
				return "eQuesAverageMedianMedium";
				break;
			case eQuesAverageMedianHard:
				return "eQuesAverageMedianHard";
				break;
			case eQuesAverageMedianVeryHard:
				return "eQuesAverageMedianVeryHard";
				break;
				
			case eQuesAverageMode:
				return "eQuesAverageMode";
				break;
			case eQuesAverageModeVeryEasy:
				return "eQuesAverageModeVeryEasy";
				break;
			case eQuesAverageModeEasy:
				return "eQuesAverageModeEasy";
				break;
			case eQuesAverageModeMedium:
				return "eQuesAverageModeMedium";
				break;
			case eQuesAverageModeHard:
				return "eQuesAverageModeHard";
				break;
			case eQuesAverageModeVeryHard:
				return "eQuesAverageModeVeryHard";
				break;
				
			case eQuesDifferentiation2BasePoly:
				return "eQuesDifferentiation2BasePoly";
				break;
			case eQuesDifferentiation2BasePolyVeryEasy:
				return "eQuesDifferentiation2BasePolyVeryEasy";
				break;
			case eQuesDifferentiation2BasePolyEasy:
				return "eQuesDifferentiation2BasePolyEasy";
				break;
			case eQuesDifferentiation2BasePolyMedium:
				return "eQuesDifferentiation2BasePolyMedium";
				break;
			case eQuesDifferentiation2BasePolyHard:
				return "eQuesDifferentiation2BasePolyHard";
				break;
			case eQuesDifferentiation2BasePolyVeryHard:
				return "eQuesDifferentiation2BasePolyVeryHard";
				break;

			case eQuesFractionsAddSub:
				return "eQuesFractionsAddSub";
				break;
			case eQuesFractionsAddSubVeryEasy:
				return "eQuesFractionsAddSubVeryEasy";
				break;
			case eQuesFractionsAddSubEasy:
				return "eQuesFractionsAddSubEasy";
				break;
			case eQuesFractionsAddSubMedium:
				return "eQuesFractionsAddSubMedium";
				break;
			case eQuesFractionsAddSubHard:
				return "eQuesFractionsAddSubHard";
				break;
			case eQuesFractionsAddSubVeryHard:
				return "eQuesFractionsAddSubVeryHard";
				break;
				
			case eQuesFractionsMultiDiv:
				return "eQuesFractionsMultiDiv";
				break;
			case eQuesFractionsMultiDivVeryEasy:
				return "eQuesFractionsMultiDivVeryEasy";
				break;
			case eQuesFractionsMultiDivEasy:
				return "eQuesFractionsMultiDivEasy";
				break;
			case eQuesFractionsMultiDivMedium:
				return "eQuesFractionsMultiDivMedium";
				break;
			case eQuesFractionsMultiDivHard:
				return "eQuesFractionsMultiDivHard";
				break;
			case eQuesFractionsMultiDivVeryHard:
				return "eQuesFractionsMultiDivVeryHard";
				break;
				
			case eQuesEquationsQuadratic:
				return "eQuesEquationsQuadratic";
				break;
			case eQuesEquationsQuadraticVeryEasy:
				return "eQuesEquationsQuadraticVeryEasy";
				break;
			case eQuesEquationsQuadraticEasy:
				return "eQuesEquationsQuadraticEasy";
				break;
			case eQuesEquationsQuadraticMedium:
				return "eQuesEquationsQuadraticMedium";
				break;
			case eQuesEquationsQuadraticHard:
				return "eQuesEquationsQuadraticHard";
				break;
			case eQuesEquationsQuadraticVeryHard:
				return "eQuesEquationsQuadraticVeryHard";
				break;
				
			case eQuesEquationsSimultaneousLinear:
				return "eQuesEquationsSimultaneousLinear";
				break;
			case eQuesEquationsSimultaneousLinearVeryEasy:
				return "eQuesEquationsSimultaneousLinearVeryEasy";
				break;
			case eQuesEquationsSimultaneousLinearEasy:
				return "eQuesEquationsSimultaneousLinearEasy";
				break;
			case eQuesEquationsSimultaneousLinearMedium:
				return "eQuesEquationsSimultaneousLinearMedium";
				break;
			case eQuesEquationsSimultaneousLinearHard:
				return "eQuesEquationsSimultaneousLinearHard";
				break;
			case eQuesEquationsSimultaneousLinearVeryHard:
				return "eQuesEquationsSimultaneousLinearVeryHard";
				break;
				
				
			case eQuesError:
				return "eQuesError";
				break;
			default:
				return "eQuesError";
				break;
		};
	};
		
	enumQuestionType convertEnumText_QuestionType(std::string sQuestionType) {
		if (sQuestionType.compare("eQuesAddingNumbers") == 0) {
			return eQuesAddingNumbers;
		} else if (sQuestionType.compare("Adding") == 0) {
				return eQuesAddingNumbers;
		} else if (sQuestionType.compare("eQuesAddingNumbersEasy") == 0) {
				return eQuesAddingNumbersEasy;
		} else if (sQuestionType.compare("eQuesAddingNumbersMedium") == 0) {
			return eQuesAddingNumbersMedium;
		} else if (sQuestionType.compare("eQuesAddingNumbersHard") == 0) {
			return eQuesAddingNumbersHard;
		} else if (sQuestionType.compare("Subtracting") == 0) {
			return eQuesSubtractingNumbers;
		} else if (sQuestionType.compare("eQuesSubtractingNumbers") == 0) {
			return eQuesSubtractingNumbers;
		} else if (sQuestionType.compare("eQuesSubtractingNumbersEasy") == 0) {
			return eQuesSubtractingNumbersEasy;
		} else if (sQuestionType.compare("eQuesSubtractingNumbersMedium") == 0) {
			return eQuesSubtractingNumbersMedium;
		} else if (sQuestionType.compare("eQuesSubtractingNumbersHard") == 0) {
			return eQuesSubtractingNumbersHard;
		} else if (sQuestionType.compare("Multipling") == 0) {
			return eQuesMultiplingNumbers;
		} else if (sQuestionType.compare("eQuesMultiplingNumbers") == 0) {
			return eQuesMultiplingNumbers;
		} else if (sQuestionType.compare("eQuesMultiplingNumbersEasy") == 0) {
			return eQuesMultiplingNumbersEasy;
		} else if (sQuestionType.compare("eQuesMultiplingNumbersMedium") == 0) {
			return eQuesMultiplingNumbersMedium;
		} else if (sQuestionType.compare("eQuesMultiplingNumbersHard") == 0) {
			return eQuesMultiplingNumbersHard;
		} else if (sQuestionType.compare("Division") == 0) {
			return eQuesDividingNumbers;
		} else if (sQuestionType.compare("eQuesDividingNumbers") == 0) {
			return eQuesDividingNumbers;
		} else if (sQuestionType.compare("eQuesDividingNumbersEasy") == 0) {
			return eQuesDividingNumbersEasy;
		} else if (sQuestionType.compare("eQuesDividingNumbersMedium") == 0) {
			return eQuesDividingNumbersMedium;
		} else if (sQuestionType.compare("eQuesDividingNumbersHard") == 0) {
			return eQuesDividingNumbersHard;
		} else if (sQuestionType.compare("Multiplication") == 0) {
			return eQuesMultiplingNumbers;
		} else if (sQuestionType.compare("eQuesMultiplingNumbers") == 0) {
			return eQuesMultiplingNumbers;
		} else if (sQuestionType.compare("eQuesMultiplingNumbersEasy") == 0) {
			return eQuesMultiplingNumbersEasy;
		} else if (sQuestionType.compare("eQuesMultiplingNumbersMedium") == 0) {
			return eQuesMultiplingNumbersMedium;
		} else if (sQuestionType.compare("eQuesMultiplingNumbersHard") == 0) {
			return eQuesMultiplingNumbersHard;
			
		} else if (sQuestionType.compare("eQuesDifferentiation2BasePoly") == 0) {
			return eQuesDifferentiation2BasePoly;
		} else if (sQuestionType.compare("eQuesDifferentiation3BasePoly") == 0) {
			return eQuesDifferentiation3BasePoly;
		} else if (sQuestionType.compare("eQuesDifferentiationExpFn") == 0) {
			return eQuesDifferentiationExpFn;
		} else if (sQuestionType.compare("eQuesDifferentiationTrigFn") == 0) {
			return eQuesDifferentiationTrigFn;
		} else if (sQuestionType.compare("eQuesDifferentiationChainRule") == 0) {
			return eQuesDifferentiationChainRule;
		} else if (sQuestionType.compare("eQuesDifferentiationEverything") == 0) {
			return eQuesDifferentiationEverything;
		
		} else if (sQuestionType.compare("eQuesTrigFromHypAdjCalcOpp") == 0) {
			return eQuesTrigFromHypAdjCalcOpp;
		} else if (sQuestionType.compare("eQuesTrigFromHypOppCalcAdj") == 0) {
			return eQuesTrigFromHypOppCalcAdj;
		} else if (sQuestionType.compare("eQuesTrigFromOppAdjCalcHyp") == 0) {
			return eQuesTrigFromOppAdjCalcHyp;
			
		} else if (sQuestionType.compare("Squared") == 0) {
			return eQuesSquare;
		} else if (sQuestionType.compare("eQuesSquare") == 0) {
			return eQuesSquare;
		} else if (sQuestionType.compare("eQuesSquare1Digit") == 0) {
			return eQuesSquare1Digit;
		} else if (sQuestionType.compare("eQuesSquare2Digit") == 0) {
			return eQuesSquare2Digit;
		} else if (sQuestionType.compare("eQuesSquare3Digit") == 0) {
			return eQuesSquare3Digit;
			
		} else if (sQuestionType.compare("Cubed") == 0) {
			return eQuesCubed;
		} else if (sQuestionType.compare("eQuesCubed") == 0) {
			return eQuesCubed;
		} else if (sQuestionType.compare("eQuesCubed1Digit") == 0) {
			return eQuesCubed1Digit;
		} else if (sQuestionType.compare("eQuesCubed2Digit") == 0) {
			return eQuesCubed2Digit;
		} else if (sQuestionType.compare("eQuesCubed3Digit") == 0) {
			return eQuesCubed3Digit;
			
		} else if (sQuestionType.compare("Squared Root") == 0) {
			return eQuesSquareRoot;
		} else if (sQuestionType.compare("eQuesSquareRoot") == 0) {
			return eQuesSquareRoot;
		} else if (sQuestionType.compare("eQuesSquareRoot1Digit") == 0) {
			return eQuesSquareRoot1Digit;
		} else if (sQuestionType.compare("eQuesSquareRoot2Digit") == 0) {
			return eQuesSquareRoot2Digit;
		} else if (sQuestionType.compare("eQuesSquareRoot3Digit") == 0) {
			return eQuesSquareRoot3Digit;
			
		} else if (sQuestionType.compare("Cubed Root") == 0) {
			return eQuesCubedRoot;
		} else if (sQuestionType.compare("eQuesCubedRoot") == 0) {
			return eQuesCubedRoot;
		} else if (sQuestionType.compare("eQuesCubedRoot1Digit") == 0) {
			return eQuesCubedRoot1Digit;
		} else if (sQuestionType.compare("eQuesCubedRoot2Digit") == 0) {
			return eQuesCubedRoot2Digit;
		} else if (sQuestionType.compare("eQuesCubedRoot3Digit") == 0) {
			return eQuesCubedRoot3Digit;
			
		} else if (sQuestionType.compare("Misc") == 0) {
			return eQuesArithmetic;
		} else if (sQuestionType.compare("eQuesArithmetic") == 0) {
			return eQuesArithmetic;
		} else if (sQuestionType.compare("eQuesArithmeticEasy") == 0) {
			return eQuesArithmeticEasy;
		} else if (sQuestionType.compare("eQuesArithmeticMedium") == 0) {
			return eQuesArithmeticMedium;
		} else if (sQuestionType.compare("eQuesArithmeticHard") == 0) {
			return eQuesArithmeticHard;

		} else if (sQuestionType.compare("eQuesPolynomialEasy") == 0) {
			return eQuesPolynomialEasy;
		} else if (sQuestionType.compare("eQuesPolynomialMedium") == 0) {
			return eQuesPolynomialMedium;
		} else if (sQuestionType.compare("eQuesPolynomialHard") == 0) {
			return eQuesPolynomialHard;
            
		} else if (sQuestionType.compare("Triangle") == 0) {
			return eQuesPythagorean;
		} else if (sQuestionType.compare("Pythagorean") == 0) {
			return eQuesPythagorean;
		} else if (sQuestionType.compare("eQuesPythagorean") == 0) {
			return eQuesPythagorean;
		} else if (sQuestionType.compare("eQuesPythagoreanVeryEasy") == 0) {
			return eQuesPythagoreanVeryEasy;
		} else if (sQuestionType.compare("eQuesPythagoreanEasy") == 0) {
			return eQuesPythagoreanEasy;
		} else if (sQuestionType.compare("eQuesPythagoreanMedium") == 0) {
			return eQuesPythagoreanMedium;
		} else if (sQuestionType.compare("eQuesPythagoreanHard") == 0) {
			return eQuesPythagoreanHard;
		} else if (sQuestionType.compare("eQuesPythagoreanVeryHard") == 0) {
			return eQuesPythagoreanVeryHard;
            
		} else if (sQuestionType.compare("Trigonometry") == 0) {
			return eQuesTrigonometry;
		} else if (sQuestionType.compare("eQuesTrigonometry") == 0) {
			return eQuesTrigonometry;
		} else if (sQuestionType.compare("eQuesTrigonometryVeryEasy") == 0) {
			return eQuesTrigonometryVeryEasy;
		} else if (sQuestionType.compare("eQuesTrigonometryEasy") == 0) {
			return eQuesTrigonometryEasy;
		} else if (sQuestionType.compare("eQuesTrigonometryMedium") == 0) {
			return eQuesTrigonometryMedium;
		} else if (sQuestionType.compare("eQuesTrigonometryHard") == 0) {
			return eQuesTrigonometryHard;
		} else if (sQuestionType.compare("eQuesTrigonometryVeryHard") == 0) {
			return eQuesTrigonometryVeryHard;
            
		} else if (sQuestionType.compare("Parallogram") == 0) {
			return eQuesParallogram;
		} else if (sQuestionType.compare("eQuesParallogram") == 0) {
			return eQuesParallogram;
		} else if (sQuestionType.compare("eQuesParallogramVeryEasy") == 0) {
			return eQuesParallogramVeryEasy;
		} else if (sQuestionType.compare("eQuesParallogramEasy") == 0) {
			return eQuesParallogramEasy;
		} else if (sQuestionType.compare("eQuesParallogramMedium") == 0) {
			return eQuesParallogramMedium;
		} else if (sQuestionType.compare("eQuesParallogramHard") == 0) {
			return eQuesParallogramHard;
		} else if (sQuestionType.compare("eQuesParallogramVeryHard") == 0) {
			return eQuesParallogramVeryHard;
		} else if (sQuestionType.compare("eQuesError") == 0) {
			return eQuesError;
			
		} else if (sQuestionType.compare("Cylinder") == 0) {
			return eQuesCylinder;
		} else if (sQuestionType.compare("eQuesCylinder") == 0) {
			return eQuesCylinder;
		} else if (sQuestionType.compare("eQuesCylinderVeryEasy") == 0) {
			return eQuesCylinderVeryEasy;
		} else if (sQuestionType.compare("eQuesCylinderEasy") == 0) {
			return eQuesCylinderEasy;
		} else if (sQuestionType.compare("eQuesCylinderMedium") == 0) {
			return eQuesCylinderMedium;
		} else if (sQuestionType.compare("eQuesCylinderHard") == 0) {
			return eQuesCylinderHard;
		} else if (sQuestionType.compare("eQuesCylinderVeryHard") == 0) {
			return eQuesCylinderVeryHard;

		} else if (sQuestionType.compare("Average Mean") == 0) {
			return eQuesAverageMean;
		} else if (sQuestionType.compare("eQuesAverageMean") == 0) {
			return eQuesAverageMean;
		} else if (sQuestionType.compare("eQuesAverageMeanVeryEasy") == 0) {
			return eQuesAverageMeanVeryEasy;
		} else if (sQuestionType.compare("eQuesAverageMeanEasy") == 0) {
			return eQuesAverageMeanEasy;
		} else if (sQuestionType.compare("eQuesAverageMeanMedium") == 0) {
			return eQuesAverageMeanMedium;
		} else if (sQuestionType.compare("eQuesAverageMeanHard") == 0) {
			return eQuesAverageMeanHard;
		} else if (sQuestionType.compare("eQuesAverageMeanVeryHard") == 0) {
			return eQuesAverageMeanVeryHard;
			
		} else if (sQuestionType.compare("Average Median") == 0) {
			return eQuesAverageMedian;
		} else if (sQuestionType.compare("eQuesAverageMedian") == 0) {
			return eQuesAverageMedian;
		} else if (sQuestionType.compare("eQuesAverageMedianVeryEasy") == 0) {
			return eQuesAverageMedianVeryEasy;
		} else if (sQuestionType.compare("eQuesAverageMedianEasy") == 0) {
			return eQuesAverageMedianEasy;
		} else if (sQuestionType.compare("eQuesAverageMedianMedium") == 0) {
			return eQuesAverageMedianMedium;
		} else if (sQuestionType.compare("eQuesAverageMedianHard") == 0) {
			return eQuesAverageMedianHard;
		} else if (sQuestionType.compare("eQuesAverageMedianVeryHard") == 0) {
			return eQuesAverageMedianVeryHard;
			
		} else if (sQuestionType.compare("Average Mode") == 0) {
			return eQuesAverageMode;
		} else if (sQuestionType.compare("eQuesAverageMode") == 0) {
			return eQuesAverageMode;
		} else if (sQuestionType.compare("eQuesAverageModeVeryEasy") == 0) {
			return eQuesAverageModeVeryEasy;
		} else if (sQuestionType.compare("eQuesAverageModeEasy") == 0) {
			return eQuesAverageModeEasy;
		} else if (sQuestionType.compare("eQuesAverageModeMedium") == 0) {
			return eQuesAverageModeMedium;
		} else if (sQuestionType.compare("eQuesAverageModeHard") == 0) {
			return eQuesAverageModeHard;
		} else if (sQuestionType.compare("eQuesAverageModeVeryHard") == 0) {
			return eQuesAverageModeVeryHard;
			
		} else if (sQuestionType.compare("Differentiation Base 2 Polynomial") == 0) {
			return eQuesDifferentiation2BasePoly;
		} else if (sQuestionType.compare("eQuesDifferentiation2BasePoly") == 0) {
			return eQuesDifferentiation2BasePoly;
		} else if (sQuestionType.compare("eQuesDifferentiation2BasePolyVeryEasy") == 0) {
			return eQuesDifferentiation2BasePolyVeryEasy;
		} else if (sQuestionType.compare("eQuesDifferentiation2BasePolyEasy") == 0) {
			return eQuesDifferentiation2BasePolyEasy;
		} else if (sQuestionType.compare("eQuesDifferentiation2BasePolyMedium") == 0) {
			return eQuesDifferentiation2BasePolyMedium;
		} else if (sQuestionType.compare("eQuesDifferentiation2BasePolyHard") == 0) {
			return eQuesDifferentiation2BasePolyHard;
		} else if (sQuestionType.compare("eQuesDifferentiation2BasePolyVeryHard") == 0) {
			return eQuesDifferentiation2BasePolyVeryHard;
			
		} else if (sQuestionType.compare("Fractions Add Subtract") == 0) {
			return eQuesFractionsAddSub;
		} else if (sQuestionType.compare("eQuesFractionsAddSub") == 0) {
			return eQuesFractionsAddSub;
		} else if (sQuestionType.compare("eQuesFractionsAddSubVeryEasy") == 0) {
			return eQuesFractionsAddSubVeryEasy;
		} else if (sQuestionType.compare("eQuesFractionsAddSubEasy") == 0) {
			return eQuesFractionsAddSubEasy;
		} else if (sQuestionType.compare("eQuesFractionsAddSubMedium") == 0) {
			return eQuesFractionsAddSubMedium;
		} else if (sQuestionType.compare("eQuesFractionsAddSubHard") == 0) {
			return eQuesFractionsAddSubHard;
		} else if (sQuestionType.compare("eQuesFractionsAddSubVeryHard") == 0) {
			return eQuesFractionsAddSubVeryHard;
			
		} else if (sQuestionType.compare("Fractions Multiply Divide") == 0) {
			return eQuesFractionsMultiDiv;
		} else if (sQuestionType.compare("eQuesFractionsMultiDiv") == 0) {
			return eQuesFractionsMultiDiv;
		} else if (sQuestionType.compare("eQuesFractionsMultiDivVeryEasy") == 0) {
			return eQuesFractionsMultiDivVeryEasy;
		} else if (sQuestionType.compare("eQuesFractionsMultiDivEasy") == 0) {
			return eQuesFractionsMultiDivEasy;
		} else if (sQuestionType.compare("eQuesFractionsMultiDivMedium") == 0) {
			return eQuesFractionsMultiDivMedium;
		} else if (sQuestionType.compare("eQuesFractionsMultiDivHard") == 0) {
			return eQuesFractionsMultiDivHard;
		} else if (sQuestionType.compare("eQuesFractionsMultiDivVeryHard") == 0) {
			return eQuesFractionsMultiDivVeryHard;
			
		} else if (sQuestionType.compare("Quadratic Equations") == 0) {
			return eQuesEquationsQuadratic;
		} else if (sQuestionType.compare("eQuesEquationsQuadratic") == 0) {
			return eQuesEquationsQuadratic;
		} else if (sQuestionType.compare("eQuesEquationsQuadraticVeryEasy") == 0) {
			return eQuesEquationsQuadraticVeryEasy;
		} else if (sQuestionType.compare("eQuesEquationsQuadraticEasy") == 0) {
			return eQuesEquationsQuadraticEasy;
		} else if (sQuestionType.compare("eQuesEquationsQuadraticMedium") == 0) {
			return eQuesEquationsQuadraticMedium;
		} else if (sQuestionType.compare("eQuesEquationsQuadraticHard") == 0) {
			return eQuesEquationsQuadraticHard;
		} else if (sQuestionType.compare("eQuesEquationsQuadraticVeryHard") == 0) {
			return eQuesEquationsQuadraticVeryHard;
			
		} else if (sQuestionType.compare("Simultaneous Linear Equations") == 0) {
			return eQuesEquationsSimultaneousLinear;
		} else if (sQuestionType.compare("eQuesEquationsSimultaneousLinear") == 0) {
			return eQuesEquationsSimultaneousLinear;
		} else if (sQuestionType.compare("eQuesEquationsSimultaneousLinearVeryEasy") == 0) {
			return eQuesEquationsSimultaneousLinearVeryEasy;
		} else if (sQuestionType.compare("eQuesEquationsSimultaneousLinearEasy") == 0) {
			return eQuesEquationsSimultaneousLinearEasy;
		} else if (sQuestionType.compare("eQuesEquationsSimultaneousLinearMedium") == 0) {
			return eQuesEquationsSimultaneousLinearMedium;
		} else if (sQuestionType.compare("eQuesEquationsSimultaneousLinearHard") == 0) {
			return eQuesEquationsSimultaneousLinearHard;
		} else if (sQuestionType.compare("eQuesEquationsSimultaneousLinearVeryHard") == 0) {
			return eQuesEquationsSimultaneousLinearVeryHard;
		
		} else {
			return eQuesError;
		};
	};
	
	
	std::string convertEnumText_AnswerType(enumAnswerType eAnswerType){
		std::string sResult;
		
		switch  (eAnswerType){
			case eAnsTypNumeric:
				sResult = "eAnsTypNumeric";
				break;
			case eAnsTypEquation:
				sResult = "eAnsTypEquation";
				break;
			case eAnsTypEquQuadratic:
				sResult = "eAnsTypEquQuadratic";
				break;
			case eAnsTypEquTrig:
				sResult = "eAnsTypEquTrig";
				break;
			case eAnsTypEquLog:
				sResult = "eAnsTypEquLog";
				break;
			case eAnsTypEquExpo:
				sResult = "eAnsTypEquExpo";
				break;
			case eAnsTypMultipleNumeric:
				sResult = "eAnsTypMultipleNumeric";
				break;
			case eAnsTypMutipleChoice:
				sResult = "eAnsTypMutipleChoice";
				break;
			default:
				sResult = "Error";
		};
		
		return sResult;
	};
	
	enumAnswerType convertEnumText_AnswerType(std::string sAnswerType) {

		if (sAnswerType.compare("eAnsTypNumeric") == 0) {
			return eAnsTypNumeric;
		} else if (sAnswerType.compare("eAnsTypEquation") == 0) {
			return eAnsTypEquation;
		} else if (sAnswerType.compare("eAnsTypEquQuadratic") == 0) {
			return eAnsTypEquQuadratic;
		} else if (sAnswerType.compare("eAnsTypEquTrig") == 0) {
			return eAnsTypEquTrig;
		} else if (sAnswerType.compare("eAnsTypEquLog") == 0) {
			return eAnsTypEquLog;
		} else if (sAnswerType.compare("eAnsTypEquExpo") == 0) {
			return eAnsTypEquExpo;
		} else if (sAnswerType.compare("eAnsTypMultipleNumeric") == 0) {
			return eAnsTypMultipleNumeric;
		} else if (sAnswerType.compare("eAnsTypMutipleChoice") == 0) {
			return eAnsTypMutipleChoice;
		} else {
			return eAnsTypError;
		};
	};
	
	std::string convertEnum_QuesTypeGroupText(enumQuestionGroup eQuestionGroup) {
		std::string sResult;
		
		if (eQuestionGroup == eQuesGrpArithmeticBasics){
			sResult = "Arithmetic Basics";
		} else if (eQuestionGroup == eQuesGrpArithmeticAdvanced){
			sResult = "Arithmetic Advanced";
		} else if (eQuestionGroup == eQuesGrpShapes){
			sResult = "Shapes";
		} else if (eQuestionGroup == eQuesGrpEquations){
			sResult = "Equations";
		} else if (eQuestionGroup == eQuesGrpDifferentiation){
			sResult = "Differentiation";
		} else if (eQuestionGroup == eQuesGrpTrig){
			sResult = "????";
		} else if (eQuestionGroup == eQuesGrpFractions){
			sResult = "Fractions";
		} else if (eQuestionGroup == eQuesGrpPolynomial){
			sResult = "????";
		} else if (eQuestionGroup == eQuesGrpAverages){
			sResult = "Averages";
		} else if (eQuestionGroup == eQuesGrpError) {
			sResult = "????";
		} else if (eQuestionGroup == eQuesGrpStatistics) {
			sResult = "Statistics";

		} else {
			sResult = "ERROR";
		};
		
		return sResult;
	};
	
	enumQuestionGroup convertEnum_QuesTypeGroupText(std::string sQuestionGroup) {
		enumQuestionGroup eResult;
		
		if (sQuestionGroup.compare("Arithmetic Basics") == 0){
			eResult = eQuesGrpArithmeticBasics;
		} else if (sQuestionGroup.compare("Arithmetic Advanced") == 0){
			eResult = eQuesGrpArithmeticAdvanced;
		} else if (sQuestionGroup.compare("Shapes") == 0){
			eResult = eQuesGrpShapes;
		} else if (sQuestionGroup.compare("Statistics") == 0){
			eResult = eQuesGrpStatistics;
		} else if (sQuestionGroup.compare("Differentiation") == 0){
			eResult = eQuesGrpDifferentiation;
		} else if (sQuestionGroup.compare("Fractions") == 0){
			eResult = eQuesGrpFractions;
		} else if (sQuestionGroup.compare("Equations") == 0){
			eResult = eQuesGrpEquations;
		} else if (sQuestionGroup.compare("Averages") == 0){
			eResult = eQuesGrpAverages;
		} else if (sQuestionGroup.compare("Statistics") == 0){
			eResult = eQuesGrpStatistics;
		} else {
			eResult = eQuesGrpError;
		};

		return eResult;
	};
	
	std::string convertEnum_QuesTypeSubjectText(enumQuestionType eQuestionType){
		std::string sResult;

		switch  (eQuestionType){
			//Arithmetic Basics
			case eQuesAddingNumbers:
			case eQuesAddingNumbersEasy:
			case eQuesAddingNumbersMedium:
			case eQuesAddingNumbersHard:
				sResult = "Adding";
				break;
			case eQuesSubtractingNumbers:
			case eQuesSubtractingNumbersEasy:
			case eQuesSubtractingNumbersMedium:
			case eQuesSubtractingNumbersHard:
				sResult = "Subtracting";
				break;
			case eQuesMultiplingNumbers:
			case eQuesMultiplingNumbersEasy:
			case eQuesMultiplingNumbersMedium:
			case eQuesMultiplingNumbersHard:
				sResult = "Multiplication";
				break;
			case eQuesDividingNumbers:
			case eQuesDividingNumbersEasy:
			case eQuesDividingNumbersMedium:
			case eQuesDividingNumbersHard:
				sResult = "Division";
				break;
				
				//Arithmetic Advanced
			case eQuesSquare:
			case eQuesSquare1Digit:
			case eQuesSquare2Digit:
			case eQuesSquare3Digit:
				sResult = "Squared";
				break;
				
			case eQuesCubed:
			case eQuesCubed1Digit:
			case eQuesCubed2Digit:
			case eQuesCubed3Digit:
				sResult = "Cubed";
				break;
				
			case eQuesSquareRoot:
			case eQuesSquareRoot1Digit:
			case eQuesSquareRoot2Digit:
			case eQuesSquareRoot3Digit:
				sResult = "Squared Root";
				break;
				
			case eQuesCubedRoot:
			case eQuesCubedRoot1Digit:
			case eQuesCubedRoot2Digit:
			case eQuesCubedRoot3Digit:
				sResult = "Cubed Root";
				break;
				
			case eQuesArithmetic:
			case eQuesArithmeticEasy:
			case eQuesArithmeticMedium:
			case eQuesArithmeticHard:
				sResult = "Misc";
				break;

				//Shapes
			case eQuesParallogram:
			case eQuesParallogramVeryEasy:
			case eQuesParallogramEasy:
			case eQuesParallogramMedium:
			case eQuesParallogramHard:
			case eQuesParallogramVeryHard:
				sResult = "Parallogram";
				break;
				
			case eQuesCylinder:
			case eQuesCylinderVeryEasy:
			case eQuesCylinderEasy:
			case eQuesCylinderMedium:
			case eQuesCylinderHard:
			case eQuesCylinderVeryHard:
				sResult = "Cylinder";
				break;
				
			case eQuesPythagorean:
			case eQuesPythagoreanVeryEasy:
			case eQuesPythagoreanEasy:
			case eQuesPythagoreanMedium:
			case eQuesPythagoreanHard:
			case eQuesPythagoreanVeryHard:
				sResult = "Triangle";
				break;
				
			case eQuesTrigonometry:
			case eQuesTrigonometryVeryEasy:
			case eQuesTrigonometryEasy:
			case eQuesTrigonometryMedium:
			case eQuesTrigonometryHard:
			case eQuesTrigonometryVeryHard:
				sResult = "Trigonometry";
				break;
				
				//Averages
			case eQuesAverageMean:
			case eQuesAverageMeanVeryEasy:
			case eQuesAverageMeanEasy:
			case eQuesAverageMeanMedium:
			case eQuesAverageMeanHard:
			case eQuesAverageMeanVeryHard:
				sResult = "Average Mean";
				break;
				
			case eQuesAverageMedian:
			case eQuesAverageMedianVeryEasy:
			case eQuesAverageMedianEasy:
			case eQuesAverageMedianMedium:
			case eQuesAverageMedianHard:
			case eQuesAverageMedianVeryHard:
				sResult = "Average Median";
				break;
				
			case eQuesAverageMode:
			case eQuesAverageModeVeryEasy:
			case eQuesAverageModeEasy:
			case eQuesAverageModeMedium:
			case eQuesAverageModeHard:
			case eQuesAverageModeVeryHard:
				sResult = "Average Mode";
				break;
				
				//Fractions
			case eQuesFractionsAddSub:
			case eQuesFractionsAddSubVeryEasy:
			case eQuesFractionsAddSubEasy:
			case eQuesFractionsAddSubMedium:
			case eQuesFractionsAddSubHard:
			case eQuesFractionsAddSubVeryHard:
				sResult = "Fractions Add Subtract";
				break;
				
			case eQuesFractionsMultiDiv:
			case eQuesFractionsMultiDivVeryEasy:
			case eQuesFractionsMultiDivEasy:
			case eQuesFractionsMultiDivMedium:
			case eQuesFractionsMultiDivHard:
			case eQuesFractionsMultiDivVeryHard:
				return "Fractions Multiply Divide";
				break;
				
				//Equations
			case eQuesEquationsQuadratic:
			case eQuesEquationsQuadraticVeryEasy:
			case eQuesEquationsQuadraticEasy:
			case eQuesEquationsQuadraticMedium:
			case eQuesEquationsQuadraticHard:
			case eQuesEquationsQuadraticVeryHard:
				return "Quadratic Equations";
				break;
				
			case eQuesEquationsSimultaneousLinear:
			case eQuesEquationsSimultaneousLinearVeryEasy:
			case eQuesEquationsSimultaneousLinearEasy:
			case eQuesEquationsSimultaneousLinearMedium:
			case eQuesEquationsSimultaneousLinearHard:
			case eQuesEquationsSimultaneousLinearVeryHard:
				return "Simultaneous Linear Equations";
				break;
				
				
/*				
			case eQuesPolynomial:
			case eQuesPolynomialEasy:
			case eQuesPolynomialMedium:
			case eQuesPolynomialHard:
				return "eQuesPolynomialHard";
				break;
				
				
				
				
			case eQuesDifferentiation2BasePoly:
				return "eQuesDifferentiation2BasePoly";
				break;
			case eQuesDifferentiation2BasePolyVeryEasy:
				return "eQuesDifferentiation2BasePolyVeryEasy";
				break;
			case eQuesDifferentiation2BasePolyEasy:
				return "eQuesDifferentiation2BasePolyEasy";
				break;
			case eQuesDifferentiation2BasePolyMedium:
				return "eQuesDifferentiation2BasePolyMedium";
				break;
			case eQuesDifferentiation2BasePolyHard:
				return "eQuesDifferentiation2BasePolyHard";
				break;
			case eQuesDifferentiation2BasePolyVeryHard:
				return "eQuesDifferentiation2BasePolyVeryHard";
				break;
				
				
				
				
				
			case eQuesDifferentiation3BasePoly:
				return "eQuesDifferentiation3BasePoly";
				break;
			case eQuesDifferentiationExpFn:
				return "eQuesDifferentiationExpFn";
				break;
			case eQuesDifferentiationTrigFn:
				return "eQuesDifferentiationTrigFn";
				break;
			case eQuesDifferentiationChainRule:
				return "eQuesDifferentiationChainRule";
				break;
			case eQuesDifferentiationEverything:
				return "eQuesDifferentiationEverything";
				break;
			case eQuesTrigFromHypAdjCalcOpp:
				return "eQuesTrigFromHypAdjCalcOpp";
				break;
			case eQuesTrigFromHypOppCalcAdj:
				return "eQuesTrigFromHypOppCalcAdj";
				break;
			case eQuesTrigFromOppAdjCalcHyp:
				return "eQuesTrigFromOppAdjCalcHyp";
				break;
*/				
				
				
				
			case eQuesError:
				return "Error";
				break;
			default:
				return "Error";
				break;
		};
		
		return sResult;
	};
	
	enumQuestionType convertEnum_QuesTypeSubjectText(std::string){
		enumQuestionType eResult;
		
		return eResult;
	};
	
	
	
	bool questionTypeIsEqual(enumQuestionType eTypeA, enumQuestionType eTypeB) {
		bool bIsMatch;

		if (eTypeA == eTypeB) {
			bIsMatch = true;
		} else {
			bIsMatch = false;
			
			//Adding
			if (eTypeA == eQuesAddingNumbers and (
												  eTypeB == eQuesAddingNumbersEasy or 
												  eTypeB == eQuesAddingNumbersMedium or 
												  eTypeB == eQuesAddingNumbersHard)) {
				bIsMatch = true;
			};
			
			if (eTypeB == eQuesAddingNumbers and (
												  eTypeA == eQuesAddingNumbersEasy or 
												  eTypeA == eQuesAddingNumbersMedium or 
												  eTypeA == eQuesAddingNumbersHard)) {
				bIsMatch = true;
			};
			
			//Subtracting
			if (eTypeA == eQuesSubtractingNumbers and (
													   eTypeB == eQuesSubtractingNumbersEasy or 
													   eTypeB == eQuesSubtractingNumbersMedium or 
													   eTypeB == eQuesSubtractingNumbersHard)) {
				bIsMatch = true;
			};
			
			if (eTypeB == eQuesSubtractingNumbers and (
													   eTypeA == eQuesSubtractingNumbersEasy or 
													   eTypeA == eQuesSubtractingNumbersMedium or 
													   eTypeA == eQuesSubtractingNumbersHard)) {
				bIsMatch = true;
			};
			
			//Multipling
			if (eTypeA == eQuesMultiplingNumbers and (
													  eTypeB == eQuesMultiplingNumbersEasy or 
													  eTypeB == eQuesMultiplingNumbersMedium or 
													  eTypeB == eQuesMultiplingNumbersHard)) {
				bIsMatch = true;
			};
			
			if (eTypeB == eQuesMultiplingNumbers and (
													  eTypeA == eQuesMultiplingNumbersEasy or 
													  eTypeA == eQuesMultiplingNumbersMedium or 
													  eTypeA == eQuesMultiplingNumbersHard)) {
				bIsMatch = true;
			};
			
			//Dividing
			if (eTypeA == eQuesDividingNumbers and (
													eTypeB == eQuesDividingNumbersEasy or 
													eTypeB == eQuesDividingNumbersMedium or 
													eTypeB == eQuesDividingNumbersHard)) {
				bIsMatch = true;
			};
			
			if (eTypeB == eQuesDividingNumbers and (
													eTypeA == eQuesDividingNumbersEasy or 
													eTypeA == eQuesDividingNumbersMedium or 
													eTypeA == eQuesDividingNumbersHard)) {
				bIsMatch = true;
			};

			//Square
			if (eTypeA == eQuesSquare and (
										   eTypeB == eQuesSquareRoot1Digit or 
										   eTypeB == eQuesSquareRoot2Digit or 
										   eTypeB == eQuesSquareRoot3Digit)) {
				bIsMatch = true;
			};
			
			if (eTypeB == eQuesSquare and (
										   eTypeA == eQuesSquareRoot1Digit or 
										   eTypeA == eQuesSquareRoot2Digit or 
										   eTypeA == eQuesSquareRoot3Digit)) {
				bIsMatch = true;
			};
			
			//Cubed
			if (eTypeA == eQuesCubed and (
										  eTypeB == eQuesCubed1Digit or 
										  eTypeB == eQuesCubed2Digit or 
										  eTypeB == eQuesCubed3Digit)) {
				bIsMatch = true;
			};
			
			if (eTypeB == eQuesCubed and (
										  eTypeA == eQuesCubed1Digit or 
										  eTypeA == eQuesCubed2Digit or 
										  eTypeA == eQuesCubed3Digit)) {
				bIsMatch = true;
			};
			
			//Square Root
			if (eTypeA == eQuesSquareRoot and (
											   eTypeB == eQuesSquareRoot1Digit or 
											   eTypeB == eQuesSquareRoot2Digit or 
											   eTypeB == eQuesSquareRoot3Digit)) {
				bIsMatch = true;
			};
			
			if (eTypeB == eQuesSquareRoot and (
											   eTypeA == eQuesSquareRoot1Digit or 
											   eTypeA == eQuesSquareRoot2Digit or 
											   eTypeA == eQuesSquareRoot3Digit)) {
				bIsMatch = true;
			};
			
			//Cubed Root
			if (eTypeA == eQuesCubedRoot and (
											  eTypeB == eQuesCubedRoot1Digit or 
											  eTypeB == eQuesCubedRoot2Digit or 
											  eTypeB == eQuesCubedRoot3Digit)) {
				bIsMatch = true;
			};
			
			if (eTypeB == eQuesCubedRoot and (
											  eTypeA == eQuesCubedRoot1Digit or 
											  eTypeA == eQuesCubedRoot2Digit or 
											  eTypeA == eQuesCubedRoot3Digit)) {
				bIsMatch = true;
			};
			
			//Misc
			if (eTypeA == eQuesArithmetic and (
											  eTypeB == eQuesArithmeticEasy or 
											  eTypeB == eQuesArithmeticMedium or 
											  eTypeB == eQuesArithmeticHard)) {
				bIsMatch = true;
			};
			
			if (eTypeB == eQuesArithmetic and (
											  eTypeA == eQuesArithmeticEasy or 
											  eTypeA == eQuesArithmeticMedium or 
											  eTypeA == eQuesArithmeticHard)) {
				bIsMatch = true;
			};
			
			//Pythagorean
			if (eTypeA == eQuesPythagorean and (
												eTypeB == eQuesPythagoreanVeryEasy or 
												eTypeB == eQuesPythagoreanEasy or 
												eTypeB == eQuesPythagoreanMedium or 
												eTypeB == eQuesPythagoreanHard or 
												eTypeB == eQuesPythagoreanVeryHard)) {
				bIsMatch = true;
			};
			
			if (eTypeB == eQuesPythagorean and (
												eTypeA == eQuesPythagoreanVeryEasy or 
												eTypeA == eQuesPythagoreanEasy or 
												eTypeA == eQuesPythagoreanMedium or 
												eTypeA == eQuesPythagoreanHard or 
												eTypeA == eQuesPythagoreanVeryHard)) {
				bIsMatch = true;
			};
			
			//Trigonometry
			if (eTypeA == eQuesTrigonometry and (
												eTypeB == eQuesTrigonometryVeryEasy or 
												eTypeB == eQuesTrigonometryEasy or 
												eTypeB == eQuesTrigonometryMedium or 
												eTypeB == eQuesTrigonometryHard or 
												eTypeB == eQuesTrigonometryVeryHard)) {
				bIsMatch = true;
			};
			
			if (eTypeB == eQuesTrigonometry and (
												eTypeA == eQuesTrigonometryVeryEasy or 
												eTypeA == eQuesTrigonometryEasy or 
												eTypeA == eQuesTrigonometryMedium or 
												eTypeA == eQuesTrigonometryHard or 
												eTypeA == eQuesTrigonometryVeryHard)) {
				bIsMatch = true;
			};
			
			//Parallogram
			if (eTypeA == eQuesParallogram and (
												eTypeB == eQuesParallogramVeryEasy or 
												eTypeB == eQuesParallogramEasy or 
												eTypeB == eQuesParallogramMedium or 
												eTypeB == eQuesParallogramHard or 
												eTypeB == eQuesParallogramVeryHard)) {
				bIsMatch = true;
			};
			
			if (eTypeB == eQuesParallogram and (
												eTypeA == eQuesParallogramVeryEasy or 
												eTypeA == eQuesParallogramEasy or 
												eTypeA == eQuesParallogramMedium or 
												eTypeA == eQuesParallogramHard or 
												eTypeA == eQuesParallogramVeryHard)) {
				bIsMatch = true;
			};
			
			//eQuesCylinder
			if (eTypeA == eQuesCylinder and (
											 eTypeB == eQuesCylinderVeryEasy or 
											 eTypeB == eQuesCylinderEasy or 
											 eTypeB == eQuesCylinderMedium or 
											 eTypeB == eQuesCylinderHard or 
											 eTypeB == eQuesCylinderVeryHard)) {
				bIsMatch = true;
			};
			
			if (eTypeB == eQuesCylinder and (
											 eTypeA == eQuesCylinderVeryEasy or 
											 eTypeA == eQuesCylinderEasy or 
											 eTypeA == eQuesCylinderMedium or 
											 eTypeA == eQuesCylinderHard or 
											 eTypeA == eQuesCylinderVeryHard)) {
				bIsMatch = true;
			};
			
			//eQuesAverageMean
			if (eTypeA == eQuesAverageMean and (
												eTypeB == eQuesAverageMeanVeryEasy or 
												eTypeB == eQuesAverageMeanEasy or 
												eTypeB == eQuesAverageMeanMedium or 
												eTypeB == eQuesAverageMeanHard or 
												eTypeB == eQuesAverageMeanVeryHard)) {
				bIsMatch = true;
			};
			
			if (eTypeB == eQuesAverageMean and (
												eTypeA == eQuesAverageMeanVeryEasy or 
												eTypeA == eQuesAverageMeanEasy or 
												eTypeA == eQuesAverageMeanMedium or 
												eTypeA == eQuesAverageMeanHard or 
												eTypeA == eQuesAverageMeanVeryHard)) {
				bIsMatch = true;
			};
			
			//eQuesAverageMedian
			if (eTypeA == eQuesAverageMedian and (
												  eTypeB == eQuesAverageMedianVeryEasy or 
												  eTypeB == eQuesAverageMedianEasy or 
												  eTypeB == eQuesAverageMedianMedium or 
												  eTypeB == eQuesAverageMedianHard or 
												  eTypeB == eQuesAverageMedianVeryHard)) {
				bIsMatch = true;
			};
			
			if (eTypeB == eQuesAverageMedian and (
												  eTypeA == eQuesAverageMedianVeryEasy or 
												  eTypeA == eQuesAverageMedianEasy or 
												  eTypeA == eQuesAverageMedianMedium or 
												  eTypeA == eQuesAverageMedianHard or 
												  eTypeA == eQuesAverageMedianVeryHard)) {
				bIsMatch = true;
			};
			
			//eQuesAverageMode
			if (eTypeA == eQuesAverageMode and (
												eTypeB == eQuesAverageModeVeryEasy or 
												eTypeB == eQuesAverageModeEasy or 
												eTypeB == eQuesAverageModeMedium or 
												eTypeB == eQuesAverageModeHard or 
												eTypeB == eQuesAverageModeVeryHard)) {
				bIsMatch = true;
			};
			
			if (eTypeB == eQuesAverageMode and (
												eTypeA == eQuesAverageModeVeryEasy or 
												eTypeA == eQuesAverageModeEasy or 
												eTypeA == eQuesAverageModeMedium or 
												eTypeA == eQuesAverageModeHard or 
												eTypeA == eQuesAverageModeVeryHard)) {
				bIsMatch = true;
			};
			
			//eQuesDifferentiation2BasePoly
			if (eTypeA == eQuesDifferentiation2BasePoly and (
															 eTypeB == eQuesDifferentiation2BasePolyVeryEasy or 
															 eTypeB == eQuesDifferentiation2BasePolyEasy or 
															 eTypeB == eQuesDifferentiation2BasePolyMedium or 
															 eTypeB == eQuesDifferentiation2BasePolyHard or 
															 eTypeB == eQuesDifferentiation2BasePolyVeryHard)) {
				bIsMatch = true;
			};
			
			if (eTypeB == eQuesDifferentiation2BasePoly and (
															 eTypeA == eQuesDifferentiation2BasePolyVeryEasy or 
															 eTypeA == eQuesDifferentiation2BasePolyEasy or 
															 eTypeA == eQuesDifferentiation2BasePolyMedium or 
															 eTypeA == eQuesDifferentiation2BasePolyHard or 
															 eTypeA == eQuesDifferentiation2BasePolyVeryHard)) {
				bIsMatch = true;
			};
			
			//eQuesFractionsAddSub
			if (eTypeA == eQuesFractionsAddSub and (
													eTypeB == eQuesFractionsAddSubVeryEasy or 
													eTypeB == eQuesFractionsAddSubEasy or 
													eTypeB == eQuesFractionsAddSubMedium or 
													eTypeB == eQuesFractionsAddSubHard or 
													eTypeB == eQuesFractionsAddSubVeryHard)) {
				bIsMatch = true;
			};
			
			if (eTypeB == eQuesFractionsAddSub and (
													eTypeA == eQuesFractionsAddSubVeryEasy or 
													eTypeA == eQuesFractionsAddSubEasy or 
													eTypeA == eQuesFractionsAddSubMedium or 
													eTypeA == eQuesFractionsAddSubHard or 
													eTypeA == eQuesFractionsAddSubVeryHard)) {
				bIsMatch = true;
			};
			
			//eQuesFractionsMultiDiv
			if (eTypeA == eQuesFractionsMultiDiv and (
													eTypeB == eQuesFractionsMultiDivVeryEasy or 
													eTypeB == eQuesFractionsMultiDivEasy or 
													eTypeB == eQuesFractionsMultiDivMedium or 
													eTypeB == eQuesFractionsMultiDivHard or 
													eTypeB == eQuesFractionsMultiDivVeryHard)) {
				bIsMatch = true;
			};
			
			if (eTypeB == eQuesFractionsMultiDiv and (
													eTypeA == eQuesFractionsMultiDivVeryEasy or 
													eTypeA == eQuesFractionsMultiDivEasy or 
													eTypeA == eQuesFractionsMultiDivMedium or 
													eTypeA == eQuesFractionsMultiDivHard or 
													eTypeA == eQuesFractionsMultiDivVeryHard)) {
				bIsMatch = true;
			};
			
			//eQuesEquationsQuadratic
			if (eTypeA == eQuesEquationsQuadratic and (
													   eTypeB == eQuesEquationsQuadraticVeryEasy or 
													   eTypeB == eQuesEquationsQuadraticEasy or 
													   eTypeB == eQuesEquationsQuadraticMedium or 
													   eTypeB == eQuesEquationsQuadraticHard or 
													   eTypeB == eQuesEquationsQuadraticVeryHard)) {
				bIsMatch = true;
			};
			
			if (eTypeB == eQuesEquationsQuadratic and (
													   eTypeA == eQuesEquationsQuadraticVeryEasy or 
													   eTypeA == eQuesEquationsQuadraticEasy or 
													   eTypeA == eQuesEquationsQuadraticMedium or 
													   eTypeA == eQuesEquationsQuadraticHard or 
													   eTypeA == eQuesEquationsQuadraticVeryHard)) {
				bIsMatch = true;
			};
			
			//eQuesEquationsSimultaneousLinear
			if (eTypeA == eQuesEquationsSimultaneousLinear and (
													   eTypeB == eQuesEquationsSimultaneousLinearVeryEasy or 
													   eTypeB == eQuesEquationsSimultaneousLinearEasy or 
													   eTypeB == eQuesEquationsSimultaneousLinearMedium or 
													   eTypeB == eQuesEquationsSimultaneousLinearHard or 
													   eTypeB == eQuesEquationsSimultaneousLinearVeryHard)) {
				bIsMatch = true;
			};
			
			if (eTypeB == eQuesEquationsSimultaneousLinear and (
													   eTypeA == eQuesEquationsSimultaneousLinearVeryEasy or 
													   eTypeA == eQuesEquationsSimultaneousLinearEasy or 
													   eTypeA == eQuesEquationsSimultaneousLinearMedium or 
													   eTypeA == eQuesEquationsSimultaneousLinearHard or 
													   eTypeA == eQuesEquationsSimultaneousLinearVeryHard)) {
				bIsMatch = true;
			};
			
			
			
		};
		

		return bIsMatch;
		
	
	
	};
/*
 enum enumPicQues {
 ePicQues_None,
 ePicQues_TriangleRightAngle,
 ePicQues_TriangleNotRight,
 ePicQues_Parallelogram,
 ePicQues_Circle,
 ePicQues_Cylinder,
 ePicQues_Square,
 ePicQues_Cube	
 };
 */	
	
	std::string convertEnumText_Pic(enumPicQues ePic) {
		std::string sResult;
		
		if (ePic == ePic_None) {
			sResult = "None";
		} else if (ePic == ePicQues_TriangleRightAngle){
			sResult = "Triangle Right Angle";
		} else if (ePic == ePicQues_TriangleNotRight){
			sResult = "Triangle Not Right";
		} else if (ePic == ePicQues_Parallelogram){
			sResult = "Parallelogram";
		} else if (ePic == ePicQues_Circle){
			sResult = "Circle";
		} else if (ePic == ePicQues_Cylinder){
			sResult = "Cylinder";
		} else if (ePic == ePicQues_Square){
			sResult = "Square";
		} else if (ePic == ePicQues_Cube){
			sResult = "Cube";
		} else {
			sResult = "ERROR";
		};
		
		return sResult;
	};
	
	enumPicQues convertEnumText_Pic(std::string sPic) {
		enumPicQues eResult;
		
		if (sPic.compare("None") == 0){
			eResult = ePic_None;
		} else if (sPic.compare("Triangle Right Angle") == 0){
			eResult = ePicQues_TriangleRightAngle;
		} else if (sPic.compare("Triangle Not Right") == 0){
			eResult = ePicQues_TriangleNotRight;
		} else if (sPic.compare("Parallelogram") == 0){
			eResult = ePicQues_Parallelogram;
		} else if (sPic.compare("Circle") == 0){
			eResult = ePicQues_Circle;
		} else if (sPic.compare("Cylinder") == 0){
			eResult = ePicQues_Cylinder;
		} else if (sPic.compare("Square") == 0){
			eResult = ePicQues_Square;
		} else if (sPic.compare("Cube") == 0){
			eResult = ePicQues_Cube;
		} else {
			eResult = ePic_None;
		};
		
		return eResult;
	};

	
	void sqliteFinalizeRapper(sqlite3_stmt* stmt) {
		int c1;
		int c2;
		int c3;
		
//		c1 = sqlite3_stmt_status(stmt, SQLITE_STMTSTATUS_FULLSCAN_STEP, 1);
//		c2 = sqlite3_stmt_status(stmt, SQLITE_STMTSTATUS_SORT, 1);
//		c3 = sqlite3_stmt_status(stmt, SQLITE_STMTSTATUS_AUTOINDEX, 1);
		c1 = sqlite3_stmt_status(stmt, 1, 1);
		c2 = sqlite3_stmt_status(stmt, 2, 1);
		c3 = sqlite3_stmt_status(stmt, 3, 1);
		
		if (not (c1 == 0 and c2 == 0 and c3 == 0)) {
			sqlite3_finalize(stmt);
		};
		
		stmt = NULL;
	};
	
	void sqliteStmtStatusRest(sqlite3_stmt* stmt) {
		int c1;
		int c2;
		int c3;
		
//		c1 = sqlite3_stmt_status(stmt, SQLITE_STMTSTATUS_FULLSCAN_STEP, 0);
//		c2 = sqlite3_stmt_status(stmt, SQLITE_STMTSTATUS_AUTOINDEX, 0);
//		c3 = sqlite3_stmt_status(stmt, SQLITE_STMTSTATUS_SORT, 0);
		c1 = sqlite3_stmt_status(stmt, 1, 0);
		c2 = sqlite3_stmt_status(stmt, 2, 0);
		c3 = sqlite3_stmt_status(stmt, 3, 0);
	};
    
    bool almostEquals(float fA, float fB, float fRange) {
        bool bEquals;
        
        if (fA + fRange >= fB or fA - fRange <= fB) {
            bEquals = false;
        } else {
            bEquals = true;
        };
        
        return bEquals;
    };
}

#endif
			
