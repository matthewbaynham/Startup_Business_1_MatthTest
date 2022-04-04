/*
 *  ClsQuizGuiLayout.cpp
 *  MatthTest
 *
 *  Created by Matthew Baynham on 2/5/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */




#ifndef CLS_QUIZ_GUI_LAYOUT_CPP
#define CLS_QUIZ_GUI_LAYOUT_CPP

#include <string>
#include <cstddef>//std array rapper
#include <vector>
//#include <cstdlib>;
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "modMisc.h"
#include "ClsQuizGuiLayout.h"

using namespace modMisc;
using namespace std;

ClsQuizGuiLayout::ClsQuizGuiLayout() {
	this->vecLayout.clear();
};

ClsQuizGuiLayout::ClsQuizGuiLayout(int iQuestionMax) {
	reInitialize(iQuestionMax);
};

ClsQuizGuiLayout::~ClsQuizGuiLayout() {
	this->vecLayout.clear();
};

void ClsQuizGuiLayout::reInitialize(int iQuestionMax) {
	int iCellNo;
	int iQuestionCount;	
	int iGapBetweenAd;
	strQuizGuiLayout sTempCell;
	int iGapBetweenAdPrevious;
	
	const int iGapMin = 2;
	const int iGapMax = 4;
	
	this->vecLayout.clear();
	iCellNo = -1;//base zero 
	
//	iCellNo++;
//	sTempCell.eCellType = eQuGui_Menu;
//	sTempCell.iQuestionNo = -1;
//	sTempCell.iCellPos = iCellNo;
	
//	this->vecLayout.push_back(sTempCell);
	
	iCellNo++;
	sTempCell.eCellType = eQuGui_Advert;
	sTempCell.iQuestionNo = -1;
	sTempCell.iCellPos = iCellNo;
	
	this->vecLayout.push_back(sTempCell);
	
	iGapBetweenAd = modMisc::iRandomNumber(iGapMin, iGapMax);
	iGapBetweenAdPrevious = iGapBetweenAd;
	
	for (iQuestionCount = 0; iQuestionCount < iQuestionMax; iQuestionCount++) {
		
		if (iGapBetweenAd == 0) {
			iCellNo++;
			sTempCell.eCellType = eQuGui_Advert;
			sTempCell.iQuestionNo = -1;
			sTempCell.iCellPos = iCellNo;
			
			this->vecLayout.push_back(sTempCell);
			
			do {
				iGapBetweenAd = modMisc::iRandomNumber(iGapMin, iGapMax);
			} while (iGapBetweenAd == iGapBetweenAdPrevious);
			iGapBetweenAdPrevious = iGapBetweenAd;
		} else {
			iGapBetweenAd--;
		};
		
		
		iCellNo++;
		sTempCell.eCellType = eQuGui_Question;
		sTempCell.iQuestionNo = iQuestionCount;
		sTempCell.iCellPos = iCellNo;
		
		this->vecLayout.push_back(sTempCell);
		
	};
	
	iCellNo++;
	sTempCell.eCellType = eQuGui_Advert;
	sTempCell.iQuestionNo = -1;
	sTempCell.iCellPos = iCellNo;
	
	this->vecLayout.push_back(sTempCell);
	
//	iCellNo++;
//	sTempCell.eCellType = eQuGui_Menu;
//	sTempCell.iQuestionNo = -1;
//	sTempCell.iCellPos = iCellNo;
	
//	this->vecLayout.push_back(sTempCell);
};


enumQuizGuiLayout ClsQuizGuiLayout::cellLayout(int iCellNo) {
	vector<strQuizGuiLayout>::iterator itCell; 
	enumQuizGuiLayout eResult;
	
	eResult = eQuGui_Advert;
	
	for (itCell=this->vecLayout.begin(); itCell != this->vecLayout.end(); itCell++) {
		if (itCell->iCellPos == iCellNo) {
			eResult = itCell->eCellType; 
		};
	};
	
	return eResult;
};

int ClsQuizGuiLayout::questionNo(int iCellNo) {
	vector<strQuizGuiLayout>::iterator itCell; 
	int iResult;
	
	iResult = -1;
	
	for (itCell=this->vecLayout.begin(); itCell != this->vecLayout.end(); itCell++) {
		if (itCell->iCellPos == iCellNo) {
			iResult = itCell->iQuestionNo; 
		};
	};
	
	return iResult;
};

int ClsQuizGuiLayout::totalCells() {
	int iResult;
	
	iResult = this->vecLayout.size();

	return iResult;
};

#endif
