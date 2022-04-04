/*
 *  ClsQuiz.cpp
 *  
 *
 *  Created by Matthew Baynham on 18/06/2010.
 *  Copyright 2010 Baynham Coding. All rights reserved.
 *
 */

#ifndef CLS_QUIZ_CPP
#define CLS_QUIZ_CPP

#include <string>
#include <cstdlib>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "modMisc.h"
#include "ClsPreferences.h"
#include "ClsQuiz.h"
#include "ClsPrimeNumbers.h"

using namespace modMisc;
using namespace std;

/*
	How questions are encoded with tags
	"What is <0> + <0>?" and <0> = 5 and <0> = 8
	gets converted into  
	"What is 5 + 8?"
*/
/*
This class can be triggered off in different modes
1) A general level of difficulty and a number of questions
2) A list of interests with their level of diffulty
 */

#pragma mark -
#pragma mark Initialization

ClsQuiz::ClsQuiz(){
	try{
		int rc;
		
		ilQuestionCurrent = 0;
		ilQuestionMax = 0;
		rc = sqlite3_close(db);
		bDbIsOpen = false;
	}
	catch(...){
		
	};
};

ClsQuiz::~ClsQuiz(){
	int rc;
	this->sFilePath.clear();
	
	
	std::vector<strHeader>::iterator itHeader;
	
	for (itHeader = this->vecHeader.begin(); itHeader < this->vecHeader.end(); itHeader++) {
		itHeader->sName.clear();
		itHeader->sValue.clear();
		itHeader->sValue2.clear();
	};
	
	this->vecHeader.clear();

	emptyQuiz();

	cPreferences = NULL;
	cPrimeNumbers = NULL;

	rc = sqlite3_close(db);
	bDbIsOpen = false;
	db = NULL;

};

#pragma mark -
#pragma mark setup

void ClsQuiz::setPath(std::string sFullPath){
	char *cPath;
	bool bIsOk;
	int rc;
	
	this->sFilePath = sFullPath;
	
	cPath = (char*)this->sFilePath.c_str();
	
	bIsOk = true;
	
	//open database connection
	rc = sqlite3_open(cPath, &db);
	bDbIsOpen = true;
	
	if(rc != SQLITE_OK){
#if TEST_ENV == true
		sErrMsg = (std::string)"Can't open DB: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
		fprintf(stderr, sErrMsg.c_str(), 0);
#endif
		bIsOk = false;
		bDbIsOpen = false;
	};
	
//	cPath = NULL;
};

std::string ClsQuiz::getPath(){
	return this->sFilePath;
};

void ClsQuiz::setPreferences(ClsPreferences *cPref){
	cPreferences = cPref;
}; 

int ClsQuiz::iQuestionCurrent(){
	return ilQuestionCurrent;
};

void ClsQuiz::iQuestionCurrent(int iQuestionNo){
	ilQuestionCurrent = iQuestionNo;
};
			
int ClsQuiz::iQuestionMax(){
	int iNoQuestions;
	
	iNoQuestions = ilQuestionMax;
	
	return iNoQuestions;
};

std::string ClsQuiz::sQuestionText(){
	strQuestion oQuestion;
	std::string sQuestion;
	
	oQuestion = vecQuestions[ilQuestionCurrent];
	sQuestion = oQuestion.sQuestionText;
	
	return sQuestion;			
};

std::string ClsQuiz::sQuestionText(int iQuestionNo) {
	strQuestion oQuestion;
	std::string sQuestion;
	
	if (iQuestionNo >= 0 and iQuestionNo <= this->iQuestionMax()) {
		oQuestion = vecQuestions[iQuestionNo];
		sQuestion = oQuestion.sQuestionText;
	} else {
		sQuestion = "";
	};
		
	return sQuestion;			
};

std::string ClsQuiz::sQuestionTextEncoded(){
	strQuestion oQuestion;
	std::string sQuestion;
	
	oQuestion = this->vecQuestions[ilQuestionCurrent];
	sQuestion = oQuestion.sQuestionEncoded;
	
	return sQuestion;			
};

std::string ClsQuiz::sQuestionTextEncoded(int iQuestionNo) {
	strQuestion oQuestion;
	std::string sQuestion;
	
	if (iQuestionNo >= 0 and iQuestionNo <= this->iQuestionMax()) {
		oQuestion = this->vecQuestions[iQuestionNo];
		sQuestion = oQuestion.sQuestionEncoded;
	} else {
		sQuestion = "";
	};
	
	return sQuestion;			
};

int ClsQuiz::iQuestionValuesCount(int iQuestionNo) {
	int iResult;
	std::string sQuestion;
	
	if (iQuestionNo >= 0 and iQuestionNo <= this->iQuestionMax()) {
		iResult = (int)this->vecQuestions[iQuestionNo].vecValues.size();
	} else {
		iResult = 0;
	};
	
	return iResult;			
};

float ClsQuiz::fQuestionValue(int iQuestionNo, int iValueNo) {
	int iResult;
	std::string sQuestion;
	
	if (iQuestionNo >= 0 and iQuestionNo <= this->iQuestionMax()) {
		if (iValueNo >= 0 and iValueNo < (int)this->vecQuestions[iQuestionNo].vecValues.size()) {
			iResult = this->vecQuestions[iQuestionNo].vecValues[iValueNo].fValue;
		} else {
			iResult = 0;
		}

	
	} else {
		iResult = 0;
	};
	
	return iResult;			
};


bool ClsQuiz::bQuestionIsRight(){
	return false;
};

std::string ClsQuiz::decodeText(std::string sEncodedQuestion){
	
	return "";
};

std::string ClsQuiz::sQuestionGroup(int iQuestionNo) {
//	strQuestion oQuestion;
	std::string sResult;
	
	if (iQuestionNo >= 0 and iQuestionNo <= this->iQuestionMax()) {
//		oQuestion = this->vecQuestions[iQuestionNo];
//		sResult = oQuestion.sGroup;
		sResult = modMisc::convertEnum_QuesTypeGroupText(this->vecQuestions[iQuestionNo].eQuestionGroup);
	} else {
		sResult = "";
	};
	
	return sResult;			
};

std::string ClsQuiz::sQuestionSubject(int iQuestionNo) {
//	strQuestion oQuestion;
	std::string sResult;
	
	if (iQuestionNo >= 0 and iQuestionNo <= this->iQuestionMax()) {
//		oQuestion = this->vecQuestions[iQuestionNo];
//		sResult = oQuestion.sSubject;
//		oQuestion = this->vecQuestions[iQuestionNo];
		sResult = modMisc::convertEnum_QuesTypeSubjectText(this->vecQuestions[iQuestionNo].eQuestionType);
	} else {
		sResult = "";
	};
	
	return sResult;			
};


#pragma mark -
#pragma mark Load Existing Quiz From File

void ClsQuiz::loadQuiz(){
	emptyQuiz();
	loadHeader();
	loadQuestions();
};

void ClsQuiz::emptyQuiz() {
	vector<strQuestion>::iterator itQuestion;

	this->vecHeader.clear();
	
	for (itQuestion = this->vecQuestions.begin(); itQuestion != this->vecQuestions.end(); itQuestion++) {
		itQuestion->vecValues.clear();
		itQuestion->vecAnswers.clear();
		itQuestion->eFnTrig.clear();
		itQuestion->vecChoices.clear();
	};
	
	this->vecQuestions.clear();
};

void ClsQuiz::loadHeader(){

};

void ClsQuiz::loadQuestions(){
	int rc;
	
	try {
		const char *cSqlSelectQuestions("select fldId, fldQuestionNo, fldQuestionType, fldQuestionGroup, fldQuestionEncoded, fldDecodeText, fldQuestionText, fldGivenAnswerText, fldGivenAnswerMultipleChoice, fldDifficulty, fldCorrect, fldAnswerType, fldIsMultipleChoice, fldHasOnlyOneAnswer, fldIsDirty, fldPic, fldHasPic from tblQuestions order by fldQuestionNo ;");
		bool bIsOk;
		sqlite3_stmt *stmt;
		std::string sErrMsg;
		char *cPath;
		std::stringstream sResult;
		std::string strResult;
		strQuestion sQuestion;
		
		this->ilQuestionMax = 0;
		
		bIsOk = true;
		rc = SQLITE_OK;
		
		if (bDbIsOpen != true){
			cPath = (char*)this->sFilePath.c_str();
			
			//open database connection
			rc = sqlite3_open(cPath, &db);
			bDbIsOpen = true;
		};
		
		if(rc != SQLITE_OK){
#if TEST_ENV == true
			sErrMsg = (std::string)"Can't open DB: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
			printf(sErrMsg.c_str());
#endif
			bIsOk = false;
			bDbIsOpen = false;
		};
		
		
		if(bIsOk) {
			rc = sqlite3_prepare_v2(db, cSqlSelectQuestions, -1, &stmt, NULL);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't Prepare SQL: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		if(bIsOk){
			while(sqlite3_step(stmt) == SQLITE_ROW){
//				sQuestion
//				this->ilQuestionMax++;
				
				//fldId, 
				sQuestion.iSqlite_Id = sqlite3_column_int(stmt, 0);
				
				//fldQuestionNo, 
				sQuestion.iId = sqlite3_column_int(stmt, 1);
				
				//fldQuestionType, 
				sResult.str("");
				sResult << sqlite3_column_text(stmt, 2);
				sQuestion.eQuestionType = modMisc::convertEnum_QuesTypeSubjectText(sResult.str());
				
				//fldQuestionGroup, 
				sResult.str("");
				sResult << sqlite3_column_text(stmt, 3);
				sQuestion.eQuestionGroup = modMisc::convertEnum_QuesTypeGroupText(sResult.str());
				
				//fldQuestionEncoded, 
				sResult.str("");
				sResult << sqlite3_column_text(stmt, 4);
				sQuestion.sQuestionEncoded = sResult.str();
				
				//fldDecodeText, 
				sResult.str("");
				sResult << sqlite3_column_text(stmt, 5);
				sQuestion.sDecodeText = sResult.str();
				
				//fldQuestionText, 
				sResult.str("");
				sResult << sqlite3_column_text(stmt, 6);
				sQuestion.sQuestionText = sResult.str();
				
				//fldGivenAnswerText, 
				sResult.str("");
				sResult << sqlite3_column_text(stmt, 7);
				sQuestion.sGivenAnswer = sResult.str();
				
				//fldGivenAnswerMultipleChoice, 
//				sSubjectGroup.iDifficulty = sqlite3_column_int(stmt_EnabledGroup, 8);
				
				//fldDifficulty, 
				sQuestion.iDifficulty = sqlite3_column_int(stmt, 9);
				
				//fldCorrect, 
				if (sqlite3_column_int(stmt, 10) == 1) {
					sQuestion.bCorrect = true;
				} else {
					sQuestion.bCorrect = false;
				};
				
				//fldAnswerType, 
				sResult.str("");
				sResult << sqlite3_column_text(stmt, 11);
				sQuestion.eAnswerType = modMisc::convertEnumText_AnswerType(sResult.str());

				//fldIsMultipleChoice, 
				if (sqlite3_column_int(stmt, 12) == 1) {
					sQuestion.bIsMultipleChoice = true;
				} else {
					sQuestion.bIsMultipleChoice = false;
				};
				
				//fldHasOnlyOneAnswer, 
				if (sqlite3_column_int(stmt, 13) == 1) {
					sQuestion.bOnlyOneAnswer = true;
				} else {
					sQuestion.bOnlyOneAnswer = false;
				};
				
				//fldIsDirty, 
				if (sqlite3_column_int(stmt, 14) == 1) {
					sQuestion.bIsDirty = true;
				} else {
					sQuestion.bIsDirty = false;
				};

				//fldPic, 
				sResult.str("");
				sResult << sqlite3_column_text(stmt, 15);
				sQuestion.ePic = modMisc::convertEnumText_Pic(sResult.str());

				//fldHasPic
				if (sqlite3_column_int(stmt, 16) == 1) {
					sQuestion.bHasPic = true;
				} else {
					sQuestion.bHasPic = false;
				};
				
				sQuestion = loadValues(&sQuestion);
				sQuestion = loadAnswers(&sQuestion);
				sQuestion = loadChoices(&sQuestion);
				
				vecQuestions.push_back(sQuestion);
				this->ilQuestionMax++;
			};//while
		};//if bIsOk
		
		if(bIsOk){
			rc = sqlite3_reset(stmt);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				fprintf(stderr, "Can't reset statment: %\n", sqlite3_errmsg(db));
#endif
				bIsOk = false;
			};
		};//if bIsOk
		
		
		cSqlSelectQuestions = NULL;
//		bool bIsOk;
		if (bIsOk) {
			sqlite3_finalize(stmt);
		};
		sErrMsg.clear();
		cPath = NULL;
//		std::stringstream sResult;
		strResult.clear();
//		strQuestion sQuestion;
		
	} catch (...) {
		//Close database connection	
		rc = sqlite3_close(db);
		bDbIsOpen = false;
	};
};

strQuestion ClsQuiz::loadValues(strQuestion* sQuestion) {
	strQuestion sQuestionTemp;
	
	sQuestionTemp = *sQuestion;
	int rc;
	
	try {
		const char *cSqlSelectTbl_QuestionValues("select fldId, fldAppId, fldQuestionID, fldValues, fldIsDirty from tblQuestionsValues where fldQuestionID = :iQuestionNo order by fldAppId ;");
		bool bIsOk;
		sqlite3_stmt *stmt;
		std::string sErrMsg;
		char *cPath;
		std::stringstream sResult;
		std::string strResult;
		strFloatValue eValue;
		int iParameterPosition;
		int iTemp;
		
		sQuestionTemp.vecValues.clear();		
		bIsOk = true;
		rc = SQLITE_OK;
		
		if (bDbIsOpen != true){
			cPath = (char*)this->sFilePath.c_str();
			
			//open database connection
			rc = sqlite3_open(cPath, &db);
			bDbIsOpen = true;
		};
		
		if(rc != SQLITE_OK){
#if TEST_ENV == true
			sErrMsg = (std::string)"Can't open DB: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
			printf(sErrMsg.c_str());
#endif
			bIsOk = false;
			bDbIsOpen = false;
		};
		
		
		if(bIsOk) {
			rc = sqlite3_prepare_v2(db, cSqlSelectTbl_QuestionValues, -1, &stmt, NULL);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't Prepare SQL: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};

		//iQuestionNo
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt, ":iQuestionNo");
			
			iTemp = sQuestionTemp.iId;
			
			rc = sqlite3_bind_int(stmt, 
								  iParameterPosition,
								  iTemp);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		
		
		if(bIsOk){
			while(sqlite3_step(stmt) == SQLITE_ROW){

				//fldId, 
				eValue.iId = sqlite3_column_int(stmt, 1);
				
				eValue.fValue = sqlite3_column_double(stmt, 3);
				
				if (sqlite3_column_int(stmt,  4) == 1) {
					eValue.bIsDirty = true;
				} else {
					eValue.bIsDirty = false;
				};
				
				sQuestionTemp.vecValues.push_back(eValue);
			};//while
		};//if bIsOk
		
		if(bIsOk){
			rc = sqlite3_reset(stmt);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				fprintf(stderr, "Can't reset statment: %\n", sqlite3_errmsg(db));
#endif
				bIsOk = false;
			};
		};//if bIsOk
		
		cSqlSelectTbl_QuestionValues = NULL;
//		bool bIsOk;
		if (bIsOk) {
			sqlite3_finalize(stmt);
		};
		sErrMsg.clear();
		cPath = NULL;
//		std::stringstream sResult;
		strResult.clear();
//		strFloatValue eValue;
//		int iParameterPosition;
//		int iTemp;

	
	} catch (...) {
		//Close database connection	
		rc = sqlite3_close(db);
		bDbIsOpen = false;
	};
	
	return sQuestionTemp;
};

strQuestion ClsQuiz::loadAnswers(strQuestion* sQuestion) {
	strQuestion sQuestionTemp;
	
	sQuestionTemp = *sQuestion;
	
	return sQuestionTemp;
};

strQuestion ClsQuiz::loadChoices(strQuestion* sQuestion) {
	strQuestion sQuestionTemp;
	
	sQuestionTemp = *sQuestion;
	int rc;
	
	try {
		const char *cSqlSelectTbl_QuestionValues("select fldId, fldAppId, fldQuestionID, fldValue, fldIsSelected, fldIsCorrect from tblQuestionsChoices where fldQuestionID = :iQuestionNo order by fldAppId ;");
		bool bIsOk;
		sqlite3_stmt *stmt;
		std::string sErrMsg;
		char *cPath;
		std::stringstream sResult;
		std::string strResult;
		strStringAnswer eChoice;
		int iParameterPosition;
		int iTemp;
		
		sQuestionTemp.vecChoices.clear();		
		bIsOk = true;
		rc = SQLITE_OK;
		
		if (bDbIsOpen != true){
			cPath = (char*)this->sFilePath.c_str();
			
			//open database connection
			rc = sqlite3_open(cPath, &db);
			bDbIsOpen = true;
		};
		
		if(rc != SQLITE_OK){
#if TEST_ENV == true
			sErrMsg = (std::string)"Can't open DB: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
			printf(sErrMsg.c_str());
#endif
			bIsOk = false;
			bDbIsOpen = false;
		};
		
		
		if(bIsOk) {
			rc = sqlite3_prepare_v2(db, cSqlSelectTbl_QuestionValues, -1, &stmt, NULL);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't Prepare SQL: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};

		//iQuestionNo
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt, ":iQuestionNo");
			
			iTemp = sQuestionTemp.iId;
			
			rc = sqlite3_bind_int(stmt, 
								  iParameterPosition,
								  iTemp);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		
		
		if(bIsOk){
			while(sqlite3_step(stmt) == SQLITE_ROW){
				//fldId, fldAppId, fldQuestionID, fldValue, fldIsSelected, fldIsCorrect from tblQuestionsChoices where fldQuestionID = :iQuestionNo order by fldAppId ;");
				

				//fldId, 
				eChoice.iId = sqlite3_column_int(stmt, 1);
				
				sResult.str("");
				sResult << sqlite3_column_text(stmt, 3);
				strResult = sResult.str();
				eChoice.sValue = strResult;
				
				if (sqlite3_column_int(stmt,  4) == 1) {
					eChoice.bIsSelected = true;
				} else {
					eChoice.bIsSelected = false;
				};
				
				if (sqlite3_column_int(stmt,  5) == 1) {
					eChoice.bIsCorrect = true;
				} else {
					eChoice.bIsCorrect = false;
				};
				
				sQuestionTemp.vecChoices.push_back(eChoice);
			};//while
		};//if bIsOk
		
		if(bIsOk){
			rc = sqlite3_reset(stmt);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				fprintf(stderr, "Can't reset statment: %\n", sqlite3_errmsg(db));
#endif
				bIsOk = false;
			};
		};//if bIsOk
		
		
		
		cSqlSelectTbl_QuestionValues = NULL;
//		bool bIsOk;
		if (bIsOk) {
			sqlite3_finalize(stmt);
		};
		sErrMsg.clear();
		cPath = NULL;
//		std::stringstream sResult;
		strResult.clear();
//		strStringAnswer eChoice;
//		int iParameterPosition;
//		int iTemp;
		
	} catch (...) {
		//Close database connection	
		rc = sqlite3_close(db);
		bDbIsOpen = false;
	};
	
	return sQuestionTemp;
};

#pragma mark -
#pragma mark Add Question

void ClsQuiz::createQuiz(){
	
};

void ClsQuiz::addQuestion(ClsPreferences *cPref){
	strQuestion oTempQuestion;
	int iDifficultyRange;
	bool bIsOk;
	int iAttempts;
	int iDifficulty;
	std::string sQuestionType;
	std::string sQuestionGroup;
	int iCounter;
	int iAttemptsMax;
	
	cPreferences = cPref;

	if (cPreferences->getType() == ePrefSimple) {
		iAttemptsMax = 500;
	} else if (cPreferences->getType() == ePrefBySubject) {
		iAttemptsMax = 5;
	} else if (cPreferences->getType() == ePrefByGroup) {
		iAttemptsMax = 5;
	} else {
		iAttemptsMax = 5;
	};
	
	
	iDifficulty = (int)(cPreferences->getDifficulty());
	
	bIsOk = false;
	iAttempts = 0;
	
	while (not bIsOk){
		//iDifficultyRange = (int)(((float)iDifficulty * (float)modMisc::iRandomNumber(50, 100)) / (float)modMisc::iRandomNumber(500, 1000) * (float)((iAttemptsMax + (2 * iAttempts)) / iAttemptsMax)); 
		iDifficultyRange = (int)(((iAttemptsMax + (2 * iAttempts)) / iAttemptsMax) * (float)(modMisc::iDifficultyMax / modMisc::iRandomNumber(7, 15)));
		
		
//		(int)(((float)iDifficulty * (float)modMisc::iRandomNumber(50, 100)) / (float)modMisc::iRandomNumber(500, 1000) * (float)((iAttemptsMax + (2 * iAttempts)) / iAttemptsMax)); 
		
		oTempQuestion = this->randomQuestionType();
		
		iAttempts++;
		
		if(iAttempts > iAttemptsMax) {
			bIsOk = true;
		} else {
			if (cPreferences->getType() == ePrefSimple) {
				if(oTempQuestion.iDifficulty>(iDifficulty+iDifficultyRange) or oTempQuestion.iDifficulty<(iDifficulty-iDifficultyRange)) {
					bIsOk = false;
				} else {
					bIsOk = true;
				};
			}; //Type = Simple
			
			if (cPreferences->getType() == ePrefBySubject) {

				
				
				
				sQuestionType = modMisc::convertEnumText_QuestionType(oTempQuestion.eQuestionType);
				sQuestionGroup = modMisc::convertEnum_QuesTypeGroupText(oTempQuestion.eQuestionGroup);
				
				if (cPreferences->getSubjectEnabled(&sQuestionGroup, &sQuestionType)) {
					bIsOk = true;
				} else {
					bIsOk = false;
				};
				
				
				
			}; //Type = By Subject

			if (cPreferences->getType() == ePrefByGroup) {
				bIsOk = true;//checks have already happened
			};
			
			for (iCounter = 0; iCounter < vecQuestions.size(); iCounter++) {
				if (QuestionsSame(oTempQuestion, vecQuestions[iCounter++])) {
					bIsOk = false;
				};
			};
		};//if attempts
	};//while not is ok
	
//	oTempQuestion.iId = this->getNextQuestionId() + 1;//iId gets set in insertQuestion
	
	if (bIsOk) {
		this->vecQuestions.push_back(oTempQuestion);
		this->insertQuestionAll(this->ilQuestionMax);
		this->ilQuestionMax++;
	};
	
//	strQuestion oTempQuestion;
//	int iDifficultyRange;
//	bool bIsOk;
//	int iAttempts;
//	int iDifficulty;
	sQuestionType.clear();
	sQuestionGroup.clear();
//	int iCounter;
//	int iAttemptsMax;
	
};

int ClsQuiz::getNextQuestionId(){
	int iSize;
	
	iSize = this->vecQuestions.size();

	return iSize;
};

/*
void ClsQuiz::addQuestions(int, ClsPreferences*){

};

void ClsQuiz::addQuestion(int iDifficulty, enumQuestionType eQuestionType){
				
};
*/

enumFnTrig ClsQuiz::generateRandomFnTrig(bool bInverse){
//if bInverse == true then return the inverse trig functions
	int iRandomNo;
	enumFnTrig eAnswer;
	modMisc::iRandomNumber(1, 3);
	
	switch(iRandomNo){
		case 1:
			if (bInverse){
				eAnswer = eFnTrigSinInv;
			}
			else {
				eAnswer = eFnTrigSin;
			};
			break;
		case 2:
			if (bInverse){
				eAnswer = eFnTrigCosInv;
			}
			else {
				eAnswer = eFnTrigCos;
			};
			break;
		case 3:
			if (bInverse){
				eAnswer = eFnTrigTanInv;
			}
			else {
				eAnswer = eFnTrigTan;
			};
			break;
	};
	
	return eAnswer;
};

void ClsQuiz::generateQuestion(int iDifficulty){
	
};
	
void ClsQuiz::generateQuestion(int iDifficulty, enumQuestionType eQuestionType){
};

strQuestion ClsQuiz::randomQuestionType() {
//	int iRandom;
	strQuestion eTemp;
	strStringAnswer sMultipleChoiceAnswer; 
	vector<strStringAnswer>::iterator itMultipleChoiceAnswer; 
//	std::stringstream ssTemp;
	int iAttemptsQuestion;
	int iAttemptsQuestionMax = 20;
	int iAttemptsGroupType;
	int iAttemptsGroupTypeMax = 20;
//	bool bIsOk;
	bool bIsOkGroupType;
	bool bIsOkQuestion;
//	int iPreviousRandom;
//	int iDifficulty;
	
//	time_t tmTimestarted;
//	time_t tmTimeCurrent;
	
//	tmTimestarted = time (NULL);
//	tmTimeCurrent = time (NULL);
	
	enumQuestionGroup eQuestionGroup;
	enumQuestionType eQuestionType;
//	enumQuestionGroup eQuestionGroupPrevious;
//	enumQuestionType eQuestionTypePrevious;
	
	do {
		bIsOkQuestion= false;
		iAttemptsQuestion = 0;
		
		do {
			bIsOkGroupType= false;
			iAttemptsGroupType = 0;
			
			do {
				eQuestionGroup = cPreferences->getRandomQuestionGroup();
				eQuestionType = cPreferences->getRandomQuestionType(eQuestionGroup);
				
				if (eQuestionGroup == eQuesGrpError or eQuestionType == eQuesError) {
					if (iAttemptsGroupType == iAttemptsGroupTypeMax) {
						bIsOkGroupType = true;
					} else {
						iAttemptsGroupType++;
					};
				} else {
					bIsOkGroupType = true;
				};
			} while (bIsOkGroupType == false);
			
			eTemp.vecValues.clear();
			eTemp.vecAnswers.clear();
			eTemp.vecChoices.clear();
			
			eTemp = randomQuestionTypeRaw(eQuestionGroup, eQuestionType);
			
			if (eTemp.eQuestionType == eQuesError) {
				if (iAttemptsQuestion == iAttemptsQuestionMax) {
					bIsOkQuestion = true;
				} else {
					iAttemptsQuestion++;
				};
			} else {
				bIsOkQuestion = true;
			};
		} while (bIsOkQuestion == false);
	} while (eTemp.eQuestionType == eQuesError);
	
	if (eTemp.bIsMultipleChoice == true) {
		//jumble the answers around randomly
		int iJumbleCounter;
		int iJumblePosA;
		int iJumblePosB;
		
		for (iJumbleCounter = 0; iJumbleCounter < 50; iJumbleCounter++) {
			iJumblePosA = modMisc::iRandomNumber(0, eTemp.vecChoices.size() - 1);
			if (iJumblePosA < 5) {
				iJumblePosB = modMisc::iRandomNumber(0, eTemp.vecChoices.size() - 1);
			} else {
				iJumblePosB = modMisc::iRandomNumber(0, 4);
			};

			if (iJumblePosA != iJumblePosB) {
				//shouldn't swap into the same place	
				if (not ((eTemp.vecChoices[iJumblePosA].bIsCorrect == true and iJumblePosB > 4)
					or   (eTemp.vecChoices[iJumblePosB].bIsCorrect == true and iJumblePosA > 4))) {
					//shouldn't swap a correct answer out of the first 5 positions because only 
					//the first 5 are displayed to the user to select from
					
					//swap answers around
					strStringAnswer sSwapAnswerA;
					strStringAnswer sSwapAnswerB;

					
					sSwapAnswerA = eTemp.vecChoices[iJumblePosA];
					sSwapAnswerB = eTemp.vecChoices[iJumblePosB];
					eTemp.vecChoices.at(iJumblePosA) = sSwapAnswerB;
					eTemp.vecChoices.at(iJumblePosB) = sSwapAnswerA;
				};
			};
		};
	};
	
	return eTemp;

//	int iRandom;
//	strFloatValue fRandomFigure;
//	strQuestion eTemp;
//	int iTemp;
//	strStringAnswer sMultipleChoiceAnswer; 
//	vector<strStringAnswer>::iterator itMultipleChoiceAnswer; 
//	std::stringstream ssTemp;
//	enumFnTrig eTempFnTrig;
//	int iAttempts;
//	int iAttemptsMax;
//	bool bIsOk;
//	int iPreviousRandom;
	//	const int iCutOffTime = 3;
//	time_t tmTimestarted;
	
};

strQuestion ClsQuiz::randomQuestionTypeRaw(enumQuestionGroup eQuestionGroup, enumQuestionType eQuestionType) {
	strQuestion eTemp;
	
	if (eQuestionGroup == eQuesGrpArithmeticBasics) {
		if (eQuestionType == eQuesAddingNumbers) {
			eTemp = randomQuestionType_AddingTwoNumbers();
		} else if (eQuestionType == eQuesSubtractingNumbers) {
			eTemp = randomQuestionType_SubtractTwoNumbers();
		} else if (eQuestionType == eQuesMultiplingNumbers) {
			eTemp = randomQuestionType_MultiplyTwoNumbers();
		} else if (eQuestionType == eQuesDividingNumbers) {
			eTemp = randomQuestionType_DivideTwoNumbers();
		} else {
			eTemp.eQuestionType = eQuesError;
			eTemp.bIsMultipleChoice = true;
			eTemp.sQuestionEncoded = "Error:";
		};
	} else if (eQuestionGroup == eQuesGrpArithmeticAdvanced) {
		if (eQuestionType == eQuesSquare) {
			eTemp = randomQuestionType_Square();
		} else if (eQuestionType == eQuesCubed) {
			eTemp = randomQuestionType_Cubed();
		} else if (eQuestionType == eQuesSquareRoot) {
			eTemp = randomQuestionType_SquareRoot();
		} else if (eQuestionType == eQuesCubedRoot) {
			eTemp = randomQuestionType_CubedRoot();
		} else if (eQuestionType == eQuesArithmetic) {
			eTemp = randomQuestionType_ArithmeticMisc3Figures();
		} else {
			eTemp.eQuestionType = eQuesError;
			eTemp.bIsMultipleChoice = true;
			eTemp.sQuestionEncoded = "Error:";
		};
	} else if (eQuestionGroup == eQuesGrpShapes) {
		if (eQuestionType == eQuesPythagorean) {
			eTemp = randomQuestionType_Pythagorean();
        } else if (eQuestionType == eQuesTrigonometry) {
            eTemp = randomQuestionType_Trigonometry();
		} else if (eQuestionType == eQuesParallogram) {
			eTemp = randomQuestionType_parallogram();
		} else if (eQuestionType == eQuesCylinder) {
			eTemp = randomQuestionType_Cylinder();
		} else {
			eTemp.eQuestionType = eQuesError;
			eTemp.bIsMultipleChoice = true;
			eTemp.sQuestionEncoded = "Error:";
		};
	} else if (eQuestionGroup == eQuesGrpAverages) {
		if (eQuestionType == eQuesAverageMean) {
			eTemp = randomQuestionType_AverageMean();
		} else if (eQuestionType == eQuesAverageMedian) {
			eTemp = randomQuestionType_AverageMedian();
		} else if (eQuestionType == eQuesAverageMode) {
			eTemp = randomQuestionType_AverageMode();
		} else {
			eTemp.eQuestionType = eQuesError;
			eTemp.bIsMultipleChoice = true;
			eTemp.sQuestionEncoded = "Error:";
		};
	} else if (eQuestionGroup == eQuesGrpFractions) {
		if (eQuestionType == eQuesFractionsAddSub) {
			eTemp = randomQuestionType_FractionsAddSub();
		} else if (eQuestionType == eQuesFractionsMultiDiv) {
			eTemp = randomQuestionType_FractionsMulitpleDivide();
		} else {
			eTemp.eQuestionType = eQuesError;
			eTemp.bIsMultipleChoice = true;
			eTemp.sQuestionEncoded = "Error:";
		};
	} else if (eQuestionGroup == eQuesGrpEquations) {
		if (eQuestionType == eQuesEquationsQuadratic) {
			eTemp = randomQuestionType_EquationsQuadratic();
        } else if (eQuestionType == eQuesEquationsSimultaneousLinear) {
                eTemp = randomQuestionType_EquationsSimultaneousLinear();
		} else {
			eTemp.eQuestionType = eQuesError;
			eTemp.bIsMultipleChoice = true;
			eTemp.sQuestionEncoded = "Error:";
		};
	} else {
		eTemp.eQuestionType = eQuesError;
		eTemp.bIsMultipleChoice = true;
		eTemp.sQuestionEncoded = "Error:";
	};

	return eTemp;
};




strQuestion ClsQuiz::randomQuestionType_AddingTwoNumbers() {
	strFloatValue fRandomFigure;
	strQuestion eTemp;
	int iTemp;
	strStringAnswer sMultipleChoiceAnswer; 
	vector<strStringAnswer>::iterator itMultipleChoiceAnswer; 
	std::stringstream ssTemp;
	int iDifficultyGroupLevel;
	std::string sGroupName;
	time_t tmStart;
	time_t tmEnd;
//time_t tmTimestarted;
	
	tmStart = time (NULL);
	
	eTemp.eQuestionGroup = eQuesGrpArithmeticBasics;
	eTemp.bHasPic = false;
	sGroupName = modMisc::convertEnum_QuesTypeGroupText(eQuesGrpArithmeticBasics);
	eTemp.ePic = ePic_None;
	
	eTemp.vecValues.clear();
	eTemp.vecAnswers.clear();
	eTemp.vecChoices.clear();
	
	
	const int gciQuestionTypeMax = 3; //note: needs to be the maximum integer in the enumQuestionType so that we can generate random question types 
	
	if (cPreferences->getType() == ePrefByGroup or cPreferences->getType() == ePrefBySubject) {
		iDifficultyGroupLevel = (int)cPreferences->getDifficulty(&sGroupName);
	} else {
		iDifficultyGroupLevel = (int)modMisc::iRandomNumber(1, gciQuestionTypeMax);
	};
	
	
	
	eTemp.sQuestionEncoded = "<0> + <1> =";
	eTemp.eAnswerType = eAnsTypNumeric;

	
	
	switch (iDifficultyGroupLevel){ 
		case 1:
			eTemp.eQuestionType = eQuesAddingNumbersEasy;
			eTemp.iDifficulty = 25;
			
			fRandomFigure.fValue = (float)modMisc::iRandomNumber(2, 12);
			eTemp.vecValues.push_back(fRandomFigure);
			fRandomFigure.fValue = (float)modMisc::iRandomNumber(2, 12);
			eTemp.vecValues.push_back(fRandomFigure);
			break;
		case 2:
			eTemp.eQuestionType = eQuesAddingNumbersMedium;
			eTemp.iDifficulty = 65;
			
			fRandomFigure.fValue = (float)modMisc::iRandomNumber(8, 15);
			eTemp.vecValues.push_back(fRandomFigure);
			fRandomFigure.fValue = (float)modMisc::iRandomNumber(8, 15);
			eTemp.vecValues.push_back(fRandomFigure);
			break;
		case 3:
			eTemp.eQuestionType = eQuesAddingNumbersHard;
			eTemp.iDifficulty = 115;
			
			fRandomFigure.fValue = (float)modMisc::iRandomNumber(12, 25);
			eTemp.vecValues.push_back(fRandomFigure);
			fRandomFigure.fValue = (float)modMisc::iRandomNumber(12, 25);
			eTemp.vecValues.push_back(fRandomFigure);
			break;
		default:
			eTemp.eQuestionType = eQuesError;
			eTemp.bIsMultipleChoice = false;
			eTemp.sQuestionEncoded = "Error:";
	};
	
			
						
			
	fRandomFigure.fValue = (float)(eTemp.vecValues[0].fValue + eTemp.vecValues[1].fValue);
	eTemp.vecAnswers.push_back(fRandomFigure);
	
//	if (modMisc::iRandomNumber(0, 10) < 5) {
	if (modMisc::iRandomNumber(0, 10) < 15) {
		//multiple choice
		
		//Correct answer
		eTemp.bIsMultipleChoice = true;
		eTemp.bOnlyOneAnswer = true;
		sMultipleChoiceAnswer.bIsCorrect = true; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		ssTemp << (int)eTemp.vecAnswers[0].fValue;
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		
		eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		
		//wrong answers
        do {
            for (int iDelta = 1; iDelta < 10; iDelta++) {
                iTemp = (int)(eTemp.vecAnswers[0].fValue + modMisc::iRandomNumberNonZero(-1 * iDelta, iDelta));
                if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
                    sMultipleChoiceAnswer.bIsCorrect = false; 
                    sMultipleChoiceAnswer.bIsSelected = false; 
                    ssTemp.str("");
                    ssTemp << iTemp;
                    sMultipleChoiceAnswer.sValue = ssTemp.str();
                    if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                        eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
                    };
                };
                
                iTemp = (int)(eTemp.vecAnswers[0].fValue + modMisc::iRandomNumberNonZero(-1 * iDelta, iDelta + 1));
                if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
                    sMultipleChoiceAnswer.bIsCorrect = false; 
                    sMultipleChoiceAnswer.bIsSelected = false; 
                    ssTemp.str("");
                    ssTemp << iTemp;
                    sMultipleChoiceAnswer.sValue = ssTemp.str();
                    if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                        eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
                    };
                };
                
                iTemp = (int)(eTemp.vecAnswers[0].fValue + modMisc::iRandomNumberNonZero(-1 * iDelta - 1, iDelta));
                if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
                    sMultipleChoiceAnswer.bIsCorrect = false; 
                    sMultipleChoiceAnswer.bIsSelected = false; 
                    ssTemp.str("");
                    ssTemp << iTemp;
                    sMultipleChoiceAnswer.sValue = ssTemp.str();
                    if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                        eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
                    };
                };
            };
            
            tmEnd = time (NULL);
        } while (eTemp.vecChoices.size() < 8 and tmStart + iCutOffTime < (int)tmEnd);

	} else {
		//NOT multiple choice
		eTemp.bIsMultipleChoice = false;
	};
	
	
//	strFloatValue fRandomFigure;
//	strQuestion eTemp;
//	int iTemp;
//	strStringAnswer sMultipleChoiceAnswer; 
//	vector<strStringAnswer>::iterator itMultipleChoiceAnswer; 
//	std::stringstream ssTemp;
//	enumFnTrig eTempFnTrig;
//	int iDifficultyGroupLevel;
	sGroupName.clear();
//	time_t tmStart;
	//time_t tmTimestarted;
	
	return eTemp;
};


strQuestion ClsQuiz::randomQuestionType_SubtractTwoNumbers(){
	strFloatValue fRandomFigure;
	strQuestion eTemp;
	int iTemp;
	strStringAnswer sMultipleChoiceAnswer; 
	vector<strStringAnswer>::iterator itMultipleChoiceAnswer; 
	std::stringstream ssTemp;
	int iDifficultyGroupLevel;
	std::string sGroupName;
	time_t tmStart;
	time_t tmEnd;

	tmStart = time (NULL);
	
	eTemp.eQuestionGroup = eQuesGrpArithmeticBasics;
	eTemp.bHasPic = false;
	sGroupName = modMisc::convertEnum_QuesTypeGroupText(eQuesGrpArithmeticBasics);
	eTemp.ePic = ePic_None;
	
	eTemp.vecValues.clear();
	eTemp.vecAnswers.clear();
	eTemp.vecChoices.clear();
	
	const int gciQuestionTypeMax = 3; //note: needs to be the maximum integer in the enumQuestionType so that we can generate random question types 
	
	if (cPreferences->getType() == ePrefByGroup or cPreferences->getType() == ePrefBySubject) {
		iDifficultyGroupLevel = (int)cPreferences->getDifficulty(&sGroupName);
	} else {
		iDifficultyGroupLevel = (int)modMisc::iRandomNumber(1, gciQuestionTypeMax);
	};
	
	
	eTemp.sQuestionEncoded = "<0> - <1> =";
	eTemp.eAnswerType = eAnsTypNumeric;
	
	
	
	switch (iDifficultyGroupLevel){ 
		case 1:
			eTemp.eQuestionType = eQuesSubtractingNumbersEasy;
			eTemp.iDifficulty = 120;
			
			fRandomFigure.fValue = (float)modMisc::iRandomNumber(4, 15);
			eTemp.vecValues.push_back(fRandomFigure);
			fRandomFigure.fValue = (float)modMisc::iRandomNumber(2, 9);
			eTemp.vecValues.push_back(fRandomFigure);
			break;
		case 2:
			eTemp.eQuestionType = eQuesSubtractingNumbersMedium;
			eTemp.iDifficulty = 170;
			
			fRandomFigure.fValue = (float)modMisc::iRandomNumber(12, 24);
			eTemp.vecValues.push_back(fRandomFigure);
			fRandomFigure.fValue = (float)modMisc::iRandomNumber(6, 15);
			eTemp.vecValues.push_back(fRandomFigure);
			break;
		case 3:
			eTemp.eQuestionType = eQuesSubtractingNumbersHard;
			eTemp.iDifficulty = 230;
			
			fRandomFigure.fValue = (float)modMisc::iRandomNumber(20, 99);
			eTemp.vecValues.push_back(fRandomFigure);
			fRandomFigure.fValue = (float)modMisc::iRandomNumber(12, 75);
			eTemp.vecValues.push_back(fRandomFigure);
			break;
		default:
			eTemp.eQuestionType = eQuesError;
			eTemp.bIsMultipleChoice = false;
			eTemp.sQuestionEncoded = "Error:";
	};
	
	
	
	
	fRandomFigure.fValue = (float)(eTemp.vecValues[0].fValue - eTemp.vecValues[1].fValue);
	eTemp.vecAnswers.push_back(fRandomFigure);
	
//	if (modMisc::iRandomNumber(0, 10) < 5) {
	if (modMisc::iRandomNumber(0, 10) < 15) {
		//multiple choice
		
		//Correct answer
		eTemp.bIsMultipleChoice = true;
		eTemp.bOnlyOneAnswer = true;
		sMultipleChoiceAnswer.bIsCorrect = true; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		ssTemp << (int)eTemp.vecAnswers[0].fValue;
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		
		eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		
		//wrong answers
		do {
            for (int iWrongNumberRange = 1; iWrongNumberRange < 10; iWrongNumberRange++) {
                iTemp = (int)(eTemp.vecAnswers[0].fValue + modMisc::iRandomNumberNonZero(-1 * iWrongNumberRange, iWrongNumberRange));
                if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
                    sMultipleChoiceAnswer.bIsCorrect = false; 
                    sMultipleChoiceAnswer.bIsSelected = false; 
                    ssTemp.str("");
                    ssTemp << iTemp;
                    sMultipleChoiceAnswer.sValue = ssTemp.str();
                    if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                        eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
                    };
                };
                
                iTemp = (int)(eTemp.vecAnswers[0].fValue + modMisc::iRandomNumberNonZero((-1 * iWrongNumberRange) - 1, iWrongNumberRange));
                if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
                    sMultipleChoiceAnswer.bIsCorrect = false; 
                    sMultipleChoiceAnswer.bIsSelected = false; 
                    ssTemp.str("");
                    ssTemp << iTemp;
                    sMultipleChoiceAnswer.sValue = ssTemp.str();
                    if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                        eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
                    };
                };
                
                iTemp = (int)(eTemp.vecAnswers[0].fValue + modMisc::iRandomNumberNonZero(-1 * iWrongNumberRange, iWrongNumberRange - 1));
                if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
                    sMultipleChoiceAnswer.bIsCorrect = false; 
                    sMultipleChoiceAnswer.bIsSelected = false; 
                    ssTemp.str("");
                    ssTemp << iTemp;
                    sMultipleChoiceAnswer.sValue = ssTemp.str();
                    if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                        eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
                    };
                };
            };
            
            tmEnd = time (NULL);
		} while (eTemp.vecChoices.size() < 8 and (int)tmStart + iCutOffTime < (int)tmEnd);
	} else {
		//NOT multiple choice
		eTemp.bIsMultipleChoice = false;
	};
	
	
//	strFloatValue fRandomFigure;
//	strQuestion eTemp;
//	int iTemp;
//	strStringAnswer sMultipleChoiceAnswer; 
//	vector<strStringAnswer>::iterator itMultipleChoiceAnswer; 
//	std::stringstream ssTemp;
//	enumFnTrig eTempFnTrig;
//	int iDifficultyGroupLevel;
	sGroupName.clear();
//	time_t tmStart;

	return eTemp;
};

strQuestion ClsQuiz::randomQuestionType_MultiplyTwoNumbers(){
	strFloatValue fRandomFigure;
	strQuestion eTemp;
	int iTemp;
	strStringAnswer sMultipleChoiceAnswer; 
	vector<strStringAnswer>::iterator itMultipleChoiceAnswer; 
	std::stringstream ssTemp;
	int iDifficultyGroupLevel;
	std::string sGroupName;
	time_t tmStart;
	
	tmStart = time (NULL);
	
	eTemp.eQuestionGroup = eQuesGrpArithmeticBasics;
	eTemp.bHasPic = false;
	sGroupName = modMisc::convertEnum_QuesTypeGroupText(eQuesGrpArithmeticBasics);
	eTemp.ePic = ePic_None;
	
	eTemp.vecValues.clear();
	eTemp.vecAnswers.clear();
	eTemp.vecChoices.clear();
	
	const int gciQuestionTypeMax = 3; //note: needs to be the maximum integer in the enumQuestionType so that we can generate random question types 
	
	if (cPreferences->getType() == ePrefByGroup or cPreferences->getType() == ePrefBySubject) {
		iDifficultyGroupLevel = (int)cPreferences->getDifficulty(&sGroupName);
	} else {
		iDifficultyGroupLevel = (int)modMisc::iRandomNumber(1, gciQuestionTypeMax);
	};
	
	
	eTemp.sQuestionEncoded = "<0> * <1> =";
	eTemp.eAnswerType = eAnsTypNumeric;
	
	
	
	switch (iDifficultyGroupLevel){ 
		case 1:
			eTemp.eQuestionType = eQuesMultiplingNumbersEasy;
			eTemp.iDifficulty = 90;
			
			fRandomFigure.fValue = (float)modMisc::iRandomNumber(2, 9);
			eTemp.vecValues.push_back(fRandomFigure);
			fRandomFigure.fValue = (float)modMisc::iRandomNumber(2, 12);
			eTemp.vecValues.push_back(fRandomFigure);
			break;
		case 2:
			eTemp.eQuestionType = eQuesMultiplingNumbersMedium;
			eTemp.iDifficulty = 135;
			
			fRandomFigure.fValue = (float)modMisc::iRandomNumber(8, 12);
			eTemp.vecValues.push_back(fRandomFigure);
			fRandomFigure.fValue = (float)modMisc::iRandomNumber(8, 15);
			eTemp.vecValues.push_back(fRandomFigure);
			break;
		case 3:
			eTemp.eQuestionType = eQuesMultiplingNumbersHard;
			eTemp.iDifficulty = 190;
			
			fRandomFigure.fValue = (float)modMisc::iRandomNumber(12, 15);
			eTemp.vecValues.push_back(fRandomFigure);
			fRandomFigure.fValue = (float)modMisc::iRandomNumber(12, 20);
			eTemp.vecValues.push_back(fRandomFigure);
			break;
		default:
			eTemp.eQuestionType = eQuesError;
			eTemp.bIsMultipleChoice = false;
			eTemp.sQuestionEncoded = "Error:";
	};
	
	
	
	
	fRandomFigure.fValue = (float)(eTemp.vecValues[0].fValue * eTemp.vecValues[1].fValue);
	eTemp.vecAnswers.push_back(fRandomFigure);
	
//	if (modMisc::iRandomNumber(0, 10) < 5) {
	if (modMisc::iRandomNumber(0, 10) < 15) {
		//multiple choice
		
		//Correct answer
		eTemp.bIsMultipleChoice = true;
		eTemp.bOnlyOneAnswer = true;
		sMultipleChoiceAnswer.bIsCorrect = true; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		ssTemp << (int)eTemp.vecAnswers[0].fValue;
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		
		eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		
		//wrong answers
		iTemp = (int)(eTemp.vecValues[0].fValue + eTemp.vecValues[1].fValue);
		if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << iTemp;
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
				eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
			};
		};
		
		iTemp = (int)(eTemp.vecValues[1].fValue * (eTemp.vecValues[0].fValue + 1));
		if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << iTemp;
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
				eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
			};
		};
		
		iTemp = (int)(eTemp.vecValues[0].fValue * (eTemp.vecValues[1].fValue + 1));
		if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << iTemp;
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
				eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
			};
		};
		
		iTemp = (int)(eTemp.vecValues[1].fValue * (eTemp.vecValues[0].fValue - 1));
		if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << iTemp;
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
				eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
			};
		};
		
		iTemp = (int)(eTemp.vecValues[0].fValue * (eTemp.vecValues[1].fValue - 1));
		if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << iTemp;
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
				eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
			};
		};
		
		iTemp = (int)(eTemp.vecValues[0].fValue * eTemp.vecValues[1].fValue) + 1;
		if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << iTemp;
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
				eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
			};
		};
		
		iTemp = (int)(eTemp.vecValues[0].fValue * eTemp.vecValues[0].fValue);
		if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << iTemp;
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
				eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
			};
		};
		
		iTemp = (int)(eTemp.vecValues[1].fValue * eTemp.vecValues[1].fValue);
		if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << iTemp;
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
				eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
			};
		};
		
		
		
		
		
		
		
		
		iTemp = modMisc::iRandomNumber(eTemp.vecValues[0].fValue, eTemp.vecValues[1].fValue) * eTemp.vecValues[1].fValue;
		if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << iTemp;
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
				eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
			};
		};
		
		iTemp = modMisc::iRandomNumber(eTemp.vecValues[0].fValue, eTemp.vecValues[1].fValue) * eTemp.vecValues[1].fValue;
		if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << iTemp;
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
				eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
			};
		};
		
		iTemp = modMisc::iRandomNumber(eTemp.vecValues[0].fValue, eTemp.vecValues[1].fValue) * eTemp.vecValues[0].fValue;
		if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << iTemp;
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
				eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
			};
		};
		
		iTemp = modMisc::iRandomNumber(eTemp.vecValues[0].fValue, eTemp.vecValues[1].fValue) * eTemp.vecValues[1].fValue;
		if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << iTemp;
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
				eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
			};
		};
	} else {
		//NOT multiple choice
		eTemp.bIsMultipleChoice = false;
	};
	
	
//	strFloatValue fRandomFigure;
//	strQuestion eTemp;
//	int iTemp;
//	strStringAnswer sMultipleChoiceAnswer; 
//	vector<strStringAnswer>::iterator itMultipleChoiceAnswer; 
//	std::stringstream ssTemp;
//	enumFnTrig eTempFnTrig;
//	int iDifficultyGroupLevel;
	sGroupName.clear();
//	time_t tmStart;
	
	return eTemp;
};

strQuestion ClsQuiz::randomQuestionType_DivideTwoNumbers(){
	strFloatValue fRandomFigure;
	strQuestion eTemp;
	int iTemp;
	strStringAnswer sMultipleChoiceAnswer; 
	vector<strStringAnswer>::iterator itMultipleChoiceAnswer; 
	std::stringstream ssTemp;
	int iDifficultyGroupLevel;
	std::string sGroupName;
	time_t tmStart;
	time_t tmEnd;
	
	tmStart = time (NULL);
	
	eTemp.eQuestionGroup = eQuesGrpArithmeticAdvanced;
	eTemp.bHasPic = false;
	sGroupName = modMisc::convertEnum_QuesTypeGroupText(eQuesGrpArithmeticAdvanced);
	eTemp.ePic = ePic_None;
	
	eTemp.vecValues.clear();
	eTemp.vecAnswers.clear();
	eTemp.vecChoices.clear();
	
	const int gciQuestionTypeMax = 3; //note: needs to be the maximum integer in the enumQuestionType so that we can generate random question types 
	
	if (cPreferences->getType() == ePrefByGroup or cPreferences->getType() == ePrefBySubject) {
		iDifficultyGroupLevel = (int)cPreferences->getDifficulty(&sGroupName);
	} else {
		iDifficultyGroupLevel = (int)modMisc::iRandomNumber(1, gciQuestionTypeMax);
	};
	
	
	eTemp.sQuestionEncoded = "<1> / <0> =";
	eTemp.eAnswerType = eAnsTypNumeric;
	
	
	
	switch (iDifficultyGroupLevel){ 
		case 1:
			eTemp.eQuestionType = eQuesDividingNumbersEasy;
			eTemp.iDifficulty = 175;
			
			fRandomFigure.fValue = (float)modMisc::iRandomNumber(2, 6);
			eTemp.vecValues.push_back(fRandomFigure);
			fRandomFigure.fValue = fRandomFigure.fValue * (float)modMisc::iRandomNumber(2, 7);
			eTemp.vecValues.push_back(fRandomFigure);
			
			break;
		case 2:
			eTemp.eQuestionType = eQuesDividingNumbersMedium;
			eTemp.iDifficulty = 250;
			
			fRandomFigure.fValue = (float)modMisc::iRandomNumber(5, 12);
			eTemp.vecValues.push_back(fRandomFigure);
			fRandomFigure.fValue = fRandomFigure.fValue * (float)modMisc::iRandomNumber(6, 12);
			eTemp.vecValues.push_back(fRandomFigure);
			break;
		case 3:
			eTemp.eQuestionType = eQuesDividingNumbersHard;
			eTemp.iDifficulty = 325;
			
			fRandomFigure.fValue = (float)modMisc::iRandomNumber(10, 15);
			eTemp.vecValues.push_back(fRandomFigure);
			fRandomFigure.fValue = fRandomFigure.fValue * (float)modMisc::iRandomNumber(10, 15);
			eTemp.vecValues.push_back(fRandomFigure);
			break;
		default:
			eTemp.eQuestionType = eQuesError;
			eTemp.bIsMultipleChoice = false;
			eTemp.sQuestionEncoded = "Error:";
	};
	
	
	fRandomFigure.fValue = (float)(eTemp.vecValues[1].fValue / eTemp.vecValues[0].fValue);
	eTemp.vecAnswers.push_back(fRandomFigure);
	
//	if (modMisc::iRandomNumber(0, 10) < 5) {
	if (modMisc::iRandomNumber(0, 10) < 15) {
		//multiple choice
		
		//Correct answer
		eTemp.bIsMultipleChoice = true;
		eTemp.bOnlyOneAnswer = true;
		sMultipleChoiceAnswer.bIsCorrect = true; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		ssTemp << (int)eTemp.vecAnswers[0].fValue;
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		
		eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		
		//wrong answers
        do {
            for (int iDelta = 1; iDelta < 10; iDelta++) {
                iTemp = (int)(eTemp.vecAnswers[0].fValue + modMisc::iRandomNumberNonZero(-1 * iDelta, iDelta));
                if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
                    sMultipleChoiceAnswer.bIsCorrect = false; 
                    sMultipleChoiceAnswer.bIsSelected = false; 
                    ssTemp.str("");
                    ssTemp << iTemp;
                    sMultipleChoiceAnswer.sValue = ssTemp.str();
                    if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                        eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
                    };
                };
                
                iTemp = (int)(eTemp.vecAnswers[0].fValue + modMisc::iRandomNumberNonZero(-1 * iDelta, iDelta + 1));
                if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
                    sMultipleChoiceAnswer.bIsCorrect = false; 
                    sMultipleChoiceAnswer.bIsSelected = false; 
                    ssTemp.str("");
                    ssTemp << iTemp;
                    sMultipleChoiceAnswer.sValue = ssTemp.str();
                    if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                        eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
                    };
                };
                
                iTemp = (int)(eTemp.vecAnswers[0].fValue + modMisc::iRandomNumberNonZero(-1 * iDelta - 1, iDelta));
                if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
                    sMultipleChoiceAnswer.bIsCorrect = false; 
                    sMultipleChoiceAnswer.bIsSelected = false; 
                    ssTemp.str("");
                    ssTemp << iTemp;
                    sMultipleChoiceAnswer.sValue = ssTemp.str();
                    if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                        eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
                    };
                };
            };
            
            tmEnd = time (NULL);
        } while (eTemp.vecChoices.size() < 8 and tmStart + iCutOffTime < (int)tmEnd);
        

	}else {
		//NOT multiple choice
		eTemp.bIsMultipleChoice = false;
	};
	
	
//	strFloatValue fRandomFigure;
//	strQuestion eTemp;
//	int iTemp;
//	strStringAnswer sMultipleChoiceAnswer; 
//	vector<strStringAnswer>::iterator itMultipleChoiceAnswer; 
//	std::stringstream ssTemp;
//	enumFnTrig eTempFnTrig;
//	int iDifficultyGroupLevel;
	sGroupName.clear();
//	time_t tmStart;
	
	return eTemp;
};

strQuestion ClsQuiz::randomQuestionType_Square(){
	strFloatValue fRandomFigure;
	strQuestion eTemp;
	int iTemp;
	strStringAnswer sMultipleChoiceAnswer; 
	vector<strStringAnswer>::iterator itMultipleChoiceAnswer; 
	std::stringstream ssTemp;
	int iDifficultyGroupLevel;
	std::string sGroupName;
	time_t tmStart;
	time_t tmEnd;
	
	tmStart = time (NULL);
	
	eTemp.eQuestionGroup = eQuesGrpArithmeticAdvanced;
	eTemp.bHasPic = false;
	sGroupName = modMisc::convertEnum_QuesTypeGroupText(eQuesGrpArithmeticAdvanced);
	eTemp.ePic = ePic_None;

	eTemp.vecValues.clear();
	eTemp.vecAnswers.clear();
	eTemp.vecChoices.clear();
	
	const int gciQuestionTypeMax = 3; //note: needs to be the maximum integer in the enumQuestionType so that we can generate random question types 
	
	if (cPreferences->getType() == ePrefByGroup or cPreferences->getType() == ePrefBySubject) {
		iDifficultyGroupLevel = (int)cPreferences->getDifficulty(&sGroupName);
	} else {
		iDifficultyGroupLevel = (int)modMisc::iRandomNumber(1, gciQuestionTypeMax);
	};
	
	
	eTemp.sQuestionEncoded = "<0><^2> =";
	eTemp.eAnswerType = eAnsTypNumeric;
	
	
	
	switch (iDifficultyGroupLevel){ 
		case 1:
			eTemp.eQuestionType = eQuesSquare1Digit;
			eTemp.iDifficulty = 110;
			
			fRandomFigure.fValue = (float)modMisc::iRandomNumber(2, 12);
			eTemp.vecValues.push_back(fRandomFigure);
			break;
		case 2:
			eTemp.eQuestionType = eQuesSquare2Digit;
			eTemp.iDifficulty = 175;
			
			fRandomFigure.fValue = (float)modMisc::iRandomNumber(8, 15);
			eTemp.vecValues.push_back(fRandomFigure);
			break;
		case 3:
			eTemp.eQuestionType = eQuesSquare3Digit;
			eTemp.iDifficulty = 225;
			
			fRandomFigure.fValue = (float)modMisc::iRandomNumber(12, 20);
			eTemp.vecValues.push_back(fRandomFigure);
			break;
		default:
			eTemp.eQuestionType = eQuesError;
			eTemp.bIsMultipleChoice = false;
			eTemp.sQuestionEncoded = "Error:";
	};
	
	
	
	
	fRandomFigure.fValue = (float)(eTemp.vecValues[0].fValue * eTemp.vecValues[0].fValue);
	eTemp.vecAnswers.push_back(fRandomFigure);
	
//	if (modMisc::iRandomNumber(0, 10) < 5) {
	if (modMisc::iRandomNumber(0, 10) < 15) {
		//multiple choice
		
		//Correct answer
		eTemp.bIsMultipleChoice = true;
		eTemp.bOnlyOneAnswer = true;
		sMultipleChoiceAnswer.bIsCorrect = true; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		ssTemp << (int)eTemp.vecAnswers[0].fValue;
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		
		eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		
		//wrong answers
		iTemp = (int)(eTemp.vecAnswers[0].fValue + (3 * modMisc::iRandomNumberNonZero(-1, 1)));
		if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << iTemp;
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
				eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
			};
		};
		
		iTemp = (int)(iTemp + (modMisc::iRandomNumberNonZero(-3, 3) * modMisc::iRandomNumberNonZero(-1, 1)));
		if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << iTemp;
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
				eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
			};
		};
		
		iTemp = (int)(eTemp.vecAnswers[0].fValue + (2 * modMisc::iRandomNumberNonZero(-1, 1)));
		if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << iTemp;
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
				eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
			};
		};
		
		iTemp = (int)(eTemp.vecValues[0].fValue * 2);
		if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << iTemp;
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
				eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
			};
		};
		
		iTemp = (int)(eTemp.vecValues[0].fValue * 10);
		if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << iTemp;
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
				eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
			};
		};
		
		iTemp = (int)(eTemp.vecValues[0].fValue + eTemp.vecValues[0].fValue);
		if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << iTemp;
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
				eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
			};
		};
		
        do {
            for (int iDelta = 1; iDelta < 10; iDelta++) {
                iTemp = (int)(eTemp.vecAnswers[0].fValue + modMisc::iRandomNumberNonZero(-1 * iDelta, iDelta));
                if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
                    sMultipleChoiceAnswer.bIsCorrect = false; 
                    sMultipleChoiceAnswer.bIsSelected = false; 
                    ssTemp.str("");
                    ssTemp << iTemp;
                    sMultipleChoiceAnswer.sValue = ssTemp.str();
                    if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                        eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
                    };
                };
                
                iTemp = (int)(eTemp.vecAnswers[0].fValue + modMisc::iRandomNumberNonZero(-1 * iDelta, iDelta + 1));
                if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
                    sMultipleChoiceAnswer.bIsCorrect = false; 
                    sMultipleChoiceAnswer.bIsSelected = false; 
                    ssTemp.str("");
                    ssTemp << iTemp;
                    sMultipleChoiceAnswer.sValue = ssTemp.str();
                    if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                        eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
                    };
                };
                
                iTemp = (int)(eTemp.vecAnswers[0].fValue + modMisc::iRandomNumberNonZero(-1 * iDelta - 1, iDelta));
                if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
                    sMultipleChoiceAnswer.bIsCorrect = false; 
                    sMultipleChoiceAnswer.bIsSelected = false; 
                    ssTemp.str("");
                    ssTemp << iTemp;
                    sMultipleChoiceAnswer.sValue = ssTemp.str();
                    if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                        eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
                    };
                };
            };
            
            tmEnd = time (NULL);
        } while (eTemp.vecChoices.size() < 8 and tmStart + iCutOffTime < (int)tmEnd);

	} else {
		//NOT multiple choice
		eTemp.bIsMultipleChoice = false;
	};
	
	
//	strFloatValue fRandomFigure;
//	strQuestion eTemp;
//	int iTemp;
//	strStringAnswer sMultipleChoiceAnswer; 
//	vector<strStringAnswer>::iterator itMultipleChoiceAnswer; 
//	std::stringstream ssTemp;
//	enumFnTrig eTempFnTrig;
//	int iDifficultyGroupLevel;
	sGroupName.clear();
//	time_t tmStart;
	
	return eTemp;
};

strQuestion ClsQuiz::randomQuestionType_Cubed(){
	strFloatValue fRandomFigure;
	strQuestion eTemp;
	int iTemp;
	strStringAnswer sMultipleChoiceAnswer; 
	vector<strStringAnswer>::iterator itMultipleChoiceAnswer; 
	std::stringstream ssTemp;
	int iDifficultyGroupLevel;
	std::string sGroupName;
	time_t tmStart;
	time_t tmEnd;
	
	tmStart = time (NULL);
	
	eTemp.eQuestionGroup = eQuesGrpArithmeticAdvanced;
	eTemp.bHasPic = false;
	sGroupName = modMisc::convertEnum_QuesTypeGroupText(eQuesGrpArithmeticAdvanced);
	eTemp.ePic = ePic_None;
	
	eTemp.vecValues.clear();
	eTemp.vecAnswers.clear();
	eTemp.vecChoices.clear();
	
	const int gciQuestionTypeMax = 3; //note: needs to be the maximum integer in the enumQuestionType so that we can generate random question types 
	
	if (cPreferences->getType() == ePrefByGroup or cPreferences->getType() == ePrefBySubject) {
		iDifficultyGroupLevel = (int)cPreferences->getDifficulty(&sGroupName);
	} else {
		iDifficultyGroupLevel = (int)modMisc::iRandomNumber(1, gciQuestionTypeMax);
	};
	
	
	eTemp.sQuestionEncoded = "<0><^3> =";
	eTemp.eAnswerType = eAnsTypNumeric;
	
	
	
	switch (iDifficultyGroupLevel){ 
		case 1:
			eTemp.eQuestionType = eQuesCubed1Digit;
			eTemp.iDifficulty = 150;
			
			fRandomFigure.fValue = (float)modMisc::iRandomNumber(2, 9);
			eTemp.vecValues.push_back(fRandomFigure);
			break;
		case 2:
			eTemp.eQuestionType = eQuesCubed2Digit;
			eTemp.iDifficulty = 200;
			
			fRandomFigure.fValue = (float)modMisc::iRandomNumber(6, 12);
			eTemp.vecValues.push_back(fRandomFigure);
			break;
		case 3:
			eTemp.eQuestionType = eQuesCubed3Digit;
			eTemp.iDifficulty = 250;
			
			fRandomFigure.fValue = (float)modMisc::iRandomNumber(9, 16);
			eTemp.vecValues.push_back(fRandomFigure);
			break;
		default:
			eTemp.eQuestionType = eQuesError;
			eTemp.bIsMultipleChoice = false;
			eTemp.sQuestionEncoded = "Error:";
	};
	
	
	
	
	fRandomFigure.fValue = (float)(eTemp.vecValues[0].fValue * eTemp.vecValues[0].fValue * eTemp.vecValues[0].fValue);
	eTemp.vecAnswers.push_back(fRandomFigure);
	
//	if (modMisc::iRandomNumber(0, 10) < 5) {
	if (modMisc::iRandomNumber(0, 10) < 15) {
		//multiple choice
		
		//Correct answer
		eTemp.bIsMultipleChoice = true;
		eTemp.bOnlyOneAnswer = true;
		sMultipleChoiceAnswer.bIsCorrect = true; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		ssTemp << (int)eTemp.vecAnswers[0].fValue;
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		
		eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		
		//wrong answers
		iTemp = (int)(eTemp.vecAnswers[0].fValue + (3 * modMisc::iRandomNumberNonZero(-1, 1)));
		if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << iTemp;
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
				eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
			};
		};
		
		iTemp = (int)(iTemp + (modMisc::iRandomNumberNonZero(-3, 3) * modMisc::iRandomNumberNonZero(-1, 1)));
		if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << iTemp;
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
				eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
			};
		};
		
		iTemp = (int)(eTemp.vecAnswers[0].fValue + (2 * modMisc::iRandomNumberNonZero(-1, 1)));
		if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << iTemp;
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
				eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
			};
		};
		
		iTemp = (int)(eTemp.vecValues[0].fValue * eTemp.vecValues[0].fValue * 2);
		if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << iTemp;
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
				eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
			};
		};
		
		iTemp = (int)(eTemp.vecValues[0].fValue * eTemp.vecValues[0].fValue);
		if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << iTemp;
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
				eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
			};
		};
		
		iTemp = (int)(eTemp.vecValues[0].fValue * eTemp.vecValues[0].fValue * 10);
		if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << iTemp;
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
				eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
			};
		};
		
		iTemp = (int)(eTemp.vecValues[0].fValue + eTemp.vecValues[0].fValue);
		if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << iTemp;
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
				eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
			};
		};
		
		do {
            iTemp = (int)(eTemp.vecAnswers[0].fValue + modMisc::iRandomNumberNonZero(-5, 5));
            if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
                sMultipleChoiceAnswer.bIsCorrect = false; 
                sMultipleChoiceAnswer.bIsSelected = false; 
                ssTemp.str("");
                ssTemp << iTemp;
                sMultipleChoiceAnswer.sValue = ssTemp.str();
                if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                    eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
                };
            };
            
            iTemp = (int)(eTemp.vecAnswers[0].fValue + modMisc::iRandomNumberNonZero(-3, 4));
            if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
                sMultipleChoiceAnswer.bIsCorrect = false; 
                sMultipleChoiceAnswer.bIsSelected = false; 
                ssTemp.str("");
                ssTemp << iTemp;
                sMultipleChoiceAnswer.sValue = ssTemp.str();
                if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                    eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
                };
            };
            iTemp = (int)(eTemp.vecAnswers[0].fValue + modMisc::iRandomNumberNonZero(-4, 3));
            if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
                sMultipleChoiceAnswer.bIsCorrect = false; 
                sMultipleChoiceAnswer.bIsSelected = false; 
                ssTemp.str("");
                ssTemp << iTemp;
                sMultipleChoiceAnswer.sValue = ssTemp.str();
                if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                    eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
                };
            };
            
           
			iTemp = (int)(eTemp.vecValues[0].fValue * eTemp.vecValues[0].fValue * (eTemp.vecValues[0].fValue + modMisc::iRandomNumberNonZero(-10, 10)));
			if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
				sMultipleChoiceAnswer.bIsCorrect = false; 
				sMultipleChoiceAnswer.bIsSelected = false; 
				ssTemp.str("");
				ssTemp << iTemp;
				sMultipleChoiceAnswer.sValue = ssTemp.str();
				if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
					eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
				};
			};

            tmEnd = time (NULL);
		} while (eTemp.vecChoices.size() < 8 and tmStart + iCutOffTime < (int)tmEnd);
		
	}else {
		//NOT multiple choice
		eTemp.bIsMultipleChoice = false;
	};
	
	
//	strFloatValue fRandomFigure;
//	strQuestion eTemp;
//	int iTemp;
//	strStringAnswer sMultipleChoiceAnswer; 
//	vector<strStringAnswer>::iterator itMultipleChoiceAnswer; 
//	std::stringstream ssTemp;
//	enumFnTrig eTempFnTrig;
//	int iDifficultyGroupLevel;
	sGroupName.clear();
//	time_t tmStart;
	
	return eTemp;
};

strQuestion ClsQuiz::randomQuestionType_SquareRoot(){
	strFloatValue fRandomFigure;
	strQuestion eTemp;
	int iTemp;
	strStringAnswer sMultipleChoiceAnswer; 
	vector<strStringAnswer>::iterator itMultipleChoiceAnswer; 
	std::stringstream ssTemp;
	int iDifficultyGroupLevel;
	std::string sGroupName;
	time_t tmStart;
	time_t tmEnd;
	
	tmStart = time (NULL);
	
	eTemp.eQuestionGroup = eQuesGrpArithmeticAdvanced;
	eTemp.bHasPic = false;
	sGroupName = modMisc::convertEnum_QuesTypeGroupText(eQuesGrpArithmeticAdvanced);
	eTemp.ePic = ePic_None;

	eTemp.vecValues.clear();
	eTemp.vecAnswers.clear();
	eTemp.vecChoices.clear();
	
	const int gciQuestionTypeMax = 3; //note: needs to be the maximum integer in the enumQuestionType so that we can generate random question types 
	
	if (cPreferences->getType() == ePrefByGroup or cPreferences->getType() == ePrefBySubject) {
		iDifficultyGroupLevel = (int)cPreferences->getDifficulty(&sGroupName);
	} else {
		iDifficultyGroupLevel = (int)modMisc::iRandomNumber(1, gciQuestionTypeMax);
	};
	
	
	eTemp.sQuestionEncoded = "<^1/2><0>=";
	eTemp.eAnswerType = eAnsTypNumeric;
	
	
	
	switch (iDifficultyGroupLevel){ 
		case 1:
			eTemp.eQuestionType = eQuesSquare1Digit;
			eTemp.iDifficulty = 220;
			
			fRandomFigure.fValue = (float)modMisc::iRandomNumber(1, 8);
			eTemp.vecAnswers.push_back(fRandomFigure);
			break;
		case 2:
			eTemp.eQuestionType = eQuesSquare2Digit;
			eTemp.iDifficulty = 295;
			
			fRandomFigure.fValue = (float)modMisc::iRandomNumber(6, 12);
			eTemp.vecAnswers.push_back(fRandomFigure);
			break;
		case 3:
			eTemp.eQuestionType = eQuesSquare3Digit;
			eTemp.iDifficulty = 350;
			
			fRandomFigure.fValue = (float)modMisc::iRandomNumber(9, 15);
			eTemp.vecAnswers.push_back(fRandomFigure);
			break;
		default:
			eTemp.eQuestionType = eQuesError;
			eTemp.bIsMultipleChoice = false;
			eTemp.sQuestionEncoded = "Error:";
	};
	
	
	
	
	fRandomFigure.fValue = (float)(eTemp.vecAnswers[0].fValue * eTemp.vecAnswers[0].fValue);
	eTemp.vecValues.push_back(fRandomFigure);
	
//	if (modMisc::iRandomNumber(0, 10) < 5) {
	if (modMisc::iRandomNumber(0, 10) < 15) {
		//multiple choice
		
		//Correct answer
		eTemp.bIsMultipleChoice = true;
		eTemp.bOnlyOneAnswer = true;
		sMultipleChoiceAnswer.bIsCorrect = true; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		ssTemp << (int)eTemp.vecAnswers[0].fValue;
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		
		eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		
		//wrong answers
		iTemp = (int)(eTemp.vecAnswers[0].fValue + (3 * modMisc::iRandomNumberNonZero(-1, 1)));
		if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << iTemp;
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
				eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
			};
		};
		
		iTemp = (int)(iTemp + (modMisc::iRandomNumberNonZero(-3, 3) * modMisc::iRandomNumberNonZero(-1, 1)));
		if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << iTemp;
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
				eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
			};
		};
		
		iTemp = (int)(eTemp.vecAnswers[0].fValue + (2 * modMisc::iRandomNumberNonZero(-1, 1)));
		if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << iTemp;
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
				eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
			};
		};
		
		iTemp = (int)(eTemp.vecAnswers[0].fValue * 2);
		if (iTemp != (int)eTemp.vecAnswers[0].fValue and iTemp != (int)(-1 * eTemp.vecAnswers[0].fValue)) {
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << iTemp;
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
				eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
			};
		};
		
		do {
            iTemp = (int)(eTemp.vecAnswers[0].fValue + modMisc::iRandomNumberNonZero(-5, 5));
            if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
                sMultipleChoiceAnswer.bIsCorrect = false; 
                sMultipleChoiceAnswer.bIsSelected = false; 
                ssTemp.str("");
                ssTemp << iTemp;
                sMultipleChoiceAnswer.sValue = ssTemp.str();
                if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                    eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
                };
            };
            
            iTemp = (int)(eTemp.vecAnswers[0].fValue + modMisc::iRandomNumberNonZero(-3, 4));
            if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
                sMultipleChoiceAnswer.bIsCorrect = false; 
                sMultipleChoiceAnswer.bIsSelected = false; 
                ssTemp.str("");
                ssTemp << iTemp;
                sMultipleChoiceAnswer.sValue = ssTemp.str();
                if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                    eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
                };
            };
            
            iTemp = (int)(eTemp.vecAnswers[0].fValue + modMisc::iRandomNumberNonZero(-4, 3));
            if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
                sMultipleChoiceAnswer.bIsCorrect = false; 
                sMultipleChoiceAnswer.bIsSelected = false; 
                ssTemp.str("");
                ssTemp << iTemp;
                sMultipleChoiceAnswer.sValue = ssTemp.str();
                if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                    eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
                };
            };
            

			iTemp = (int)(eTemp.vecAnswers[0].fValue * (eTemp.vecAnswers[0].fValue + modMisc::iRandomNumberNonZero(-10, 10)));
			if (iTemp != (int)eTemp.vecAnswers[0].fValue and iTemp != (int)(-1 * eTemp.vecAnswers[0].fValue)) {
				sMultipleChoiceAnswer.bIsCorrect = false; 
				sMultipleChoiceAnswer.bIsSelected = false; 
				ssTemp.str("");
				ssTemp << iTemp;
				sMultipleChoiceAnswer.sValue = ssTemp.str();
				if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
					eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
				};
			};

            tmEnd = time (NULL);
		} while (eTemp.vecChoices.size() < 8 and (int)tmStart + iCutOffTime < (int)tmEnd);
		
	} else {
		//NOT multiple choice
		eTemp.bIsMultipleChoice = false;
	};
	
	
//	strFloatValue fRandomFigure;
//	strQuestion eTemp;
//	int iTemp;
//	strStringAnswer sMultipleChoiceAnswer; 
//	vector<strStringAnswer>::iterator itMultipleChoiceAnswer; 
//	std::stringstream ssTemp;
//	enumFnTrig eTempFnTrig;
//	int iDifficultyGroupLevel;
	sGroupName.clear();
//	time_t tmStart;
	return eTemp;
};

strQuestion ClsQuiz::randomQuestionType_CubedRoot(){
	strFloatValue fRandomFigure;
	strQuestion eTemp;
	int iTemp;
	strStringAnswer sMultipleChoiceAnswer; 
	vector<strStringAnswer>::iterator itMultipleChoiceAnswer; 
	std::stringstream ssTemp;
	bool bIsNegative;
	int iDiff;
	std::string sGroupName;
	time_t tmStart;
	time_t tmEnd;
	
	tmStart = time (NULL);
	
	eTemp.eQuestionGroup = eQuesGrpArithmeticAdvanced;
	eTemp.bHasPic = false;
	sGroupName = modMisc::convertEnum_QuesTypeGroupText(eQuesGrpArithmeticAdvanced);
	eTemp.ePic = ePic_None;

	eTemp.vecValues.clear();
	eTemp.vecAnswers.clear();
	eTemp.vecChoices.clear();
	
	const int gciQuestionTypeMax = 3; //note: needs to be the maximum integer in the enumQuestionType so that we can generate random question types 
	
	if (cPreferences->getType() == ePrefByGroup or cPreferences->getType() == ePrefBySubject) {
		iDiff = (int)cPreferences->getDifficulty(&sGroupName);
	} else {
		iDiff = (int)modMisc::iRandomNumber(1, gciQuestionTypeMax);
	};
	
	
	eTemp.sQuestionEncoded = "<^1/3><0>=";
	eTemp.eAnswerType = eAnsTypNumeric;
	
	if (modMisc::iRandomNumber(1, 3) == 1) {
		bIsNegative = false;
	} else {
		bIsNegative = true;
	}
	
	switch (iDiff){ 
		case 1:
			eTemp.eQuestionType = eQuesCubed1Digit;
			eTemp.iDifficulty = 320;
			
			if (bIsNegative == true) {
				fRandomFigure.fValue = (float)modMisc::iRandomNumber(1, 6) * -1;
			} else {
				fRandomFigure.fValue = (float)modMisc::iRandomNumber(1, 6);
			};
			eTemp.vecAnswers.push_back(fRandomFigure);
			break;
		case 2:
			eTemp.eQuestionType = eQuesCubed2Digit;
			eTemp.iDifficulty = 380;
			
			if (bIsNegative == true) {
				fRandomFigure.fValue = (float)modMisc::iRandomNumber(4, 9) * -1;
			} else {
				fRandomFigure.fValue = (float)modMisc::iRandomNumber(4, 9);
			};
			eTemp.vecAnswers.push_back(fRandomFigure);
			break;
		case 3:
			eTemp.eQuestionType = eQuesCubed3Digit;
			eTemp.iDifficulty = 460;
			
			if (bIsNegative == true) {
				fRandomFigure.fValue = (float)modMisc::iRandomNumber(7, 12) * -1;
			} else {
				fRandomFigure.fValue = (float)modMisc::iRandomNumber(7, 12);
			};
			eTemp.vecAnswers.push_back(fRandomFigure);
			break;
		default:
			eTemp.eQuestionType = eQuesError;
			eTemp.bIsMultipleChoice = false;
			eTemp.sQuestionEncoded = "Error:";
	};
	
	
	
	
	fRandomFigure.fValue = (float)(eTemp.vecAnswers[0].fValue * eTemp.vecAnswers[0].fValue * eTemp.vecAnswers[0].fValue);
	eTemp.vecValues.push_back(fRandomFigure);
	
//	if (modMisc::iRandomNumber(0, 10) < 5) {
	if (modMisc::iRandomNumber(0, 10) < 15) {
		//multiple choice
		
		//Correct answer
		eTemp.bIsMultipleChoice = true;
		eTemp.bOnlyOneAnswer = true;
		sMultipleChoiceAnswer.bIsCorrect = true; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		ssTemp << (int)eTemp.vecAnswers[0].fValue;
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		
		eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		
		//wrong answers
		do {
            iTemp = (int)(eTemp.vecAnswers[0].fValue + modMisc::iRandomNumberNonZero(-5, 5));
            if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
                sMultipleChoiceAnswer.bIsCorrect = false; 
                sMultipleChoiceAnswer.bIsSelected = false; 
                ssTemp.str("");
                ssTemp << iTemp;
                sMultipleChoiceAnswer.sValue = ssTemp.str();
                if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                    eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
                };
            };
            
            iTemp = (int)(eTemp.vecAnswers[0].fValue + modMisc::iRandomNumberNonZero(-3, 4));
            if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
                sMultipleChoiceAnswer.bIsCorrect = false; 
                sMultipleChoiceAnswer.bIsSelected = false; 
                ssTemp.str("");
                ssTemp << iTemp;
                sMultipleChoiceAnswer.sValue = ssTemp.str();
                if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                    eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
                };
            };
            
            iTemp = (int)(eTemp.vecAnswers[0].fValue + modMisc::iRandomNumberNonZero(-4, 3));
            if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
                sMultipleChoiceAnswer.bIsCorrect = false; 
                sMultipleChoiceAnswer.bIsSelected = false; 
                ssTemp.str("");
                ssTemp << iTemp;
                sMultipleChoiceAnswer.sValue = ssTemp.str();
                if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                    eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
                };
            };
            
            iTemp = (int)(eTemp.vecAnswers[0].fValue + modMisc::iRandomNumberNonZero(-3, 3));
            if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
                sMultipleChoiceAnswer.bIsCorrect = false; 
                sMultipleChoiceAnswer.bIsSelected = false; 
                ssTemp.str("");
                ssTemp << iTemp;
                sMultipleChoiceAnswer.sValue = ssTemp.str();
                if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                    eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
                };
            };
            
			iTemp = (int)(eTemp.vecAnswers[0].fValue * eTemp.vecAnswers[0].fValue * (eTemp.vecAnswers[0].fValue + modMisc::iRandomNumberNonZero(-10, 10)));
			if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
				sMultipleChoiceAnswer.bIsCorrect = false; 
				sMultipleChoiceAnswer.bIsSelected = false; 
				ssTemp.str("");
				ssTemp << iTemp;
				sMultipleChoiceAnswer.sValue = ssTemp.str();
				if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
					eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
				};
			};

            tmEnd = time (NULL);
		} while (eTemp.vecChoices.size() < 8 and (int)tmStart + iCutOffTime < (int)tmEnd);
		
	} else {
		//NOT multiple choice
		eTemp.bIsMultipleChoice = false;
	};
	
	
//	strFloatValue fRandomFigure;
//	strQuestion eTemp;
//	int iTemp;
//	strStringAnswer sMultipleChoiceAnswer; 
//	vector<strStringAnswer>::iterator itMultipleChoiceAnswer; 
//	std::stringstream ssTemp;
//	enumFnTrig eTempFnTrig;
//	bool bIsNegative;
//	int iDiff;
	sGroupName.clear();
//	time_t tmStart;
	
	return eTemp;
};


strQuestion ClsQuiz::randomQuestionType_Pythagorean(){
	strFloatValue fRandomFigure;
	strQuestion eTemp;
	int iTemp;
	strStringAnswer sMultipleChoiceAnswer; 
	vector<strStringAnswer>::iterator itMultipleChoiceAnswer; 
	std::stringstream ssTemp;
	bool bIsNegative;
	int iSideToGuess;
	strTrigNumbers eTrigNo;
	int iDifficultyGroupLevel;
	std::string sGroupName;
	time_t tmStart;
	time_t tmEnd;
	
	tmStart = time (NULL);
	
	eTemp.eQuestionGroup = eQuesGrpShapes;
	sGroupName = modMisc::convertEnum_QuesTypeGroupText(eQuesGrpShapes);
	
	eTemp.bHasPic = true;
	eTemp.ePic = ePicQues_TriangleRightAngle;
	
	eTemp.vecValues.clear();
	eTemp.vecAnswers.clear();
	eTemp.vecChoices.clear();
	
	
	const int gciQuestionTypeMax = 3; //note: needs to be the maximum integer in the enumQuestionType so that we can generate random question types 
	iSideToGuess = (int)modMisc::iRandomNumber(1, 3);
	//1 = Hyp
	//2 = Opp
	//3 = Adj
    
	if (cPreferences->getType() == ePrefByGroup or cPreferences->getType() == ePrefBySubject) {
		iDifficultyGroupLevel = (int)cPreferences->getDifficulty(&sGroupName);
	} else {
		iDifficultyGroupLevel = (int)modMisc::iRandomNumber(1, gciQuestionTypeMax);
	};
	
	eTemp.eAnswerType = eAnsTypNumeric;
	
	if (modMisc::iRandomNumber(1, 3) == 1) {
		bIsNegative = false;
	} else {
		bIsNegative = true;
	};
	
	switch (iDifficultyGroupLevel){ 
		case 1:
			eTemp.eQuestionType = eQuesPythagoreanVeryEasy;
			eTemp.iDifficulty = 605;
			
			eTrigNo = modMisc::friendlyTrigNumbers(1);
			break;
		case 2:
			eTemp.eQuestionType = eQuesPythagoreanEasy;
			eTemp.iDifficulty = 655;
			
			eTrigNo = modMisc::friendlyTrigNumbers(2);
			break;
		case 3:
			eTemp.eQuestionType = eQuesPythagoreanMedium;
			eTemp.iDifficulty = 715;
			
			eTrigNo = modMisc::friendlyTrigNumbers(3);
			break;
		case 4:
			eTemp.eQuestionType = eQuesPythagoreanHard;
			eTemp.iDifficulty = 785;
			
			eTrigNo = modMisc::friendlyTrigNumbers(4);
			break;
		case 5:
			eTemp.eQuestionType = eQuesPythagoreanVeryHard;
			eTemp.iDifficulty = 855;
			
			eTrigNo = modMisc::friendlyTrigNumbers(5);
			break;
		default:
			eTemp.eQuestionType = eQuesError;
			eTemp.bIsMultipleChoice = false;
			eTemp.sQuestionEncoded = "Error:";
	};
	
	
	switch (iSideToGuess) {
		case 1:
			eTemp.sQuestionEncoded = "Opp = <0>, Adj = <1>\n What is the Hyp?";
            
			fRandomFigure.fValue = (float)eTrigNo.fOpposite;
			eTemp.vecValues.push_back(fRandomFigure);
			fRandomFigure.fValue = (float)eTrigNo.fAdjacent;
			eTemp.vecValues.push_back(fRandomFigure);
			fRandomFigure.fValue = (float)eTrigNo.fHypotenuse;
			eTemp.vecAnswers.push_back(fRandomFigure);
            
			break;
		case 2:
			eTemp.sQuestionEncoded = "Hyp = <0>, Adj = <1>\n What is the Opp?";
            
			fRandomFigure.fValue = (float)eTrigNo.fHypotenuse;
			eTemp.vecValues.push_back(fRandomFigure);
			fRandomFigure.fValue = (float)eTrigNo.fAdjacent;
			eTemp.vecValues.push_back(fRandomFigure);
			fRandomFigure.fValue = (float)eTrigNo.fOpposite;
			eTemp.vecAnswers.push_back(fRandomFigure);
			break;
		case 3:
			eTemp.sQuestionEncoded = "Opp = <0>, Hyp = <1>\n What is the Adj?";
            
			fRandomFigure.fValue = (float)eTrigNo.fOpposite;
			eTemp.vecValues.push_back(fRandomFigure);
			fRandomFigure.fValue = (float)eTrigNo.fHypotenuse;
			eTemp.vecValues.push_back(fRandomFigure);
			fRandomFigure.fValue = (float)eTrigNo.fAdjacent;
			eTemp.vecAnswers.push_back(fRandomFigure);
			break;
		default:
			break;
	};
	
	
	
    //	if (modMisc::iRandomNumber(0, 10) < 5) {
	if (modMisc::iRandomNumber(0, 10) < 15) {
		//multiple choice
		
		//Correct answer
		eTemp.bIsMultipleChoice = true;
		eTemp.bOnlyOneAnswer = true;
		sMultipleChoiceAnswer.bIsCorrect = true; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		ssTemp << (int)eTemp.vecAnswers[0].fValue;
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		
		eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		
		//wrong answers
		iTemp = (int)(eTemp.vecValues[0].fValue * eTemp.vecValues[1].fValue);
		if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << iTemp;
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
				eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
			};
		};
        
		iTemp = (int)((eTemp.vecValues[0].fValue + eTemp.vecValues[1].fValue) * 2);
		if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << iTemp;
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
				eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
			};
		};
        
		do {
            iTemp = (int)eTemp.vecAnswers[0].fValue + modMisc::iRandomNumberNonZero(-1, 1);
            if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
                sMultipleChoiceAnswer.bIsCorrect = false; 
                sMultipleChoiceAnswer.bIsSelected = false; 
                ssTemp.str("");
                ssTemp << iTemp;
                sMultipleChoiceAnswer.sValue = ssTemp.str();
                if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                    eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
                };
            };
            
            iTemp = (int)eTemp.vecAnswers[0].fValue + modMisc::iRandomNumberNonZero(-1, 2);
            if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
                sMultipleChoiceAnswer.bIsCorrect = false; 
                sMultipleChoiceAnswer.bIsSelected = false; 
                ssTemp.str("");
                ssTemp << iTemp;
                sMultipleChoiceAnswer.sValue = ssTemp.str();
                if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                    eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
                };
            };
            
            iTemp = (int)eTemp.vecAnswers[0].fValue + modMisc::iRandomNumberNonZero(-2, 1);
            if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
                sMultipleChoiceAnswer.bIsCorrect = false; 
                sMultipleChoiceAnswer.bIsSelected = false; 
                ssTemp.str("");
                ssTemp << iTemp;
                sMultipleChoiceAnswer.sValue = ssTemp.str();
                if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                    eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
                };
            };
            
            iTemp = (int)eTemp.vecAnswers[0].fValue + modMisc::iRandomNumberNonZero(-2, 2);
            if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
                sMultipleChoiceAnswer.bIsCorrect = false; 
                sMultipleChoiceAnswer.bIsSelected = false; 
                ssTemp.str("");
                ssTemp << iTemp;
                sMultipleChoiceAnswer.sValue = ssTemp.str();
                if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                    eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
                };
            };
            
            
			iTemp = (int)(eTemp.vecAnswers[0].fValue + modMisc::iRandomNumber(1, (int)eTemp.vecValues[1].fValue));
			if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
				sMultipleChoiceAnswer.bIsCorrect = false; 
				sMultipleChoiceAnswer.bIsSelected = false; 
				ssTemp.str("");
				ssTemp << iTemp;
				sMultipleChoiceAnswer.sValue = ssTemp.str();
				if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
					eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
				};
			};
			
			iTemp = (int)(eTemp.vecValues[1].fValue + modMisc::iRandomNumber(1, (int)eTemp.vecValues[0].fValue));
			if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
				sMultipleChoiceAnswer.bIsCorrect = false; 
				sMultipleChoiceAnswer.bIsSelected = false; 
				ssTemp.str("");
				ssTemp << iTemp;
				sMultipleChoiceAnswer.sValue = ssTemp.str();
				if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
					eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
				};
			};
			
			if ((int)(fabs(eTemp.vecAnswers[0].fValue) + fabs(eTemp.vecValues[0].fValue) + fabs(eTemp.vecValues[1].fValue)) < 20) {
				iTemp = (int)modMisc::iRandomNumber(0, 20);
				if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
					sMultipleChoiceAnswer.bIsCorrect = false; 
					sMultipleChoiceAnswer.bIsSelected = false; 
					ssTemp.str("");
					ssTemp << iTemp;
					sMultipleChoiceAnswer.sValue = ssTemp.str();
					if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
						eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
					};
				};
			} else {
				iTemp = (int)modMisc::iRandomNumber(0, (int)(fabs(eTemp.vecAnswers[0].fValue) + fabs(eTemp.vecValues[0].fValue) + fabs(eTemp.vecValues[1].fValue)));
				if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
					sMultipleChoiceAnswer.bIsCorrect = false; 
					sMultipleChoiceAnswer.bIsSelected = false; 
					ssTemp.str("");
					ssTemp << iTemp;
					sMultipleChoiceAnswer.sValue = ssTemp.str();
					if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
						eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
					};
				};
			};
            
            tmEnd = time (NULL);
		} while (eTemp.vecChoices.size() < 9 and (int)tmStart + iCutOffTime < (int)tmEnd);
		
	}else {
		//NOT multiple choice
		eTemp.bIsMultipleChoice = false;
	};
	
    
    //	strFloatValue fRandomFigure;
    //	strQuestion eTemp;
    //	int iTemp;
    //	strStringAnswer sMultipleChoiceAnswer; 
    //	vector<strStringAnswer>::iterator itMultipleChoiceAnswer; 
    //	std::stringstream ssTemp;
    //	enumFnTrig eTempFnTrig;
    //	bool bIsNegative;
    //	int iSideToGuess;
    //	strTrigNumbers eTrigNo;
    //	int iDifficultyGroupLevel;
	sGroupName.clear();
    //	time_t tmStart;
    
	return eTemp;
};

strQuestion ClsQuiz::randomQuestionType_Trigonometry(){
	strFloatValue fRandomFigure;
	strQuestion eTemp;
	int iTemp;
	strStringAnswer sMultipleChoiceAnswer; 
	vector<strStringAnswer>::iterator itMultipleChoiceAnswer; 
	std::stringstream ssTemp;
	bool bIsNegative;
	int iSideToGuess;
	int iRangeMax;
	int iRangeMin;
    int iOpp;
    int iHyp;
    int iAdj;
    int iAngle;
    int iCommon;
	int iDifficultyGroupLevel;
    float fAnswer;
    float fAnswerTemp;
    float fPi = 2 * acos(0);
    const float fAnswerAccuracy = 0.001;
    const float fAnswerAccuracyAngle = 0.01;
	std::string sGroupName;
    std::string sAnswerPrefix;
	time_t tmStart;
	time_t tmEnd;
	
	tmStart = time (NULL);
	
	eTemp.eQuestionGroup = eQuesGrpShapes;
	sGroupName = modMisc::convertEnum_QuesTypeGroupText(eQuesGrpShapes);
	
	eTemp.bHasPic = true;
	eTemp.ePic = ePicQues_TriangleRightAngle;
	
	eTemp.vecValues.clear();
	eTemp.vecAnswers.clear();
	eTemp.vecChoices.clear();
	
    if (cPrimeNumbers == NULL) {
		cPrimeNumbers = new ClsPrimeNumbers;
	};

	const int gciQuestionTypeMax = 9; //note: needs to be the maximum integer in the enumQuestionType so that we can generate random question types 
	iSideToGuess = (int)modMisc::iRandomNumber(1, 9);
	//1 SinA = Opp / Hyp
	//2 Opp = Hyp * SinA
	//3 Hyp = Opp / Sin A
    
    //4 CosA = Adj / Hyp
    //5 Adj = Hyp * CosA
    //6 Hyp = Adj / CosA
    
    //7 TanA = Opp / Adj
    //8 Opp = Adj * TanA
    //9 Adj = Opp / TanA
    
	if (cPreferences->getType() == ePrefByGroup or cPreferences->getType() == ePrefBySubject) {
		iDifficultyGroupLevel = (int)cPreferences->getDifficulty(&sGroupName);
	} else {
		iDifficultyGroupLevel = (int)modMisc::iRandomNumber(1, gciQuestionTypeMax);
	};
	
	eTemp.eAnswerType = eAnsTypNumeric;
	
	switch (iDifficultyGroupLevel){ 
		case 1:
			eTemp.eQuestionType = eQuesTrigonometryVeryEasy;
			eTemp.iDifficulty = 605;
			
            iRangeMax = 1;
            iRangeMin = 20;

            break;
		case 2:
			eTemp.eQuestionType = eQuesTrigonometryEasy;
			eTemp.iDifficulty = 655;
			
            iRangeMax = 1;
            iRangeMin = 20;
            
            break;
		case 3:
			eTemp.eQuestionType = eQuesTrigonometryMedium;
			eTemp.iDifficulty = 715;
			
            iRangeMax = 1;
            iRangeMin = 20;
            
            break;
		case 4:
			eTemp.eQuestionType = eQuesTrigonometryHard;
			eTemp.iDifficulty = 785;
			
            iRangeMax = 1;
            iRangeMin = 20;
			
            break;
		case 5:
			eTemp.eQuestionType = eQuesTrigonometryVeryHard;
			eTemp.iDifficulty = 855;

            iRangeMax = 1;
            iRangeMin = 20;
            
            break;
		default:
			eTemp.eQuestionType = eQuesError;
			eTemp.bIsMultipleChoice = false;
			eTemp.sQuestionEncoded = "Error:";
	};
	

    //1 SinA = Opp / Hyp
	//2 Opp = Hyp * SinA
	//3 Hyp = Opp / Sin A
    
    //4 CosA = Adj / Hyp
    //5 Adj = Hyp * CosA
    //6 Hyp = Adj / CosA
    
    //7 TanA = Opp / Adj
    //8 Opp = Adj * TanA
    //9 Adj = Opp / TanA

	switch (iSideToGuess) {
		case 1:
            //1 SinA = Opp / Hyp
			eTemp.sQuestionEncoded = "Opp = <0>, Hyp = <1>\n What Formula can we use to calculate the angle A?";
            
            do {
                iOpp = modMisc::iRandomNumber(iRangeMin, iRangeMax);
                do {
                    iHyp = iOpp + modMisc::iRandomNumber(iRangeMin, iRangeMax);
                } while (2 * pow(iOpp, 2) == pow(iHyp, 2)); //No 45 degrees
            } while (10 * iOpp > 9 * iHyp or 3 * iOpp < iHyp);//Don't want to much of narrow angle or to much a steep angle
            //iAdj;
            //iAngle;
            
            fRandomFigure.fValue = (float)iOpp;
			eTemp.vecValues.push_back(fRandomFigure);
            fRandomFigure.fValue = (float)iHyp;
			eTemp.vecValues.push_back(fRandomFigure);

            iCommon = cPrimeNumbers->commonDenominator(iOpp, iHyp);
            
            fAnswer = asin(iOpp / iHyp) * 180 / fPi;
            sAnswerPrefix = "Sin(A) = ";

            //Correct answer
            eTemp.bIsMultipleChoice = true;
            eTemp.bOnlyOneAnswer = true;
            sMultipleChoiceAnswer.bIsCorrect = true; 
            sMultipleChoiceAnswer.bIsSelected = false; 
            ssTemp.str("");
            ssTemp << "Sin(A) = " << (iOpp / iCommon) << " / " << (iHyp / iCommon);
            sMultipleChoiceAnswer.sValue = ssTemp.str();
            
            eTemp.vecChoices.push_back(sMultipleChoiceAnswer);

			break;
		case 2:
            //2 Opp = Hyp * SinA
			eTemp.sQuestionEncoded = "Hyp = <0>, Angle = <1>degrees\n What Formula can we use to calculate the Opp?";

            //iOpp = modMisc::iRandomNumber(iRangeMin, iRangeMax);
            iHyp = modMisc::iRandomNumber(iRangeMin, iRangeMax);
            //iAdj;
            do {
                iAngle = 5 * modMisc::iRandomNumber(1 , 17);
            } while (iAngle == 45);
            
            fRandomFigure.fValue = (float)iHyp;
			eTemp.vecValues.push_back(fRandomFigure);
            fRandomFigure.fValue = (float)iAngle;
			eTemp.vecValues.push_back(fRandomFigure);
            
            fAnswer = iHyp * sin((double)iAngle * fPi / 180);
            sAnswerPrefix = "Opp = ";

            //Correct answer
            eTemp.bIsMultipleChoice = true;
            eTemp.bOnlyOneAnswer = true;
            sMultipleChoiceAnswer.bIsCorrect = true; 
            sMultipleChoiceAnswer.bIsSelected = false; 
            ssTemp.str("");
            ssTemp << "Opp = " << iHyp << " Sin(" << iAngle << ")";
            sMultipleChoiceAnswer.sValue = ssTemp.str();
            
            eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
            
            break;
		case 3:
            //3 Hyp = Opp / Sin A
			eTemp.sQuestionEncoded = "Opp = <0>, Angle = <1>degrees\n What Formula can we use to calculate the Hyp?";
            
            iOpp = modMisc::iRandomNumber(iRangeMin, iRangeMax);
            //iHyp = modMisc::iRandomNumber(iRangeMin, iRangeMax);
            //iAdj;
            do {
                iAngle = 5 * modMisc::iRandomNumber(1 , 17);
            } while (iAngle == 45);
            
            fRandomFigure.fValue = (float)iOpp;
			eTemp.vecValues.push_back(fRandomFigure);
            fRandomFigure.fValue = (float)iAngle;
			eTemp.vecValues.push_back(fRandomFigure);
            
            fAnswer = iOpp / sin((double)(iAngle * fPi / 180));
            sAnswerPrefix = "Hyp = ";
            
            iCommon = cPrimeNumbers->commonDenominator(iOpp, iHyp);
            
            //Correct answer
            eTemp.bIsMultipleChoice = true;
            eTemp.bOnlyOneAnswer = true;
            sMultipleChoiceAnswer.bIsCorrect = true; 
            sMultipleChoiceAnswer.bIsSelected = false; 
            ssTemp.str("");
            ssTemp << "Hyp = " << iOpp << " / Sin(" << iAngle << ")";
            sMultipleChoiceAnswer.sValue = ssTemp.str();
            
            eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
            
			break;
		case 4:
            //4 CosA = Adj / Hyp
			eTemp.sQuestionEncoded = "Adj = <0>, Hyp = <1>\n What Formula can we use to calculate the angle A?";
            
            do {
                //iOpp = modMisc::iRandomNumber(iRangeMin, iRangeMax);
                iAdj = modMisc::iRandomNumber(iRangeMin, iRangeMax);
                do {
                    iHyp = iAdj + modMisc::iRandomNumber(iRangeMin, iRangeMax);
                } while (2 * pow(iAdj, 2) == pow(iHyp, 2)); //No 45 degrees
                //iAngle = 5 * modMisc::iRandomNumber(1 , 17);
            } while (10 * iAdj > 9 * iHyp or 3 * iAdj < iHyp);//Don't want to much of narrow angle or to much a steep angle

            fRandomFigure.fValue = (float)iAdj;
			eTemp.vecValues.push_back(fRandomFigure);
            fRandomFigure.fValue = (float)iHyp;
			eTemp.vecValues.push_back(fRandomFigure);
            
            fAnswer = acos((double)(iAdj / iHyp));
            sAnswerPrefix = "Cos(A) = ";
            
            iCommon = cPrimeNumbers->commonDenominator(iAdj, iHyp);
            
            //Correct answer
            eTemp.bIsMultipleChoice = true;
            eTemp.bOnlyOneAnswer = true;
            sMultipleChoiceAnswer.bIsCorrect = true; 
            sMultipleChoiceAnswer.bIsSelected = false; 
            ssTemp.str("");
            ssTemp << "Cos(A) = " << (iAdj / iCommon) << " / " << (iHyp / iCommon);
            sMultipleChoiceAnswer.sValue = ssTemp.str();
            
            eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
            
			break;
		case 5:
            //5 Adj = Hyp * CosA
			eTemp.sQuestionEncoded = "Hyp = <0>, Angle = <1>degrees\n What Formula can we use to calculate the adj?";
            
            //iOpp = modMisc::iRandomNumber(iRangeMin, iRangeMax);
            //iAdj = modMisc::iRandomNumber(iRangeMin, iRangeMax);
            iHyp = modMisc::iRandomNumber(iRangeMin, iRangeMax);
            do {
                iAngle = 5 * modMisc::iRandomNumber(1 , 17);
            } while (iAngle == 45);
            
            fRandomFigure.fValue = (float)iHyp;
			eTemp.vecValues.push_back(fRandomFigure);
            fRandomFigure.fValue = (float)iAngle;
			eTemp.vecValues.push_back(fRandomFigure);
            
            fAnswer = iHyp * cos((double)(iAngle * fPi / 180));
            sAnswerPrefix = "Adj = ";
            
            //Correct answer
            eTemp.bIsMultipleChoice = true;
            eTemp.bOnlyOneAnswer = true;
            sMultipleChoiceAnswer.bIsCorrect = true; 
            sMultipleChoiceAnswer.bIsSelected = false; 
            ssTemp.str("");
            ssTemp << "Adj = " << iHyp << " Cos(" << iAngle << ")";
            sMultipleChoiceAnswer.sValue = ssTemp.str();
            
            eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
            
			break;
		case 6:
            //6 Hyp = Adj / CosA
			eTemp.sQuestionEncoded = "Adj = <0>, Angle = <1>\n What Formula can we use to calculate the Hyp?";
            
            //iOpp = modMisc::iRandomNumber(iRangeMin, iRangeMax);
            iAdj = modMisc::iRandomNumber(iRangeMin, iRangeMax);
            //iHyp = iAdj + modMisc::iRandomNumber(iRangeMin, iRangeMax);
            do {
                iAngle = 5 * modMisc::iRandomNumber(1 , 17);
            } while (iAngle == 45);
            
            fRandomFigure.fValue = (float)iAdj;
			eTemp.vecValues.push_back(fRandomFigure);
            fRandomFigure.fValue = (float)iAngle;
			eTemp.vecValues.push_back(fRandomFigure);
            
            fAnswer = iAdj / cos((double)(iAngle * fPi / 180));
            sAnswerPrefix = "Hyp = ";
            
            iCommon = cPrimeNumbers->commonDenominator(iOpp, iHyp);
            
            //Correct answer
            eTemp.bIsMultipleChoice = true;
            eTemp.bOnlyOneAnswer = true;
            sMultipleChoiceAnswer.bIsCorrect = true; 
            sMultipleChoiceAnswer.bIsSelected = false; 
            ssTemp.str("");
            ssTemp << "Hyp = " << iAdj << " / Cos(" << iAngle << ")";
            sMultipleChoiceAnswer.sValue = ssTemp.str();
            
            eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
            
			break;
		case 7:
            //7 TanA = Opp / Adj
			eTemp.sQuestionEncoded = "Opp = <0>, Adj = <1>\n What Formula can we use to calculate the angle?";
            
            do {
                iOpp = modMisc::iRandomNumber(iRangeMin, iRangeMax);
                do {
                    iAdj = modMisc::iRandomNumber(iRangeMin, iRangeMax);
                } while (iAdj == iOpp); //No 45 degrees
            } while (iOpp > 4 * iAdj or 4 * iOpp < iAdj);//Don't want to much of narrow angle or to much a steep angle

            //iHyp = iAdj + modMisc::iRandomNumber(iRangeMin, iRangeMax);
            //iAngle = 5 * modMisc::iRandomNumber(1 , 17);
            
            fRandomFigure.fValue = (float)iOpp;
			eTemp.vecValues.push_back(fRandomFigure);
            fRandomFigure.fValue = (float)iAdj;
			eTemp.vecValues.push_back(fRandomFigure);
            
            fAnswer = iOpp / iAdj;
            sAnswerPrefix = "Tan(A) = ";
            
            iCommon = cPrimeNumbers->commonDenominator(iOpp, iAdj);
            
            //Correct answer
            eTemp.bIsMultipleChoice = true;
            eTemp.bOnlyOneAnswer = true;
            sMultipleChoiceAnswer.bIsCorrect = true; 
            sMultipleChoiceAnswer.bIsSelected = false; 
            ssTemp.str("");
            ssTemp << "Tan(A) = " << (iOpp / iCommon) << " / " << (iAdj / iCommon);
            sMultipleChoiceAnswer.sValue = ssTemp.str();
            
            eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
            
			break;
		case 8:
            //8 Opp = Adj * TanA
			eTemp.sQuestionEncoded = "Adj = <0>, Angle = <1>\n What Formula can we use to calculate the Opp?";
            
            //iOpp = modMisc::iRandomNumber(iRangeMin, iRangeMax);
            iAdj = modMisc::iRandomNumber(iRangeMin, iRangeMax);
            //iHyp = iAdj + modMisc::iRandomNumber(iRangeMin, iRangeMax);
            do {
                iAngle = 5 * modMisc::iRandomNumber(1 , 17);
            } while (iAngle == 45);
            
            fRandomFigure.fValue = (float)iAdj;
			eTemp.vecValues.push_back(fRandomFigure);
            fRandomFigure.fValue = (float)iAngle;
			eTemp.vecValues.push_back(fRandomFigure);
            
            fAnswer = iAdj * tan((double)(iAngle * fPi / 180));
            sAnswerPrefix = "Opp = ";
            
            //Correct answer
            eTemp.bIsMultipleChoice = true;
            eTemp.bOnlyOneAnswer = true;
            sMultipleChoiceAnswer.bIsCorrect = true; 
            sMultipleChoiceAnswer.bIsSelected = false; 
            ssTemp.str("");
            ssTemp << "Opp = " << iAdj << " Tan(" << iAngle << ")";
            sMultipleChoiceAnswer.sValue = ssTemp.str();
            
            eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
            
			break;
		case 9:
            //9 Adj = Opp / TanA
			eTemp.sQuestionEncoded = "Opp = <0>, Angle = <1>\n What Formula can we use to calculate the Adj?";
            
            iOpp = modMisc::iRandomNumber(iRangeMin, iRangeMax);
            //iAdj = modMisc::iRandomNumber(iRangeMin, iRangeMax);
            //iHyp = iAdj + modMisc::iRandomNumber(iRangeMin, iRangeMax);
            do {
                iAngle = 5 * modMisc::iRandomNumber(1 , 17);
            } while (iAngle == 45);
            
            fRandomFigure.fValue = (float)iOpp;
			eTemp.vecValues.push_back(fRandomFigure);
            fRandomFigure.fValue = (float)iAngle;
			eTemp.vecValues.push_back(fRandomFigure);
            
            fAnswer = iOpp / tan((double)(iAngle * fPi / 180));
            sAnswerPrefix = "Adj = ";
            
            //Correct answer
            eTemp.bIsMultipleChoice = true;
            eTemp.bOnlyOneAnswer = true;
            sMultipleChoiceAnswer.bIsCorrect = true; 
            sMultipleChoiceAnswer.bIsSelected = false; 
            ssTemp.str("");
            ssTemp << "Adj = " << iOpp << " / Tan(" << iAngle << ")";
            sMultipleChoiceAnswer.sValue = ssTemp.str();
            
            eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
            
			break;
		default:
			break;
	};
	
   	if (iSideToGuess == 1 or iSideToGuess == 4 or iSideToGuess == 7) {
		//Answer is the angle
        //1 SinA = Opp / Hyp
        
        iOpp = (int)eTemp.vecValues[0].fValue;
        iHyp = (int)eTemp.vecValues[1].fValue;
        
        fAnswerTemp = asin(iOpp / iHyp) * 180 / fPi;
        
        if (modMisc::almostEquals(fAnswer, fAnswerTemp, fAnswerAccuracyAngle) == false) {
            iCommon = cPrimeNumbers->commonDenominator(iOpp, iHyp);
            
            //Wrong answer
            eTemp.bIsMultipleChoice = true;
            eTemp.bOnlyOneAnswer = true;
            sMultipleChoiceAnswer.bIsCorrect = false; 
            sMultipleChoiceAnswer.bIsSelected = false; 
            ssTemp.str("");
            ssTemp << "Sin(A) = " << (iOpp / iCommon) << " / " << (iHyp / iCommon);
            sMultipleChoiceAnswer.sValue = ssTemp.str();
            
			if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
            };
        };
        
        //4 CosA = Adj / Hyp
        iAdj = eTemp.vecValues[0].fValue;
        iHyp = eTemp.vecValues[1].fValue;
        
        fAnswerTemp = acos((double)(iAdj / iHyp));
        
        if (modMisc::almostEquals(fAnswer, fAnswerTemp, fAnswerAccuracyAngle) == false) {
            iCommon = cPrimeNumbers->commonDenominator(iAdj, iHyp);
            
            //Wrong answer
            eTemp.bIsMultipleChoice = true;
            eTemp.bOnlyOneAnswer = true;
            sMultipleChoiceAnswer.bIsCorrect = false; 
            sMultipleChoiceAnswer.bIsSelected = false; 
            ssTemp.str("");
            ssTemp << "Cos(A) = " << (iAdj / iCommon) << " / " << (iHyp / iCommon);
            sMultipleChoiceAnswer.sValue = ssTemp.str();
            
			if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
            };
        };
        
        //7 TanA = Opp / Adj
        iOpp = eTemp.vecValues[0].fValue;
        iAdj = eTemp.vecValues[1].fValue;
        
        fAnswerTemp = iOpp / iAdj;
        
        if (modMisc::almostEquals(fAnswer, fAnswerTemp, fAnswerAccuracyAngle) == false) {
            iCommon = cPrimeNumbers->commonDenominator(iOpp, iAdj);
            
            //Wrong answer
            eTemp.bIsMultipleChoice = true;
            eTemp.bOnlyOneAnswer = true;
            sMultipleChoiceAnswer.bIsCorrect = false; 
            sMultipleChoiceAnswer.bIsSelected = false; 
            ssTemp.str("");
            ssTemp << "Tan(A) = " << (iOpp / iCommon) << " / " << (iAdj / iCommon);
            sMultipleChoiceAnswer.sValue = ssTemp.str();
            
			if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
            };
        };
        
        
        do {
            //Answer is the angle
            //1 SinA = Opp / Hyp
            
            switch (modMisc::iRandomNumber(1, 3)) {
                case 1:
                    iOpp = (int)eTemp.vecValues[0].fValue * modMisc::iRandomNumberNonZero(-2, 2);
                    iHyp = (int)eTemp.vecValues[1].fValue * modMisc::iRandomNumberNonZero(-2, 2);
                    break;
                case 2:
                    iOpp = (int)eTemp.vecValues[0].fValue * modMisc::iRandomNumberNonZero(-2, 2);
                    iHyp = (int)eTemp.vecValues[1].fValue;
                    break;
                case 3:
                    iOpp = (int)eTemp.vecValues[0].fValue;
                    iHyp = (int)eTemp.vecValues[1].fValue * modMisc::iRandomNumberNonZero(-2, 2);
                    break;
                default:
                    break;
            };
            
            if (iHyp < 0) {
                iHyp = -1 * iHyp;
                iOpp = -1 * iOpp;
            };
            
            fAnswerTemp = asin(iOpp / iHyp) * 180 / fPi;
            
            if (modMisc::almostEquals(fAnswer, fAnswerTemp, fAnswerAccuracyAngle) == false) {
                iCommon = cPrimeNumbers->commonDenominator(iOpp, iHyp);
                
                //Wrong answer
                eTemp.bIsMultipleChoice = true;
                eTemp.bOnlyOneAnswer = true;
                sMultipleChoiceAnswer.bIsCorrect = false; 
                sMultipleChoiceAnswer.bIsSelected = false; 
                ssTemp.str("");
                ssTemp << "Sin(A) = " << (iOpp / iCommon) << " / " << (iHyp / iCommon);
                sMultipleChoiceAnswer.sValue = ssTemp.str();
                
                if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                    eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
                };
            };
            
            //4 CosA = Adj / Hyp
            switch (modMisc::iRandomNumber(1, 3)) {
                case 1:
                    iAdj = eTemp.vecValues[0].fValue * modMisc::iRandomNumberNonZero(-2, 2);
                    iHyp = eTemp.vecValues[1].fValue * modMisc::iRandomNumberNonZero(-2, 2);
                    break;
                case 2:
                    iAdj = eTemp.vecValues[0].fValue * modMisc::iRandomNumberNonZero(-2, 2);
                    iHyp = eTemp.vecValues[1].fValue;
                    break;
                case 3:
                    iAdj = eTemp.vecValues[0].fValue;
                    iHyp = eTemp.vecValues[1].fValue * modMisc::iRandomNumberNonZero(-2, 2);
                    break;
                default:
                    break;
            };

            if (iHyp < 0) {
                iHyp = -1 * iHyp;
                iAdj = -1 * iAdj;
            };
            
            fAnswerTemp = acos((double)(iAdj / iHyp));
            
            if (modMisc::almostEquals(fAnswer, fAnswerTemp, fAnswerAccuracyAngle) == false) {
                iCommon = cPrimeNumbers->commonDenominator(iAdj, iHyp);
                
                //Wrong answer
                eTemp.bIsMultipleChoice = true;
                eTemp.bOnlyOneAnswer = true;
                sMultipleChoiceAnswer.bIsCorrect = false; 
                sMultipleChoiceAnswer.bIsSelected = false; 
                ssTemp.str("");
                ssTemp << "Cos(A) = " << (iAdj / iCommon) << " / " << (iHyp / iCommon);
                sMultipleChoiceAnswer.sValue = ssTemp.str();
                
                if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                    eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
                };
            };
            
            //7 TanA = Opp / Adj
            switch (modMisc::iRandomNumber(1, 3)) {
                case 1:
                    iOpp = eTemp.vecValues[0].fValue * modMisc::iRandomNumberNonZero(-2, 2);
                    iAdj = eTemp.vecValues[1].fValue * modMisc::iRandomNumberNonZero(-2, 2);
                    break;
                case 2:
                    iOpp = eTemp.vecValues[0].fValue * modMisc::iRandomNumberNonZero(-2, 2);
                    iAdj = eTemp.vecValues[1].fValue;
                    break;
                case 3:
                    iOpp = eTemp.vecValues[0].fValue;
                    iAdj = eTemp.vecValues[1].fValue * modMisc::iRandomNumberNonZero(-2, 2);
                    break;
                default:
                    break;
            };
            
            if (iAdj < 0) {
                iAdj = -1 * iAdj;
                iOpp = -1 * iOpp;
            };
            
            fAnswerTemp = iOpp / iAdj;
            
            if (modMisc::almostEquals(fAnswer, fAnswerTemp, fAnswerAccuracyAngle) == false) {
                iCommon = cPrimeNumbers->commonDenominator(iOpp, iAdj);
                
                //Wrong answer
                eTemp.bIsMultipleChoice = true;
                eTemp.bOnlyOneAnswer = true;
                sMultipleChoiceAnswer.bIsCorrect = false; 
                sMultipleChoiceAnswer.bIsSelected = false; 
                ssTemp.str("");
                ssTemp << "Tan(A) = " << (iOpp / iCommon) << " / " << (iAdj / iCommon);
                sMultipleChoiceAnswer.sValue = ssTemp.str();
                
                if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                    eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
                };
            };

            tmEnd = time (NULL);
		} while (eTemp.vecChoices.size() < 6 and (int)tmStart + iCutOffTime < (int)tmEnd);
        
    } else {
        //2 Opp = Hyp * SinA
        iHyp = eTemp.vecValues[0].fValue;
        iAngle = eTemp.vecValues[1].fValue;

        fAnswerTemp = iHyp * sin(iAngle * fPi / 180);
        
        if (modMisc::almostEquals(fAnswer, fAnswerTemp, fAnswerAccuracy) == false) {
            //Wrong answer
            eTemp.bIsMultipleChoice = true;
            eTemp.bOnlyOneAnswer = true;
            sMultipleChoiceAnswer.bIsCorrect = false; 
            sMultipleChoiceAnswer.bIsSelected = false; 
            ssTemp.str("");
            ssTemp << sAnswerPrefix.c_str() << iHyp << " Sin(" << iAngle << ")";
            sMultipleChoiceAnswer.sValue = ssTemp.str();
            
			if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
            };
        };
        
        //3 Hyp = Opp / Sin A
        iOpp = eTemp.vecValues[0].fValue;
        iAngle = eTemp.vecValues[1].fValue;
        
        fAnswerTemp = iOpp / sin((double)(iAngle * fPi / 180));
        
        if (modMisc::almostEquals(fAnswer, fAnswerTemp, fAnswerAccuracy) == false) {
            iCommon = cPrimeNumbers->commonDenominator(iOpp, iHyp);
            
            //Wrong answer
            eTemp.bIsMultipleChoice = true;
            eTemp.bOnlyOneAnswer = true;
            sMultipleChoiceAnswer.bIsCorrect = false; 
            sMultipleChoiceAnswer.bIsSelected = false; 
            ssTemp.str("");
            ssTemp << sAnswerPrefix.c_str() << iOpp << " / Sin(" << iAngle << ")";
            sMultipleChoiceAnswer.sValue = ssTemp.str();
            
			if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
            };
        };
        
        //5 Adj = Hyp * CosA
        iHyp = eTemp.vecValues[0].fValue;
        iAngle = eTemp.vecValues[1].fValue;
        
        fAnswerTemp = iHyp * cos((double)(iAngle * fPi / 180));
        
        if (modMisc::almostEquals(fAnswer, fAnswerTemp, fAnswerAccuracy) == false) {
            //Wrong answer
            eTemp.bIsMultipleChoice = true;
            eTemp.bOnlyOneAnswer = true;
            sMultipleChoiceAnswer.bIsCorrect = false; 
            sMultipleChoiceAnswer.bIsSelected = false; 
            ssTemp.str("");
            ssTemp << sAnswerPrefix.c_str() << iHyp << " Cos(" << iAngle << ")";
            sMultipleChoiceAnswer.sValue = ssTemp.str();
            
			if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
            };
        };
        
        //6 Hyp = Adj / CosA
        iAdj = eTemp.vecValues[0].fValue;
        iAngle = eTemp.vecValues[1].fValue;
        
        fAnswerTemp = iAdj / cos((double)(iAngle * fPi / 180));
        
        if (modMisc::almostEquals(fAnswer, fAnswerTemp, fAnswerAccuracy) == false) {
            iCommon = cPrimeNumbers->commonDenominator(iOpp, iHyp);
            
            //Wrong answer
            eTemp.bIsMultipleChoice = true;
            eTemp.bOnlyOneAnswer = true;
            sMultipleChoiceAnswer.bIsCorrect = false; 
            sMultipleChoiceAnswer.bIsSelected = false; 
            ssTemp.str("");
            ssTemp << sAnswerPrefix.c_str() << iAdj << " / Cos(" << iAngle << ")";
            sMultipleChoiceAnswer.sValue = ssTemp.str();
            
			if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
            };
        };
        
        //8 Opp = Adj * TanA
        iAdj = eTemp.vecValues[0].fValue;
        iAngle = eTemp.vecValues[1].fValue;
        
        fAnswerTemp = iAdj * tan((double)(iAngle * fPi / 180));
        
        if (modMisc::almostEquals(fAnswer, fAnswerTemp, fAnswerAccuracy) == false) {
            //Wrong answer
            eTemp.bIsMultipleChoice = true;
            eTemp.bOnlyOneAnswer = true;
            sMultipleChoiceAnswer.bIsCorrect = false; 
            sMultipleChoiceAnswer.bIsSelected = false; 
            ssTemp.str("");
            ssTemp << sAnswerPrefix.c_str() << iAdj << " Tan(" << iAngle << ")";
            sMultipleChoiceAnswer.sValue = ssTemp.str();
            
			if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
            };
        };
        
        //9 Adj = Opp / TanA
        iOpp = eTemp.vecValues[0].fValue;
        iAngle = eTemp.vecValues[1].fValue;
        
        fAnswerTemp = iOpp / tan((double)(iAngle * fPi / 180));
        
        if (modMisc::almostEquals(fAnswer, fAnswerTemp, fAnswerAccuracy) == false) {
            //Wrong answer
            eTemp.bIsMultipleChoice = true;
            eTemp.bOnlyOneAnswer = true;
            sMultipleChoiceAnswer.bIsCorrect = false; 
            sMultipleChoiceAnswer.bIsSelected = false; 
            ssTemp.str("");
            ssTemp << sAnswerPrefix.c_str() << iOpp << " / Tan(" << iAngle << ")";
            sMultipleChoiceAnswer.sValue = ssTemp.str();
            
			if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
            };
        };

        do {
            //2 Opp = Hyp * SinA
            switch (modMisc::iRandomNumber(1, 3)) {
                case 1:
                    iHyp = eTemp.vecValues[0].fValue * modMisc::iRandomNumberNonZero(-2, 2);
                    iAngle = eTemp.vecValues[1].fValue * modMisc::iRandomNumberNonZero(-2, 2);
                    break;
                case 2:
                    iHyp = eTemp.vecValues[0].fValue * modMisc::iRandomNumberNonZero(-2, 2);
                    iAngle = eTemp.vecValues[1].fValue;
                    break;
                case 3:
                    iHyp = eTemp.vecValues[0].fValue;
                    iAngle = eTemp.vecValues[1].fValue * modMisc::iRandomNumberNonZero(-2, 2);
                    break;
                default:
                    break;
            };

            fAnswerTemp = iHyp * sin(iAngle * fPi / 180);
            
            if (modMisc::almostEquals(fAnswer, fAnswerTemp, fAnswerAccuracy) == false) {
                //Wrong answer
                eTemp.bIsMultipleChoice = true;
                eTemp.bOnlyOneAnswer = true;
                sMultipleChoiceAnswer.bIsCorrect = false; 
                sMultipleChoiceAnswer.bIsSelected = false; 
                ssTemp.str("");
                ssTemp << sAnswerPrefix.c_str() << iHyp << " Sin(" << iAngle << ")";
                sMultipleChoiceAnswer.sValue = ssTemp.str();
                
                if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                    eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
                };
            };
            
            //3 Hyp = Opp / Sin A
            switch (modMisc::iRandomNumber(1, 3)) {
                case 1:
                    iOpp = eTemp.vecValues[0].fValue * modMisc::iRandomNumberNonZero(-2, 2);
                    iAngle = eTemp.vecValues[1].fValue * modMisc::iRandomNumberNonZero(-2, 2);
                    break;
                case 2:
                    iOpp = eTemp.vecValues[0].fValue * modMisc::iRandomNumberNonZero(-2, 2);
                    iAngle = eTemp.vecValues[1].fValue;
                    break;
                case 3:
                    iOpp = eTemp.vecValues[0].fValue;
                    iAngle = eTemp.vecValues[1].fValue * modMisc::iRandomNumberNonZero(-2, 2);
                    break;
                default:
                    break;
            };
            
            fAnswerTemp = iOpp / sin((double)(iAngle * fPi / 180));
            
            if (modMisc::almostEquals(fAnswer, fAnswerTemp, fAnswerAccuracy) == false) {
                iCommon = cPrimeNumbers->commonDenominator(iOpp, iHyp);
                
                //Wrong answer
                eTemp.bIsMultipleChoice = true;
                eTemp.bOnlyOneAnswer = true;
                sMultipleChoiceAnswer.bIsCorrect = false; 
                sMultipleChoiceAnswer.bIsSelected = false; 
                ssTemp.str("");
                ssTemp << sAnswerPrefix.c_str() << iOpp << " / Sin(" << iAngle << ")";
                sMultipleChoiceAnswer.sValue = ssTemp.str();
                
                if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                    eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
                };
            };
            
            //5 Adj = Hyp * CosA
            switch (modMisc::iRandomNumber(1, 3)) {
                case 1:
                    iHyp = eTemp.vecValues[0].fValue * modMisc::iRandomNumberNonZero(-2, 2);
                    iAngle = eTemp.vecValues[1].fValue * modMisc::iRandomNumberNonZero(-2, 2);
                    break;
                case 2:
                    iHyp = eTemp.vecValues[0].fValue * modMisc::iRandomNumberNonZero(-2, 2);
                    iAngle = eTemp.vecValues[1].fValue;
                    break;
                case 3:
                    iHyp = eTemp.vecValues[0].fValue;
                    iAngle = eTemp.vecValues[1].fValue * modMisc::iRandomNumberNonZero(-2, 2);
                    break;
                default:
                    break;
            };

            fAnswerTemp = iHyp * cos((double)(iAngle * fPi / 180));
            
            if (modMisc::almostEquals(fAnswer, fAnswerTemp, fAnswerAccuracy) == false) {
                //Wrong answer
                eTemp.bIsMultipleChoice = true;
                eTemp.bOnlyOneAnswer = true;
                sMultipleChoiceAnswer.bIsCorrect = false; 
                sMultipleChoiceAnswer.bIsSelected = false; 
                ssTemp.str("");
                ssTemp << sAnswerPrefix.c_str() << iHyp << " Cos(" << iAngle << ")";
                sMultipleChoiceAnswer.sValue = ssTemp.str();
                
                if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                    eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
                };
            };
            
            //6 Hyp = Adj / CosA
            switch (modMisc::iRandomNumber(1, 3)) {
                case 1:
                    iAdj = eTemp.vecValues[0].fValue * modMisc::iRandomNumberNonZero(-2, 2);
                    iAngle = eTemp.vecValues[1].fValue * modMisc::iRandomNumberNonZero(-2, 2);
                    break;
                case 2:
                    iAdj = eTemp.vecValues[0].fValue * modMisc::iRandomNumberNonZero(-2, 2);
                    iAngle = eTemp.vecValues[1].fValue;
                    break;
                case 3:
                    iAdj = eTemp.vecValues[0].fValue;
                    iAngle = eTemp.vecValues[1].fValue * modMisc::iRandomNumberNonZero(-2, 2);
                    break;
                default:
                    break;
            };
            
            fAnswerTemp = iAdj / cos((double)(iAngle * fPi / 180));
            
            if (modMisc::almostEquals(fAnswer, fAnswerTemp, fAnswerAccuracy) == false) {
                iCommon = cPrimeNumbers->commonDenominator(iOpp, iHyp);
                
                //Wrong answer
                eTemp.bIsMultipleChoice = true;
                eTemp.bOnlyOneAnswer = true;
                sMultipleChoiceAnswer.bIsCorrect = false; 
                sMultipleChoiceAnswer.bIsSelected = false; 
                ssTemp.str("");
                ssTemp << sAnswerPrefix.c_str() << iAdj << " / Cos(" << iAngle << ")";
                sMultipleChoiceAnswer.sValue = ssTemp.str();
                
                if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                    eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
                };
            };
            
            //8 Opp = Adj * TanA
            switch (modMisc::iRandomNumber(1, 3)) {
                case 1:
                    iAdj = eTemp.vecValues[0].fValue * modMisc::iRandomNumberNonZero(-2, 2);
                    iAngle = eTemp.vecValues[1].fValue * modMisc::iRandomNumberNonZero(-2, 2);
                    break;
                case 2:
                    iAdj = eTemp.vecValues[0].fValue * modMisc::iRandomNumberNonZero(-2, 2);
                    iAngle = eTemp.vecValues[1].fValue;
                    break;
                case 3:
                    iAdj = eTemp.vecValues[0].fValue;
                    iAngle = eTemp.vecValues[1].fValue * modMisc::iRandomNumberNonZero(-2, 2);
                    break;
                default:
                    break;
            };
            
            fAnswerTemp = iAdj * tan((double)(iAngle * fPi / 180));
            
            if (modMisc::almostEquals(fAnswer, fAnswerTemp, fAnswerAccuracy) == false) {
                //Wrong answer
                eTemp.bIsMultipleChoice = true;
                eTemp.bOnlyOneAnswer = true;
                sMultipleChoiceAnswer.bIsCorrect = false; 
                sMultipleChoiceAnswer.bIsSelected = false; 
                ssTemp.str("");
                ssTemp << sAnswerPrefix.c_str() << iAdj << " Tan(" << iAngle << ")";
                sMultipleChoiceAnswer.sValue = ssTemp.str();
                
                if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                    eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
                };
            };
            
            //9 Adj = Opp / TanA
            switch (modMisc::iRandomNumber(1, 3)) {
                case 1:
                    iOpp = eTemp.vecValues[0].fValue * modMisc::iRandomNumberNonZero(-2, 2);
                    iAngle = eTemp.vecValues[1].fValue * modMisc::iRandomNumberNonZero(-2, 2);
                    break;
                case 2:
                    iOpp = eTemp.vecValues[0].fValue * modMisc::iRandomNumberNonZero(-2, 2);
                    iAngle = eTemp.vecValues[1].fValue;
                    break;
                case 3:
                    iOpp = eTemp.vecValues[0].fValue;
                    iAngle = eTemp.vecValues[1].fValue * modMisc::iRandomNumberNonZero(-2, 2);
                    break;
                default:
                    break;
            };
            
            fAnswerTemp = iOpp / tan((double)(iAngle * fPi / 180));
            
            if (modMisc::almostEquals(fAnswer, fAnswerTemp, fAnswerAccuracy) == false) {
                //Wrong answer
                eTemp.bIsMultipleChoice = true;
                eTemp.bOnlyOneAnswer = true;
                sMultipleChoiceAnswer.bIsCorrect = false; 
                sMultipleChoiceAnswer.bIsSelected = false; 
                ssTemp.str("");
                ssTemp << sAnswerPrefix.c_str() << iOpp << " / Tan(" << iAngle << ")";
                sMultipleChoiceAnswer.sValue = ssTemp.str();
                
                if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                    eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
                };
            };
            
            
            tmEnd = time (NULL);
		} while (eTemp.vecChoices.size() < 6 and (int)tmStart + iCutOffTime < (int)tmEnd);
    };
	
    //	strFloatValue fRandomFigure;
    //	strQuestion eTemp;
    //	int iTemp;
    //	strStringAnswer sMultipleChoiceAnswer; 
    //	vector<strStringAnswer>::iterator itMultipleChoiceAnswer; 
    //	std::stringstream ssTemp;
    //	enumFnTrig eTempFnTrig;
    //	bool bIsNegative;
    //	int iSideToGuess;
    //	strTrigNumbers eTrigNo;
    //	int iDifficultyGroupLevel;
//    delete cPrimeNumbers;

	sGroupName.clear();
    //	time_t tmStart;
    
	return eTemp;
};

strQuestion ClsQuiz::randomQuestionType_parallogram(){
	int iRandom;
	strFloatValue fRandomFigure;
	strQuestion eTemp;
	int iTemp;
	strStringAnswer sMultipleChoiceAnswer; 
	vector<strStringAnswer>::iterator itMultipleChoiceAnswer; 
	std::stringstream ssTemp;
	int iRangeMax;
	int iRangeMin;
	int iDiff;
	std::string sGroupName;
	time_t tmStart;
	time_t tmEnd;
	
	tmStart = time (NULL);
	
	eTemp.eQuestionGroup = eQuesGrpShapes;
	sGroupName = modMisc::convertEnum_QuesTypeGroupText(eQuesGrpShapes);
	
	eTemp.bHasPic = true;
	eTemp.ePic = ePicQues_Parallelogram;
	
	eTemp.vecValues.clear();
	eTemp.vecAnswers.clear();
	eTemp.vecChoices.clear();
	
	
	const int gciQuestionTypeMax = 3; //note: needs to be the maximum integer in the enumQuestionType so that we can generate random question types 
	iRandom = (int)modMisc::iRandomNumber(1, gciQuestionTypeMax);
	iDiff = (int)modMisc::iRandomNumber(1, 3);

	if (cPreferences->getType() == ePrefByGroup or cPreferences->getType() == ePrefBySubject) {
		iDiff = (int)cPreferences->getDifficulty(&sGroupName);
	} else {
		iDiff = (int)modMisc::iRandomNumber(1, 3);
	};
	
	eTemp.eAnswerType = eAnsTypNumeric;
	
	switch (iDiff) {
		case 1:
			eTemp.eQuestionType = eQuesParallogramVeryEasy;
			eTemp.iDifficulty = 140;
			
			iRangeMin = 1;
			iRangeMax = 8;
			
			break;
		case 2:
			eTemp.eQuestionType = eQuesParallogramEasy;
			eTemp.iDifficulty = 180;
			
			iRangeMin = 6;
			iRangeMax = 12;
			
			break;
		case 3:
			eTemp.eQuestionType = eQuesParallogramMedium;
			eTemp.iDifficulty = 215;

			iRangeMin = 8;
			iRangeMax = 15;

			break;
		case 4:
			eTemp.eQuestionType = eQuesParallogramHard;
			eTemp.iDifficulty = 235;
			
			iRangeMin = 11;
			iRangeMax = 20;
			
			break;
		case 5:
			eTemp.eQuestionType = eQuesParallogramVeryHard;
			eTemp.iDifficulty = 250;
			
			iRangeMin = 15;
			iRangeMax = 25;
			
			break;
		default:
			break;
	}; //switch iDiff

	switch (iRandom){ 
		case 1:
			eTemp.sQuestionEncoded = "Height = <0>, Length = <1>\n What is the area?";

			fRandomFigure.fValue = (float)modMisc::iRandomNumber(iRangeMin, iRangeMax);
			eTemp.vecValues.push_back(fRandomFigure);
			fRandomFigure.fValue = (float)modMisc::iRandomNumber(iRangeMin, iRangeMax);
			eTemp.vecValues.push_back(fRandomFigure);

			fRandomFigure.fValue = (float)(eTemp.vecValues[0].fValue * eTemp.vecValues[1].fValue);
			eTemp.vecAnswers.push_back(fRandomFigure);
			break;
		case 2:
			eTemp.sQuestionEncoded = "Length = <0>, Area = <1>\n What is the height?";

			fRandomFigure.fValue = (float)modMisc::iRandomNumber(iRangeMin, iRangeMax);
			eTemp.vecValues.push_back(fRandomFigure);
			fRandomFigure.fValue = (float)modMisc::iRandomNumber(iRangeMin, iRangeMax);
			eTemp.vecAnswers.push_back(fRandomFigure);
			
			fRandomFigure.fValue = (float)(eTemp.vecValues[0].fValue * eTemp.vecAnswers[0].fValue);
			eTemp.vecValues.push_back(fRandomFigure);

			break;
		case 3:
			eTemp.sQuestionEncoded = "Height = <0>, Area = <1>\n What is the length?";
			
			fRandomFigure.fValue = (float)modMisc::iRandomNumber(iRangeMin, iRangeMax);
			eTemp.vecValues.push_back(fRandomFigure);
			fRandomFigure.fValue = (float)modMisc::iRandomNumber(iRangeMin, iRangeMax);
			eTemp.vecAnswers.push_back(fRandomFigure);
			
			fRandomFigure.fValue = (float)(eTemp.vecValues[0].fValue * eTemp.vecAnswers[0].fValue);
			eTemp.vecValues.push_back(fRandomFigure);
			
			break;
		default:
			eTemp.eQuestionType = eQuesError;
			eTemp.bIsMultipleChoice = false;
			eTemp.sQuestionEncoded = "Error:";
			break;
	};//switch iRandom
	
	
//	if (modMisc::iRandomNumber(0, 10) < 5) {
	if (modMisc::iRandomNumber(0, 10) < 15) {
		//multiple choice
		
		//Correct answer
		eTemp.bIsMultipleChoice = true;
		eTemp.bOnlyOneAnswer = true;
		sMultipleChoiceAnswer.bIsCorrect = true; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		ssTemp << (int)eTemp.vecAnswers[0].fValue;
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		
		eTemp.vecChoices.push_back(sMultipleChoiceAnswer);

		iTemp = (int)(eTemp.vecAnswers[0].fValue + (3 * modMisc::iRandomNumberNonZero(-1, 1)));
		if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << iTemp;
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
				eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
			};
		};
		
		iTemp = (int)(iTemp + (modMisc::iRandomNumberNonZero(-3, 3) * modMisc::iRandomNumberNonZero(-1, 1)));
		if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << iTemp;
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
				eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
			};
		};
		
		iTemp = (int)(eTemp.vecAnswers[0].fValue + (2 * modMisc::iRandomNumberNonZero(-1, 1)));
		if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << iTemp;
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
				eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
			};
		};
		
		iTemp = (int)(eTemp.vecAnswers[0].fValue + modMisc::iRandomNumberNonZero(-5, 5));
		if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << iTemp;
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
				eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
			};
		};
		
		iTemp = (int)(eTemp.vecAnswers[0].fValue + modMisc::iRandomNumberNonZero(-3, 4));
		if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << iTemp;
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
				eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
			};
		};
		
		
		//wrong answers
		if (iRandom == 0) {
			//answer is area
		
		
		
			iTemp = (int)(eTemp.vecValues[0].fValue * eTemp.vecValues[1].fValue * 2);
			if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
				sMultipleChoiceAnswer.bIsCorrect = false; 
				sMultipleChoiceAnswer.bIsSelected = false; 
				ssTemp.str("");
				ssTemp << iTemp;
				sMultipleChoiceAnswer.sValue = ssTemp.str();
				if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
					eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
				};
			};
			
			iTemp = (int)(eTemp.vecValues[0].fValue * eTemp.vecValues[1].fValue / 2);
			if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
				sMultipleChoiceAnswer.bIsCorrect = false; 
				sMultipleChoiceAnswer.bIsSelected = false; 
				ssTemp.str("");
				ssTemp << iTemp;
				sMultipleChoiceAnswer.sValue = ssTemp.str();
				if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
					eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
				};
			};
			
			iTemp = (int)(eTemp.vecValues[0].fValue + eTemp.vecValues[1].fValue);
			if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
				sMultipleChoiceAnswer.bIsCorrect = false; 
				sMultipleChoiceAnswer.bIsSelected = false; 
				ssTemp.str("");
				ssTemp << iTemp;
				sMultipleChoiceAnswer.sValue = ssTemp.str();
				if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
					eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
				};
			};
			
			do {
				iTemp = (int)(eTemp.vecValues[0].fValue * modMisc::iRandomNumber(1, 2 * (int)eTemp.vecValues[1].fValue));
				if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
					sMultipleChoiceAnswer.bIsCorrect = false; 
					sMultipleChoiceAnswer.bIsSelected = false; 
					ssTemp.str("");
					ssTemp << iTemp;
					sMultipleChoiceAnswer.sValue = ssTemp.str();
					if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
						eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
					};
				};
				
				iTemp = (int)(eTemp.vecValues[1].fValue + modMisc::iRandomNumber(1, 2 * (int)eTemp.vecValues[0].fValue));
				if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
					sMultipleChoiceAnswer.bIsCorrect = false; 
					sMultipleChoiceAnswer.bIsSelected = false; 
					ssTemp.str("");
					ssTemp << iTemp;
					sMultipleChoiceAnswer.sValue = ssTemp.str();
					if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
						eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
					};
				};
				
				iTemp = (int)modMisc::iRandomNumberNonZero((int)(-1 * (fabs(eTemp.vecValues[0].fValue) + fabs(eTemp.vecValues[1].fValue))), 3 * (int)(fabs(eTemp.vecValues[0].fValue) + fabs(eTemp.vecValues[1].fValue)));
				if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
					sMultipleChoiceAnswer.bIsCorrect = false; 
					sMultipleChoiceAnswer.bIsSelected = false; 
					ssTemp.str("");
					ssTemp << iTemp;
					sMultipleChoiceAnswer.sValue = ssTemp.str();
					if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
						eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
					};
				};
				
				iTemp = (int)modMisc::iRandomNumberNonZero((int)(-1 * fabs(eTemp.vecValues[0].fValue * eTemp.vecValues[1].fValue)), 2 * (int)(fabs(eTemp.vecValues[0].fValue * eTemp.vecValues[1].fValue)));
				if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
					sMultipleChoiceAnswer.bIsCorrect = false; 
					sMultipleChoiceAnswer.bIsSelected = false; 
					ssTemp.str("");
					ssTemp << iTemp;
					sMultipleChoiceAnswer.sValue = ssTemp.str();
					if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
						eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
					};
				};

                tmEnd = time (NULL);
                
			} while (eTemp.vecChoices.size() < 9 and (int)tmStart + iCutOffTime < (int)tmEnd); //while size
		} else {
			//answer is not area
			iTemp = (int)(2 * eTemp.vecAnswers[0].fValue);
			if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
				sMultipleChoiceAnswer.bIsCorrect = false; 
				sMultipleChoiceAnswer.bIsSelected = false; 
				ssTemp.str("");
				ssTemp << iTemp;
				sMultipleChoiceAnswer.sValue = ssTemp.str();
				if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
					eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
				};
			};
				
			iTemp = (int)(eTemp.vecAnswers[0].fValue / 2);
			if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
				sMultipleChoiceAnswer.bIsCorrect = false; 
				sMultipleChoiceAnswer.bIsSelected = false; 
				ssTemp.str("");
				ssTemp << iTemp;
				sMultipleChoiceAnswer.sValue = ssTemp.str();
				if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
					eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
				};
			};

			iTemp = (int)modMisc::iRandomNumber(1, 2 * (int)eTemp.vecAnswers[0].fValue);
			if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
				sMultipleChoiceAnswer.bIsCorrect = false; 
				sMultipleChoiceAnswer.bIsSelected = false; 
				ssTemp.str("");
				ssTemp << iTemp;
				sMultipleChoiceAnswer.sValue = ssTemp.str();
				if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
					eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
				};
			};
			
			iTemp = (int)modMisc::iRandomNumber(1, 3 * (int)eTemp.vecAnswers[0].fValue);
			if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
				sMultipleChoiceAnswer.bIsCorrect = false; 
				sMultipleChoiceAnswer.bIsSelected = false; 
				ssTemp.str("");
				ssTemp << iTemp;
				sMultipleChoiceAnswer.sValue = ssTemp.str();
				if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
					eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
				};
			};
			
			do {
				iTemp = (int)(eTemp.vecValues[0].fValue * modMisc::iRandomNumber(1, (int)eTemp.vecAnswers[0].fValue));
				if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
					sMultipleChoiceAnswer.bIsCorrect = false; 
					sMultipleChoiceAnswer.bIsSelected = false; 
					ssTemp.str("");
					ssTemp << iTemp;
					sMultipleChoiceAnswer.sValue = ssTemp.str();
					if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
						eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
					};
				};
					
				iTemp = (int)(eTemp.vecAnswers[0].fValue + modMisc::iRandomNumberNonZero((int)(-1 * (int)eTemp.vecValues[0].fValue), (int)eTemp.vecValues[0].fValue));
				if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
					sMultipleChoiceAnswer.bIsCorrect = false; 
					sMultipleChoiceAnswer.bIsSelected = false; 
					ssTemp.str("");
					ssTemp << iTemp;
					sMultipleChoiceAnswer.sValue = ssTemp.str();
					if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
						eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
					};
				};
				
				if ((int)(fabs(eTemp.vecAnswers[0].fValue) + fabs(eTemp.vecValues[0].fValue) + fabs(eTemp.vecValues[1].fValue)) < 10) {
					iTemp = (int)modMisc::iRandomNumber(0, 20);
					if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
						sMultipleChoiceAnswer.bIsCorrect = false; 
						sMultipleChoiceAnswer.bIsSelected = false; 
						ssTemp.str("");
						ssTemp << iTemp;
						sMultipleChoiceAnswer.sValue = ssTemp.str();
						if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
							eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
						};
					};
				} else {
					iTemp = (int)modMisc::iRandomNumber(0, (int)(fabs(eTemp.vecAnswers[0].fValue) + fabs(eTemp.vecValues[0].fValue) + fabs(eTemp.vecValues[1].fValue)));
					if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
						sMultipleChoiceAnswer.bIsCorrect = false; 
						sMultipleChoiceAnswer.bIsSelected = false; 
						ssTemp.str("");
						ssTemp << iTemp;
						sMultipleChoiceAnswer.sValue = ssTemp.str();
						if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
							eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
						};
					};
				};
				
				iTemp = (int)modMisc::iRandomNumberNonZero((int)(-1 * fabs(eTemp.vecValues[0].fValue) + fabs(eTemp.vecValues[1].fValue)), (int)(fabs(eTemp.vecValues[0].fValue) + fabs(eTemp.vecValues[1].fValue)));
				if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
					sMultipleChoiceAnswer.bIsCorrect = false; 
					sMultipleChoiceAnswer.bIsSelected = false; 
					ssTemp.str("");
					ssTemp << iTemp;
					sMultipleChoiceAnswer.sValue = ssTemp.str();
					if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
						eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
					};
				};
				
				iTemp = (int)modMisc::iRandomNumberNonZero((int)(-1 * fabs(eTemp.vecValues[0].fValue * eTemp.vecValues[1].fValue)), (int)(fabs(eTemp.vecValues[0].fValue * eTemp.vecValues[1].fValue)));
				if (iTemp != (int)eTemp.vecAnswers[0].fValue) {
					sMultipleChoiceAnswer.bIsCorrect = false; 
					sMultipleChoiceAnswer.bIsSelected = false; 
					ssTemp.str("");
					ssTemp << iTemp;
					sMultipleChoiceAnswer.sValue = ssTemp.str();
					if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
						eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
					};
				};
                
                tmEnd = time (NULL);
			} while (eTemp.vecChoices.size() < 9 and (int)tmStart + iCutOffTime < (int)tmEnd); //while size
		}; //if random i.e. area check
		
	} else {
		//NOT multiple choice
		eTemp.bIsMultipleChoice = false;
	};//if multiple choice
	

//	int iRandom;
//	strFloatValue fRandomFigure;
//	strQuestion eTemp;
//	int iTemp;
//	strStringAnswer sMultipleChoiceAnswer; 
//	vector<strStringAnswer>::iterator itMultipleChoiceAnswer; 
//	std::stringstream ssTemp;
//	int iRangeMax;
//	int iRangeMin;
//	int iDiff;
	sGroupName.clear();
//	time_t tmStart;

	return eTemp;
};

strQuestion ClsQuiz::randomQuestionType_Cylinder(){
	int iRandom;
	strFloatValue fRandomFigure;
	strQuestion eTemp;
	strStringAnswer sMultipleChoiceAnswer; 
	vector<strStringAnswer>::iterator itMultipleChoiceAnswer; 
	std::stringstream ssTemp;
	int iRangeMax;
	int iRangeMin;
	int iDiff;
	std::string sGroupName;
	time_t tmStart;
	time_t tmEnd;
	
	tmStart = time (NULL);
	
	eTemp.eQuestionGroup = eQuesGrpShapes;
	sGroupName = modMisc::convertEnum_QuesTypeGroupText(eQuesGrpShapes);
	
	eTemp.bHasPic = true;
	eTemp.ePic = ePicQues_Cylinder;
	
	eTemp.vecValues.clear();
	eTemp.vecAnswers.clear();
	eTemp.vecChoices.clear();
	
	
	const int gciQuestionTypeMax = 3; //note: needs to be the maximum integer in the enumQuestionType so that we can generate random question types 
	iRandom = (int)modMisc::iRandomNumber(1, gciQuestionTypeMax);

	if (cPreferences->getType() == ePrefByGroup or cPreferences->getType() == ePrefBySubject) {
		iDiff = (int)cPreferences->getDifficulty(&sGroupName);
	} else {
		iDiff = (int)modMisc::iRandomNumber(1, 3);
	};
	
	eTemp.eAnswerType = eAnsTypEquation;
	
	switch (iDiff) {
		case 1:
			eTemp.eQuestionType = eQuesCylinderVeryEasy;
			eTemp.iDifficulty = 450;
			
			iRangeMin = 1;
			iRangeMax = 6;
			
			break;
		case 2:
			eTemp.eQuestionType = eQuesCylinderEasy;
			eTemp.iDifficulty = 515;
			
			iRangeMin = 3;
			iRangeMax = 10;
			
			break;
		case 3:
			eTemp.eQuestionType = eQuesCylinderMedium;
			eTemp.iDifficulty = 565;
			
			iRangeMin = 8;
			iRangeMax = 15;
			
			break;
		case 4:
			eTemp.eQuestionType = eQuesCylinderHard;
			eTemp.iDifficulty = 600;
			
			iRangeMin = 10;
			iRangeMax = 18;
			
			break;
		case 5:
			eTemp.eQuestionType = eQuesCylinderVeryHard;
			eTemp.iDifficulty = 635;
			
			iRangeMin = 15;
			iRangeMax = 25;
			
			break;
		default:
			break;
	}; //switch iDiff
	

	switch (iRandom){ 
		case 1:
			eTemp.sQuestionEncoded = "Height = <0>, Radius = <1>\n What is the volume of this cylinder?";
			
			fRandomFigure.fValue = (float)modMisc::iRandomNumber(iRangeMin, iRangeMax);
			eTemp.vecValues.push_back(fRandomFigure);
			fRandomFigure.fValue = (float)modMisc::iRandomNumber(iRangeMin, iRangeMax);
			eTemp.vecValues.push_back(fRandomFigure);
						
			//Correct answer
			eTemp.bIsMultipleChoice = true;
			eTemp.bOnlyOneAnswer = true;
			sMultipleChoiceAnswer.bIsCorrect = true; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << (int)(eTemp.vecValues[0].fValue * eTemp.vecValues[1].fValue * eTemp.vecValues[1].fValue) << "<pi>";
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			
			eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
			
			
			//wrong answers

			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << (int)(eTemp.vecValues[0].fValue * eTemp.vecValues[1].fValue) << "<pi>";
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (sMultipleChoiceAnswer.sValue.c_str() != eTemp.vecChoices[0].sValue.c_str()) {
				if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
					eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
				};
			};
			
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << (int)(eTemp.vecValues[0].fValue + eTemp.vecValues[1].fValue) << "<pi>";
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (sMultipleChoiceAnswer.sValue.c_str() != eTemp.vecChoices[0].sValue.c_str()) {
				if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
					eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
				};
			};
			
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << (int)(eTemp.vecValues[0].fValue * eTemp.vecValues[1].fValue) << "<pi><^2>";
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (sMultipleChoiceAnswer.sValue.c_str() != eTemp.vecChoices[0].sValue.c_str()) {
				if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
					eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
				};
			};
			
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << (int)(eTemp.vecValues[0].fValue * eTemp.vecValues[1].fValue * eTemp.vecValues[1].fValue) << "<pi><^2>";
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (sMultipleChoiceAnswer.sValue.c_str() != eTemp.vecChoices[0].sValue.c_str()) {
				if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
					eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
				};
			};
			
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << (int)(eTemp.vecValues[0].fValue * eTemp.vecValues[1].fValue) << "<pi><^3>";
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (sMultipleChoiceAnswer.sValue.c_str() != eTemp.vecChoices[0].sValue.c_str()) {
				if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
					eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
				};
			};
			
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << (int)(eTemp.vecValues[0].fValue * eTemp.vecValues[1].fValue * eTemp.vecValues[1].fValue) << "<pi><^3>";
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (sMultipleChoiceAnswer.sValue.c_str() != eTemp.vecChoices[0].sValue.c_str()) {
				if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
					eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
				};
			};
			
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << (int)(eTemp.vecValues[0].fValue * eTemp.vecValues[1].fValue) << " <^1/3><pi>";
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (sMultipleChoiceAnswer.sValue.c_str() != eTemp.vecChoices[0].sValue.c_str()) {
				if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
					eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
				};
			};
			
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << (int)(eTemp.vecValues[0].fValue * eTemp.vecValues[1].fValue * eTemp.vecValues[1].fValue) << " <^1/3><pi>";
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (sMultipleChoiceAnswer.sValue.c_str() != eTemp.vecChoices[0].sValue.c_str()) {
				if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
					eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
				};
			};

			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << (int)(eTemp.vecValues[0].fValue * eTemp.vecValues[0].fValue * eTemp.vecValues[1].fValue) << "/<pi>";
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (sMultipleChoiceAnswer.sValue.c_str() != eTemp.vecChoices[0].sValue.c_str()) {
				if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
					eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
				};
			};
			
			do {
                sMultipleChoiceAnswer.bIsCorrect = false; 
                sMultipleChoiceAnswer.bIsSelected = false; 
                ssTemp.str("");
                ssTemp << (int)(eTemp.vecValues[0].fValue * modMisc::iRandomNumber(1, (eTemp.vecValues[0].fValue + eTemp.vecValues[1].fValue))) << "<pi><^2>";
                sMultipleChoiceAnswer.sValue = ssTemp.str();
                if (sMultipleChoiceAnswer.sValue.c_str() != eTemp.vecChoices[0].sValue.c_str()) {
                    if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                        eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
                    };
                };
                
                sMultipleChoiceAnswer.bIsCorrect = false; 
                sMultipleChoiceAnswer.bIsSelected = false; 
                ssTemp.str("");
                ssTemp << (int)(eTemp.vecValues[0].fValue * modMisc::iRandomNumber(1, (eTemp.vecValues[0].fValue + eTemp.vecValues[1].fValue))) << "<pi><^3>";
                sMultipleChoiceAnswer.sValue = ssTemp.str();
                if (sMultipleChoiceAnswer.sValue.c_str() != eTemp.vecChoices[0].sValue.c_str()) {
                    if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                        eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
                    };
                };
                
                sMultipleChoiceAnswer.bIsCorrect = false; 
                sMultipleChoiceAnswer.bIsSelected = false; 
                ssTemp.str("");
                ssTemp << (int)(eTemp.vecValues[0].fValue * modMisc::iRandomNumber(1, (eTemp.vecValues[0].fValue + eTemp.vecValues[1].fValue))) << " <^1/2><pi>";
                sMultipleChoiceAnswer.sValue = ssTemp.str();
                if (sMultipleChoiceAnswer.sValue.c_str() != eTemp.vecChoices[0].sValue.c_str()) {
                    if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                        eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
                    };
                };

                sMultipleChoiceAnswer.bIsCorrect = false; 
                sMultipleChoiceAnswer.bIsSelected = false; 
                ssTemp.str("");
                ssTemp << (int)(eTemp.vecValues[0].fValue * modMisc::iRandomNumber(1, (eTemp.vecValues[0].fValue + eTemp.vecValues[1].fValue))) << " <^1/3><pi>";
                sMultipleChoiceAnswer.sValue = ssTemp.str();
                if (sMultipleChoiceAnswer.sValue.c_str() != eTemp.vecChoices[0].sValue.c_str()) {
                    if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                        eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
                    };
                };
                

                sMultipleChoiceAnswer.bIsCorrect = false; 
                sMultipleChoiceAnswer.bIsSelected = false; 
                ssTemp.str("");
                ssTemp << (int)(eTemp.vecValues[0].fValue * modMisc::iRandomNumber(1, (eTemp.vecValues[0].fValue + eTemp.vecValues[1].fValue))) << "/<pi>";
                sMultipleChoiceAnswer.sValue = ssTemp.str();
                if (sMultipleChoiceAnswer.sValue.c_str() != eTemp.vecChoices[0].sValue.c_str()) {
                    if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                        eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
                    };
                };
                
                sMultipleChoiceAnswer.bIsCorrect = false; 
                sMultipleChoiceAnswer.bIsSelected = false; 
                ssTemp.str("");
                ssTemp << (int)(eTemp.vecValues[0].fValue * modMisc::iRandomNumber(1, (eTemp.vecValues[0].fValue + eTemp.vecValues[1].fValue))) << "/<pi><^2>";
                sMultipleChoiceAnswer.sValue = ssTemp.str();
                if (sMultipleChoiceAnswer.sValue.c_str() != eTemp.vecChoices[0].sValue.c_str()) {
                    if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                        eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
                    };
                };
                
				sMultipleChoiceAnswer.bIsCorrect = false; 
				sMultipleChoiceAnswer.bIsSelected = false; 
				ssTemp.str("");
				ssTemp << (int)(eTemp.vecValues[1].fValue * modMisc::iRandomNumber(1, (eTemp.vecValues[0].fValue + eTemp.vecValues[1].fValue))) << "<pi>";
				sMultipleChoiceAnswer.sValue = ssTemp.str();
				if (sMultipleChoiceAnswer.sValue.c_str() != eTemp	.vecChoices[0].sValue.c_str()) {
					if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
						eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
					};
				};
				
				sMultipleChoiceAnswer.bIsCorrect = false; 
				sMultipleChoiceAnswer.bIsSelected = false; 
				ssTemp.str("");
				ssTemp << (int)(eTemp.vecValues[0].fValue * modMisc::iRandomNumber(1, (eTemp.vecValues[0].fValue + eTemp.vecValues[1].fValue))) << "<pi>";
				sMultipleChoiceAnswer.sValue = ssTemp.str();
				if (sMultipleChoiceAnswer.sValue.c_str() != eTemp.vecChoices[0].sValue.c_str()) {
					if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
						eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
					};
				};

                tmEnd = time (NULL);
			} while (eTemp.vecChoices.size() < 10 and (int)tmStart + iCutOffTime < (int)tmEnd);
				
			break;
		case 2:
			eTemp.sQuestionEncoded = "Radius = <0>, Volume = <1><pi>\n What is the height?";
				
			fRandomFigure.fValue = (float)modMisc::iRandomNumber(iRangeMin, iRangeMax);
			eTemp.vecValues.push_back(fRandomFigure);
			fRandomFigure.fValue = (float)modMisc::iRandomNumber(iRangeMin, iRangeMax);
			eTemp.vecAnswers.push_back(fRandomFigure);
			
			fRandomFigure.fValue = (float)(eTemp.vecValues[0].fValue * eTemp.vecValues[0].fValue * eTemp.vecAnswers[0].fValue);
			eTemp.vecValues.push_back(fRandomFigure);

			
			
			
			
			
			//Correct answer

			eTemp.bIsMultipleChoice = true;
			eTemp.bOnlyOneAnswer = true;
			sMultipleChoiceAnswer.bIsCorrect = true; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << (int)eTemp.vecValues[1].fValue << "/" << (int)(eTemp.vecValues[0].fValue * eTemp.vecValues[0].fValue);
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			
			eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
			

			//wrong answers
			
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << (int)eTemp.vecValues[0].fValue << "/" << (int)(eTemp.vecValues[1].fValue * eTemp.vecValues[1].fValue);
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (sMultipleChoiceAnswer.sValue.c_str() != eTemp.vecChoices[0].sValue.c_str()) {
				if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
					eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
				};
			};
			
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << (int)eTemp.vecValues[1].fValue << "/" << (int)eTemp.vecValues[0].fValue << "<pi>";
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (sMultipleChoiceAnswer.sValue.c_str() != eTemp.vecChoices[0].sValue.c_str()) {
				if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
					eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
				};
			};
						
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << (int)eTemp.vecValues[1].fValue << "/" << (int)eTemp.vecValues[0].fValue;
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (sMultipleChoiceAnswer.sValue.c_str() != eTemp.vecChoices[0].sValue.c_str()) {
				if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
					eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
				};
			};
			
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << (int)eTemp.vecValues[1].fValue << "/" << (int)eTemp.vecValues[0].fValue << "<pi><^2>";
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (sMultipleChoiceAnswer.sValue.c_str() != eTemp.vecChoices[0].sValue.c_str()) {
				if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
					eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
				};
			};
			
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << (int)eTemp.vecValues[0].fValue << "<pi>/" << (int)(eTemp.vecValues[1].fValue * eTemp.vecValues[1].fValue);
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (sMultipleChoiceAnswer.sValue.c_str() != eTemp.vecChoices[0].sValue.c_str()) {
				if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
					eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
				};
			};
			
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << "<^1/2>" << (int)(eTemp.vecValues[0].fValue * eTemp.vecValues[1].fValue * eTemp.vecValues[1].fValue);
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (sMultipleChoiceAnswer.sValue.c_str() != eTemp.vecChoices[0].sValue.c_str()) {
				if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
					eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
				};
			};
			
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << (int)eTemp.vecValues[1].fValue << "/" << (int)(eTemp.vecValues[0].fValue + eTemp.vecValues[0].fValue) << "<pi>";
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (sMultipleChoiceAnswer.sValue.c_str() != eTemp.vecChoices[0].sValue.c_str()) {
				if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
					eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
				};
			};
			
			do {
				sMultipleChoiceAnswer.bIsCorrect = false; 
				sMultipleChoiceAnswer.bIsSelected = false; 
				ssTemp.str("");
				ssTemp << modMisc::iRandomNumber(1, (int)(2 * eTemp.vecValues[1].fValue)) << "/" << (int)(eTemp.vecValues[0].fValue * eTemp.vecValues[0].fValue) << "<pi>";
				sMultipleChoiceAnswer.sValue = ssTemp.str();
				if (sMultipleChoiceAnswer.sValue.c_str() != eTemp.vecChoices[0].sValue.c_str()) {
					if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
						eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
					};
				};

			
			
				sMultipleChoiceAnswer.bIsCorrect = false; 
				sMultipleChoiceAnswer.bIsSelected = false; 
				ssTemp.str("");
				ssTemp << (int)eTemp.vecValues[1].fValue << "/" << modMisc::iRandomNumber(1, (int)(2 * eTemp.vecValues[0].fValue * eTemp.vecValues[0].fValue)) << "<pi>";
				sMultipleChoiceAnswer.sValue = ssTemp.str();
				if (sMultipleChoiceAnswer.sValue.c_str() != eTemp.vecChoices[0].sValue.c_str()) {
					if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
						eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
					};
				};
				
				sMultipleChoiceAnswer.bIsCorrect = false; 
				sMultipleChoiceAnswer.bIsSelected = false; 
				ssTemp.str("");
				ssTemp << modMisc::iRandomNumber(1, (int)(2 * eTemp.vecValues[1].fValue)) << "/" << (int)(eTemp.vecValues[0].fValue * eTemp.vecValues[0].fValue);
				sMultipleChoiceAnswer.sValue = ssTemp.str();
				if (sMultipleChoiceAnswer.sValue.c_str() != eTemp.vecChoices[0].sValue.c_str()) {
					if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
						eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
					};
				};
				
				
				
				sMultipleChoiceAnswer.bIsCorrect = false; 
				sMultipleChoiceAnswer.bIsSelected = false; 
				ssTemp.str("");
				ssTemp << (int)eTemp.vecValues[1].fValue << "/" << modMisc::iRandomNumber(1, (int)(2 * eTemp.vecValues[0].fValue * eTemp.vecValues[0].fValue));
				sMultipleChoiceAnswer.sValue = ssTemp.str();
				if (sMultipleChoiceAnswer.sValue.c_str() != eTemp.vecChoices[0].sValue.c_str()) {
					if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
						eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
					};
				};
                
                tmEnd = time (NULL);
			} while (eTemp.vecChoices.size() < 15 and (int)tmStart + iCutOffTime < (int)tmEnd);

			break;
		case 3:
			eTemp.sQuestionEncoded = "Height = <0>, Volume = <pi><1>\n What is the radius?";
			
			fRandomFigure.fValue = (float)modMisc::iRandomNumber(iRangeMin, iRangeMax);
			eTemp.vecValues.push_back(fRandomFigure);
			fRandomFigure.fValue = (float)modMisc::iRandomNumber(iRangeMin, iRangeMax);
			eTemp.vecAnswers.push_back(fRandomFigure);
			
			fRandomFigure.fValue = (float)(eTemp.vecValues[0].fValue * eTemp.vecAnswers[0].fValue);
			eTemp.vecValues.push_back(fRandomFigure);

			
			eTemp.bIsMultipleChoice = true;
			eTemp.bOnlyOneAnswer = true;
			sMultipleChoiceAnswer.bIsCorrect = true; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << "<^1/2>(" << (int)eTemp.vecValues[1].fValue << "/" << (int)eTemp.vecValues[0].fValue << ")";
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			
			eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
			
			
			//wrong answers
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << "<^1/2>(" << (int)eTemp.vecValues[1].fValue << "/" << (int)eTemp.vecValues[0].fValue << "<pi>)";
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (sMultipleChoiceAnswer.sValue.c_str() != eTemp.vecChoices[0].sValue.c_str()) {
				if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
					eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
				};
			};
			
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << "<^1/2>(" << (int)eTemp.vecValues[0].fValue << "/" << (int)eTemp.vecValues[1].fValue << "<pi>)";
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (sMultipleChoiceAnswer.sValue.c_str() != eTemp.vecChoices[0].sValue.c_str()) {
				if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
					eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
				};
			};
			
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << "<^1/2>(" << (int)eTemp.vecValues[0].fValue << "/" << (int)eTemp.vecValues[1].fValue << ")";
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (sMultipleChoiceAnswer.sValue.c_str() != eTemp.vecChoices[0].sValue.c_str()) {
				if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
					eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
				};
			};
			
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << "<^1/3>(" << (int)eTemp.vecValues[1].fValue << "/" << (int)eTemp.vecValues[0].fValue << "<pi>)";
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (sMultipleChoiceAnswer.sValue.c_str() != eTemp.vecChoices[0].sValue.c_str()) {
				if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
					eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
				};
			};
			
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << "<^1/3>(" << (int)eTemp.vecValues[1].fValue << "/" << (int)eTemp.vecValues[0].fValue << ")";
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (sMultipleChoiceAnswer.sValue.c_str() != eTemp.vecChoices[0].sValue.c_str()) {
				if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
					eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
				};
			};
			
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << "(" << (int)eTemp.vecValues[1].fValue << "/" << (int)eTemp.vecValues[0].fValue << ")<^2>";
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (sMultipleChoiceAnswer.sValue.c_str() != eTemp.vecChoices[0].sValue.c_str()) {
				if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
					eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
				};
			};
			
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << "(" << (int)eTemp.vecValues[0].fValue << "/" << (int)eTemp.vecValues[1].fValue << "<pi>)<^2>";
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (sMultipleChoiceAnswer.sValue.c_str() != eTemp.vecChoices[0].sValue.c_str()) {
				if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
					eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
				};
			};
			
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << "(" << (int)eTemp.vecValues[0].fValue << "/" << (int)eTemp.vecValues[1].fValue << ")<^2>";
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (sMultipleChoiceAnswer.sValue.c_str() != eTemp.vecChoices[0].sValue.c_str()) {
				if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
					eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
				};
			};
			
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << "<^1/2>(" << (int)eTemp.vecValues[1].fValue << "<pi>/" << (int)eTemp.vecValues[0].fValue << ")";
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (sMultipleChoiceAnswer.sValue.c_str() != eTemp.vecChoices[0].sValue.c_str()) {
				if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
					eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
				};
			};
			
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << "<^1/2>(" << (int)eTemp.vecValues[0].fValue << "<pi>/" << (int)eTemp.vecValues[1].fValue << ")";
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (sMultipleChoiceAnswer.sValue.c_str() != eTemp.vecChoices[0].sValue.c_str()) {
				if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
					eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
				};
			};
			
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << "<pi><^1/2>(" << (int)eTemp.vecValues[1].fValue << "/" << (int)eTemp.vecValues[0].fValue << ")";
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (sMultipleChoiceAnswer.sValue.c_str() != eTemp.vecChoices[0].sValue.c_str()) {
				if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
					eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
				};
			};
			
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << "<^1/2>(" << (int)eTemp.vecValues[1].fValue << "/" << (int)eTemp.vecValues[0].fValue << ")";
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (sMultipleChoiceAnswer.sValue.c_str() != eTemp.vecChoices[0].sValue.c_str()) {
				if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
					eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
				};
			};
			
			break;
		default:
			eTemp.eQuestionType = eQuesError;
			eTemp.bIsMultipleChoice = false;
			eTemp.sQuestionEncoded = "Error:";
			break;
	};//switch iRandom
	
	
	
//	int iRandom;
//	strFloatValue fRandomFigure;
//	strQuestion eTemp;
//	int iTemp;
//	strStringAnswer sMultipleChoiceAnswer; 
//	vector<strStringAnswer>::iterator itMultipleChoiceAnswer; 
//	std::stringstream ssTemp;
//	int iRangeMax;
//	int iRangeMin;
//	int iDiff;
	sGroupName.clear();
//	time_t tmStart;
	
	return eTemp;
};



strQuestion ClsQuiz::randomQuestionType_Differentiation(){
	/*
	do all the different types of differetiation here so when the user click on the preferences
	there is only one option for differentiation and in this code everything works ok 
	*/
	int iRandom;
	strFloatValue fRandomFigure;
	strQuestion eTemp;
	int iTemp;
	strStringAnswer sMultipleChoiceAnswer; 
	vector<strStringAnswer>::iterator itMultipleChoiceAnswer; 
	std::stringstream ssTemp;
	int iRangeMax;
	int iRangeMin;
	int iDiff;
	int iType;
	enumFnTrig eTempFnTrig;
	
	eTemp.bHasPic = false;
	eTemp.ePic = ePic_None;
	
	eTemp.vecValues.clear();
	eTemp.vecAnswers.clear();
	eTemp.vecChoices.clear();

	iType = modMisc::iRandomNumber(1, 5);
	//1) base 2 ploynomial
	//2) base 3 ploynomial
	//3) Exponenial function
	//4) Trig function
	//5) Chain rule
	
	const int gciQuestionTypeMax = 3; //note: needs to be the maximum integer in the enumQuestionType so that we can generate random question types 
	iRandom = (int)modMisc::iRandomNumber(1, gciQuestionTypeMax);
	iDiff = (int)modMisc::iRandomNumber(1, 3);
	
	eTemp.eAnswerType = eAnsTypNumeric;
	
	switch (iType) {
		case 1:
			//1) base 2 ploynomial
			switch (iDiff) {
				case 1:
					eTemp.eQuestionType = eQuesDifferentiation2BasePolyVeryEasy;
					eTemp.iDifficulty = 600;
					
					iRangeMin = 1;
					iRangeMax = 6;
					
					break;
				case 2:
					eTemp.eQuestionType = eQuesDifferentiation2BasePolyEasy;
					eTemp.iDifficulty = 625;
					
					iRangeMin = 3;
					iRangeMax = 10;
					
					break;
				case 3:
					eTemp.eQuestionType = eQuesDifferentiation2BasePolyMedium;
					eTemp.iDifficulty = 650;
					
					iRangeMin = 8;
					iRangeMax = 15;
					
					break;
				case 4:
					eTemp.eQuestionType = eQuesDifferentiation2BasePolyHard;
					eTemp.iDifficulty = 675;
					
					iRangeMin = 10;
					iRangeMax = 18;
					
					break;
				case 5:
					eTemp.eQuestionType = eQuesDifferentiation2BasePolyVeryHard;
					eTemp.iDifficulty = 700;
					
					iRangeMin = 15;
					iRangeMax = 25;
					
					break;
				default:
					break;
			}; //switch iDiff
			
			eTemp.sQuestionEncoded = "f(x) = <0>x<^2> + <1>x + <2>x\nWhat is the differenial of the function f?";
			
			fRandomFigure.fValue = (float)modMisc::iRandomNumber(iRangeMin, iRangeMax);
			eTemp.vecValues.push_back(fRandomFigure);
			fRandomFigure.fValue = (float)modMisc::iRandomNumber(iRangeMin, iRangeMax);
			eTemp.vecValues.push_back(fRandomFigure);
			fRandomFigure.fValue = (float)modMisc::iRandomNumber(iRangeMin, iRangeMax);
			eTemp.vecValues.push_back(fRandomFigure);

			break;
		case 2:
			//2) base 3 ploynomial
			break;
		case 3:
			//3) Exponenial function
			break;
		case 4:
			//4) Trig function
			break;
		case 5:
			//5) Chain rule
			break;
		default:
			break;
	}
	
	
	
	switch (iDiff) {
		case 1:
			eTemp.eQuestionType = eQuesDifferentiation2BasePolyVeryEasy;
			eTemp.iDifficulty = 600;
			
			iRangeMin = 1;
			iRangeMax = 6;
			
			break;
		case 2:
			eTemp.eQuestionType = eQuesCylinderEasy;
			eTemp.iDifficulty = 400;
			
			iRangeMin = 3;
			iRangeMax = 10;
			
			break;
		case 3:
			eTemp.eQuestionType = eQuesCylinderMedium;
			eTemp.iDifficulty = 500;
			
			iRangeMin = 8;
			iRangeMax = 15;
			
			break;
		case 4:
			eTemp.eQuestionType = eQuesCylinderHard;
			eTemp.iDifficulty = 600;
			
			iRangeMin = 10;
			iRangeMax = 18;
			
			break;
		case 5:
			eTemp.eQuestionType = eQuesCylinderVeryHard;
			eTemp.iDifficulty = 500;
			
			iRangeMin = 15;
			iRangeMax = 25;
			
			break;
		default:
			break;
	}; //switch iDiff
	
	switch (iRandom){ 
		case 1:
			eTemp.sQuestionEncoded = "Height = <0>, Radius = <1>\n What is the volume of this cylinder?";
			
			fRandomFigure.fValue = (float)modMisc::iRandomNumber(iRangeMin, iRangeMax);
			eTemp.vecValues.push_back(fRandomFigure);
			fRandomFigure.fValue = (float)modMisc::iRandomNumber(iRangeMin, iRangeMax);
			eTemp.vecValues.push_back(fRandomFigure);
			
			//Correct answer
			eTemp.bIsMultipleChoice = true;
			eTemp.bOnlyOneAnswer = true;
			sMultipleChoiceAnswer.bIsCorrect = true; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << (int)(eTemp.vecValues[0].fValue * eTemp.vecValues[1].fValue * eTemp.vecValues[1].fValue) << "<pi>";
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			
			eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
			
			
			//wrong answers
			
			
			
			
			
			
			do {

			} while (eTemp.vecChoices.size() < 10);
			
			break;
		case 2:
			eTemp.sQuestionEncoded = "Radius = <0>, Volume = <1><pi>\n What is the height?";
			
			fRandomFigure.fValue = (float)modMisc::iRandomNumber(iRangeMin, iRangeMax);
			eTemp.vecValues.push_back(fRandomFigure);
			fRandomFigure.fValue = (float)modMisc::iRandomNumber(iRangeMin, iRangeMax);
			eTemp.vecAnswers.push_back(fRandomFigure);
			
			fRandomFigure.fValue = (float)(eTemp.vecValues[0].fValue * eTemp.vecValues[0].fValue * eTemp.vecAnswers[0].fValue);
			eTemp.vecValues.push_back(fRandomFigure);
			
			
			
			
			
			
			//Correct answer
			
			eTemp.bIsMultipleChoice = true;
			eTemp.bOnlyOneAnswer = true;
			sMultipleChoiceAnswer.bIsCorrect = true; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << (int)eTemp.vecValues[1].fValue << "/" << (int)(eTemp.vecValues[0].fValue * eTemp.vecValues[0].fValue);
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			
			eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
			
			
			//wrong answers
			
			
			
			do {
				
				
				

			} while (eTemp.vecChoices.size() < 15);
			
			break;
		case 3:
			eTemp.sQuestionEncoded = "Height = <0>, Volume = <pi><1>\n What is the radius?";
			
			fRandomFigure.fValue = (float)modMisc::iRandomNumber(iRangeMin, iRangeMax);
			eTemp.vecValues.push_back(fRandomFigure);
			fRandomFigure.fValue = (float)modMisc::iRandomNumber(iRangeMin, iRangeMax);
			eTemp.vecAnswers.push_back(fRandomFigure);
			
			fRandomFigure.fValue = (float)(eTemp.vecValues[0].fValue * eTemp.vecAnswers[0].fValue);
			eTemp.vecValues.push_back(fRandomFigure);
			
			
			eTemp.bIsMultipleChoice = true;
			eTemp.bOnlyOneAnswer = true;
			sMultipleChoiceAnswer.bIsCorrect = true; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << "<^1/2>(" << (int)eTemp.vecValues[1].fValue << "/" << (int)eTemp.vecValues[0].fValue << ")";
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			
			eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
			
			
			//wrong answers

			

			

			break;
		case -66:
			eTemp.eQuestionType = eQuesDifferentiation2BasePoly;
			eTemp.sQuestionEncoded = "<0>x<^2> + <1> x + <2> = 0"; 
			eTemp.bIsMultipleChoice = false;
			eTemp.iDifficulty = 110;
			eTemp.eAnswerType = eAnsTypNumeric;
			eTemp.bHasPic = false;
			
			fRandomFigure.fValue = (float)modMisc::iRandomNumberNonZero(-9, 9);
			eTemp.vecValues.push_back(fRandomFigure);
			fRandomFigure.fValue = (float)modMisc::iRandomNumberNonZero(-9, 9) * eTemp.vecValues[0].fValue;
			eTemp.vecAnswers.push_back(fRandomFigure);
			fRandomFigure.fValue = (float)modMisc::iRandomNumberNonZero(-9, 9) * eTemp.vecValues[0].fValue;
			eTemp.vecAnswers.push_back(fRandomFigure);
			
			fRandomFigure.fValue = (float)((eTemp.vecAnswers[0].fValue + eTemp.vecAnswers[1].fValue) / eTemp.vecValues[0].fValue);
			eTemp.vecValues.push_back(fRandomFigure);
			fRandomFigure.fValue = (float)((eTemp.vecAnswers[0].fValue * eTemp.vecAnswers[1].fValue) / eTemp.vecValues[0].fValue);
			eTemp.vecValues.push_back(fRandomFigure);
			
			
			
			if (modMisc::iRandomNumber(0, 10) < 5) {
				//multiple choice
				
				//Correct answer
				eTemp.bIsMultipleChoice = true;
				eTemp.bOnlyOneAnswer = true;
				sMultipleChoiceAnswer.bIsCorrect = true; 
				sMultipleChoiceAnswer.bIsSelected = false; 
				ssTemp.str("");
				ssTemp << (float)eTemp.vecAnswers[0].fValue;
				sMultipleChoiceAnswer.sValue = ssTemp.str();
				
				eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
				
				
				eTemp.bIsMultipleChoice = true;
				eTemp.bOnlyOneAnswer = true;
				sMultipleChoiceAnswer.bIsCorrect = true; 
				sMultipleChoiceAnswer.bIsSelected = false; 
				ssTemp.str("");
				ssTemp << (float)eTemp.vecAnswers[1].fValue;
				sMultipleChoiceAnswer.sValue = ssTemp.str();
				
				eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
				
				
				//wrong answers
				iTemp = (int)(eTemp.vecValues[0].fValue + eTemp.vecValues[1].fValue + 1);
				if (iTemp != (int)eTemp.vecAnswers[0].fValue and iTemp != (int)eTemp.vecAnswers[1].fValue) {
					sMultipleChoiceAnswer.bIsCorrect = false; 
					sMultipleChoiceAnswer.bIsSelected = false; 
					ssTemp.str("");
					ssTemp << iTemp;
					sMultipleChoiceAnswer.sValue = ssTemp.str();
					if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
						eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
					};
				};
				
				iTemp = (int)(eTemp.vecValues[1].fValue - eTemp.vecValues[0].fValue);
				if (iTemp != (int)eTemp.vecAnswers[0].fValue and iTemp != (int)eTemp.vecAnswers[1].fValue) {
					sMultipleChoiceAnswer.bIsCorrect = false; 
					sMultipleChoiceAnswer.bIsSelected = false; 
					ssTemp.str("");
					ssTemp << iTemp;
					sMultipleChoiceAnswer.sValue = ssTemp.str();
					if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
						eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
					};
				};
				
				iTemp = (int)(eTemp.vecValues[0].fValue * eTemp.vecValues[0].fValue);
				if (iTemp != (int)eTemp.vecAnswers[0].fValue and iTemp != (int)eTemp.vecAnswers[1].fValue) {
					sMultipleChoiceAnswer.bIsCorrect = false; 
					sMultipleChoiceAnswer.bIsSelected = false; 
					ssTemp.str("");
					ssTemp << iTemp;
					sMultipleChoiceAnswer.sValue = ssTemp.str();
					if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
						eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
					};
				};
				
				iTemp = (int)(eTemp.vecValues[0].fValue / 2);
				if (iTemp != (int)eTemp.vecAnswers[0].fValue and iTemp != (int)eTemp.vecAnswers[1].fValue) {
					sMultipleChoiceAnswer.bIsCorrect = false; 
					sMultipleChoiceAnswer.bIsSelected = false; 
					ssTemp.str("");
					ssTemp << iTemp;
					sMultipleChoiceAnswer.sValue = ssTemp.str();
					if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
						eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
					};
				};
				
				iTemp = (int)(eTemp.vecValues[1].fValue / 2);
				if (iTemp != (int)eTemp.vecAnswers[0].fValue and iTemp != (int)eTemp.vecAnswers[1].fValue) {
					sMultipleChoiceAnswer.bIsCorrect = false; 
					sMultipleChoiceAnswer.bIsSelected = false; 
					ssTemp.str("");
					ssTemp << iTemp;
					sMultipleChoiceAnswer.sValue = ssTemp.str();
					if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
						eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
					};
				};
				
				iTemp = (int)(eTemp.vecValues[0].fValue * eTemp.vecValues[0].fValue);
				if (iTemp != (int)eTemp.vecAnswers[0].fValue and iTemp != (int)eTemp.vecAnswers[1].fValue) {
					sMultipleChoiceAnswer.bIsCorrect = false; 
					sMultipleChoiceAnswer.bIsSelected = false; 
					ssTemp.str("");
					ssTemp << iTemp;
					sMultipleChoiceAnswer.sValue = ssTemp.str();
					if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
						eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
					};
				};
				
				iTemp = (int)eTemp.vecValues[0].fValue;
				if (iTemp != (int)eTemp.vecAnswers[0].fValue and iTemp != (int)eTemp.vecAnswers[1].fValue) {
					sMultipleChoiceAnswer.bIsCorrect = false; 
					sMultipleChoiceAnswer.bIsSelected = false; 
					ssTemp.str("");
					ssTemp << iTemp;
					sMultipleChoiceAnswer.sValue = ssTemp.str();
					if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
						eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
					};
				};
				
				iTemp = (int)eTemp.vecValues[1].fValue;
				if (iTemp != (int)eTemp.vecAnswers[0].fValue and iTemp != (int)eTemp.vecAnswers[1].fValue) {
					sMultipleChoiceAnswer.bIsCorrect = false; 
					sMultipleChoiceAnswer.bIsSelected = false; 
					ssTemp.str("");
					ssTemp << iTemp;
					sMultipleChoiceAnswer.sValue = ssTemp.str();
					if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
						eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
					};
				};
				
				iTemp = (int)eTemp.vecValues[2].fValue;
				if (iTemp != (int)eTemp.vecAnswers[0].fValue and iTemp != (int)eTemp.vecAnswers[1].fValue) {
					sMultipleChoiceAnswer.bIsCorrect = false; 
					sMultipleChoiceAnswer.bIsSelected = false; 
					ssTemp.str("");
					ssTemp << iTemp;
					sMultipleChoiceAnswer.sValue = ssTemp.str();
					if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
						eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
					};
				};
				
				iTemp = modMisc::iRandomNumber(2, 15);
				if (iTemp != (int)eTemp.vecAnswers[0].fValue and iTemp != (int)eTemp.vecAnswers[1].fValue) {
					sMultipleChoiceAnswer.bIsCorrect = false; 
					sMultipleChoiceAnswer.bIsSelected = false; 
					ssTemp.str("");
					ssTemp << iTemp;
					sMultipleChoiceAnswer.sValue = ssTemp.str();
					if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
						eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
					};
				};
				
				iTemp = modMisc::iRandomNumber(2, 9);
				if (iTemp != (int)eTemp.vecAnswers[0].fValue and iTemp != (int)eTemp.vecAnswers[1].fValue) {
					sMultipleChoiceAnswer.bIsCorrect = false; 
					sMultipleChoiceAnswer.bIsSelected = false; 
					ssTemp.str("");
					ssTemp << iTemp;
					sMultipleChoiceAnswer.sValue = ssTemp.str();
					if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
						eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
					};
				};
				
				iTemp = (int)(eTemp.vecValues[0].fValue + eTemp.vecValues[1].fValue) + 10;
				if (iTemp != (int)eTemp.vecAnswers[0].fValue and iTemp != (int)eTemp.vecAnswers[1].fValue) {
					sMultipleChoiceAnswer.bIsCorrect = false; 
					sMultipleChoiceAnswer.bIsSelected = false; 
					ssTemp.str("");
					ssTemp << iTemp;
					sMultipleChoiceAnswer.sValue = ssTemp.str();
					if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
						eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
					};
				};
				
				iTemp = (int)(eTemp.vecValues[0].fValue * eTemp.vecValues[1].fValue) - 10;
				if (iTemp != (int)eTemp.vecAnswers[0].fValue and iTemp != (int)eTemp.vecAnswers[1].fValue) {
					sMultipleChoiceAnswer.bIsCorrect = false; 
					sMultipleChoiceAnswer.bIsSelected = false; 
					ssTemp.str("");
					ssTemp << iTemp;
					sMultipleChoiceAnswer.sValue = ssTemp.str();
					if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
						eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
					};
				};
				
			} else {
				//NOT multiple choice
				eTemp.bIsMultipleChoice = false;
			};
			break;
		case -666:
			eTemp.eQuestionType = eQuesDifferentiation3BasePoly;
			eTemp.sQuestionEncoded = "<0>x<^3> + <1><x^2> + <2>x + <3> = 0"; 
			eTemp.bIsMultipleChoice = false;
			eTemp.iDifficulty = 120;
			eTemp.eAnswerType = eAnsTypNumeric;
			eTemp.bHasPic = false;
			
			fRandomFigure.fValue = (float)modMisc::iRandomNumberNonZero(-9, 9);
			eTemp.vecValues.push_back(fRandomFigure);
			fRandomFigure.fValue = (float)modMisc::iRandomNumberNonZero(-9, 9) * eTemp.vecValues[0].fValue;
			eTemp.vecAnswers.push_back(fRandomFigure);
			fRandomFigure.fValue = (float)modMisc::iRandomNumberNonZero(-9, 9) * eTemp.vecValues[0].fValue;
			eTemp.vecAnswers.push_back(fRandomFigure);
			fRandomFigure.fValue = (float)modMisc::iRandomNumberNonZero(-9, 9) * eTemp.vecValues[0].fValue;
			eTemp.vecAnswers.push_back(fRandomFigure);
			
			fRandomFigure.fValue = (float)((eTemp.vecAnswers[0].fValue + (eTemp.vecAnswers[1].fValue + eTemp.vecAnswers[2].fValue)) / eTemp.vecValues[0].fValue);
			eTemp.vecValues.push_back(fRandomFigure);
			fRandomFigure.fValue = (float)(((eTemp.vecAnswers[0].fValue * eTemp.vecAnswers[1].fValue) + eTemp.vecAnswers[2].fValue) / eTemp.vecValues[0].fValue);
			eTemp.vecValues.push_back(fRandomFigure);
			fRandomFigure.fValue = (float)(((eTemp.vecAnswers[0].fValue * eTemp.vecAnswers[2].fValue) + eTemp.vecAnswers[1].fValue) / eTemp.vecValues[0].fValue);
			eTemp.vecValues.push_back(fRandomFigure);
			fRandomFigure.fValue = (float)((eTemp.vecAnswers[0].fValue * eTemp.vecAnswers[1].fValue * eTemp.vecAnswers[2].fValue) / eTemp.vecValues[0].fValue);
			eTemp.vecValues.push_back(fRandomFigure);
			
			
			if (modMisc::iRandomNumber(0, 10) < 5) {
				//multiple choice
				
				//Correct answer
				eTemp.bIsMultipleChoice = true;
				eTemp.bOnlyOneAnswer = true;
				sMultipleChoiceAnswer.bIsCorrect = true; 
				sMultipleChoiceAnswer.bIsSelected = false; 
				ssTemp.str("");
				ssTemp << (float)eTemp.vecAnswers[0].fValue;
				sMultipleChoiceAnswer.sValue = ssTemp.str();
				
				eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
				
				
				eTemp.bIsMultipleChoice = true;
				eTemp.bOnlyOneAnswer = true;
				sMultipleChoiceAnswer.bIsCorrect = true; 
				sMultipleChoiceAnswer.bIsSelected = false; 
				ssTemp.str("");
				ssTemp << (float)eTemp.vecAnswers[1].fValue;
				sMultipleChoiceAnswer.sValue = ssTemp.str();
				
				eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
				
				
				//wrong answers
				iTemp = (int)(eTemp.vecValues[0].fValue * eTemp.vecValues[0].fValue);
				if (iTemp != (int)eTemp.vecAnswers[0].fValue and iTemp != (int)eTemp.vecAnswers[1].fValue and iTemp != (int)eTemp.vecAnswers[2].fValue) {
					sMultipleChoiceAnswer.bIsCorrect = false; 
					sMultipleChoiceAnswer.bIsSelected = false; 
					ssTemp.str("");
					ssTemp << iTemp;
					sMultipleChoiceAnswer.sValue = ssTemp.str();
					if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
						eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
					};
				};
				
				iTemp = (int)(eTemp.vecValues[1].fValue * eTemp.vecValues[1].fValue);
				if (iTemp != (int)eTemp.vecAnswers[0].fValue and iTemp != (int)eTemp.vecAnswers[1].fValue and iTemp != (int)eTemp.vecAnswers[2].fValue) {
					sMultipleChoiceAnswer.bIsCorrect = false; 
					sMultipleChoiceAnswer.bIsSelected = false; 
					ssTemp.str("");
					ssTemp << iTemp;
					sMultipleChoiceAnswer.sValue = ssTemp.str();
					if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
						eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
					};
				};
				
				iTemp = (int)(eTemp.vecValues[2].fValue * eTemp.vecValues[2].fValue);
				if (iTemp != (int)eTemp.vecAnswers[0].fValue and iTemp != (int)eTemp.vecAnswers[1].fValue and iTemp != (int)eTemp.vecAnswers[2].fValue) {
					sMultipleChoiceAnswer.bIsCorrect = false; 
					sMultipleChoiceAnswer.bIsSelected = false; 
					ssTemp.str("");
					ssTemp << iTemp;
					sMultipleChoiceAnswer.sValue = ssTemp.str();
					if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
						eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
					};
				};
				
				iTemp = (int)(eTemp.vecValues[3].fValue * eTemp.vecValues[3].fValue);
				if (iTemp != (int)eTemp.vecAnswers[0].fValue and iTemp != (int)eTemp.vecAnswers[1].fValue and iTemp != (int)eTemp.vecAnswers[2].fValue) {
					sMultipleChoiceAnswer.bIsCorrect = false; 
					sMultipleChoiceAnswer.bIsSelected = false; 
					ssTemp.str("");
					ssTemp << iTemp;
					sMultipleChoiceAnswer.sValue = ssTemp.str();
					if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
						eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
					};
				};
				
				iTemp = (int)(eTemp.vecValues[0].fValue * eTemp.vecValues[1].fValue);
				if (iTemp != (int)eTemp.vecAnswers[0].fValue and iTemp != (int)eTemp.vecAnswers[1].fValue and iTemp != (int)eTemp.vecAnswers[2].fValue) {
					sMultipleChoiceAnswer.bIsCorrect = false; 
					sMultipleChoiceAnswer.bIsSelected = false; 
					ssTemp.str("");
					ssTemp << iTemp;
					sMultipleChoiceAnswer.sValue = ssTemp.str();
					if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
						eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
					};
				};
				
				iTemp = (int)(eTemp.vecValues[2].fValue * eTemp.vecValues[3].fValue);
				if (iTemp != (int)eTemp.vecAnswers[0].fValue and iTemp != (int)eTemp.vecAnswers[1].fValue and iTemp != (int)eTemp.vecAnswers[2].fValue) {
					sMultipleChoiceAnswer.bIsCorrect = false; 
					sMultipleChoiceAnswer.bIsSelected = false; 
					ssTemp.str("");
					ssTemp << iTemp;
					sMultipleChoiceAnswer.sValue = ssTemp.str();
					if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
						eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
					};
				};
				
				iTemp = (int)(eTemp.vecValues[1].fValue * eTemp.vecValues[2].fValue);
				if (iTemp != (int)eTemp.vecAnswers[0].fValue and iTemp != (int)eTemp.vecAnswers[1].fValue and iTemp != (int)eTemp.vecAnswers[2].fValue) {
					sMultipleChoiceAnswer.bIsCorrect = false; 
					sMultipleChoiceAnswer.bIsSelected = false; 
					ssTemp.str("");
					ssTemp << iTemp;
					sMultipleChoiceAnswer.sValue = ssTemp.str();
					if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
						eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
					};
				};
				
				iTemp = (int)eTemp.vecValues[0].fValue;
				if (iTemp != (int)eTemp.vecAnswers[0].fValue and iTemp != (int)eTemp.vecAnswers[1].fValue and iTemp != (int)eTemp.vecAnswers[2].fValue) {
					sMultipleChoiceAnswer.bIsCorrect = false; 
					sMultipleChoiceAnswer.bIsSelected = false; 
					ssTemp.str("");
					ssTemp << iTemp;
					sMultipleChoiceAnswer.sValue = ssTemp.str();
					if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
						eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
					};
				};
				
				iTemp = (int)eTemp.vecValues[1].fValue;
				if (iTemp != (int)eTemp.vecAnswers[0].fValue and iTemp != (int)eTemp.vecAnswers[1].fValue and iTemp != (int)eTemp.vecAnswers[2].fValue) {
					sMultipleChoiceAnswer.bIsCorrect = false; 
					sMultipleChoiceAnswer.bIsSelected = false; 
					ssTemp.str("");
					ssTemp << iTemp;
					sMultipleChoiceAnswer.sValue = ssTemp.str();
					if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
						eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
					};
				};
				
				iTemp = (int)eTemp.vecValues[2].fValue;
				if (iTemp != (int)eTemp.vecAnswers[0].fValue and iTemp != (int)eTemp.vecAnswers[1].fValue and iTemp != (int)eTemp.vecAnswers[2].fValue) {
					sMultipleChoiceAnswer.bIsCorrect = false; 
					sMultipleChoiceAnswer.bIsSelected = false; 
					ssTemp.str("");
					ssTemp << iTemp;
					sMultipleChoiceAnswer.sValue = ssTemp.str();
					if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
						eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
					};
				};
				
				iTemp = (int)eTemp.vecValues[3].fValue;
				if (iTemp != (int)eTemp.vecAnswers[0].fValue and iTemp != (int)eTemp.vecAnswers[1].fValue and iTemp != (int)eTemp.vecAnswers[2].fValue) {
					sMultipleChoiceAnswer.bIsCorrect = false; 
					sMultipleChoiceAnswer.bIsSelected = false; 
					ssTemp.str("");
					ssTemp << iTemp;
					sMultipleChoiceAnswer.sValue = ssTemp.str();
					if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
						eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
					};
				};
				
				iTemp = modMisc::iRandomNumber(2, 15);
				if (iTemp != (int)eTemp.vecAnswers[0].fValue and iTemp != (int)eTemp.vecAnswers[1].fValue and iTemp != (int)eTemp.vecAnswers[2].fValue) {
					sMultipleChoiceAnswer.bIsCorrect = false; 
					sMultipleChoiceAnswer.bIsSelected = false; 
					ssTemp.str("");
					ssTemp << iTemp;
					sMultipleChoiceAnswer.sValue = ssTemp.str();
					if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
						eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
					};
				};
				
				iTemp = modMisc::iRandomNumber(2, 9);
				if (iTemp != (int)eTemp.vecAnswers[0].fValue and iTemp != (int)eTemp.vecAnswers[1].fValue and iTemp != (int)eTemp.vecAnswers[2].fValue) {
					sMultipleChoiceAnswer.bIsCorrect = false; 
					sMultipleChoiceAnswer.bIsSelected = false; 
					ssTemp.str("");
					ssTemp << iTemp;
					sMultipleChoiceAnswer.sValue = ssTemp.str();
					if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
						eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
					};
				};
				
			} else {
				//NOT multiple choice
				eTemp.bIsMultipleChoice = false;
			};
			break;

		
		
		case -1234:
			//Unchecked????????????????????????????????????????????????????????????????????????????????????????????????????????????????????
			eTemp.eQuestionType = eQuesDifferentiationExpFn;
			eTemp.sQuestionEncoded = "??? <0>exp(<1>x)"; 
			eTemp.bIsMultipleChoice = false;
			eTemp.iDifficulty = 140;
			eTemp.eAnswerType = eAnsTypNumeric;
			eTemp.bHasPic = false;
			
			fRandomFigure.fValue = (float)modMisc::iRandomNumberNonZero(-9, 9);
			eTemp.vecValues.push_back(fRandomFigure);
			fRandomFigure.fValue = (float)modMisc::iRandomNumberNonZero(-9, 9);
			eTemp.vecValues.push_back(fRandomFigure);
			
			
			fRandomFigure.fValue = (float)(eTemp.vecValues[0].fValue + eTemp.vecValues[1].fValue);
			eTemp.vecAnswers.push_back(fRandomFigure);
			
			break;
		case 11:
			//Unchecked????????????????????????????????????????????????????????????????????????????????????????????????????????????????????
			eTemp.eQuestionType = eQuesDifferentiationTrigFn;
			eTemp.sQuestionEncoded = "??? <0><trig>(<1>x)="; 
			eTemp.bIsMultipleChoice = false;
			eTemp.iDifficulty = 150;
			//			eTemp.eAnswerType = eAnsTypNumeric;
			eTemp.bHasPic = false;
			
			eTempFnTrig = generateRandomFnTrig(1);
			eTemp.eFnTrig.push_back(eTempFnTrig);
			
			fRandomFigure.fValue = (float)modMisc::iRandomNumberNonZero(-9, 9);
			eTemp.vecValues.push_back(fRandomFigure);
			fRandomFigure.fValue = (float)modMisc::iRandomNumberNonZero(-9, 9);
			eTemp.vecValues.push_back(fRandomFigure);
			
			fRandomFigure.fValue = (float)(eTemp.vecValues[0].fValue + eTemp.vecValues[1].fValue);
			eTemp.vecAnswers.push_back(fRandomFigure);
			
			break;
		case 12:
			//Unchecked????????????????????????????????????????????????????????????????????????????????????????????????????????????????????
			eTemp.eQuestionType = eQuesDifferentiationChainRule;
			eTemp.sQuestionEncoded = "??? <0> + <1> =";
			eTemp.bIsMultipleChoice = false;
			eTemp.iDifficulty = 200;
			eTemp.bHasPic = false;
			
			break;
		case 13:
			//Unchecked????????????????????????????????????????????????????????????????????????????????????????????????????????????????????
			eTemp.eQuestionType = eQuesDifferentiationEverything;
			eTemp.sQuestionEncoded = "??? <0> + <1> ="; 
			eTemp.bIsMultipleChoice = false;
			eTemp.iDifficulty = 300;
			eTemp.bHasPic = false;
			
			break;
			
		
		
		default:
			eTemp.eQuestionType = eQuesError;
			eTemp.bIsMultipleChoice = false;
			eTemp.sQuestionEncoded = "Error:";
			break;
	};//switch iRandom
	
	
	return eTemp;

//	int iRandom;
//	strFloatValue fRandomFigure;
//	strQuestion eTemp;
//	int iTemp;
//	strStringAnswer sMultipleChoiceAnswer; 
//	vector<strStringAnswer>::iterator itMultipleChoiceAnswer; 
//	std::stringstream ssTemp;
//	int iRangeMax;
//	int iRangeMin;
//	int iDiff;
//	int iType;
//	enumFnTrig eTempFnTrig;
//	int iDifficultyGroupLevel;
};

strQuestion ClsQuiz::randomQuestionType_Polynomial(){

};

strQuestion ClsQuiz::randomQuestionType_ArithmeticMisc3Figures(){
	int iRandom;
	strFloatValue fRandomFigure;
	strQuestion eTemp;
	strStringAnswer sMultipleChoiceAnswer; 
	vector<strStringAnswer>::iterator itMultipleChoiceAnswer; 
	std::stringstream ssTemp;
	int iRangeMax;
	int iRangeMin;
	int iDiff;
	std::string sGroupName;
	time_t tmStart;
	time_t tmEnd;
	
	tmStart = time (NULL);
	
	eTemp.eQuestionGroup = eQuesGrpArithmeticAdvanced;
	sGroupName = modMisc::convertEnum_QuesTypeGroupText(eQuesGrpArithmeticAdvanced);
	
	eTemp.bHasPic = false;
	eTemp.ePic = ePic_None;
	
	eTemp.vecValues.clear();
	eTemp.vecAnswers.clear();
	eTemp.vecChoices.clear();
	
	
	const int gciQuestionTypeMax = 6; //note: needs to be the maximum integer in the enumQuestionType so that we can generate random question types 
	iRandom = (int)modMisc::iRandomNumber(1, gciQuestionTypeMax);
	
	if (cPreferences->getType() == ePrefByGroup or cPreferences->getType() == ePrefBySubject) {
		iDiff = (int)cPreferences->getDifficulty(&sGroupName);
	} else {
		iDiff = (int)modMisc::iRandomNumber(1, 3);
	};
	
	eTemp.eAnswerType = eAnsTypNumeric;
	
	switch (iDiff) {
		case 1:
			eTemp.eQuestionType = eQuesArithmeticEasy;
			eTemp.iDifficulty = 250;
			
			iRangeMin = 2;
			iRangeMax = 6;
			
			break;
		case 2:
			eTemp.eQuestionType = eQuesArithmeticMedium;
			eTemp.iDifficulty = 350;
			
			iRangeMin = 5;
			iRangeMax = 12;
			
			break;
		case 3:
			eTemp.eQuestionType = eQuesArithmeticHard;
			eTemp.iDifficulty = 450;
			
			iRangeMin = 9;
			iRangeMax = 15;
			
			break;
		default:
			break;
	}; //switch iDiff
	

	

	switch (iRandom){ 
		case 1:
			eTemp.sQuestionEncoded = "<0> + (<1> * <2>) =";

			fRandomFigure.fValue = (float)modMisc::iRandomNumber(iRangeMin, iRangeMax);
			eTemp.vecValues.push_back(fRandomFigure);
			fRandomFigure.fValue = (float)modMisc::iRandomNumber(iRangeMin, iRangeMax);
			eTemp.vecValues.push_back(fRandomFigure);
			fRandomFigure.fValue = (float)modMisc::iRandomNumber(iRangeMin, iRangeMax);
			eTemp.vecValues.push_back(fRandomFigure);
			
			fRandomFigure.fValue = (float)(eTemp.vecValues[0].fValue + (eTemp.vecValues[1].fValue * eTemp.vecValues[2].fValue));
			eTemp.vecAnswers.push_back(fRandomFigure);
			
			break;
		case 2:
			eTemp.sQuestionEncoded = "(<2> + <1>) / <0> =";
			
			fRandomFigure.fValue = (float)modMisc::iRandomNumber(iRangeMin, iRangeMax);
			eTemp.vecValues.push_back(fRandomFigure);
			fRandomFigure.fValue = (float)modMisc::iRandomNumber(iRangeMin, iRangeMax);
			eTemp.vecValues.push_back(fRandomFigure);
			fRandomFigure.fValue = (float)modMisc::iRandomNumber(iRangeMin, iRangeMax);
			eTemp.vecAnswers.push_back(fRandomFigure);
			
			fRandomFigure.fValue = (float)((eTemp.vecAnswers[0].fValue * eTemp.vecValues[0].fValue) -  eTemp.vecValues[1].fValue);
			eTemp.vecValues.push_back(fRandomFigure);
			
			break;
		case 3:
			eTemp.sQuestionEncoded = "(<2> / <0>) + <1> =";
			
			fRandomFigure.fValue = (float)modMisc::iRandomNumber(iRangeMin, iRangeMax);
			eTemp.vecValues.push_back(fRandomFigure);
			fRandomFigure.fValue = (float)modMisc::iRandomNumber(iRangeMin, iRangeMax);
			eTemp.vecValues.push_back(fRandomFigure);
			fRandomFigure.fValue = (float)(modMisc::iRandomNumber(iRangeMin, iRangeMax) * eTemp.vecValues[0].fValue);
			eTemp.vecValues.push_back(fRandomFigure);
			
			fRandomFigure.fValue = (float)((eTemp.vecValues[2].fValue / eTemp.vecValues[0].fValue) + eTemp.vecValues[1].fValue);
			eTemp.vecAnswers.push_back(fRandomFigure);
			
			break;
		case 4:
			eTemp.sQuestionEncoded = "<0> + <1><^2> + <2><^3> =";
			
			fRandomFigure.fValue = (float)modMisc::iRandomNumber(iRangeMin, iRangeMax);
			eTemp.vecValues.push_back(fRandomFigure);
			fRandomFigure.fValue = (float)modMisc::iRandomNumber(iRangeMin, iRangeMax);
			eTemp.vecValues.push_back(fRandomFigure);
			fRandomFigure.fValue = (float)modMisc::iRandomNumber(iRangeMin, iRangeMax);
			eTemp.vecValues.push_back(fRandomFigure);
			
			fRandomFigure.fValue = (float)(eTemp.vecValues[0].fValue + (eTemp.vecValues[1].fValue * eTemp.vecValues[1].fValue) + (eTemp.vecValues[2].fValue * eTemp.vecValues[2].fValue * eTemp.vecValues[2].fValue));
			eTemp.vecAnswers.push_back(fRandomFigure);
			
			break;
		case 5:
			eTemp.sQuestionEncoded = "<0> * <1> * <2> =";
			
			fRandomFigure.fValue = (float)modMisc::iRandomNumber(iRangeMin, iRangeMax);
			eTemp.vecValues.push_back(fRandomFigure);
			fRandomFigure.fValue = (float)modMisc::iRandomNumber(iRangeMin, iRangeMax);
			eTemp.vecValues.push_back(fRandomFigure);
			fRandomFigure.fValue = (float)modMisc::iRandomNumber(iRangeMin, iRangeMax);
			eTemp.vecValues.push_back(fRandomFigure);
			
			fRandomFigure.fValue = (float)(eTemp.vecValues[0].fValue * eTemp.vecValues[1].fValue * eTemp.vecValues[2].fValue);
			eTemp.vecAnswers.push_back(fRandomFigure);
			
			break;
		case 6:
			eTemp.sQuestionEncoded = "<0><^2> + (<1> * <2>) =";
			
			fRandomFigure.fValue = (float)modMisc::iRandomNumber(iRangeMin, iRangeMax);
			eTemp.vecValues.push_back(fRandomFigure);
			fRandomFigure.fValue = (float)modMisc::iRandomNumber(iRangeMin, iRangeMax);
			eTemp.vecValues.push_back(fRandomFigure);
			fRandomFigure.fValue = (float)modMisc::iRandomNumber(iRangeMin, iRangeMax);
			eTemp.vecValues.push_back(fRandomFigure);
			
			fRandomFigure.fValue = (float)((eTemp.vecValues[0].fValue * eTemp.vecValues[0].fValue) + (eTemp.vecValues[1].fValue * eTemp.vecValues[2].fValue));
			eTemp.vecAnswers.push_back(fRandomFigure);
			
			break;
		default:
			eTemp.eQuestionType = eQuesError;
			eTemp.bIsMultipleChoice = false;
			eTemp.sQuestionEncoded = "Error:";
			break;
	};
	
	//Correct answer
	eTemp.bIsMultipleChoice = true;
	eTemp.bOnlyOneAnswer = true;
	sMultipleChoiceAnswer.bIsCorrect = true; 
	sMultipleChoiceAnswer.bIsSelected = false; 
	ssTemp.str("");
	ssTemp << (int)eTemp.vecAnswers[0].fValue;
	sMultipleChoiceAnswer.sValue = ssTemp.str();
	
	eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
	
	
	//wrong answers
	do {
		sMultipleChoiceAnswer.bIsCorrect = false; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		ssTemp << (int)(eTemp.vecAnswers[0].fValue + modMisc::iRandomNumberNonZero(iRangeMin - iRangeMax, iRangeMax - iRangeMin));
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
			eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		};
		
		sMultipleChoiceAnswer.bIsCorrect = false; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		ssTemp << (int)(eTemp.vecAnswers[0].fValue + modMisc::iRandomNumberNonZero(iRangeMin - iRangeMax, iRangeMax - iRangeMin));
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
			eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		};
		
		sMultipleChoiceAnswer.bIsCorrect = false; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		ssTemp << (int)(eTemp.vecAnswers[0].fValue + modMisc::iRandomNumberNonZero(iRangeMin - iRangeMax, iRangeMax - iRangeMin));
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
			eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		};
		
		sMultipleChoiceAnswer.bIsCorrect = false; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		ssTemp << (int)((modMisc::iRandomNumberNonZero(-1, 1) * eTemp.vecValues[0].fValue) + ((eTemp.vecValues[1].fValue + modMisc::iRandomNumberNonZero(-1, 1)) * eTemp.vecValues[2].fValue));
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
			eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		};
		
		sMultipleChoiceAnswer.bIsCorrect = false; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		ssTemp << (int)((eTemp.vecValues[2].fValue / eTemp.vecValues[0].fValue) + (modMisc::iRandomNumber(-2, 2) * eTemp.vecValues[1].fValue));
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
			eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		};
		
		sMultipleChoiceAnswer.bIsCorrect = false; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		ssTemp << (int)(eTemp.vecValues[0].fValue + (eTemp.vecValues[1].fValue * eTemp.vecValues[1].fValue * modMisc::iRandomNumber(-1, 1)) + (eTemp.vecValues[2].fValue * eTemp.vecValues[2].fValue * eTemp.vecValues[2].fValue * modMisc::iRandomNumber(-1, 1)));
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
			eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		};
		
		sMultipleChoiceAnswer.bIsCorrect = false; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		ssTemp << (int)(eTemp.vecValues[0].fValue * eTemp.vecValues[1].fValue * (eTemp.vecValues[2].fValue + modMisc::iRandomNumber(-1, 1)));
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
			eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		};
		
		sMultipleChoiceAnswer.bIsCorrect = false; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		ssTemp << (int)((eTemp.vecValues[0].fValue * eTemp.vecValues[0].fValue) + (eTemp.vecValues[1].fValue * (eTemp.vecValues[2].fValue + modMisc::iRandomNumber(-1, 1))));
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
			eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		};
        
        tmEnd = time (NULL);
	} while (eTemp.vecChoices.size() < 15 and tmStart + iCutOffTime < (int)tmEnd);
	

//	int iRandom;
//	strFloatValue fRandomFigure;
//	strQuestion eTemp;
//	int iTemp;
//	strStringAnswer sMultipleChoiceAnswer; 
//	vector<strStringAnswer>::iterator itMultipleChoiceAnswer; 
//	std::stringstream ssTemp;
//	int iRangeMax;
//	int iRangeMin;
//	int iDiff;
	sGroupName.clear();
//	time_t tmStart;

	return eTemp;
};








strQuestion ClsQuiz::randomQuestionType_AverageMean(){
	strFloatValue fRandomFigure;
	strQuestion eTemp;
	strStringAnswer sMultipleChoiceAnswer; 
	std::stringstream ssTemp;
	int iRangeMax;
	int iRangeMin;
	int iDiff;
	std::string sGroupName;
	time_t tmStart;
	time_t tmEnd;
	vector<int> vecSetOfData;
	int iNoOfFigures;
	std::stringstream ssQuestion;
	int iAnswer;
	int iDeltaToAnswer;
	int iFigure;
	int iTotal;
	int iPreviousFigure;
	
	tmStart = time (NULL);
	
	eTemp.eQuestionGroup = eQuesGrpAverages;
	sGroupName = modMisc::convertEnum_QuesTypeGroupText(eQuesGrpAverages);
	
	eTemp.bHasPic = false;
	eTemp.ePic = ePic_None;
	
	eTemp.vecValues.clear();
	eTemp.vecAnswers.clear();
	eTemp.vecChoices.clear();
	vecSetOfData.clear();
	
//	const int gciQuestionTypeMax = 6; //note: needs to be the maximum integer in the enumQuestionType so that we can generate random question types 
//	iRandom = (int)modMisc::iRandomNumber(1, gciQuestionTypeMax);
	
	if (cPreferences->getType() == ePrefByGroup or cPreferences->getType() == ePrefBySubject) {
		iDiff = (int)cPreferences->getDifficulty(&sGroupName);
	} else {
		iDiff = (int)modMisc::iRandomNumber(1, 5);
	};
	
	eTemp.eAnswerType = eAnsTypNumeric;
	
	switch (iDiff) {
		case 1:
			eTemp.eQuestionType = eQuesAverageMeanVeryEasy;
			eTemp.iDifficulty = 175;
			
			iRangeMax = 15;
			iRangeMin = 2;
			iNoOfFigures = 4;
			
			break;
		case 2:
			eTemp.eQuestionType = eQuesAverageMeanEasy;
			eTemp.iDifficulty = 215;
			
			iRangeMax = 20;
			iRangeMin = 6;
			iNoOfFigures = 6;
			
			break;
		case 3:
			eTemp.eQuestionType = eQuesAverageMeanMedium;
			eTemp.iDifficulty = 275;
			
			iRangeMax = 25;
			iRangeMin = 9;
			iNoOfFigures = 8;
			
			break;
		case 4:
			eTemp.eQuestionType = eQuesAverageMeanHard;
			eTemp.iDifficulty = 325;
			
			iRangeMax = 30;
			iRangeMin = 15;
			iNoOfFigures = 10;
			
			break;
		case 5:
			eTemp.eQuestionType = eQuesAverageMeanVeryHard;
			eTemp.iDifficulty = 375;
			
			iRangeMax = 50;
			iRangeMin = 20;
			iNoOfFigures = 12;
			
			break;
		default:
			break;
	}; //switch iDiff
	
	iAnswer = modMisc::iRandomNumber(iRangeMin, iRangeMax);
	iTotal = 0;
	iPreviousFigure = 0;
	
	ssQuestion.str("");
	ssQuestion << "What is the mean average of these figures?\n";
	do {
	    if (vecSetOfData.size() == iNoOfFigures - 1) {
			//last figure
			iFigure = (iNoOfFigures * iAnswer) - iTotal;
		} else {

			do {
				iFigure = iAnswer + modMisc::iRandomNumber((int)(-1 * fabs(iRangeMax - iRangeMin) / 2), (int)fabs(iRangeMax - iRangeMin) / 2);
				iDeltaToAnswer = iFigure - iAnswer;
				iFigure = iFigure + (1.5 * iDeltaToAnswer);
			} while (iFigure == iPreviousFigure);
		};
		
		iPreviousFigure = iFigure;
		
		iTotal = iTotal + iFigure;

		if (vecSetOfData.size() == iNoOfFigures - 1) {
			ssQuestion << iFigure << "?";
		} else {
			ssQuestion << iFigure << ", ";
		};
		vecSetOfData.push_back(iFigure);

		iDeltaToAnswer = iAnswer - (iTotal / vecSetOfData.size());
	} while (vecSetOfData.size() < iNoOfFigures);
	

	eTemp.sQuestionEncoded = ssQuestion.str();
			
	fRandomFigure.fValue = (float)(iAnswer);
	eTemp.vecAnswers.push_back(fRandomFigure);
	
	//Correct answer
	eTemp.bIsMultipleChoice = true;
	eTemp.bOnlyOneAnswer = true;
	sMultipleChoiceAnswer.bIsCorrect = true; 
	sMultipleChoiceAnswer.bIsSelected = false; 
	ssTemp.str("");
	ssTemp << (int)eTemp.vecAnswers[0].fValue;
	sMultipleChoiceAnswer.sValue = ssTemp.str();
	
	eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
	
	
	//wrong answers
	do {
		sMultipleChoiceAnswer.bIsCorrect = false; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		ssTemp << (int)vecSetOfData[modMisc::iRandomNumber(0,iNoOfFigures - 1)];
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
			eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		};
		
		sMultipleChoiceAnswer.bIsCorrect = false; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		ssTemp << (int)vecSetOfData[modMisc::iRandomNumber((int)(iNoOfFigures / 2),iNoOfFigures - 1)];
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
			eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		};
		
		sMultipleChoiceAnswer.bIsCorrect = false; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		ssTemp << (int)vecSetOfData[modMisc::iRandomNumber(0,(int)((iNoOfFigures - 1) / 2))];
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
			eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		};
		
		sMultipleChoiceAnswer.bIsCorrect = false; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		ssTemp << (int)modMisc::iRandomNumberNonZero(iRangeMin, iRangeMax);
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
			eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		};
		
		sMultipleChoiceAnswer.bIsCorrect = false; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		ssTemp << (int)modMisc::iRandomNumberNonZero(iRangeMin, iRangeMax - 1);
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
			eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		};
		
		sMultipleChoiceAnswer.bIsCorrect = false; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		ssTemp << (int)modMisc::iRandomNumberNonZero(iRangeMin + 1, iRangeMax);
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
			eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		};
		
		sMultipleChoiceAnswer.bIsCorrect = false; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		ssTemp << (int)modMisc::iRandomNumberNonZero(iRangeMin, iRangeMax - 2);
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
			eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		};
		
		sMultipleChoiceAnswer.bIsCorrect = false; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		ssTemp << (int)modMisc::iRandomNumberNonZero(iRangeMin + 2, iRangeMax);
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
			eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		};
		
		sMultipleChoiceAnswer.bIsCorrect = false; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		ssTemp << (int)(iAnswer + modMisc::iRandomNumberNonZero(-2, 2));
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
			eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		};
		
		sMultipleChoiceAnswer.bIsCorrect = false; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		ssTemp << (int)(iAnswer + modMisc::iRandomNumberNonZero(-3, 3));
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
			eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		};
		
		sMultipleChoiceAnswer.bIsCorrect = false; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		ssTemp << (int)(iAnswer + modMisc::iRandomNumberNonZero(-4, 4));
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
			eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		};
        
        tmEnd = time (NULL);
	} while (eTemp.vecChoices.size() < 7 and (int)tmStart + iCutOffTime < (int)tmEnd);
	
	
//	strFloatValue fRandomFigure;
//	strQuestion eTemp;
//	strStringAnswer sMultipleChoiceAnswer; 
//	std::stringstream ssTemp;
//	int iRangeMax;
//	int iRangeMin;
//	int iDiff;
	sGroupName.clear();
//	time_t tmStart;
	vecSetOfData.clear();
//	int iNoOfFigures;
//	std::stringstream ssQuestion;
//	int iAnswer;
//	int iDeltaToAnswer;
//	int iFigure;
//	int iTotal;
//	int iPreviousFigure;

	return eTemp;
};

strQuestion ClsQuiz::randomQuestionType_AverageMedian(){
	strFloatValue fRandomFigure;
	strQuestion eTemp;
	strStringAnswer sMultipleChoiceAnswer; 
	std::stringstream ssTemp;
	int iRangeMax;
	int iRangeMin;
	int iDiff;
	std::string sGroupName;
	time_t tmStart;
	time_t tmEnd;
	vector<int> vecSetOfData;
	int iNoOfFigures;
	std::stringstream ssQuestion;
	int iAnswer;
	int iAnswerA;
	int iAnswerB;
	int iFigure;
	int iTotal;
	int iPreviousFigure;
	bool bOddSampleSize;
	bool bIsFound;
	vector<int>::iterator itSetOfData;
	vector<int>::iterator itSetOfDataInner;
	vector<int>::iterator itSetOfDataOutter;
	int iCountNumbersBigger;
	int iCountNumbersSmaller;
	
	tmStart = time (NULL);
	
	eTemp.eQuestionGroup = eQuesGrpAverages;
	sGroupName = modMisc::convertEnum_QuesTypeGroupText(eQuesGrpAverages);
	
	eTemp.bHasPic = false;
	eTemp.ePic = ePic_None;
	
	eTemp.vecValues.clear();
	eTemp.vecAnswers.clear();
	eTemp.vecChoices.clear();
	vecSetOfData.clear();
	
	//	const int gciQuestionTypeMax = 6; //note: needs to be the maximum integer in the enumQuestionType so that we can generate random question types 
	//	iRandom = (int)modMisc::iRandomNumber(1, gciQuestionTypeMax);
	
	if (cPreferences->getType() == ePrefByGroup or cPreferences->getType() == ePrefBySubject) {
		iDiff = (int)cPreferences->getDifficulty(&sGroupName);
	} else {
		iDiff = (int)modMisc::iRandomNumber(1, 5);
	};
	
	eTemp.eAnswerType = eAnsTypNumeric;
	
	switch (iDiff) {
		case 1:
			eTemp.eQuestionType = eQuesAverageMedianVeryEasy;
			eTemp.iDifficulty = 105;
			
			iRangeMax = 18;
			iRangeMin = 2;
			iNoOfFigures = 5;
			bOddSampleSize = true;
			
			break;
		case 2:
			eTemp.eQuestionType = eQuesAverageMedianEasy;
			eTemp.iDifficulty = 125;
			
			iRangeMax = 24;
			iRangeMin = 6;
			iNoOfFigures = 7;
			bOddSampleSize = true;
			
			break;
		case 3:
			eTemp.eQuestionType = eQuesAverageMedianMedium;
			eTemp.iDifficulty = 165;
			
			iRangeMax = 30;
			iRangeMin = 9;
			iNoOfFigures = 8;
			bOddSampleSize = false;
			
			break;
		case 4:
			eTemp.eQuestionType = eQuesAverageMedianHard;
			eTemp.iDifficulty = 265;
			
			iRangeMax = 50;
			iRangeMin = 15;
			iNoOfFigures = 12;
			bOddSampleSize = false;
			
			break;
		case 5:
			eTemp.eQuestionType = eQuesAverageMedianVeryHard;
			eTemp.iDifficulty = 300;
			
			iRangeMax = 99;
			iRangeMin = 20;
			iNoOfFigures = 16;
			bOddSampleSize = false;
			
			break;
		default:
			break;
	}; //switch iDiff
	
//	iAnswer = modMisc::iRandomNumber(iRangeMin, iRangeMax);
//	iTotal = 0;
	iPreviousFigure = 0;
	
	ssQuestion.str("");
	ssQuestion << "What is the median average of these figures?\n";
	
	
	
	iTotal = 0;

	do {
		do {
			iFigure = modMisc::iRandomNumber(iRangeMin, iRangeMax);
			
			bIsFound = false;
			
			for (itSetOfData = vecSetOfData.begin(); itSetOfData != vecSetOfData.end();itSetOfData++) {
				if (*itSetOfData == iFigure) {
					bIsFound = true;
				};
			};
		} while (bIsFound == true);

		iTotal = iTotal + iFigure;
		
		if (vecSetOfData.size() == iNoOfFigures - 1) {
			ssQuestion << iFigure << "?";
		} else {
			ssQuestion << iFigure << ", ";
		};
		
		vecSetOfData.push_back(iFigure);
		
//		iDeltaToAnswer = iAnswer - (iTotal / vecSetOfData.size());
	} while (vecSetOfData.size() < iNoOfFigures);

	
	for (itSetOfDataOutter = vecSetOfData.begin(); itSetOfDataOutter != vecSetOfData.end(); itSetOfDataOutter++) {
		iCountNumbersBigger = 0;
		iCountNumbersSmaller = 0;
		
		for (itSetOfDataInner = vecSetOfData.begin(); itSetOfDataInner != vecSetOfData.end();itSetOfDataInner++) {
			if (*itSetOfDataOutter < *itSetOfDataInner) {
				iCountNumbersBigger++;	
			};

			if (*itSetOfDataOutter > *itSetOfDataInner) {
				iCountNumbersSmaller++;	
			};
		};
		
		if (bOddSampleSize == true) {
			if (iCountNumbersSmaller == iCountNumbersBigger) { 
				iAnswer = *itSetOfDataOutter;
			};
		} else {
			if (iCountNumbersSmaller == iCountNumbersBigger + 1) { 
				iAnswerA = *itSetOfDataOutter;
			};

			if (iCountNumbersSmaller + 1 == iCountNumbersBigger) { 
				iAnswerB = *itSetOfDataOutter;
			};
		};
	};
	
	eTemp.sQuestionEncoded = ssQuestion.str();
	
	if (bOddSampleSize == false) {
		fRandomFigure.fValue = (((float)(iAnswerA + iAnswerB)) / 2);
	} else {
		fRandomFigure.fValue = (float)iAnswer;
	};

	
	eTemp.vecAnswers.push_back(fRandomFigure);
	
	
	
	//Correct answer
	eTemp.bIsMultipleChoice = true;
	eTemp.bOnlyOneAnswer = true;
	sMultipleChoiceAnswer.bIsCorrect = true; 
	sMultipleChoiceAnswer.bIsSelected = false; 
	ssTemp.str("");
	if (bOddSampleSize == false) {
		if ((iAnswerA + iAnswerB) % 2 == 0.0) {
			ssTemp << (int)eTemp.vecAnswers[0].fValue;
		} else {
			ssTemp << (int)eTemp.vecAnswers[0].fValue << "<1/2>";
		};
	} else {
		ssTemp << (int)eTemp.vecAnswers[0].fValue;
	};
	sMultipleChoiceAnswer.sValue = ssTemp.str();
	
	eTemp.vecChoices.push_back(sMultipleChoiceAnswer);

	
	
	//wrong answers
	do {
		sMultipleChoiceAnswer.bIsCorrect = false; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		ssTemp << (int)(iTotal / iNoOfFigures);
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
			eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		};
		
		sMultipleChoiceAnswer.bIsCorrect = false; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		ssTemp << (int)(iTotal / iNoOfFigures) + modMisc::iRandomNumberNonZero(-2, 2);
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
			eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		};
		
		sMultipleChoiceAnswer.bIsCorrect = false; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		ssTemp << (int)(iTotal / iNoOfFigures) + modMisc::iRandomNumberNonZero(-1, 2);
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
			eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		};
		
		sMultipleChoiceAnswer.bIsCorrect = false; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		ssTemp << (int)(iTotal / iNoOfFigures) << "<1/2>";
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
			eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		};
		
		sMultipleChoiceAnswer.bIsCorrect = false; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		ssTemp << (int)((iTotal / iNoOfFigures) + modMisc::iRandomNumberNonZero(-2, 2)) << "<1/2>";
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
			eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		};
		
		sMultipleChoiceAnswer.bIsCorrect = false; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		ssTemp << (int)((iTotal / iNoOfFigures) + modMisc::iRandomNumberNonZero(-1, 2)) << "<1/2>";
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
			eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		};
		
		sMultipleChoiceAnswer.bIsCorrect = false; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		if (bOddSampleSize == false) {
			ssTemp << (int)(((iAnswerA + iAnswerB) / 2) + modMisc::iRandomNumberNonZero(-1, 1));
		} else {
			ssTemp << (int)(iAnswer + modMisc::iRandomNumberNonZero(-1, 2));
		};
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
			eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		};
		
		sMultipleChoiceAnswer.bIsCorrect = false; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		if (bOddSampleSize == false) {
			ssTemp << (int)(((iAnswerA + iAnswerB) / 2) + modMisc::iRandomNumberNonZero(-1, 2));
		} else {
			ssTemp << (int)(iAnswer + modMisc::iRandomNumberNonZero(-1, 2));
		};
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
			eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		};
		
		sMultipleChoiceAnswer.bIsCorrect = false; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		if (bOddSampleSize == false) {
			ssTemp << (int)(((iAnswerA + iAnswerB) / 2) + modMisc::iRandomNumberNonZero(-2, 2));
		} else {
			ssTemp << (int)(iAnswer + modMisc::iRandomNumberNonZero(-1, 2));
		};
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
			eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		};
		
		sMultipleChoiceAnswer.bIsCorrect = false; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		if (bOddSampleSize == false) {
			ssTemp << (int)(((iAnswerA + iAnswerB) / 2) + modMisc::iRandomNumberNonZero(-1, 1)) << "<1/2>";
		} else {
			ssTemp << (int)(iAnswer + modMisc::iRandomNumberNonZero(-1, 2)) << "<1/2>";
		};
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
			eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		};
		
		sMultipleChoiceAnswer.bIsCorrect = false; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		if (bOddSampleSize == false) {
			ssTemp << (int)(((iAnswerA + iAnswerB) / 2) + modMisc::iRandomNumberNonZero(-1, 2)) << "<1/2>";
		} else {
			ssTemp << (int)(iAnswer + modMisc::iRandomNumberNonZero(-1, 2)) << "<1/2>";
		};
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
			eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		};
		
		sMultipleChoiceAnswer.bIsCorrect = false; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		if (bOddSampleSize == false) {
			ssTemp << (int)(((iAnswerA + iAnswerB) / 2) + modMisc::iRandomNumberNonZero(-2, 2)) << "<1/2>";
		} else {
			ssTemp << (int)(iAnswer + modMisc::iRandomNumberNonZero(-1, 2)) << "<1/2>";
		};
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
			eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		};
		
		sMultipleChoiceAnswer.bIsCorrect = false; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		if (bOddSampleSize == false) {
			ssTemp << modMisc::iRandomNumber(1, (iAnswerA + iAnswerB + 2)) << "<1/2>";
		} else {
			ssTemp << modMisc::iRandomNumber(1, (iAnswer * 2) + 1) << "<1/2>";
		};
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
			eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		};
		
		sMultipleChoiceAnswer.bIsCorrect = false; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		if (bOddSampleSize == false) {
			ssTemp << modMisc::iRandomNumber(1, (iAnswerA + iAnswerB + 2));
		} else {
			ssTemp << modMisc::iRandomNumber(1, (iAnswer * 2) + 2);
		};
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
			eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		};
		
        tmEnd = time (NULL);
	} while (eTemp.vecChoices.size() < 7 and (int)tmStart + iCutOffTime < (int)tmEnd);

	
//	strFloatValue fRandomFigure;
//	strQuestion eTemp;
//	strStringAnswer sMultipleChoiceAnswer; 
//	std::stringstream ssTemp;
//	int iRangeMax;
//	int iRangeMin;
//	int iDiff;
//	sGroupName.clear();
//	time_t tmStart;
//	vecSetOfData.clear();
//	int iNoOfFigures;
//	std::stringstream ssQuestion;
//	int iAnswer;
//	int iAnswerA;
//	int iAnswerB;
//	int iFigure;
//	int iTotal;
//	int iPreviousFigure;
//	bool bOddSampleSize;
//	bool bIsFound;
//	vector<int>::iterator itSetOfData;
//	vector<int>::iterator itSetOfDataInner;
//	vector<int>::iterator itSetOfDataOutter;
//	int iCountNumbersBigger;
//	int iCountNumbersSmaller;
	
	return eTemp;
};

strQuestion ClsQuiz::randomQuestionType_AverageMode(){
	strFloatValue fRandomFigure;
	strQuestion eTemp;
	strStringAnswer sMultipleChoiceAnswer; 
	std::stringstream ssTemp;
	int iRangeMax;
	int iRangeMin;
	int iFigureMax;
	int iFigureMin;
	int iDiff;
	std::string sGroupName;
	time_t tmStart;
	time_t tmCurrent;
	vector<int> vecSetOfData;
	int iNoOfFigures;
	int iFigureCounter;
	std::stringstream ssQuestion;
	int iFigure;
	int iTotal;
	int iPreviousFigure;
	bool bOddSampleSize;
	bool bIsFound;
	vector<int>::iterator itSetOfData;
	vector<int>::iterator itSetOfDataInner;
	vector<int>::iterator itSetOfDataOutter;
	int iMaxCount;
	int iPos;
	
	tmStart = time (NULL);
	
	eTemp.eQuestionGroup = eQuesGrpAverages;
	sGroupName = modMisc::convertEnum_QuesTypeGroupText(eQuesGrpAverages);
	
	eTemp.bHasPic = false;
	eTemp.ePic = ePic_None;
	eTemp.bIsMultipleChoice = true;
	eTemp.bOnlyOneAnswer = true;

	eTemp.vecValues.clear();
	eTemp.vecAnswers.clear();
	eTemp.vecChoices.clear();
	vecSetOfData.clear();
	
	//	const int gciQuestionTypeMax = 6; //note: needs to be the maximum integer in the enumQuestionType so that we can generate random question types 
	//	iRandom = (int)modMisc::iRandomNumber(1, gciQuestionTypeMax);
	
	if (cPreferences->getType() == ePrefByGroup or cPreferences->getType() == ePrefBySubject) {
		iDiff = (int)cPreferences->getDifficulty(&sGroupName);
	} else {
		iDiff = (int)modMisc::iRandomNumber(1, 5);
	};
	
	eTemp.eAnswerType = eAnsTypNumeric;
	
	switch (iDiff) {
		case 1:
			eTemp.eQuestionType = eQuesAverageModeVeryEasy;
			eTemp.iDifficulty = 80;

			iFigureMax = 9;
			iFigureMin = 1;

			iRangeMax = 3;
			iRangeMin = 2;
			iNoOfFigures = 5;
			bOddSampleSize = true;
			
			break;
		case 2:
			eTemp.eQuestionType = eQuesAverageModeEasy;
			eTemp.iDifficulty = 100;
			
			iFigureMax = 15;
			iFigureMin = 4;
			
			iRangeMax = 5;
			iRangeMin = 2;
			iNoOfFigures = 8;
			bOddSampleSize = true;
			
			break;
		case 3:
			eTemp.eQuestionType = eQuesAverageModeMedium;
			eTemp.iDifficulty = 135;
			
			iFigureMax = 15;
			iFigureMin = 4;
			
			iRangeMax = 5;
			iRangeMin = 2;
			iNoOfFigures = 12;
			bOddSampleSize = false;
			
			break;
		case 4:
			eTemp.eQuestionType = eQuesAverageModeHard;
			eTemp.iDifficulty = 165;
			
			iFigureMax = 18;
			iFigureMin = 4;
			
			iRangeMax = 5;
			iRangeMin = 2;
			iNoOfFigures = 15;
			bOddSampleSize = false;
			
			break;
		case 5:
			eTemp.eQuestionType = eQuesAverageModeVeryHard;
			eTemp.iDifficulty = 205;
			
			iFigureMax = 99;
			iFigureMin = 6;
			
			iRangeMax = 5;
			iRangeMin = 2;
			iNoOfFigures = 24;
			bOddSampleSize = false;
			
			break;
		default:
			break;
	}; //switch iDiff
	
	iPreviousFigure = 0;
	
	ssQuestion.str("");
	ssQuestion << "What is the mode average of these figures?\n";
	

	iTotal = 0;
	iMaxCount = modMisc::iRandomNumber(iRangeMin, iRangeMax);

	//put the correct answer into the vector
	iFigure = modMisc::iRandomNumber(iFigureMin, iFigureMax);
	
	for (iFigureCounter = 0; iFigureCounter < iMaxCount; iFigureCounter++) {
		vecSetOfData.push_back(iFigure);
	};
	
	fRandomFigure.fValue = (float)iFigure;
	eTemp.vecAnswers.push_back(fRandomFigure);
		
	//put all the other numbers into the vector
	do {
		//create a random number and check if it's already in the vector
		do {
			iFigure = modMisc::iRandomNumber(iFigureMin, iFigureMax);
			
			bIsFound = false;
			
			for (itSetOfData = vecSetOfData.begin(); itSetOfData < vecSetOfData.end();itSetOfData++) {
				if (*itSetOfData == iFigure) {
					bIsFound = true;
				};
			};
		} while (bIsFound == true);
		
		
		//put the wrong answer into the vector a random amount of times 
		for (iFigureCounter = modMisc::iRandomNumber(1, iMaxCount - 1); iFigureCounter > 0; iFigureCounter--) {
			if (vecSetOfData.size() < iNoOfFigures) {
				iTotal = iTotal + iFigure;
				vecSetOfData.push_back(iFigure);
			};
		};
	} while (vecSetOfData.size() < iNoOfFigures);
	
	//loop through the vector and create the string for the question
	
	std::random_shuffle(vecSetOfData.begin(), vecSetOfData.end());
	
	iPos = 0;
	for (itSetOfData = vecSetOfData.begin(); itSetOfData < vecSetOfData.end(); itSetOfData++) {
		iPos++;
		if (iPos == iNoOfFigures) {
			ssQuestion << *itSetOfData << "?";
		} else {
			ssQuestion << *itSetOfData << ", ";
		};
	};
	
	eTemp.sQuestionEncoded = ssQuestion.str();

	
	//Correct answer
	eTemp.bIsMultipleChoice = true;
	eTemp.bOnlyOneAnswer = true;
	sMultipleChoiceAnswer.bIsCorrect = true; 
	sMultipleChoiceAnswer.bIsSelected = false; 
	ssTemp.str("");
	ssTemp << (int)eTemp.vecAnswers[0].fValue;
	sMultipleChoiceAnswer.sValue = ssTemp.str();
	
	eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
	
	
	
	//wrong answers
	do {
		for (iFigureCounter = 0;iFigureCounter < 9;iFigureCounter++) {
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << (int)vecSetOfData[modMisc::iRandomNumber(0, vecSetOfData.size() - 1)];
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
				eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
			};
		};
		
		sMultipleChoiceAnswer.bIsCorrect = false; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		ssTemp << (int)((int)eTemp.vecAnswers[0].fValue + modMisc::iRandomNumberNonZero(-1, 1));
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
			eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		};
        
		sMultipleChoiceAnswer.bIsCorrect = false; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		ssTemp << (int)((int)eTemp.vecAnswers[0].fValue + modMisc::iRandomNumberNonZero(-2, 1));
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
			eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		};
        
		sMultipleChoiceAnswer.bIsCorrect = false; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		ssTemp << (int)((int)eTemp.vecAnswers[0].fValue + modMisc::iRandomNumberNonZero(-1, 2));
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
			eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		};
        
		sMultipleChoiceAnswer.bIsCorrect = false; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		ssTemp << (int)((int)eTemp.vecAnswers[0].fValue + modMisc::iRandomNumberNonZero(-2, 2));
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
			eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		};
		
		sMultipleChoiceAnswer.bIsCorrect = false; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		ssTemp << (int)((int)eTemp.vecAnswers[0].fValue + modMisc::iRandomNumberNonZero(-3, 2));
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
			eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		};
        
		sMultipleChoiceAnswer.bIsCorrect = false; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		ssTemp << (int)((int)eTemp.vecAnswers[0].fValue + modMisc::iRandomNumberNonZero(-2, 3));
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
			eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		};
        
		sMultipleChoiceAnswer.bIsCorrect = false; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		ssTemp << (int)((int)eTemp.vecAnswers[0].fValue + modMisc::iRandomNumberNonZero(-3, 3));
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
			eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		};
        
		sMultipleChoiceAnswer.bIsCorrect = false; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		ssTemp << (int)((int)eTemp.vecAnswers[0].fValue + modMisc::iRandomNumberNonZero(-3, 4));
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
			eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		};
        
		sMultipleChoiceAnswer.bIsCorrect = false; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		ssTemp << (int)((int)eTemp.vecAnswers[0].fValue + modMisc::iRandomNumberNonZero(-4, 3));
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
			eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		};
        
		sMultipleChoiceAnswer.bIsCorrect = false; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		ssTemp << (int)((int)eTemp.vecAnswers[0].fValue + modMisc::iRandomNumberNonZero(-4, 4));
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
			eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		};

		sMultipleChoiceAnswer.bIsCorrect = false; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		ssTemp << (int)(iTotal / iNoOfFigures);
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
			eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		};
        
		sMultipleChoiceAnswer.bIsCorrect = false; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		ssTemp << (int)(iTotal / iNoOfFigures) + modMisc::iRandomNumberNonZero(-2, 2);
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
			eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		};
		
		sMultipleChoiceAnswer.bIsCorrect = false; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		ssTemp << (int)(iTotal / iNoOfFigures) + modMisc::iRandomNumberNonZero(-1, 2);
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
			eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		};
		
		sMultipleChoiceAnswer.bIsCorrect = false; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		ssTemp << (int)(iTotal / iNoOfFigures) + modMisc::iRandomNumberNonZero(-1, 1);
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
			eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		};
		
        tmCurrent = time (NULL);

	} while (eTemp.vecChoices.size() < 7 and tmStart + iCutOffTime < (int)(tmCurrent));
	
	
//	strFloatValue fRandomFigure;
//	strQuestion eTemp;
//	strStringAnswer sMultipleChoiceAnswer; 
//	std::stringstream ssTemp;
//	int iRangeMax;
//	int iRangeMin;
//	int iFigureMax;
//	int iFigureMin;
//	int iDiff;
//	sGroupName.clear();
//	time_t tmStart;
//	vecSetOfData.clear();
//	int iNoOfFigures;
//	int iFigureCounter;
//	std::stringstream ssQuestion;
//	int iFigure;
//	int iTotal;
//	int iPreviousFigure;
//	bool bOddSampleSize;
//	bool bIsFound;
//	vector<int>::iterator itSetOfData;
//	vector<int>::iterator itSetOfDataInner;
//	vector<int>::iterator itSetOfDataOutter;
//	int iMaxCount;

	return eTemp;
};

strQuestion ClsQuiz::randomQuestionType_FractionsAddSub(){
	strFloatValue fRandomFigure;
	strQuestion eTemp;
	strStringAnswer sMultipleChoiceAnswer; 
	vector<strStringAnswer>::iterator itMultipleChoiceAnswer; 
	std::stringstream ssTemp;
	int iDifficultyGroupLevel;
	std::string sGroupName;
	time_t tmStart;
	time_t tmTemp;
	int iFigureA;
	int iFigureB;
	int iFigureC;
	int iFigureD;
	int iRangeMin;
	int iRangeMax;
	int iNumerator;
	int iDenominator;
	int iCommon;
	
	//time_t tmTimestarted;
	
	tmStart = time (NULL);
	
	if (cPrimeNumbers == NULL) {
		cPrimeNumbers = new ClsPrimeNumbers;
	};
	
	
	eTemp.eQuestionGroup = eQuesGrpFractions;
	eTemp.bHasPic = false;
	sGroupName = modMisc::convertEnum_QuesTypeGroupText(eQuesGrpFractions);
	eTemp.ePic = ePic_None;
	
	eTemp.vecValues.clear();
	eTemp.vecAnswers.clear();
	eTemp.vecChoices.clear();
	
	
	const int gciQuestionTypeMax = 3; //note: needs to be the maximum integer in the enumQuestionType so that we can generate random question types 
	
	if (cPreferences->getType() == ePrefByGroup or cPreferences->getType() == ePrefBySubject) {
		iDifficultyGroupLevel = (int)cPreferences->getDifficulty(&sGroupName);
	} else {
		iDifficultyGroupLevel = (int)modMisc::iRandomNumber(1, gciQuestionTypeMax);
	};
	
	
	
	eTemp.sQuestionEncoded = "<0> / <1> + <2> / <3> =";
	eTemp.eAnswerType = eAnsTypEquation;
	
	
	
	switch (iDifficultyGroupLevel){ 
		case 1:
			eTemp.eQuestionType = eQuesFractionsAddSubVeryEasy;
			eTemp.iDifficulty = 660;

			iRangeMin = 2;
			iRangeMax = 8;
			break;
		case 2:
			eTemp.eQuestionType = eQuesFractionsAddSubEasy;
			eTemp.iDifficulty = 720;
			
			iRangeMin = 5;
			iRangeMax = 10;
			break;
		case 3:
			eTemp.eQuestionType = eQuesFractionsAddSubMedium;
			eTemp.iDifficulty = 775;
			
			iRangeMin = 6;
			iRangeMax = 12;
			break;
		case 4:
			eTemp.eQuestionType = eQuesFractionsAddSubHard;
			eTemp.iDifficulty = 830;
			
			iRangeMin = 10;
			iRangeMax = 16;
			break;
		case 5:
			eTemp.eQuestionType = eQuesFractionsAddSubVeryHard;
			eTemp.iDifficulty = 965;
			
			iRangeMin = 14;
			iRangeMax = 20;
			break;
		default:
			eTemp.eQuestionType = eQuesError;
			eTemp.bIsMultipleChoice = false;
			eTemp.sQuestionEncoded = "Error:";
	};
	
	if (modMisc::iRandomNumberNonZero(-2, 2) == 1) {
		iFigureA = modMisc::iRandomNumber(iRangeMin, modMisc::iRandomNumber(iRangeMin + 1, iRangeMax));
	} else {
		iFigureA = cPrimeNumbers->iRandomPrime(modMisc::iRandomNumber(iRangeMin, iRangeMax));
	};

	do {
		if (modMisc::iRandomNumber(4, 6) == 6) {
			iFigureB = modMisc::iRandomNumber(modMisc::iRandomNumber(iRangeMin, iRangeMax - 1), iRangeMax);
		} else {
			iFigureB = cPrimeNumbers->iRandomPrime(modMisc::iRandomNumber(iRangeMin, iRangeMax));
		};
	} while (iFigureA == iFigureB);
	
	do {
		if (modMisc::iRandomNumber(1, 4) == 3) {
			iFigureC = modMisc::iRandomNumber(iRangeMin, modMisc::iRandomNumber(iRangeMin + 1, iRangeMax));
		} else {
			iFigureC = cPrimeNumbers->iRandomPrime(modMisc::iRandomNumber(iRangeMin, iRangeMax));
		};
	} while (iFigureA == iFigureC or iFigureB == iFigureC);
	
	do {
		if (modMisc::iRandomNumber(10, 12) == 12) {
			iFigureD = modMisc::iRandomNumber(modMisc::iRandomNumber(iRangeMin, iRangeMax - 1), iRangeMax);
		} else {
			iFigureD = cPrimeNumbers->iRandomPrime(modMisc::iRandomNumber(iRangeMin, iRangeMax));
		};
	} while (iFigureA == iFigureD or iFigureB == iFigureD or iFigureC == iFigureD);
	
	iFigureC = iFigureC * modMisc::iRandomNumberNonZero(-1, 1);
	
	fRandomFigure.fValue = (float)iFigureA;
	eTemp.vecValues.push_back(fRandomFigure);
	fRandomFigure.fValue = (float)iFigureB;
	eTemp.vecValues.push_back(fRandomFigure);
	fRandomFigure.fValue = (float)iFigureC;
	eTemp.vecValues.push_back(fRandomFigure);
	fRandomFigure.fValue = (float)iFigureD;
	eTemp.vecValues.push_back(fRandomFigure);
	
	//iFigureA / iFigureB + iFigureC / iFigureD 
	//(iFigureA * iFigureD + iFigureC * iFigureB) / (iFigureD * iFigureB)
	
	iNumerator = ((iFigureA * iFigureD) + (iFigureC * iFigureB));
	iDenominator = iFigureD * iFigureB;
	
	iCommon = cPrimeNumbers->commonDenominator(iNumerator, iDenominator);

	if (iCommon != 0) {
		iNumerator = iNumerator / iCommon;
		iDenominator = iDenominator / iCommon;
	};
	

	
	fRandomFigure.fValue = (float)(eTemp.vecValues[0].fValue + eTemp.vecValues[1].fValue);
	eTemp.vecAnswers.push_back(fRandomFigure);
	
	//	if (modMisc::iRandomNumber(0, 10) < 5) {
	if (modMisc::iRandomNumber(0, 10) < 15) {
		//multiple choice
		
		//Correct answer
		eTemp.bIsMultipleChoice = true;
		eTemp.bOnlyOneAnswer = true;
		sMultipleChoiceAnswer.bIsCorrect = true; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		ssTemp << iNumerator << "/" << iDenominator;
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		
		eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		
		//wrong answers
		sMultipleChoiceAnswer.bIsCorrect = false; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		iNumerator = ((iFigureA * iFigureD) - (iFigureC * iFigureB));
		iDenominator = iFigureD * iFigureB;
		
        if (iDenominator < 0) {
            iNumerator = -1 * iNumerator;
            iDenominator = -1 * iDenominator;
        };
        
		iCommon = cPrimeNumbers->commonDenominator(iNumerator, iDenominator);
		
		if (iCommon != 0) {
			iNumerator = iNumerator / iCommon;
			iDenominator = iDenominator / iCommon;
		};
		
		ssTemp << iNumerator << "/" << iDenominator;
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
			eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		};
		
		
		
		
		sMultipleChoiceAnswer.bIsCorrect = false; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		iNumerator = iFigureA + iFigureC;
		iDenominator = iFigureD + iFigureB;

        if (iDenominator < 0) {
            iNumerator = -1 * iNumerator;
            iDenominator = -1 * iDenominator;
        };

		iCommon = cPrimeNumbers->commonDenominator(iNumerator, iDenominator);
		
		if (iCommon != 0) {
			iNumerator = iNumerator / iCommon;
			iDenominator = iDenominator / iCommon;
		};
		
		ssTemp << iNumerator << "/" << iDenominator;
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
			eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		};
		
		
		
		
		sMultipleChoiceAnswer.bIsCorrect = false; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		iNumerator = ((iFigureA * iFigureB) + (iFigureC * iFigureD));
		iDenominator = iFigureD * iFigureB;
		
        if (iDenominator < 0) {
            iNumerator = -1 * iNumerator;
            iDenominator = -1 * iDenominator;
        };

		iCommon = cPrimeNumbers->commonDenominator(iNumerator, iDenominator);
		
		if (iCommon != 0) {
			iNumerator = iNumerator / iCommon;
			iDenominator = iDenominator / iCommon;
		};
		
		ssTemp << iNumerator << "/" << iDenominator;
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
			eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		};
		
		do {
			
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			iNumerator = ((iFigureA * iFigureD) + (iFigureC * iFigureB));
			iDenominator = iFigureD * (iFigureB + modMisc::iRandomNumberNonZero(-1, 1));
			
            if (iDenominator < 0) {
                iNumerator = -1 * iNumerator;
                iDenominator = -1 * iDenominator;
            };

            iCommon = cPrimeNumbers->commonDenominator(iNumerator, iDenominator);
			
			if (iCommon != 0) {
				iNumerator = iNumerator / iCommon;
				iDenominator = iDenominator / iCommon;
			};
			
			ssTemp << iNumerator << "/" << iDenominator;
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
				eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
			};
			
			
			
			
			
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			iNumerator = ((iFigureA * iFigureD) + (iFigureC * iFigureB));
			iDenominator = (iFigureD + modMisc::iRandomNumberNonZero(-1, 1)) * iFigureB;
			
            if (iDenominator < 0) {
                iNumerator = -1 * iNumerator;
                iDenominator = -1 * iDenominator;
            };

			iCommon = cPrimeNumbers->commonDenominator(iNumerator, iDenominator);
			
			if (iCommon != 0) {
				iNumerator = iNumerator / iCommon;
				iDenominator = iDenominator / iCommon;
			};
			
			ssTemp << iNumerator << "/" << iDenominator;
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
				eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
			};
			
			
			
			
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			iNumerator = ((iFigureA * iFigureD) + (iFigureC * (iFigureB + modMisc::iRandomNumberNonZero(-1, 1))));
			iDenominator = iFigureD * iFigureB;
            
            if (iDenominator < 0) {
                iNumerator = -1 * iNumerator;
                iDenominator = -1 * iDenominator;
            };
            
			iCommon = cPrimeNumbers->commonDenominator(iNumerator, iDenominator);
			
			if (iCommon != 0) {
				iNumerator = iNumerator / iCommon;
				iDenominator = iDenominator / iCommon;
			};
			
			ssTemp << iNumerator << "/" << iDenominator;
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
				eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
			};
			
			
			
			
			
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			iNumerator = ((iFigureA * iFigureD) + (iFigureC * iFigureB)) + (modMisc::iRandomNumberNonZero(-1, 1) * modMisc::iRandomNumber(iRangeMin, iRangeMax)) ;
			iDenominator = iFigureD * iFigureB;
            
            if (iDenominator < 0) {
                iNumerator = -1 * iNumerator;
                iDenominator = -1 * iDenominator;
            };
            
			iCommon = cPrimeNumbers->commonDenominator(iNumerator, iDenominator);
			
			if (iCommon != 0) {
				iNumerator = iNumerator / iCommon;
				iDenominator = iDenominator / iCommon;
			};
			
			ssTemp << iNumerator << "/" << iDenominator;
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
				eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
			};
			
			
			
			
			
			
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			iNumerator = ((iFigureA * iFigureD) + (iFigureC * iFigureB));
			iDenominator = (iFigureD * iFigureB) + modMisc::iRandomNumberNonZero(-2, 2);
            
            if (iDenominator < 0) {
                iNumerator = -1 * iNumerator;
                iDenominator = -1 * iDenominator;
            };
            
			iCommon = cPrimeNumbers->commonDenominator(iNumerator, iDenominator);
			
			if (iCommon != 0) {
				iNumerator = iNumerator / iCommon;
				iDenominator = iDenominator / iCommon;
			};
			
			ssTemp << iNumerator << "/" << iDenominator;
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
				eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
			};
			
			
			
			
			
			
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			iNumerator = ((iFigureA * iFigureD) + (iFigureC * iFigureB));
			iDenominator = (iFigureD * iFigureB) + modMisc::iRandomNumber((int)(iRangeMin / 3), (int)(iRangeMax / 2));
            
            if (iDenominator < 0) {
                iNumerator = -1 * iNumerator;
                iDenominator = -1 * iDenominator;
            };
            
			iCommon = cPrimeNumbers->commonDenominator(iNumerator, iDenominator);
			
			if (iCommon != 0) {
				iNumerator = iNumerator / iCommon;
				iDenominator = iDenominator / iCommon;
			};
			
			ssTemp << iNumerator << "/" << iDenominator;
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
				eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
			};
			
			
			
			
			
			
			
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			iNumerator = ((iFigureA * iFigureD) + (iFigureC * iFigureB));
			iDenominator = iFigureD * iFigureB;
            
            if (iDenominator < 0) {
                iNumerator = -1 * iNumerator;
                iDenominator = -1 * iDenominator;
            };
            
			iCommon = cPrimeNumbers->commonDenominator(iNumerator, iDenominator) + modMisc::iRandomNumberNonZero(-3, 3);
			
			if (iCommon != 0) {
				iNumerator = iNumerator / iCommon;
				iDenominator = iDenominator / iCommon;
			};
			
			ssTemp << iNumerator << "/" << iDenominator;
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
				eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
			};
			
			tmTemp = time (NULL);
		} while (eTemp.vecChoices.size() < 7 and ((int)tmStart + iCutOffTime) < ((int)tmTemp));
			
//		};
		
	} else {
		//NOT multiple choice
		eTemp.bIsMultipleChoice = false;
	};
	

//	strFloatValue fRandomFigure;
//	strQuestion eTemp;
//	strStringAnswer sMultipleChoiceAnswer; 
//	vector<strStringAnswer>::iterator itMultipleChoiceAnswer; 
//	std::stringstream ssTemp;
//	int iDifficultyGroupLevel;
	sGroupName.clear();
//	time_t tmStart;
//	time_t tmTemp;
//	int iFigureA;
//	int iFigureB;
//	int iFigureC;
//	int iFigureD;
//	int iRangeMin;
//	int iRangeMax;
//	int iNumerator;
//	int iDenominator;
//	int iCommon;
	
	return eTemp;
};


strQuestion ClsQuiz::randomQuestionType_FractionsMulitpleDivide(){
	strFloatValue fRandomFigure;
	strQuestion eTemp;
	strStringAnswer sMultipleChoiceAnswer; 
	vector<strStringAnswer>::iterator itMultipleChoiceAnswer; 
	std::stringstream ssTemp;
	int iDifficultyGroupLevel;
	std::string sGroupName;
	time_t tmStart;
	time_t tmTemp;
	int iFigureA;
	int iFigureB;
	int iFigureC;
	int iFigureD;
	int iRangeMin;
	int iRangeMax;
	int iNumerator;
	int iDenominator;
	int iCommon;
	bool bIsDivide;
	
	//time_t tmTimestarted;
	
	tmStart = time (NULL);
	
	if (cPrimeNumbers == NULL) {
		cPrimeNumbers = new ClsPrimeNumbers;
	};
	
	eTemp.eQuestionGroup = eQuesGrpFractions;
	eTemp.bHasPic = false;
	sGroupName = modMisc::convertEnum_QuesTypeGroupText(eQuesGrpFractions);
	eTemp.ePic = ePic_None;
	
	eTemp.vecValues.clear();
	eTemp.vecAnswers.clear();
	eTemp.vecChoices.clear();
	
	
	const int gciQuestionTypeMax = 3; //note: needs to be the maximum integer in the enumQuestionType so that we can generate random question types 
	
	if (cPreferences->getType() == ePrefByGroup or cPreferences->getType() == ePrefBySubject) {
		iDifficultyGroupLevel = (int)cPreferences->getDifficulty(&sGroupName);
	} else {
		iDifficultyGroupLevel = (int)modMisc::iRandomNumber(1, gciQuestionTypeMax);
	};
	
	
	if (modMisc::iRandomNumber(1, 2) == 1) {
		bIsDivide = true;
	} else {
		bIsDivide = false;
	};

	
	if (bIsDivide == true) {
		eTemp.sQuestionEncoded = "(<0>/<1>) / (<2>/<3>) =";
	} else {
		eTemp.sQuestionEncoded = "<0>/<1> * <2>/<3> =";
	};

	eTemp.eAnswerType = eAnsTypEquation;
	
	
	
	switch (iDifficultyGroupLevel){ 
		case 1:
			eTemp.eQuestionType = eQuesFractionsMultiDivVeryEasy;
			eTemp.iDifficulty = 475;
			
			iRangeMin = 2;
			iRangeMax = 8;
			break;
		case 2:
			eTemp.eQuestionType = eQuesFractionsMultiDivEasy;
			eTemp.iDifficulty = 530;
			
			iRangeMin = 5;
			iRangeMax = 10;
			break;
		case 3:
			eTemp.eQuestionType = eQuesFractionsMultiDivMedium;
			eTemp.iDifficulty = 570;
			
			iRangeMin = 8;
			iRangeMax = 15;
			break;
		case 4:
			eTemp.eQuestionType = eQuesFractionsMultiDivHard;
			eTemp.iDifficulty = 615;
			
			iRangeMin = 10;
			iRangeMax = 18;
			break;
		case 5:
			eTemp.eQuestionType = eQuesFractionsMultiDivVeryHard;
			eTemp.iDifficulty = 660;
			
			iRangeMin = 12;
			iRangeMax = 25;
			break;
		default:
			eTemp.eQuestionType = eQuesError;
			eTemp.bIsMultipleChoice = false;
			eTemp.sQuestionEncoded = "Error:";
	};
	
	if (modMisc::iRandomNumberNonZero(-2, 2) == 1) {
		iFigureA = modMisc::iRandomNumber(iRangeMin, modMisc::iRandomNumber(iRangeMin + 1, iRangeMax));
	} else {
		iFigureA = cPrimeNumbers->iRandomPrime(modMisc::iRandomNumber(iRangeMin, iRangeMax));
	};

	do {
		iFigureB = modMisc::iRandomNumber(modMisc::iRandomNumber(iRangeMin, iRangeMax - 1), iRangeMax);
	} while (iFigureA == iFigureB);
	
	do {
		iFigureC = modMisc::iRandomNumber(iRangeMin, modMisc::iRandomNumber(iRangeMin + 1, iRangeMax));
	} while (iFigureA == iFigureC or iFigureB == iFigureC);
	
	do {
		iFigureD = modMisc::iRandomNumber(modMisc::iRandomNumber(iRangeMin, iRangeMax - 1), iRangeMax);
	} while (iFigureA == iFigureD or iFigureB == iFigureD or iFigureC == iFigureD);
	
	fRandomFigure.fValue = (float)iFigureA;
	eTemp.vecValues.push_back(fRandomFigure);
	fRandomFigure.fValue = (float)iFigureB;
	eTemp.vecValues.push_back(fRandomFigure);
	fRandomFigure.fValue = (float)iFigureC;
	eTemp.vecValues.push_back(fRandomFigure);
	fRandomFigure.fValue = (float)iFigureD;
	eTemp.vecValues.push_back(fRandomFigure);
	

	if (bIsDivide == true) {
		//(iFigureA / iFigureB) / (iFigureC / iFigureD) 
		//(iFigureA * iFigureD) / (iFigureC * iFigureB)
		int iSwap;
		
		iSwap = iFigureD;
		iFigureD = iFigureC;
		iFigureC = iSwap;
	} else {
		//iFigureA / iFigureB * iFigureC / iFigureD 
		//(iFigureA * iFigureC) / (iFigureD * iFigureB)
	};
	
	iNumerator = iFigureA * iFigureC;
	iDenominator = iFigureD * iFigureB;
	
	iCommon = cPrimeNumbers->commonDenominator(iNumerator, iDenominator);
	
	if (iCommon != 0) {
		iNumerator = iNumerator / iCommon;
		iDenominator = iDenominator / iCommon;
	};
	
	
	
	fRandomFigure.fValue = (float)(eTemp.vecValues[0].fValue + eTemp.vecValues[1].fValue);
	eTemp.vecAnswers.push_back(fRandomFigure);
	
	//	if (modMisc::iRandomNumber(0, 10) < 5) {
	if (modMisc::iRandomNumber(0, 10) < 15) {
		//multiple choice
		
		//Correct answer
		eTemp.bIsMultipleChoice = true;
		eTemp.bOnlyOneAnswer = true;
		sMultipleChoiceAnswer.bIsCorrect = true; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		ssTemp << iNumerator << "/" << iDenominator;
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		
		eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		
		//wrong answers
		sMultipleChoiceAnswer.bIsCorrect = false; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		iNumerator = ((iFigureA * iFigureD) - (iFigureC * iFigureB));
		iDenominator = iFigureD * iFigureB;
		
		iCommon = cPrimeNumbers->commonDenominator(iNumerator, iDenominator);
		
		if (iCommon != 0) {
			iNumerator = iNumerator / iCommon;
			iDenominator = iDenominator / iCommon;
		};
		
		ssTemp << iNumerator << "/" << iDenominator;
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
			eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		};
		
		
		
		
		sMultipleChoiceAnswer.bIsCorrect = false; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		iNumerator = ((iFigureA * iFigureD) + (iFigureC * iFigureB));
		iDenominator = iFigureD * iFigureB;
		
		iCommon = cPrimeNumbers->commonDenominator(iNumerator, iDenominator);
		
		if (iCommon != 0) {
			iNumerator = iNumerator / iCommon;
			iDenominator = iDenominator / iCommon;
		};
		
		ssTemp << iNumerator << "/" << iDenominator;
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
			eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		};
		
		
		
		
		sMultipleChoiceAnswer.bIsCorrect = false; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		iNumerator = iFigureA + iFigureC;
		iDenominator = iFigureD + iFigureB;
		
		iCommon = cPrimeNumbers->commonDenominator(iNumerator, iDenominator);
		
		if (iCommon != 0) {
			iNumerator = iNumerator / iCommon;
			iDenominator = iDenominator / iCommon;
		};
		
		ssTemp << iNumerator << "/" << iDenominator;
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
			eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		};
		
		
		
		
		sMultipleChoiceAnswer.bIsCorrect = false; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		iNumerator = ((iFigureA * iFigureB) + (iFigureC * iFigureD));
		iDenominator = iFigureD * iFigureB;
		
		iCommon = cPrimeNumbers->commonDenominator(iNumerator, iDenominator);
		
		if (iCommon != 0) {
			iNumerator = iNumerator / iCommon;
			iDenominator = iDenominator / iCommon;
		};
		
		ssTemp << iNumerator << "/" << iDenominator;
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
			eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		};
		
		do {

			
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");

			iNumerator = iFigureA * iFigureC;
			iDenominator = iFigureD * (iFigureB + modMisc::iRandomNumberNonZero(-1, 1));
			
			iCommon = cPrimeNumbers->commonDenominator(iNumerator, iDenominator);
			
			if (iCommon != 0) {
				iNumerator = iNumerator / iCommon;
				iDenominator = iDenominator / iCommon;
			};
			
			ssTemp << iNumerator << "/" << iDenominator;
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
				eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
			};
			
			
			
			
			
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			iNumerator = iFigureA * iFigureC;
			iDenominator = (iFigureD + modMisc::iRandomNumberNonZero(-1, 1)) * iFigureB;
			
			iCommon = cPrimeNumbers->commonDenominator(iNumerator, iDenominator);
			
			if (iCommon != 0) {
				iNumerator = iNumerator / iCommon;
				iDenominator = iDenominator / iCommon;
			};
			
			ssTemp << iNumerator << "/" << iDenominator;
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
				eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
			};
			
			
			
			
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			iNumerator = (iFigureA + modMisc::iRandomNumberNonZero(-1, 1)) * iFigureC;
			iDenominator = iFigureD * iFigureB;
			
			iCommon = cPrimeNumbers->commonDenominator(iNumerator, iDenominator);
			
			if (iCommon != 0) {
				iNumerator = iNumerator / iCommon;
				iDenominator = iDenominator / iCommon;
			};
			
			ssTemp << iNumerator << "/" << iDenominator;
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
				eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
			};
			
			
			
			
			
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			iNumerator = iFigureA * (iFigureC + modMisc::iRandomNumberNonZero(-1, 1));
			iDenominator = iFigureD * iFigureB;
			
			iCommon = cPrimeNumbers->commonDenominator(iNumerator, iDenominator);
			
			if (iCommon != 0) {
				iNumerator = iNumerator / iCommon;
				iDenominator = iDenominator / iCommon;
			};
			
			ssTemp << iNumerator << "/" << iDenominator;
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
				eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
			};
			
			
			
			
			
			
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			iNumerator = iFigureA * iFigureC;
			iDenominator = iFigureD * iFigureB + modMisc::iRandomNumberNonZero(-2, 2);
			
			iCommon = cPrimeNumbers->commonDenominator(iNumerator, iDenominator);
			
			if (iCommon != 0) {
				iNumerator = iNumerator / iCommon;
				iDenominator = iDenominator / iCommon;
			};
			
			ssTemp << iNumerator << "/" << iDenominator;
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
				eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
			};
			
			
			
			
			
			
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			iNumerator = iFigureA * iFigureC;
			iDenominator = iFigureD * iFigureB + modMisc::iRandomNumberNonZero((int)(iRangeMin / 3), (int)(iRangeMax / 2));
			
			iCommon = cPrimeNumbers->commonDenominator(iNumerator, iDenominator);
			
			if (iCommon != 0) {
				iNumerator = iNumerator / iCommon;
				iDenominator = iDenominator / iCommon;
			};
			
			ssTemp << iNumerator << "/" << iDenominator;
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
				eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
			};
			
			
			
			
			
			
			
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			iNumerator = iFigureA * iFigureC;
			iDenominator = iFigureD * iFigureB;
			
			iCommon = cPrimeNumbers->commonDenominator(iNumerator, iDenominator) + modMisc::iRandomNumberNonZero(-3, 3);
			
			if (iCommon != 0) {
				iNumerator = iNumerator / iCommon;
				iDenominator = iDenominator / iCommon;
			};
			
			ssTemp << iNumerator << "/" << iDenominator;
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
				eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
			};
			
			tmTemp = time (NULL);
		} while (eTemp.vecChoices.size() < 7 and ((int)tmStart + modMisc::iCutOffTime) < ((int)tmTemp));
		
		//		};
		
	} else {
		//NOT multiple choice
		eTemp.bIsMultipleChoice = false;
	};
	

//	strFloatValue fRandomFigure;
//	strQuestion eTemp;
//	strStringAnswer sMultipleChoiceAnswer; 
//	vector<strStringAnswer>::iterator itMultipleChoiceAnswer; 
//	std::stringstream ssTemp;
//	int iDifficultyGroupLevel;
	sGroupName.clear();
//	time_t tmStart;
//	time_t tmTemp;
//	int iFigureA;
//	int iFigureB;
//	int iFigureC;
//	int iFigureD;
//	int iRangeMin;
//	int iRangeMax;
//	int iNumerator;
//	int iDenominator;
//	int iCommon;
//	bool bIsDivide;
	return eTemp;
};

strQuestion ClsQuiz::randomQuestionType_EquationsQuadratic(){
	strFloatValue fRandomFigure;
	strQuestion eTemp;
	strStringAnswer sMultipleChoiceAnswer; 
	vector<strStringAnswer>::iterator itMultipleChoiceAnswer; 
	std::stringstream ssTemp;
	int iDifficultyGroupLevel;
	std::string sGroupName;
	std::stringstream ssEquation;
	time_t tmStart;
	time_t tmTemp;
	int iFigureA;
	int iFigureB;
	int iFigureC;
	int iRangeMin;
	int iRangeMax;
	
	//time_t tmTimestarted;
	
	tmStart = time (NULL);
	
//	if (cPrimeNumbers == NULL) {
//		cPrimeNumbers = new ClsPrimeNumbers;
//	};
	
	eTemp.eQuestionGroup = eQuesGrpEquations;
	eTemp.bHasPic = false;
	sGroupName = modMisc::convertEnum_QuesTypeGroupText(eQuesGrpEquations);
	eTemp.ePic = ePic_None;
	
	eTemp.vecValues.clear();
	eTemp.vecAnswers.clear();
	eTemp.vecChoices.clear();
	
	
	const int gciQuestionTypeMax = 3; //note: needs to be the maximum integer in the enumQuestionType so that we can generate random question types 
	
	if (cPreferences->getType() == ePrefByGroup or cPreferences->getType() == ePrefBySubject) {
		iDifficultyGroupLevel = (int)cPreferences->getDifficulty(&sGroupName);
	} else {
		iDifficultyGroupLevel = (int)modMisc::iRandomNumber(1, gciQuestionTypeMax);
	};
	
	//eTemp.sQuestionEncoded = "<0>X<^2> + <1>X + <2> = 0/What is X?";
	eTemp.eAnswerType = eAnsTypEquation;
	
	switch (iDifficultyGroupLevel){ 
		case 1:
			eTemp.eQuestionType = eQuesEquationsQuadraticVeryEasy;
			eTemp.iDifficulty = 675;
			
			iRangeMin = 1;
			iRangeMax = 6;
			break;
		case 2:
			eTemp.eQuestionType = eQuesEquationsQuadraticEasy;
			eTemp.iDifficulty = 750;
			
			iRangeMin = 5;
			iRangeMax = 10;
			break;
		case 3:
			eTemp.eQuestionType = eQuesEquationsQuadraticMedium;
			eTemp.iDifficulty = 815;
			
			iRangeMin = 8;
			iRangeMax = 15;
			break;
		case 4:
			eTemp.eQuestionType = eQuesEquationsQuadraticHard;
			eTemp.iDifficulty = 870;
			
			iRangeMin = 10;
			iRangeMax = 18;
			break;
		case 5:
			eTemp.eQuestionType = eQuesEquationsQuadraticVeryHard;
			eTemp.iDifficulty = 915;
			
			iRangeMin = 12;
			iRangeMax = 25;
			break;
		default:
			eTemp.eQuestionType = eQuesError;
			eTemp.bIsMultipleChoice = false;
			eTemp.sQuestionEncoded = "Error:";
	};
	
	iFigureA = modMisc::iRandomNumber(iRangeMin, iRangeMax);
	iFigureA = iFigureA * modMisc::iRandomNumberNonZero(-1, 1); 
	
	do {
		iFigureB = modMisc::iRandomNumber(iRangeMin, iRangeMax);
		iFigureB = iFigureB * modMisc::iRandomNumberNonZero(-1, 1); 
	} while (iFigureA == iFigureB);
	
	fRandomFigure.fValue = (float)iFigureA;
	eTemp.vecAnswers.push_back(fRandomFigure);
	
	fRandomFigure.fValue = (float)iFigureB;
	eTemp.vecAnswers.push_back(fRandomFigure);

	iFigureC = modMisc::iRandomNumberNonZero(1, (int)sqrt((float)(iRangeMin + iRangeMax)));
	iFigureC = iFigureC * modMisc::iRandomNumberNonZero(-1, 1); 
/*
	if (iFigureC == 1) {
		if (iFigureB == 1) {
			eTemp.sQuestionEncoded = "X<^2> + X + <0> = 0\nWhat is X?";
		} else if (iFigureB == -1) {
			eTemp.sQuestionEncoded = "X<^2> - X + <0> = 0\nWhat is X?";
		} else {
			eTemp.sQuestionEncoded = "X<^2> + <1>X + <0> = 0\nWhat is X?";
		};
	} else {
		if (iFigureB == 1) {
			eTemp.sQuestionEncoded = "<2>X<^2> + X + <0> = 0\nWhat is X?";
		} else if (iFigureB == -1) {
			eTemp.sQuestionEncoded = "<2>X<^2> - X + <0> = 0\nWhat is X?";
		} else {
			eTemp.sQuestionEncoded = "<2>X<^2> + <1>X + <0> = 0\nWhat is X?";
		};
	};
*/
	ssEquation.clear();
	
	if (iFigureC == 1) {
		ssEquation << "X<^2>";
	} else if (iFigureC == -1) {
		ssEquation << "-X<^2>";
	} else {
		ssEquation << "<2>X<^2>";
	};
	
	if (((iFigureA + iFigureB) * iFigureC) == 0) {
		ssEquation << " ";
	} else if (((iFigureA + iFigureB) * iFigureC) == 1) {
		ssEquation << " + X";
	} else if (((iFigureA + iFigureB) * iFigureC) == -1) {
		ssEquation << " - X";
	} else {
		ssEquation << " + <1>X";
	};	

	if (((iFigureA * iFigureB) * iFigureC) != 0) {
		ssEquation << " + <0>";
	};

	ssEquation << " = 0\nWhat are all the values of X?";

	eTemp.sQuestionEncoded = ssEquation.str();
	
	fRandomFigure.fValue = (float)((iFigureA * iFigureB) * iFigureC);
	eTemp.vecValues.push_back(fRandomFigure);
	fRandomFigure.fValue = (float)((iFigureA + iFigureB) * iFigureC);
	eTemp.vecValues.push_back(fRandomFigure);
	fRandomFigure.fValue = (float)iFigureC;
	eTemp.vecValues.push_back(fRandomFigure);
	
	eTemp.bIsMultipleChoice = true;
	eTemp.bOnlyOneAnswer = false;
	
	//	if (modMisc::iRandomNumber(0, 10) < 5) {
	if (modMisc::iRandomNumber(0, 10) < 15) {
		//multiple choice
		
		//Correct answer
		sMultipleChoiceAnswer.bIsCorrect = true; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		ssTemp << (-1 * iFigureA);
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		
		eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		
		sMultipleChoiceAnswer.bIsCorrect = true; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		ssTemp << (-1 * iFigureB);
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		
		eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		
		//wrong answers
		sMultipleChoiceAnswer.bIsCorrect = false; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		ssTemp << (((iFigureA + iFigureB) / iFigureC) * modMisc::iRandomNumberNonZero(-1, 1));
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
			eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		};
		
		sMultipleChoiceAnswer.bIsCorrect = false; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		ssTemp << (((iFigureA * iFigureB) / iFigureC) * modMisc::iRandomNumberNonZero(-1, 1));
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
			eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		};
		
		sMultipleChoiceAnswer.bIsCorrect = false; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		ssTemp << ((iFigureA / iFigureC) * modMisc::iRandomNumberNonZero(-1, 1));
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
			eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		};
		
		sMultipleChoiceAnswer.bIsCorrect = false; 
		sMultipleChoiceAnswer.bIsSelected = false; 
		ssTemp.str("");
		ssTemp << ((iFigureB / iFigureC) * modMisc::iRandomNumberNonZero(-1, 1));
		sMultipleChoiceAnswer.sValue = ssTemp.str();
		if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
			eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
		};
		
		do {
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << ((((iFigureA + iFigureB) / iFigureC) * modMisc::iRandomNumberNonZero(-1, 1)) + modMisc::iRandomNumberNonZero(-3, 3));
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
				eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
			};
			
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << ((((iFigureA * iFigureB) / iFigureC) * modMisc::iRandomNumberNonZero(-1, 1)) + modMisc::iRandomNumberNonZero(-3, 3));
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
				eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
			};
			
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << ((((iFigureA + iFigureB) / iFigureC) * modMisc::iRandomNumberNonZero(-1, 1)) + modMisc::iRandomNumberNonZero(-2, 2));
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
				eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
			};
			
			sMultipleChoiceAnswer.bIsCorrect = false; 
			sMultipleChoiceAnswer.bIsSelected = false; 
			ssTemp.str("");
			ssTemp << ((((iFigureA * iFigureB) / iFigureC) * modMisc::iRandomNumberNonZero(-1, 1)) + modMisc::iRandomNumberNonZero(-2, 2));
			sMultipleChoiceAnswer.sValue = ssTemp.str();
			if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
				eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
			};
			
			tmTemp = time (NULL);
		} while (eTemp.vecChoices.size() < 7 and ((int)tmStart + iCutOffTime) < ((int)tmTemp));
		
	} else {
		//NOT multiple choice
		eTemp.bIsMultipleChoice = false;
	};
	
	
//	strFloatValue fRandomFigure;
//	strQuestion eTemp;
//	strStringAnswer sMultipleChoiceAnswer; 
//	vector<strStringAnswer>::iterator itMultipleChoiceAnswer; 
//	std::stringstream ssTemp;
//	int iDifficultyGroupLevel;
	sGroupName.clear();
//	std::stringstream ssEquation;
//	time_t tmStart;
//	time_t tmTemp;
//	int iFigureA;
//	int iFigureB;
//	int iFigureC;
//	int iRangeMin;
//	int iRangeMax;
	
	return eTemp;
};

strQuestion ClsQuiz::randomQuestionType_EquationsSimultaneousLinear(){
	strFloatValue fRandomFigure;
	strQuestion eTemp;
	strStringAnswer sMultipleChoiceAnswer; 
	vector<strStringAnswer>::iterator itMultipleChoiceAnswer; 
	std::stringstream ssTemp;
	int iDifficultyGroupLevel;
	std::string sGroupName;
	std::stringstream ssEquation;
	time_t tmStart;
	time_t tmTemp;
	int iFigureA;
	int iFigureB;
	int iFigureC;
	int iFigureD;
	int iFigureE;
	int iFigureF;
	int iFigureG;
	int iFigureH;
	int iFigureI;
	int iFigureJ;
	int iFigureK;
	int iFigureL;
    int iFigureM;
    int iFigureN;
    int iFigureO;
    int iFigureP;
    int iFigureQ;
    int iFigureR;
    int iFigureS;
    int iFigureT;
	int iFigureW;
	int iFigureX;
	int iFigureY;
	int iFigureZ;
	int iRangeMin;
	int iRangeMax;
	
	//time_t tmTimestarted;
	
	tmStart = time (NULL);
	
    //	if (cPrimeNumbers == NULL) {
    //		cPrimeNumbers = new ClsPrimeNumbers;
    //	};
	
	eTemp.eQuestionGroup = eQuesGrpEquations;
	eTemp.bHasPic = false;
	sGroupName = modMisc::convertEnum_QuesTypeGroupText(eQuesGrpEquations);
	eTemp.ePic = ePic_None;
	
	eTemp.vecValues.clear();
	eTemp.vecAnswers.clear();
	eTemp.vecChoices.clear();
	
	
	const int gciQuestionTypeMax = 3; //note: needs to be the maximum integer in the enumQuestionType so that we can generate random question types 
	
	if (cPreferences->getType() == ePrefByGroup or cPreferences->getType() == ePrefBySubject) {
		iDifficultyGroupLevel = (int)cPreferences->getDifficulty(&sGroupName);
	} else {
		iDifficultyGroupLevel = (int)modMisc::iRandomNumber(1, gciQuestionTypeMax);
	};
	
	eTemp.eAnswerType = eAnsTypEquation;
	
	switch (iDifficultyGroupLevel){ 
		case 1:
            // just X and Y small numbers
			eTemp.eQuestionType = eQuesEquationsSimultaneousLinearVeryEasy;
			eTemp.iDifficulty = 865;
			
			iRangeMin = 1;
			iRangeMax = 9;
			break;
		case 2:
            // just X and Y very big numbers
			eTemp.eQuestionType = eQuesEquationsSimultaneousLinearEasy;
			eTemp.iDifficulty = 780;
			
			iRangeMin = 9;
			iRangeMax = 20;
			break;
		case 3:
            // just X, Y and Z very small numbers
			eTemp.eQuestionType = eQuesEquationsSimultaneousLinearMedium;
			eTemp.iDifficulty = 895;
			
			iRangeMin = 1;
			iRangeMax = 8;
			break;
		case 4:
            // just X, Y and Z very big numbers
			eTemp.eQuestionType = eQuesEquationsSimultaneousLinearHard;
			eTemp.iDifficulty = 910;
			
			iRangeMin = 7;
			iRangeMax = 20;
			break;
		case 5:
            // just W, X, Y and Z very small numbers
			eTemp.eQuestionType = eQuesEquationsSimultaneousLinearVeryHard;
			eTemp.iDifficulty = 975;
			
			iRangeMin = 1;
			iRangeMax = 8;
			break;
		default:
			eTemp.eQuestionType = eQuesError;
			eTemp.bIsMultipleChoice = false;
			eTemp.sQuestionEncoded = "Error:";
	};
	
    eTemp.bIsMultipleChoice = true;
	eTemp.bOnlyOneAnswer = true;

    switch (iDifficultyGroupLevel){ 
		case 1:
		case 2:
            //AX+BY=C
            //DX+EY=F
            do {
                iFigureA = modMisc::iRandomNumberNonZero(-1, 1) * modMisc::iRandomNumber(iRangeMin, iRangeMax);
                iFigureB = modMisc::iRandomNumberNonZero(-1, 1) * modMisc::iRandomNumber(iRangeMin, iRangeMax);
                iFigureD = modMisc::iRandomNumberNonZero(-1, 1) * modMisc::iRandomNumber(iRangeMin, iRangeMax);
                iFigureE = modMisc::iRandomNumberNonZero(-1, 1) * modMisc::iRandomNumber(iRangeMin, iRangeMax);
                
                //check the lines are not paralell
            } while (modMisc::almostEquals((float)(iFigureA / iFigureB), (float)(iFigureD / iFigureE), 0.1) == true);
            
            iFigureX = modMisc::iRandomNumberNonZero(-1, 1) * modMisc::iRandomNumber(iRangeMin, iRangeMax);
            iFigureY = modMisc::iRandomNumberNonZero(-1, 1) * modMisc::iRandomNumber(iRangeMin, iRangeMax);
            
            iFigureC = (iFigureA * iFigureX) + (iFigureB * iFigureY);
            iFigureF = (iFigureD * iFigureX) + (iFigureE * iFigureY);
            
            fRandomFigure.fValue = (float)iFigureX;
            eTemp.vecAnswers.push_back(fRandomFigure);
            fRandomFigure.fValue = (float)iFigureY;
            eTemp.vecAnswers.push_back(fRandomFigure);
            
            fRandomFigure.fValue = (float)iFigureA;
            eTemp.vecValues.push_back(fRandomFigure);
            fRandomFigure.fValue = (float)iFigureB;
            eTemp.vecValues.push_back(fRandomFigure);
            fRandomFigure.fValue = (float)iFigureC;
            eTemp.vecValues.push_back(fRandomFigure);
            fRandomFigure.fValue = (float)iFigureD;
            eTemp.vecValues.push_back(fRandomFigure);
            fRandomFigure.fValue = (float)iFigureE;
            eTemp.vecValues.push_back(fRandomFigure);
            fRandomFigure.fValue = (float)iFigureF;
            eTemp.vecValues.push_back(fRandomFigure);

            
            
            ssEquation.clear();
            // "What values of X and Y solve these simultaneous linear equations?\n<0>X+<1>Y=<2>\n<3>X+<4>Y=<5>"
            ssEquation << "What values of X and Y solve these simultaneous linear equations?\n";
            
            ////////////////////////////

            if (iFigureA == -1) {
                ssEquation << "-";
            } else if (iFigureA != 1) {
                ssEquation << "<0>";
            };
            
            ssEquation << "X + ";

            if (iFigureB == -1) {
                ssEquation << "-";
            } else if (iFigureB != 1) {
                ssEquation << "<1>";
            };
            
            ssEquation << "Y = <2>\n";

            ////////////////////////////

            if (iFigureD == -1) {
                ssEquation << "-";
            } else if (iFigureD != 1) {
                ssEquation << "<3>";
            };
            
            ssEquation << "X + ";
            
            if (iFigureE == -1) {
                ssEquation << "-";
            } else if (iFigureE != 1) {
                ssEquation << "<4>";
            };
            
            ssEquation << "Y = <5>";

            ////////////////////////////

            eTemp.sQuestionEncoded = ssEquation.str();
            
            //Correct answer
            sMultipleChoiceAnswer.bIsCorrect = true; 
            sMultipleChoiceAnswer.bIsSelected = false; 
            ssTemp.str("");
            ssTemp << "X=" << iFigureX << " Y=" << iFigureY;
            sMultipleChoiceAnswer.sValue = ssTemp.str();
            
            eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
            
            //wrong answers
            do {
                for (int iRange = 1; iRange < 5; iRange++) {
                    sMultipleChoiceAnswer.bIsCorrect = false; 
                    sMultipleChoiceAnswer.bIsSelected = false; 
                    ssTemp.str("");
                    ssTemp << "X=" << (iFigureX + modMisc::iRandomNumberNonZero(-1 * iRange, iRange)) << " Y=" << iFigureY;
                    sMultipleChoiceAnswer.sValue = ssTemp.str();
                    if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                        eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
                    };
                    
                    sMultipleChoiceAnswer.bIsCorrect = false; 
                    sMultipleChoiceAnswer.bIsSelected = false; 
                    ssTemp.str("");
                    ssTemp << "X=" << iFigureX << " Y=" << (iFigureY + modMisc::iRandomNumberNonZero(-1 * iRange, iRange));
                    sMultipleChoiceAnswer.sValue = ssTemp.str();
                    if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                        eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
                    };
                    
                    sMultipleChoiceAnswer.bIsCorrect = false; 
                    sMultipleChoiceAnswer.bIsSelected = false; 
                    ssTemp.str("");
                    ssTemp << "X=" << (iFigureX + modMisc::iRandomNumberNonZero(-1 * iRange, iRange)) << " Y=" << (iFigureY + modMisc::iRandomNumberNonZero(-1 * iRange, iRange));
                    sMultipleChoiceAnswer.sValue = ssTemp.str();
                    if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                        eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
                    };
                    
                    sMultipleChoiceAnswer.bIsCorrect = false; 
                    sMultipleChoiceAnswer.bIsSelected = false; 
                    ssTemp.str("");
                    ssTemp << "X=" << (iFigureX + modMisc::iRandomNumberNonZero(-1 * iRange, iRange)) << " Y=" << (iFigureY + modMisc::iRandomNumberNonZero(-1 * iRange, iRange));
                    sMultipleChoiceAnswer.sValue = ssTemp.str();
                    if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                        eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
                    };
                };
                
                tmTemp = time (NULL);
            } while (eTemp.vecChoices.size() < 7 and ((int)tmStart + iCutOffTime) < ((int)tmTemp));
            
			break;
		case 3:
		case 4:
            //AX+BY+CZ=D
            //EX+FY+GZ=H
            //IX+JY+KZ=L
            do {
                iFigureA = modMisc::iRandomNumberNonZero(-1, 1) * modMisc::iRandomNumber(iRangeMin, iRangeMax);
                iFigureB = modMisc::iRandomNumberNonZero(-1, 1) * modMisc::iRandomNumber(iRangeMin, iRangeMax);
                iFigureC = modMisc::iRandomNumberNonZero(-1, 1) * modMisc::iRandomNumber(iRangeMin, iRangeMax);
                iFigureE = modMisc::iRandomNumberNonZero(-1, 1) * modMisc::iRandomNumber(iRangeMin, iRangeMax);
                iFigureF = modMisc::iRandomNumberNonZero(-1, 1) * modMisc::iRandomNumber(iRangeMin, iRangeMax);
                iFigureG = modMisc::iRandomNumberNonZero(-1, 1) * modMisc::iRandomNumber(iRangeMin, iRangeMax);
                iFigureI = modMisc::iRandomNumberNonZero(-1, 1) * modMisc::iRandomNumber(iRangeMin, iRangeMax);
                iFigureJ = modMisc::iRandomNumberNonZero(-1, 1) * modMisc::iRandomNumber(iRangeMin, iRangeMax);
                iFigureK = modMisc::iRandomNumberNonZero(-1, 1) * modMisc::iRandomNumber(iRangeMin, iRangeMax);
                
                //check the lines are not paralell
            } while (modMisc::almostEquals((float)(iFigureA / iFigureB), (float)(iFigureE / iFigureF), 0.1) == true or modMisc::almostEquals((float)(iFigureA / iFigureB), (float)(iFigureI / iFigureJ), 0.1) == true or modMisc::almostEquals((float)(iFigureE / iFigureF), (float)(iFigureI / iFigureJ), 0.1) == true);
            
            iFigureX = modMisc::iRandomNumberNonZero(-1, 1) * modMisc::iRandomNumber(iRangeMin, iRangeMax);
            iFigureY = modMisc::iRandomNumberNonZero(-1, 1) * modMisc::iRandomNumber(iRangeMin, iRangeMax);
            iFigureZ = modMisc::iRandomNumberNonZero(-1, 1) * modMisc::iRandomNumber(iRangeMin, iRangeMax);
            
            iFigureD = (iFigureA * iFigureX) + (iFigureB * iFigureY) + (iFigureC * iFigureZ);
            iFigureH = (iFigureE * iFigureX) + (iFigureF * iFigureY) + (iFigureG * iFigureZ);
            iFigureL = (iFigureI * iFigureX) + (iFigureJ * iFigureY) + (iFigureK * iFigureZ);
            
            fRandomFigure.fValue = (float)iFigureX;
            eTemp.vecAnswers.push_back(fRandomFigure);
            fRandomFigure.fValue = (float)iFigureY;
            eTemp.vecAnswers.push_back(fRandomFigure);
            fRandomFigure.fValue = (float)iFigureZ;
            eTemp.vecAnswers.push_back(fRandomFigure);
            
            fRandomFigure.fValue = (float)iFigureA;
            eTemp.vecValues.push_back(fRandomFigure);
            fRandomFigure.fValue = (float)iFigureB;
            eTemp.vecValues.push_back(fRandomFigure);
            fRandomFigure.fValue = (float)iFigureC;
            eTemp.vecValues.push_back(fRandomFigure);
            fRandomFigure.fValue = (float)iFigureD;
            eTemp.vecValues.push_back(fRandomFigure);
            fRandomFigure.fValue = (float)iFigureE;
            eTemp.vecValues.push_back(fRandomFigure);
            fRandomFigure.fValue = (float)iFigureF;
            eTemp.vecValues.push_back(fRandomFigure);
            fRandomFigure.fValue = (float)iFigureG;
            eTemp.vecValues.push_back(fRandomFigure);
            fRandomFigure.fValue = (float)iFigureH;
            eTemp.vecValues.push_back(fRandomFigure);
            fRandomFigure.fValue = (float)iFigureI;
            eTemp.vecValues.push_back(fRandomFigure);
            fRandomFigure.fValue = (float)iFigureJ;
            eTemp.vecValues.push_back(fRandomFigure);
            fRandomFigure.fValue = (float)iFigureK;
            eTemp.vecValues.push_back(fRandomFigure);
            fRandomFigure.fValue = (float)iFigureL;
            eTemp.vecValues.push_back(fRandomFigure);
            
            ssEquation.clear();
            // "What values of X and Y solve these simultaneous linear equations?\n<0>X+<1>Y=<2>\n<3>X+<4>Y=<5>"
            ssEquation << "What values of X, Y and Z solve these simultaneous linear equations?\n";
            
            ////////////////////////////

            if (iFigureA == -1) {
                ssEquation << "-";
            } else if (iFigureA != 1) {
                ssEquation << "<0>";
            };
            
            ssEquation << "X + ";
            
            if (iFigureB == -1) {
                ssEquation << "-";
            } else if (iFigureB != 1) {
                ssEquation << "<1>";
            };
            
            ssEquation << "Y + ";
            
            if (iFigureC == -1) {
                ssEquation << "-";
            } else if (iFigureC != 1) {
                ssEquation << "<2>";
            };
            
            ssEquation << "Z = <3>\n";
            
            ////////////////////////////
            
            if (iFigureE == -1) {
                ssEquation << "-";
            } else if (iFigureE != 1) {
                ssEquation << "<4>";
            };
            
            ssEquation << "X + ";
            
            if (iFigureF == -1) {
                ssEquation << "-";
            } else if (iFigureF != 1) {
                ssEquation << "<5>";
            };
            
            ssEquation << "Y + ";
            
            if (iFigureG == -1) {
                ssEquation << "-";
            } else if (iFigureG != 1) {
                ssEquation << "<6>";
            };
            
            ssEquation << "Z = <7>\n";
            
            ////////////////////////////
            
            if (iFigureI == -1) {
                ssEquation << "-";
            } else if (iFigureI != 1) {
                ssEquation << "<8>";
            };
            
            ssEquation << "X + ";
            
            if (iFigureJ == -1) {
                ssEquation << "-";
            } else if (iFigureJ != 1) {
                ssEquation << "<9>";
            };
            
            ssEquation << "Y + ";
            
            if (iFigureK == -1) {
                ssEquation << "-";
            } else if (iFigureK != 1) {
                ssEquation << "<10>";
            };
            
            ssEquation << "Z = <11>\n";
            
            ////////////////////////////
            
            eTemp.sQuestionEncoded = ssEquation.str();
            
            //Correct answer
            sMultipleChoiceAnswer.bIsCorrect = true; 
            sMultipleChoiceAnswer.bIsSelected = false; 
            ssTemp.str("");
            ssTemp << "X=" << iFigureX << " Y=" << iFigureY << " Z=" << iFigureZ;
            sMultipleChoiceAnswer.sValue = ssTemp.str();
            
            eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
            
            //wrong answers
            do {
                for (int iRange = 1; iRange < 6; iRange++) {
                    switch (modMisc::iRandomNumberNonZero(1, 7)) {
                        case 1:
                            sMultipleChoiceAnswer.bIsCorrect = false; 
                            sMultipleChoiceAnswer.bIsSelected = false; 
                            ssTemp.str("");
                            ssTemp << "X=" << (iFigureX + modMisc::iRandomNumberNonZero(-1 * iRange, iRange)) << " Y=" << iFigureY << " Z=" << iFigureZ;
                            sMultipleChoiceAnswer.sValue = ssTemp.str();
                            if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                                eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
                            };
                            break;
                        case 2:
                            sMultipleChoiceAnswer.bIsCorrect = false; 
                            sMultipleChoiceAnswer.bIsSelected = false; 
                            ssTemp.str("");
                            ssTemp << "X=" << iFigureX << " Y=" << (iFigureY + modMisc::iRandomNumberNonZero(-1 * iRange, iRange)) << " Z=" << iFigureZ;
                            sMultipleChoiceAnswer.sValue = ssTemp.str();
                            if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                                eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
                            };
                            break;
                        case 3:
                            sMultipleChoiceAnswer.bIsCorrect = false; 
                            sMultipleChoiceAnswer.bIsSelected = false; 
                            ssTemp.str("");
                            ssTemp << "X=" << iFigureX << " Y=" << iFigureY << " Z=" << (iFigureZ + modMisc::iRandomNumberNonZero(-1 * iRange, iRange));
                            sMultipleChoiceAnswer.sValue = ssTemp.str();
                            if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                                eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
                            };
                            break;
                        case 4:
                            sMultipleChoiceAnswer.bIsCorrect = false; 
                            sMultipleChoiceAnswer.bIsSelected = false; 
                            ssTemp.str("");
                            ssTemp << "X=" << (iFigureX + modMisc::iRandomNumberNonZero(-1 * iRange, iRange)) << " Y=" << (iFigureY + modMisc::iRandomNumberNonZero(-1 * iRange, iRange)) << " Z=" << iFigureZ;
                            sMultipleChoiceAnswer.sValue = ssTemp.str();
                            if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                                eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
                            };
                            break;
                        case 5:
                            sMultipleChoiceAnswer.bIsCorrect = false; 
                            sMultipleChoiceAnswer.bIsSelected = false; 
                            ssTemp.str("");
                            ssTemp << "X=" << (iFigureX + modMisc::iRandomNumberNonZero(-1 * iRange, iRange)) << " Y=" << iFigureY << " Z=" << (iFigureZ + modMisc::iRandomNumberNonZero(-1 * iRange, iRange));
                            sMultipleChoiceAnswer.sValue = ssTemp.str();
                            if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                                eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
                            };
                            break;
                        case 6:
                            sMultipleChoiceAnswer.bIsCorrect = false; 
                            sMultipleChoiceAnswer.bIsSelected = false; 
                            ssTemp.str("");
                            ssTemp << "X=" << iFigureX << " Y=" << (iFigureY + modMisc::iRandomNumberNonZero(-1 * iRange, iRange)) << " Z=" << (iFigureZ + modMisc::iRandomNumberNonZero(-1 * iRange, iRange));
                            sMultipleChoiceAnswer.sValue = ssTemp.str();
                            if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                                eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
                            };
                            break;
                        case 7:
                            sMultipleChoiceAnswer.bIsCorrect = false; 
                            sMultipleChoiceAnswer.bIsSelected = false; 
                            ssTemp.str("");
                            ssTemp << "X=" << (iFigureX + modMisc::iRandomNumberNonZero(-1 * iRange, iRange)) << " Y=" << (iFigureY + modMisc::iRandomNumberNonZero(-1 * iRange, iRange)) << " Z=" << (iFigureZ + modMisc::iRandomNumberNonZero(-1 * iRange, iRange));
                            sMultipleChoiceAnswer.sValue = ssTemp.str();
                            if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                                eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
                            };
                        default:
                            break;
                    };
                };
                
                tmTemp = time (NULL);
            } while (eTemp.vecChoices.size() < 7 and ((int)tmStart + iCutOffTime) < ((int)tmTemp));
            
			break;
		case 5:
            //AW+BX+CY+DZ=E
            //FW+GX+HY+IZ=J
            //KW+LX+MY+NZ=O
            //PW+QX+RY+SZ=T
            do {
                iFigureA = modMisc::iRandomNumberNonZero(-1, 1) * modMisc::iRandomNumber(iRangeMin, iRangeMax);
                iFigureB = modMisc::iRandomNumberNonZero(-1, 1) * modMisc::iRandomNumber(iRangeMin, iRangeMax);
                iFigureC = modMisc::iRandomNumberNonZero(-1, 1) * modMisc::iRandomNumber(iRangeMin, iRangeMax);
                iFigureE = modMisc::iRandomNumberNonZero(-1, 1) * modMisc::iRandomNumber(iRangeMin, iRangeMax);
                iFigureF = modMisc::iRandomNumberNonZero(-1, 1) * modMisc::iRandomNumber(iRangeMin, iRangeMax);
                iFigureG = modMisc::iRandomNumberNonZero(-1, 1) * modMisc::iRandomNumber(iRangeMin, iRangeMax);
                iFigureI = modMisc::iRandomNumberNonZero(-1, 1) * modMisc::iRandomNumber(iRangeMin, iRangeMax);
                iFigureJ = modMisc::iRandomNumberNonZero(-1, 1) * modMisc::iRandomNumber(iRangeMin, iRangeMax);
                iFigureK = modMisc::iRandomNumberNonZero(-1, 1) * modMisc::iRandomNumber(iRangeMin, iRangeMax);
                iFigureL = modMisc::iRandomNumberNonZero(-1, 1) * modMisc::iRandomNumber(iRangeMin, iRangeMax);
                iFigureM = modMisc::iRandomNumberNonZero(-1, 1) * modMisc::iRandomNumber(iRangeMin, iRangeMax);
                iFigureN = modMisc::iRandomNumberNonZero(-1, 1) * modMisc::iRandomNumber(iRangeMin, iRangeMax);
                iFigureO = modMisc::iRandomNumberNonZero(-1, 1) * modMisc::iRandomNumber(iRangeMin, iRangeMax);
                iFigureP = modMisc::iRandomNumberNonZero(-1, 1) * modMisc::iRandomNumber(iRangeMin, iRangeMax);
                iFigureQ = modMisc::iRandomNumberNonZero(-1, 1) * modMisc::iRandomNumber(iRangeMin, iRangeMax);
                iFigureR = modMisc::iRandomNumberNonZero(-1, 1) * modMisc::iRandomNumber(iRangeMin, iRangeMax);
                iFigureS = modMisc::iRandomNumberNonZero(-1, 1) * modMisc::iRandomNumber(iRangeMin, iRangeMax);
                iFigureT = modMisc::iRandomNumberNonZero(-1, 1) * modMisc::iRandomNumber(iRangeMin, iRangeMax);
                
                //check the lines are not paralell
            } while (modMisc::almostEquals((float)(iFigureA / iFigureB), (float)(iFigureD / iFigureE), 0.1) == true);
            
            iFigureW = modMisc::iRandomNumberNonZero(-1, 1) * modMisc::iRandomNumber(iRangeMin, iRangeMax);
            iFigureX = modMisc::iRandomNumberNonZero(-1, 1) * modMisc::iRandomNumber(iRangeMin, iRangeMax);
            iFigureY = modMisc::iRandomNumberNonZero(-1, 1) * modMisc::iRandomNumber(iRangeMin, iRangeMax);
            iFigureZ = modMisc::iRandomNumberNonZero(-1, 1) * modMisc::iRandomNumber(iRangeMin, iRangeMax);
            
            iFigureE = (iFigureA * iFigureW) + (iFigureB * iFigureX) + (iFigureC * iFigureY) + (iFigureD * iFigureZ);
            iFigureJ = (iFigureF * iFigureW) + (iFigureG * iFigureX) + (iFigureH * iFigureY) + (iFigureI * iFigureZ);
            iFigureO = (iFigureK * iFigureW) + (iFigureL * iFigureX) + (iFigureM * iFigureY) + (iFigureN * iFigureZ);
            iFigureT = (iFigureP * iFigureW) + (iFigureQ * iFigureX) + (iFigureR * iFigureY) + (iFigureS * iFigureZ);
            
            fRandomFigure.fValue = (float)iFigureW;
            eTemp.vecAnswers.push_back(fRandomFigure);
            fRandomFigure.fValue = (float)iFigureX;
            eTemp.vecAnswers.push_back(fRandomFigure);
            fRandomFigure.fValue = (float)iFigureY;
            eTemp.vecAnswers.push_back(fRandomFigure);
            fRandomFigure.fValue = (float)iFigureZ;
            eTemp.vecAnswers.push_back(fRandomFigure);
            
            fRandomFigure.fValue = (float)iFigureA;
            eTemp.vecValues.push_back(fRandomFigure);
            fRandomFigure.fValue = (float)iFigureB;
            eTemp.vecValues.push_back(fRandomFigure);
            fRandomFigure.fValue = (float)iFigureC;
            eTemp.vecValues.push_back(fRandomFigure);
            fRandomFigure.fValue = (float)iFigureD;
            eTemp.vecValues.push_back(fRandomFigure);
            fRandomFigure.fValue = (float)iFigureE;
            eTemp.vecValues.push_back(fRandomFigure);

            fRandomFigure.fValue = (float)iFigureF;
            eTemp.vecValues.push_back(fRandomFigure);
            fRandomFigure.fValue = (float)iFigureG;
            eTemp.vecValues.push_back(fRandomFigure);
            fRandomFigure.fValue = (float)iFigureH;
            eTemp.vecValues.push_back(fRandomFigure);
            fRandomFigure.fValue = (float)iFigureI;
            eTemp.vecValues.push_back(fRandomFigure);
            fRandomFigure.fValue = (float)iFigureJ;
            eTemp.vecValues.push_back(fRandomFigure);

            fRandomFigure.fValue = (float)iFigureK;
            eTemp.vecValues.push_back(fRandomFigure);
            fRandomFigure.fValue = (float)iFigureL;
            eTemp.vecValues.push_back(fRandomFigure);
            fRandomFigure.fValue = (float)iFigureM;
            eTemp.vecValues.push_back(fRandomFigure);
            fRandomFigure.fValue = (float)iFigureN;
            eTemp.vecValues.push_back(fRandomFigure);
            fRandomFigure.fValue = (float)iFigureO;
            eTemp.vecValues.push_back(fRandomFigure);

            fRandomFigure.fValue = (float)iFigureP;
            eTemp.vecValues.push_back(fRandomFigure);
            fRandomFigure.fValue = (float)iFigureQ;
            eTemp.vecValues.push_back(fRandomFigure);
            fRandomFigure.fValue = (float)iFigureR;
            eTemp.vecValues.push_back(fRandomFigure);
            fRandomFigure.fValue = (float)iFigureS;
            eTemp.vecValues.push_back(fRandomFigure);
            fRandomFigure.fValue = (float)iFigureT;
            eTemp.vecValues.push_back(fRandomFigure);
            
            ssEquation.clear();
            // "What values of X and Y solve these simultaneous linear equations?\n<0>X+<1>Y=<2>\n<3>X+<4>Y=<5>"
            ssEquation << "What values of W, X, Y and Z solve these simultaneous linear equations?\n";
            
            ////////////////////////////
            //AW+BX+CY+DZ=E
            
            if (iFigureA == -1) {
                ssEquation << "-";
            } else if (iFigureA != 1) {
                ssEquation << "<0>";
            };
            
            ssEquation << "W + ";

            if (iFigureB == -1) {
                ssEquation << "-";
            } else if (iFigureB != 1) {
                ssEquation << "<1>";
            };
            
            ssEquation << "X + ";
            
            if (iFigureC == -1) {
                ssEquation << "-";
            } else if (iFigureC != 1) {
                ssEquation << "<2>";
            };
            
            ssEquation << "Y + ";
            
            if (iFigureD == -1) {
                ssEquation << "-";
            } else if (iFigureD != 1) {
                ssEquation << "<3>";
            };
            
            ssEquation << "Z = <4>\n";
            
            ////////////////////////////
            //FW+GX+HY+IZ=J

            if (iFigureF == -1) {
                ssEquation << "-";
            } else if (iFigureF != 1) {
                ssEquation << "<5>";
            };
            
            ssEquation << "W + ";
            
            if (iFigureG == -1) {
                ssEquation << "-";
            } else if (iFigureG != 1) {
                ssEquation << "<6>";
            };
            
            ssEquation << "X + ";

            if (iFigureH == -1) {
                ssEquation << "-";
            } else if (iFigureH != 1) {
                ssEquation << "<7>";
            };
            
            ssEquation << "Y + ";
            
            if (iFigureI == -1) {
                ssEquation << "-";
            } else if (iFigureI != 1) {
                ssEquation << "<8>";
            };
            
            ssEquation << "Z = <9>\n";
            
            ////////////////////////////
            //KW+LX+MY+NZ=O
            
            if (iFigureK == -1) {
                ssEquation << "-";
            } else if (iFigureK != 1) {
                ssEquation << "<10>";
            };
            
            ssEquation << "W + ";

            if (iFigureL == -1) {
                ssEquation << "-";
            } else if (iFigureL != 1) {
                ssEquation << "<11>";
            };
            
            ssEquation << "X + ";
            
            if (iFigureM == -1) {
                ssEquation << "-";
            } else if (iFigureM != 1) {
                ssEquation << "<12>";
            };
            
            ssEquation << "Y + ";
            
            if (iFigureN == -1) {
                ssEquation << "-";
            } else if (iFigureN != 1) {
                ssEquation << "<13>";
            };
            
            ssEquation << "Z = <14>\n";
            
            ////////////////////////////
            //PW+QX+RY+SZ=T
            
            if (iFigureP == -1) {
                ssEquation << "-";
            } else if (iFigureP != 1) {
                ssEquation << "<15>";
            };
            
            ssEquation << "W + ";
            
            if (iFigureQ == -1) {
                ssEquation << "-";
            } else if (iFigureQ != 1) {
                ssEquation << "<16>";
            };
            
            ssEquation << "X + ";
            
            if (iFigureR == -1) {
                ssEquation << "-";
            } else if (iFigureR != 1) {
                ssEquation << "<17>";
            };
            
            ssEquation << "Y + ";
            
            if (iFigureS == -1) {
                ssEquation << "-";
            } else if (iFigureS != 1) {
                ssEquation << "<18>";
            };
            
            ssEquation << "Z = <19>\n";
            
            eTemp.sQuestionEncoded = ssEquation.str();
            
            //Correct answer
            sMultipleChoiceAnswer.bIsCorrect = true; 
            sMultipleChoiceAnswer.bIsSelected = false; 
            ssTemp.str("");
            ssTemp << "W=" << iFigureW << " X=" << iFigureX << " Y=" << iFigureY << " Z=" << iFigureZ;
            sMultipleChoiceAnswer.sValue = ssTemp.str();
            
            eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
            
            //wrong answers
            do {
                for (int iRange = 1; iRange < 4; iRange++) {
                    sMultipleChoiceAnswer.bIsCorrect = false; 
                    sMultipleChoiceAnswer.bIsSelected = false; 
                    ssTemp.str("");
                    ssTemp << "W=" << eTemp.vecValues[modMisc::iRandomNumber(0, eTemp.vecValues.size() - 1)].fValue << " X=" << eTemp.vecValues[modMisc::iRandomNumber(0, eTemp.vecValues.size() - 1)].fValue << " Y=" << eTemp.vecValues[modMisc::iRandomNumber(0, eTemp.vecValues.size() - 1)].fValue << " Z=" << eTemp.vecValues[modMisc::iRandomNumber(0, eTemp.vecValues.size() - 1)].fValue;
                    sMultipleChoiceAnswer.sValue = ssTemp.str();
                    if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                        eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
                    };
                };
                
                //Swapping Values around
                switch (modMisc::iRandomNumber(1, 6)) {
                    case 1:
                        sMultipleChoiceAnswer.bIsCorrect = false; 
                        sMultipleChoiceAnswer.bIsSelected = false; 
                        ssTemp.str("");
                        ssTemp << "W=" << iFigureW << " X=" << iFigureX << " Y=" << iFigureZ << " Z=" << iFigureY;
                        sMultipleChoiceAnswer.sValue = ssTemp.str();
                        if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                            eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
                        };
                        break;
                    case 2:
                        sMultipleChoiceAnswer.bIsCorrect = false; 
                        sMultipleChoiceAnswer.bIsSelected = false; 
                        ssTemp.str("");
                        ssTemp << "W=" << iFigureW << " X=" << iFigureZ << " Y=" << iFigureX << " Z=" << iFigureY;
                        sMultipleChoiceAnswer.sValue = ssTemp.str();
                        if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                            eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
                        };
                        break;
                    case 3:
                        sMultipleChoiceAnswer.bIsCorrect = false; 
                        sMultipleChoiceAnswer.bIsSelected = false; 
                        ssTemp.str("");
                        ssTemp << "W=" << iFigureZ << " X=" << iFigureW << " Y=" << iFigureX << " Z=" << iFigureY;
                        sMultipleChoiceAnswer.sValue = ssTemp.str();
                        if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                            eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
                        };
                        break;
                    case 4:
                        sMultipleChoiceAnswer.bIsCorrect = false; 
                        sMultipleChoiceAnswer.bIsSelected = false; 
                        ssTemp.str("");
                        ssTemp << "W=" << iFigureW << " X=" << iFigureY << " Y=" << iFigureX << " Z=" << iFigureZ;
                        sMultipleChoiceAnswer.sValue = ssTemp.str();
                        if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                            eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
                        };
                        break;
                    case 5:
                        sMultipleChoiceAnswer.bIsCorrect = false; 
                        sMultipleChoiceAnswer.bIsSelected = false; 
                        ssTemp.str("");
                        ssTemp << "W=" << iFigureX << " X=" << iFigureW << " Y=" << iFigureY << " Z=" << iFigureZ;
                        sMultipleChoiceAnswer.sValue = ssTemp.str();
                        if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                            eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
                        };
                        break;
                    case 6:
                        sMultipleChoiceAnswer.bIsCorrect = false; 
                        sMultipleChoiceAnswer.bIsSelected = false; 
                        ssTemp.str("");
                        ssTemp << "W=" << iFigureX << " X=" << iFigureY << " Y=" << iFigureW << " Z=" << iFigureZ;
                        sMultipleChoiceAnswer.sValue = ssTemp.str();
                        if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                            eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
                        };
                    default:
                        break;
                };
                
                for (int iRange = 1; iRange < 6; iRange++) {
                    switch (modMisc::iRandomNumber(1, 10)) {
                        case 1:
                            //////////////
                            //1 value changing
                            sMultipleChoiceAnswer.bIsCorrect = false; 
                            sMultipleChoiceAnswer.bIsSelected = false; 
                            ssTemp.str("");
                            ssTemp << "W=" << (iFigureW + modMisc::iRandomNumberNonZero(-1 * iRange, iRange)) << " X=" << iFigureX << " Y=" << iFigureY << " Z=" << iFigureZ;
                            sMultipleChoiceAnswer.sValue = ssTemp.str();
                            if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                                eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
                            };
                            break;
                        case 2:
                            sMultipleChoiceAnswer.bIsCorrect = false; 
                            sMultipleChoiceAnswer.bIsSelected = false; 
                            ssTemp.str("");
                            ssTemp << "W=" << iFigureX  << " X=" << (iFigureX + modMisc::iRandomNumberNonZero(-1 * iRange, iRange)) << " Y=" << iFigureY << " Z=" << iFigureZ;
                            sMultipleChoiceAnswer.sValue = ssTemp.str();
                            if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                                eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
                            };
                            break;
                        case 3:
                            sMultipleChoiceAnswer.bIsCorrect = false; 
                            sMultipleChoiceAnswer.bIsSelected = false; 
                            ssTemp.str("");
                            ssTemp << "W=" << iFigureW << " X=" << iFigureX << " Y=" << (iFigureY + modMisc::iRandomNumberNonZero(-1 * iRange, iRange)) << " Z=" << iFigureZ;
                            sMultipleChoiceAnswer.sValue = ssTemp.str();
                            if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                                eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
                            };
                            break;
                        case 4:
                            sMultipleChoiceAnswer.bIsCorrect = false; 
                            sMultipleChoiceAnswer.bIsSelected = false; 
                            ssTemp.str("");
                            ssTemp << "W=" << iFigureW << " X=" << iFigureX << " Y=" << iFigureY << " Z=" << (iFigureZ + modMisc::iRandomNumberNonZero(-1 * iRange, iRange));
                            sMultipleChoiceAnswer.sValue = ssTemp.str();
                            if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                                eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
                            };
                            break;
                        case 5:
                            //////////////
                            //2 values changing
                            
                            sMultipleChoiceAnswer.bIsCorrect = false; 
                            sMultipleChoiceAnswer.bIsSelected = false; 
                            ssTemp.str("");
                            ssTemp << "W=" << iFigureW << " X=" << (iFigureX + modMisc::iRandomNumberNonZero(-1 * iRange, iRange)) << " Y=" << (iFigureY + modMisc::iRandomNumberNonZero(-1 * iRange, iRange)) << " Z=" << iFigureZ;
                            sMultipleChoiceAnswer.sValue = ssTemp.str();
                            if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                                eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
                            };
                            break;
                        case 6:
                            sMultipleChoiceAnswer.bIsCorrect = false; 
                            sMultipleChoiceAnswer.bIsSelected = false; 
                            ssTemp.str("");
                            ssTemp << "W=" << iFigureW << " X=" << (iFigureX + modMisc::iRandomNumberNonZero(-1 * iRange, iRange)) << " Y=" << iFigureY << " Z=" << (iFigureZ + modMisc::iRandomNumberNonZero(-1 * iRange, iRange));
                            sMultipleChoiceAnswer.sValue = ssTemp.str();
                            if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                                eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
                            };
                            break;
                        case 7:
                            sMultipleChoiceAnswer.bIsCorrect = false; 
                            sMultipleChoiceAnswer.bIsSelected = false; 
                            ssTemp.str("");
                            ssTemp << "W=" << iFigureW << " X=" << iFigureX << " Y=" << (iFigureY + modMisc::iRandomNumberNonZero(-1 * iRange, iRange)) << " Z=" << (iFigureZ + modMisc::iRandomNumberNonZero(-1 * iRange, iRange));
                            sMultipleChoiceAnswer.sValue = ssTemp.str();
                            if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                                eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
                            };
                            break;
                        case 8:
                            sMultipleChoiceAnswer.bIsCorrect = false; 
                            sMultipleChoiceAnswer.bIsSelected = false; 
                            ssTemp.str("");
                            ssTemp << "W=" << (iFigureW + modMisc::iRandomNumberNonZero(-1 * iRange, iRange)) << " X=" << (iFigureX + modMisc::iRandomNumberNonZero(-1 * iRange, iRange)) << " Y=" << iFigureY << " Z=" << iFigureZ;
                            sMultipleChoiceAnswer.sValue = ssTemp.str();
                            if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                                eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
                            };
                            break;
                        case 9:
                            sMultipleChoiceAnswer.bIsCorrect = false; 
                            sMultipleChoiceAnswer.bIsSelected = false; 
                            ssTemp.str("");
                            ssTemp << "W=" << (iFigureW + modMisc::iRandomNumberNonZero(-1 * iRange, iRange)) << " X=" << iFigureX << " Y=" << (iFigureY + modMisc::iRandomNumberNonZero(-1 * iRange, iRange)) << " Z=" << iFigureZ;
                            sMultipleChoiceAnswer.sValue = ssTemp.str();
                            if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                                eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
                            };
                            break;
                        case 10:
                            sMultipleChoiceAnswer.bIsCorrect = false; 
                            sMultipleChoiceAnswer.bIsSelected = false; 
                            ssTemp.str("");
                            ssTemp << "W=" << (iFigureW + modMisc::iRandomNumberNonZero(-1 * iRange, iRange)) << " X=" << iFigureX << " Y=" << iFigureY << " Z=" << (iFigureZ + modMisc::iRandomNumberNonZero(-1 * iRange, iRange));
                            sMultipleChoiceAnswer.sValue = ssTemp.str();
                            if (modMisc::MultipleChoiceAnswerFound(eTemp.vecChoices, sMultipleChoiceAnswer.sValue) == false) {
                                eTemp.vecChoices.push_back(sMultipleChoiceAnswer);
                            };
                        default:
                            break;
                    };
                };
                
                tmTemp = time (NULL);
            } while (eTemp.vecChoices.size() < 7 and ((int)tmStart + iCutOffTime) < ((int)tmTemp));
            
			break;
        default:
            break;
    };
    
    
    
    
    
    
    
    
    
    
	
	
	
    //	strFloatValue fRandomFigure;
    //	strQuestion eTemp;
    //	strStringAnswer sMultipleChoiceAnswer; 
    //	vector<strStringAnswer>::iterator itMultipleChoiceAnswer; 
    //	std::stringstream ssTemp;
    //	int iDifficultyGroupLevel;
	sGroupName.clear();
    //	std::stringstream ssEquation;
    //	time_t tmStart;
    //	time_t tmTemp;
    //	int iFigureA;
    //	int iFigureB;
    //	int iFigureC;
    //	int iRangeMin;
    //	int iRangeMax;
	
	return eTemp;
};


bool ClsQuiz::QuestionsSame(strQuestion eQuestionA, strQuestion eQuestionB) {
	if (eQuestionA.sQuestionEncoded == eQuestionB.sQuestionEncoded) {
		std::vector<strFloatValue>::iterator itValueA;
		std::vector<strFloatValue>::iterator itValueB;
		bool bIsFoundValue;
		bool bIsFoundAllValue;
		
		bIsFoundAllValue = true;
		
		for(itValueA=eQuestionA.vecValues.begin(); itValueA < eQuestionA.vecValues.end(); itValueA++){
			bIsFoundValue = false;
			for(itValueB=eQuestionB.vecValues.begin(); itValueB < eQuestionB.vecValues.end(); itValueB++){
				if (itValueA->fValue == itValueB->fValue) {
					bIsFoundValue = true;
				};	
			};

			if (bIsFoundValue == false) {
				bIsFoundAllValue = false;
			};
		};

		if (bIsFoundAllValue == true) {
			return true;
		} else {
			return false;
		};
	} else {
		return false;
	};
};





void ClsQuiz::generateNewQuizFilePath(){

};

void ClsQuiz::createEmptyQuizTables(){
	char *zErr;
	int rc;

	try {
		std::string sErrMsg;
		bool bIsOk;
		const char *cSqlCreateTbl_Header("create table tblHeader (fldId integer primary key, fldName varchar(255), fldValue varchar(255), fldValue2 varchar(255));");
		const char *cSqlCreateTbl_Question("create table tblQuestions (fldId integer primary key, fldQuestionNo int, fldQuestionType varchar(256), fldQuestionGroup varchar(256), fldQuestionEncoded varchar(1024), fldDecodeText varchar(1024), fldQuestionText varchar(1024), fldGivenAnswerText varchar(1024), fldGivenAnswerMultipleChoice int, fldDifficulty int, fldCorrect int, fldAnswerType varchar(256), fldIsMultipleChoice int, fldHasOnlyOneAnswer int, fldIsDirty int, fldPic varchar(1024), fldHasPic int);");
		const char *cSqlCreateTbl_QuestionValues("create table tblQuestionsValues (fldId integer primary key, fldAppId integer, fldQuestionID int, fldValues float, fldIsDirty integer);");
		const char *cSqlCreateTbl_QuestionAnswersWanted("create table tblQuestionsAnswersWanted (fldId integer primary key, fldQuestionID int, fldAnswer float);");
		const char *cSqlCreateTbl_QuestionAnswersGiven("create table tblQuestionsAnswersGiven (fldId integer primary key, fldQuestionID int, fldAnswer varchar(1024));");
		const char *cSqlCreateTbl_QuestionTrigFn("create table tblQuestionsTrigFn (fldId integer primary key, fldQuestionID int, fldTrigFn varchar(64));");
		const char *cSqlCreateTbl_QuestionChoices("create table tblQuestionsChoices (fldId integer primary key, fldAppId integer, fldQuestionID int, fldValue varchar(1024), fldIsSelected integer, fldIsCorrect integer);");

		char *cPath;

		bIsOk = true;
		rc = SQLITE_OK;
		
		if (bDbIsOpen != true){
			cPath = (char*)this->sFilePath.c_str();
		
			//open database connection
			rc = sqlite3_open(cPath, &db);
		};
		
		if(rc != SQLITE_OK){
#if TEST_ENV == true
			sErrMsg = (std::string)"Can't open DB: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
			fprintf(stderr, sErrMsg.c_str(), 0);
#endif
			bIsOk = false;
		};

		//create header table to store the level of difficulty
		if(bIsOk) {
			rc = sqlite3_exec(db, cSqlCreateTbl_Header, NULL, NULL, &zErr);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't open DB: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//create difficulty table to store the level of difficulty
		if(bIsOk) {
			rc = sqlite3_exec(db, cSqlCreateTbl_Question, NULL, NULL, &zErr);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't open DB: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//create enabled table to store which subjects are enabled
		if(bIsOk) {
			rc = sqlite3_exec(db, cSqlCreateTbl_QuestionValues, NULL, NULL, &zErr);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't open DB: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//create enabled table to store which subjects are enabled
		if(bIsOk) {
			rc = sqlite3_exec(db, cSqlCreateTbl_QuestionAnswersWanted, NULL, NULL, &zErr);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't open DB: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//create enabled table to store which subjects are enabled
		if(bIsOk) {
			rc = sqlite3_exec(db, cSqlCreateTbl_QuestionAnswersGiven, NULL, NULL, &zErr);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't open DB: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//create enabled table to store which subjects are enabled
		if(bIsOk) {
			rc = sqlite3_exec(db, cSqlCreateTbl_QuestionTrigFn, NULL, NULL, &zErr);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't open DB: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//create enabled table to store choices for the multiple choice questions
		if(bIsOk) {
			rc = sqlite3_exec(db, cSqlCreateTbl_QuestionChoices, NULL, NULL, &zErr);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't open DB: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//	this->load_subjects();
	

		sErrMsg.clear();
//		bool bIsOk;
		cSqlCreateTbl_Header = NULL;
		cSqlCreateTbl_Question = NULL;
		cSqlCreateTbl_QuestionValues = NULL;
		cSqlCreateTbl_QuestionAnswersWanted = NULL;
		cSqlCreateTbl_QuestionAnswersGiven = NULL;
		cSqlCreateTbl_QuestionTrigFn = NULL;
		cSqlCreateTbl_QuestionChoices = NULL;
		cPath = NULL;
		
	} catch(...) {
		rc = sqlite3_close(db);
		bDbIsOpen = false;
	};

	zErr = NULL;
//	int rc;
};

void ClsQuiz::updateQuestionValue(int iQuestionNo, int iValueNo, float fValueValue) {
	char *zErr;
	int rc;
	
	try {
		std::string sErrMsg;
		bool bIsOk;
		const char *cSqlUpdateTbl_QuestionValues("update tblQuestionsValues set fldQuestionID = :iQuestionNo, fldValues = :fValue where fldId = :iId ;");
		sqlite3_stmt *stmt_QuestionValues;
		int iParameterPosition;
		char *cPath;
		std::string sTemp;
		float fTemp;
		int iTemp;
		
		bIsOk = true;
		rc = SQLITE_OK;
		
		if (bDbIsOpen != true){
			cPath = (char*)this->sFilePath.c_str();
			
			//open database connection
			rc = sqlite3_open(cPath, &db);
		};
		
		if(rc != SQLITE_OK){
#if TEST_ENV == true
			sErrMsg = (std::string)"Can't open DB: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
			fprintf(stderr, sErrMsg.c_str(), 0);
#endif
			bIsOk = false;
		};
		
		//Populate Difficulty table with default values	
		if(bIsOk) {
			rc = sqlite3_prepare_v2(db, cSqlUpdateTbl_QuestionValues, -1, &stmt_QuestionValues, NULL);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't Prepare SQL: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//iQuestionNo
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt_QuestionValues, ":iQuestionNo");
			
			iTemp = iQuestionNo;
			
			rc = sqlite3_bind_int(stmt_QuestionValues, 
								  iParameterPosition,
								  iTemp);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//fValue
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt_QuestionValues, ":fValue");
			
			fTemp = vecQuestions[iQuestionNo].vecValues[iValueNo].fValue;
			
			rc = sqlite3_bind_double(stmt_QuestionValues, 
								     iParameterPosition,
								     fTemp);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//iId
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt_QuestionValues, ":iId");
			
			iTemp = vecQuestions[iQuestionNo].vecValues[iValueNo].iId;
			
			rc = sqlite3_bind_int(stmt_QuestionValues, 
								  iParameterPosition,
								  iTemp);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//step (i.e. run the command)
		if(bIsOk){
			rc = sqlite3_step(stmt_QuestionValues);
			
			if(rc == SQLITE_ERROR){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't Step statment: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};//if bIsOk
		
		if(bIsOk){
			rc = sqlite3_reset(stmt_QuestionValues);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				fprintf(stderr, "Can't reset statment: %\n", sqlite3_errmsg(db));
#endif
				bIsOk = false;
			};
		};//if bIsOk
		
		
		
		sErrMsg.clear();
//		bool bIsOk;
		cSqlUpdateTbl_QuestionValues = NULL;
		if (bIsOk) {
			sqlite3_finalize(stmt_QuestionValues);
		};
//		int iParameterPosition;
		cPath = NULL;
		sTemp.clear();
//		float fTemp;
//		int iTemp;
//		int iQuestionId;
		
		
		
	} catch (...) {
		//Close database connection	
		rc = sqlite3_close(db);
		bDbIsOpen = false;
	};


	zErr = NULL;
//	int rc;
};


void ClsQuiz::updateQuestionChoices(int iQuestionNo, int iChoiceNo) {
	char *zErr;
	int rc;
	
	try {
		std::string sErrMsg;
		bool bIsOk;
		const char *cSqlUpdateTbl_QuestionChoices("update tblQuestionsChoices set fldValue = :sValue, fldIsSelected = :iIsSelected, fldIsCorrect = :iIsCorrect where fldAppId = :iChoiceNo and fldQuestionID = :iQuestionNo ;"); 
		sqlite3_stmt *stmt;
		int iParameterPosition;
		char *cPath;
		std::string sTemp;
		int iTemp;
		
		bIsOk = true;
		rc = SQLITE_OK;
		
		if (bDbIsOpen != true){
			cPath = (char*)this->sFilePath.c_str();
			
			//open database connection
			rc = sqlite3_open(cPath, &db);
		};
		
		if(rc != SQLITE_OK){
#if TEST_ENV == true
			sErrMsg = (std::string)"Can't open DB: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
			fprintf(stderr, sErrMsg.c_str(), 0);
#endif
			bIsOk = false;
		};
		
		//Populate Difficulty table with default values	
		if(bIsOk) {
			rc = sqlite3_prepare_v2(db, cSqlUpdateTbl_QuestionChoices, -1, &stmt, NULL);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't Prepare SQL: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//iQuestionNo
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt, ":iQuestionNo");
			
			iTemp = iQuestionNo;
			
			rc = sqlite3_bind_int(stmt, 
								  iParameterPosition,
								  iTemp);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//iChoiceNo
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt, ":iChoiceNo");
			
			iTemp = iChoiceNo;
			
			rc = sqlite3_bind_int(stmt, 
								  iParameterPosition,
								  iTemp);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//:sValue
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt, ":sValue");
			
			sTemp = vecQuestions[iQuestionNo].vecChoices[iChoiceNo].sValue;
			
			rc = sqlite3_bind_text(stmt, 
								   iParameterPosition,
								   sTemp.c_str(), 
								   -1, 
								   SQLITE_TRANSIENT);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add double parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//iIsSelected
		if (bIsOk) {
			iParameterPosition = sqlite3_bind_parameter_index(stmt, ":iIsSelected");
			
			if (vecQuestions[iQuestionNo].vecChoices[iChoiceNo].bIsSelected == true) {
				iTemp = 1;
			} else {
				iTemp = 0;
			};
			
			rc = sqlite3_bind_int(stmt, 
								  iParameterPosition, 
								  iTemp);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//iIsCorrect
		if (bIsOk) {
			iParameterPosition = sqlite3_bind_parameter_index(stmt, ":iIsCorrect");
			
			if (vecQuestions[iQuestionNo].vecChoices[iChoiceNo].bIsCorrect == true) {
				iTemp = 1;
			} else {
				iTemp = 0;
			};
			
			rc = sqlite3_bind_int(stmt, 
								  iParameterPosition, 
								  iTemp);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//step (i.e. run the command)
		if(bIsOk){
			rc = sqlite3_step(stmt);
			
			if(rc == SQLITE_ERROR){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't Step statment: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};//if bIsOk
		
		if(bIsOk){
			rc = sqlite3_reset(stmt);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				fprintf(stderr, "Can't reset statment: %\n", sqlite3_errmsg(db));
#endif
				bIsOk = false;
			};
		};//if bIsOk
		
		
		
		
		sErrMsg.clear();
//		bool bIsOk;
		cSqlUpdateTbl_QuestionChoices = NULL; 
		if (bIsOk) {
			sqlite3_finalize(stmt);
		};
//		int iParameterPosition;
		cPath = NULL;
		sTemp.clear();
//		float fTemp;
//		int iTemp;
//		int iQuestionId;
		
		
	} catch (...) {
		//Close database connection	
		rc = sqlite3_close(db);
		bDbIsOpen = false;
	};

	zErr = NULL;
//	int rc;
};

void ClsQuiz::updateQuestionAnswer(int iQuestionNo, int iAnswerNo, float fAnswerValue) {

};

void ClsQuiz::saveQuestion(){

};

#pragma mark -
#pragma mark header

std::string ClsQuiz::headerValue(std::string sHeaderName){
	vector<strHeader>::iterator itHeader;
	bool bIsFound;
	std::string sReturn;

	bIsFound=false;
	
	for(itHeader=this->vecHeader.begin(); itHeader < this->vecHeader.end(); itHeader++){
		if (itHeader->sName.compare(sHeaderName) == 0) {
			bIsFound = true;
			sReturn = itHeader->sValue;
		};
	};
	
	if (bIsFound == true) {
		return sReturn;
	} else {
		return "";
	};
};//if there is only one value for that header name

std::string ClsQuiz::headerValue(std::string sHeaderName, int iCount) {
	vector<strHeader>::iterator itHeader;
	bool bIsFound;
	std::string sReturn;
	int iCounter;
	
	bIsFound=false;
	iCounter = 0;
	
	for(itHeader=this->vecHeader.begin(); itHeader < this->vecHeader.end(); itHeader++){
		if (itHeader->sName.compare(sHeaderName) == 0) {
			bIsFound = true;
			iCounter++;
			
			if (iCounter == iCount) {
				sReturn = itHeader->sValue;
			};
		};
	};
	
	if (bIsFound == true) {
		return sReturn;
	} else {
		return "";
	};
}; //if there are many header values for that header name return the Nth

int ClsQuiz::headerValueCount(std::string sHeaderName){
	vector<strHeader>::iterator itHeader;
	int iCounter;
	
	iCounter = 0;
	
	for(itHeader=this->vecHeader.begin(); itHeader < this->vecHeader.end(); itHeader++){
		if (itHeader->sName.compare(sHeaderName) == 0) {
			iCounter++;
		};
	};
	
	return iCounter;
	
}; //if there are many header values for that header name return a count of them

#pragma mark -
#pragma mark Mark

void ClsQuiz::mark() {
	vector<strQuestion>::iterator itQuestion;
	int iCounter;
	bool bIsCorrect;
	int iCountCorrect;
	
	iCounter = 0;
	iCountCorrect = 0;
	
	for(itQuestion=vecQuestions.begin(); itQuestion < vecQuestions.end(); itQuestion++){
		if (itQuestion->bIsMultipleChoice == true) {
//			bIsCorrect = this->isAnswerCorrectMultipleChoice(itQuestion->iId);
			bIsCorrect = this->isAnswerCorrectMultipleChoice(iCounter);
		} else {
			bIsCorrect = false;
//			bIsCorrect == this->isAnswerCorrectText();
		};
		
		if (bIsCorrect == true) {
			//update header
			iCountCorrect++;
		} else {
			//update header
		};
		
//		this->updateQuestion(itQuestion->iId);
		this->updateQuestion(iCounter);
		iCounter++;
	};

	std::stringstream ssCountCorrect;
	std::string sCountCorrect;
	
	ssCountCorrect.str("");
	ssCountCorrect << iCountCorrect;
	sCountCorrect = ssCountCorrect.str();
	
	this->changeHeader(modMisc::sHeader_QuestionCount, sCountCorrect);
//	cQuiz.changeHeader(modMisc::sHeader_QuizStatus, (std::string)"Incomplete");

};

bool ClsQuiz::isAnswerCorrectMultipleChoice(int iQuestionNo){
	vector<strStringAnswer>::iterator itAnswer;
	bool bIsCorrectChoice;
	bool bIsCorrectQuestion;
	strQuestion oQuestion;
	
	bIsCorrectQuestion = true;
	
	oQuestion = vecQuestions[iQuestionNo];
	
	for(itAnswer=oQuestion.vecChoices.begin(); itAnswer < oQuestion.vecChoices.end(); itAnswer++){
		if (itAnswer->bIsSelected != itAnswer->bIsCorrect) {
			bIsCorrectChoice = false;
		} else {
			//if the answer is a correct answer and the answer is selected
			//if the answer is NOT a correct answer and the answer is NOT selected
			bIsCorrectChoice = true;
		};

		if (bIsCorrectChoice == false) {
			bIsCorrectQuestion = false;
		};
	};
	
	vecQuestions[iQuestionNo].bCorrect = bIsCorrectQuestion;
	
	return bIsCorrectQuestion;
};

bool ClsQuiz::isAnswerCorrectText(int iQuestionNo) {
	vector<strFloatValue>::iterator itAnswer;
	bool bIsCorrectQuestion;
	strQuestion oQuestion;
	int iCounterAnswerFound;
	int iCounterAnswerNotFound;
	
	bIsCorrectQuestion = false;
	
	oQuestion = vecQuestions[iQuestionNo];
	
	if (oQuestion.eAnswerType == eAnsTypNumeric) {
		if (oQuestion.eAnswerType == eAnsTypNumeric) {
			
			iCounterAnswerFound = 0;
			iCounterAnswerNotFound = 0;
/*
			//assuming integer
			for(itAnswer=oQuestion.vecAnswers.begin(); itAnswer < oQuestion.vecAnswers.end(); itAnswer++){
				bIsFound = false;
				iPos = 0;
				do {
					iPos = oQuestion.sGivenAnswer.find(itAnswer.sGivenAnswer, iPos);
					
					if (iPos == oQuestion.sGivenAnswer.npos) {
						bIsFound = false;
					} else {
						
						
						 check if there isn't any ajoining text 
						 (eg if the answer we want is "23" and we are given "1234" then the 
						 answer is found but it's still wrong because of the ajoining text)
						 
						
						//iPos != 0 and if char before is a space or seperator
						//iPos != last char and if char after is a space or seperator
						//else
						bIsFound = true;
						
						//decimal points ???????????????????????????????????????
					};
				
				} while (iPos != itAnswer.sGivenAnswer.npos or bIsFound == false);
				
				//count how many of the answers have been found or not
				if (bIsFound == true) {
					iCounterAnswerNotFound++;
				} else {
					iCounterAnswerFound++;
				};
			};
			
			if (iCounterAnswerNotFound == 0) {
					//wrong
			};
*/
		}
	}
	
	vecQuestions[iQuestionNo].bCorrect = bIsCorrectQuestion;
	
	return bIsCorrectQuestion;
	
};

void ClsQuiz::insertQuestionAll(int iQuestionNo) {
	//loop through all the different parts of the strQuestion structure 
	//and call all the different functions to insert the data in the DB
	//so that we have a question saved
	int iQuestionNoSqlLite;
	int iValueNo;
	int iChoiceNo;
	
	vector<strFloatValue>::iterator itValue;
	vector<strStringAnswer>::iterator itChoice;
	
	insertQuestion(iQuestionNo);
	
	iValueNo = 0;
	for (itValue = vecQuestions[iQuestionNo].vecValues.begin(); itValue != vecQuestions[iQuestionNo].vecValues.end(); itValue++) {
		insertQuestionValue(iQuestionNo, iValueNo);
		iValueNo++;
	};

	iChoiceNo = 0;
	for (itChoice = vecQuestions[iQuestionNo].vecChoices.begin(); itChoice != vecQuestions[iQuestionNo].vecChoices.end(); itChoice++) {
		insertQuestionChoice(iQuestionNo, iChoiceNo);
		iChoiceNo++;
	};
	
	iQuestionNoSqlLite = vecQuestions[iQuestionNo].iId;

};

#pragma mark -
#pragma mark SQLite

void ClsQuiz::updateHeader(std::string sName, std::string sValue) {
	updateHeader(sName, sValue, "");
};

void ClsQuiz::updateHeader(std::string sName, std::string sValue, std::string sValue2){
	char *zErr;
	int rc;
	
	try {
		std::string sErrMsg;
		bool bIsOk;
		const char *cSqlUpdateTbl_Header("update tblHeader set fldValue = :sValue, fldValue2 = :sValue2 where fldName = :sName ;");
		sqlite3_stmt *stmt_Header;
		int iParameterPosition;
		char *cPath;
		
		bIsOk = true;
		rc = SQLITE_OK;
		
		if (bDbIsOpen != true){
			cPath = (char*)this->sFilePath.c_str();
			
			//open database connection
			rc = sqlite3_open(cPath, &db);
		};
		
		if(rc != SQLITE_OK){
#if TEST_ENV == true
			sErrMsg = (std::string)"Can't open DB: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
			fprintf(stderr, sErrMsg.c_str(), 0);
#endif
			bIsOk = false;
		};
		
		//Populate Difficulty table with default values	
		if(bIsOk) {
			rc = sqlite3_prepare_v2(db, cSqlUpdateTbl_Header, -1, &stmt_Header, NULL);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't Prepare SQL: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//sName
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt_Header, ":sName");
			
			rc = sqlite3_bind_text(stmt_Header, 
								   iParameterPosition,
								   sName.c_str(), 
								   -1,
								   SQLITE_TRANSIENT);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//Value
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt_Header, ":sValue");
			
			rc = sqlite3_bind_text(stmt_Header, 
								   iParameterPosition,
								   sValue.c_str(), 
								   -1,
								   SQLITE_TRANSIENT);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//Value2
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt_Header, ":sValue2");
			
			rc = sqlite3_bind_text(stmt_Header, 
								   iParameterPosition,
								   sValue2.c_str(), 
								   -1,
								   SQLITE_TRANSIENT);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//step (i.e. run the command)
		if(bIsOk){
			rc = sqlite3_step(stmt_Header);
			
			if(rc == SQLITE_ERROR){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't Step statment: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};//if bIsOk
		
		if(bIsOk){
			rc = sqlite3_reset(stmt_Header);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				fprintf(stderr, "Can't reset statment: %\n", sqlite3_errmsg(db));
#endif
				bIsOk = false;
			};
		};//if bIsOk
		
		
		sErrMsg.clear();
//		bool bIsOk;
		cSqlUpdateTbl_Header = NULL;
		if (bIsOk) {
			sqlite3_finalize(stmt_Header);
		};
//		int iParameterPosition;
		cPath = NULL;
		
	} catch (...) {
		//Close database connection	
		rc = sqlite3_close(db);
		bDbIsOpen = false;
	};
	
	zErr = NULL;
//	int rc;
};


void ClsQuiz::updateHeaderValue2(std::string sName, std::string sValue, std::string sValue2){
	char *zErr;
	int rc;
	
	try {
		std::string sErrMsg;
		bool bIsOk;
		const char *cSqlUpdateTbl_Header("update tblHeader set fldValue2 = :sValue2 where fldName = :sName and fldValue = :sValue;");
		sqlite3_stmt *stmt_Header;
		int iParameterPosition;
		char *cPath;
		
		bIsOk = true;
		rc = SQLITE_OK;
		
		if (bDbIsOpen != true){
			cPath = (char*)this->sFilePath.c_str();
			
			//open database connection
			rc = sqlite3_open(cPath, &db);
		};
		
		if(rc != SQLITE_OK){
#if TEST_ENV == true
			sErrMsg = (std::string)"Can't open DB: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
			fprintf(stderr, sErrMsg.c_str(), 0);
#endif
			bIsOk = false;
		};
		
		//Populate Difficulty table with default values	
		if(bIsOk) {
			rc = sqlite3_prepare_v2(db, cSqlUpdateTbl_Header, -1, &stmt_Header, NULL);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't Prepare SQL: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//sName
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt_Header, ":sName");
			
			rc = sqlite3_bind_text(stmt_Header, 
								   iParameterPosition,
								   sName.c_str(), 
								   -1,
								   SQLITE_TRANSIENT);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//Value
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt_Header, ":sValue");
			
			rc = sqlite3_bind_text(stmt_Header, 
								   iParameterPosition,
								   sValue.c_str(), 
								   -1,
								   SQLITE_TRANSIENT);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//Value2
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt_Header, ":sValue2");
			
			rc = sqlite3_bind_text(stmt_Header, 
								   iParameterPosition,
								   sValue2.c_str(), 
								   -1,
								   SQLITE_TRANSIENT);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//step (i.e. run the command)
		if(bIsOk){
			rc = sqlite3_step(stmt_Header);
			
			if(rc == SQLITE_ERROR){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't Step statment: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};//if bIsOk
		
		if(bIsOk){
			rc = sqlite3_reset(stmt_Header);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				fprintf(stderr, "Can't reset statment: %\n", sqlite3_errmsg(db));
#endif
				bIsOk = false;
			};
		};//if bIsOk
	
		sErrMsg.clear();
//		bool bIsOk;
		cSqlUpdateTbl_Header = NULL;
		if (bIsOk) {
			sqlite3_finalize(stmt_Header);
		};
//		int iParameterPosition;
		cPath = NULL;

	} catch (...) {
		//Close database connection	
		rc = sqlite3_close(db);
		bDbIsOpen = false;
	};

	zErr = NULL;
//	int rc;
	
};


void ClsQuiz::changeHeader(std::string sName, std::string sValue){
	changeHeader(sName, sValue, "");
};

void ClsQuiz::changeHeader(std::string sName, std::string sValue, std::string sValue2){
	int rc;
	
	try {
		const char *cSqlSelectTbl_Header("select fldId, fldName, fldValue from tblHeader where fldName = :sName ;");
		bool bIsOk;
		sqlite3_stmt *stmt;
		std::string sErrMsg;
		char *cPath;
		std::stringstream sResult;
		int iParameterPosition;
		
		bIsOk = true;
		rc = SQLITE_OK;
		
		if (bDbIsOpen != true){
			cPath = (char*)this->sFilePath.c_str();
			
			//open database connection
			rc = sqlite3_open(cPath, &db);
		};
		
		if(rc != SQLITE_OK){
#if TEST_ENV == true
			sErrMsg = (std::string)"Can't open DB: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
			printf(sErrMsg.c_str());
#endif
			bIsOk = false;
		};
		
		if(bIsOk) {
			rc = sqlite3_prepare_v2(db, cSqlSelectTbl_Header, -1, &stmt, NULL);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't Prepare SQL: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//sName
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt, ":sName");
			
			rc = sqlite3_bind_text(stmt, 
								   iParameterPosition,
								   sName.c_str(), 
								   -1,
								   SQLITE_TRANSIENT);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		int iCounter = 0;
		if(bIsOk){
			while(sqlite3_step(stmt) == SQLITE_ROW){
				iCounter++; 
			};//while
		};//if bIsOk
		
		if(bIsOk){
			rc = sqlite3_reset(stmt);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				fprintf(stderr, "Can't reset statment: %\n", sqlite3_errmsg(db));
#endif
				bIsOk = false;
			};
		};//if bIsOk
		
		if (iCounter == 0) {
			insertHeader(sName, sValue);
		} else {
			updateHeader(sName, sValue);
		};
	
		
		cSqlSelectTbl_Header = NULL;
//		bool bIsOk;
		if (bIsOk) {
			sqlite3_finalize(stmt);
		};
		sErrMsg.clear();
		cPath = NULL;
//		std::stringstream sResult;
//		int iParameterPosition;
		
	} catch (...) {
		//Close database connection	
		rc = sqlite3_close(db);
		bDbIsOpen = false;
	};

//	int rc;
	
}; //don't know if we should insert or update




void ClsQuiz::updateQuestion(int iQuestionNo) {
	int rc;
	
	try {
		std::string sErrMsg;
		bool bIsOk;
		const char *cSqlUpdateTbl_Question("update tblQuestions set fldQuestionNo = :iQuestionNo, fldQuestionType = :sQuestionType, fldQuestionGroup = :sQuestionGroup, fldQuestionEncoded = :sQuestionEncoded, fldDecodeText = :sDecodedText, fldQuestionText = :sQuestionText, fldGivenAnswerText = :sGivenAnswerText, fldGivenAnswerMultipleChoice = :iGivenAnswerMultipleChoice, fldDifficulty = :iDifficulty, fldCorrect = :iCorrect, fldAnswerType =:sAnswerType, fldIsMultipleChoice =:iIsMultipleChoice, fldHasOnlyOneAnswer =:iHasOnlyOneAnswer, fldIsDirty =:iIsDirty, fldPic =:sPic, fldHasPic =:iHasPic where fldId = :iId;");
		sqlite3_stmt *stmt_Question;
		int iParameterPosition;
		char *cPath;
		std::string sTemp;
		int iTemp;
		
		bIsOk = true;
		rc = SQLITE_OK;
		
		if (bDbIsOpen != true){
			cPath = (char*)this->sFilePath.c_str();
			
			//open database connection
			rc = sqlite3_open(cPath, &db);
		};
		
		if(rc != SQLITE_OK){
#if TEST_ENV == true
			sErrMsg = (std::string)"Can't open DB: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
			fprintf(stderr, sErrMsg.c_str(), 0);
#endif
			bIsOk = false;
		};
		
		//Populate Difficulty table with default values	
		if(bIsOk) {
			rc = sqlite3_prepare_v2(db, cSqlUpdateTbl_Question, -1, &stmt_Question, NULL);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't Prepare SQL: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//iId
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt_Question, ":iId");
			
			iTemp = vecQuestions[iQuestionNo].iId;
			
			rc = sqlite3_bind_int(stmt_Question, 
								  iParameterPosition,
								  iTemp);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//iQuestionNo
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt_Question, ":iQuestionNo");
			
			iTemp = iQuestionNo;
			
			rc = sqlite3_bind_int(stmt_Question, 
								  iParameterPosition,
								  iTemp);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//sQuestionType
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt_Question, ":sQuestionType");
			
			sTemp = modMisc::convertEnumText_QuestionType(vecQuestions[iQuestionNo].eQuestionType);
			
			rc = sqlite3_bind_text(stmt_Question, 
								   iParameterPosition,
								   sTemp.c_str(), 
								   -1,
								   SQLITE_TRANSIENT);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//sQuestionGroup
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt_Question, ":sQuestionGroup");
			
			sTemp = modMisc::convertEnum_QuesTypeGroupText(vecQuestions[iQuestionNo].eQuestionGroup);
			
			rc = sqlite3_bind_text(stmt_Question, 
								   iParameterPosition,
								   sTemp.c_str(), 
								   -1,
								   SQLITE_TRANSIENT);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//sQuestionEncoded 
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt_Question, ":sQuestionEncoded");
			
			sTemp = vecQuestions[iQuestionNo].sQuestionEncoded;
			
			rc = sqlite3_bind_text(stmt_Question, 
								   iParameterPosition,
								   sTemp.c_str(), 
								   -1,
								   SQLITE_TRANSIENT);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//sDecodedText
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt_Question, ":sDecodedText");
			
			sTemp = vecQuestions[iQuestionNo].sQuestionEncoded;
			
			rc = sqlite3_bind_text(stmt_Question, 
								   iParameterPosition,
								   sTemp.c_str(), 
								   -1,
								   SQLITE_TRANSIENT);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//sQuestionText 
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt_Question, ":sQuestionText");
			
			sTemp = vecQuestions[iQuestionNo].sQuestionEncoded;
			
			rc = sqlite3_bind_text(stmt_Question, 
								   iParameterPosition,
								   sTemp.c_str(), 
								   -1,
								   SQLITE_TRANSIENT);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		
		//sGivenAnswerText 
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt_Question, ":sGivenAnswerText");
			
			sTemp = vecQuestions[iQuestionNo].sGivenAnswer;
			
			rc = sqlite3_bind_text(stmt_Question, 
								   iParameterPosition,
								   sTemp.c_str(), 
								   -1,
								   SQLITE_TRANSIENT);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//iGivenAnswerMultipleChoice
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt_Question, ":iGivenAnswerMultipleChoice");
			
			iTemp = vecQuestions[iQuestionNo].iGivenAnswerMultipleChoice;
			
			rc = sqlite3_bind_int(stmt_Question, 
								  iParameterPosition,
								  iTemp);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//iDifficulty
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt_Question, ":iDifficulty");
			
			iTemp = vecQuestions[iQuestionNo].iDifficulty;
			
			rc = sqlite3_bind_int(stmt_Question, 
								  iParameterPosition,
								  iTemp);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//iCorrect
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt_Question, ":iCorrect");
			
			if(vecQuestions[iQuestionNo].bCorrect == true) {
				iTemp = 1;
			}else{
				iTemp = 0;
			};
			
			rc = sqlite3_bind_int(stmt_Question, 
								  iParameterPosition,
								  iTemp);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//sAnswerType
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt_Question, ":sAnswerType");
			
			enumAnswerType eAnsTypeTemp;
			
			eAnsTypeTemp = vecQuestions[iQuestionNo].eAnswerType;
			
			sTemp = modMisc::convertEnumText_AnswerType(eAnsTypeTemp);
			
			rc = sqlite3_bind_text(stmt_Question, 
								   iParameterPosition,
								   sTemp.c_str(), 
								   -1,
								   SQLITE_TRANSIENT);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//fldIsMultipleChoice =:iIsMultipleChoice, 

		
		//iIsMultipleChoice
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt_Question, ":iIsMultipleChoice");
			
			if(vecQuestions[iQuestionNo].bIsMultipleChoice == true) {
				iTemp = 1;
			}else{
				iTemp = 0;
			};
			
			rc = sqlite3_bind_int(stmt_Question, 
								  iParameterPosition,
								  iTemp);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//fldHasOnlyOneAnswer =:iHasOnlyOneAnswer, 
		
		//iHasOnlyOneAnswer
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt_Question, ":iHasOnlyOneAnswer");
			
			if(vecQuestions[iQuestionNo].bOnlyOneAnswer == true) {
				iTemp = 1;
			}else{
				iTemp = 0;
			};
			
			rc = sqlite3_bind_int(stmt_Question, 
								  iParameterPosition,
								  iTemp);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//fldIsDirty =:iIsDirty, 
		
		//iIsDirty
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt_Question, ":iIsDirty");
			
			if(vecQuestions[iQuestionNo].bIsDirty == true) {
				iTemp = 1;
			}else{
				iTemp = 0;
			};
			
			rc = sqlite3_bind_int(stmt_Question, 
								  iParameterPosition,
								  iTemp);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//fldPic =:sPic, 
		
		//sPic 
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt_Question, ":sPic");
			
			sTemp = modMisc::convertEnumText_Pic(vecQuestions[iQuestionNo].ePic);
			
			rc = sqlite3_bind_text(stmt_Question, 
								   iParameterPosition,
								   sTemp.c_str(), 
								   -1,
								   SQLITE_TRANSIENT);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//fldHasPic =:iHasPic
		
		//iHasPic
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt_Question, ":iHasPic");
			
			if(vecQuestions[iQuestionNo].bHasPic == true) {
				iTemp = 1;
			}else{
				iTemp = 0;
			};
			
			rc = sqlite3_bind_int(stmt_Question, 
								  iParameterPosition,
								  iTemp);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		

		
		
		
		
		
		
		
		
		
		
		if(bIsOk){
			rc = sqlite3_step(stmt_Question);
			
			if(rc == SQLITE_ERROR){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't Step statment: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};//if bIsOk
		
		vecQuestions[iQuestionNo].iId = sqlite3_last_insert_rowid(db);
		
		if(bIsOk){
			rc = sqlite3_reset(stmt_Question);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				fprintf(stderr, "Can't reset statment: %\n", sqlite3_errmsg(db));
#endif
				bIsOk = false;
			};
		};//if bIsOk
	
		
		sErrMsg.clear();
//		bool bIsOk;
		cSqlUpdateTbl_Question = NULL;
		if (bIsOk) {
			sqlite3_finalize(stmt_Question);
		};
//		int iParameterPosition;
		cPath = NULL;
		sTemp.clear();
//		int iTemp;
			
	} catch (...) {
		//Close database connection	
		rc = sqlite3_close(db);
		bDbIsOpen = false;
	};
//	int rc;
};

void ClsQuiz::insertHeader(std::string sName, std::string sValue){
	insertHeader(sName, sValue, "");
};

void ClsQuiz::insertHeader(std::string sName, std::string sValue, std::string sValue2){
	int rc;

	try {
		std::string sErrMsg;
		bool bIsOk;
		const char *cSqlInsertTbl_Header("insert into tblHeader (fldName, fldValue, fldValue2) values (:sName, :sValue, :sValue2);");
		sqlite3_stmt *stmt_Header;
		int iParameterPosition;
		char *cPath;
		
		bIsOk = true;
		rc = SQLITE_OK;
		
		if (bDbIsOpen != true){
			cPath = (char*)this->sFilePath.c_str();
			
			//open database connection
			rc = sqlite3_open(cPath, &db);
		};
		
		if(rc != SQLITE_OK){
#if TEST_ENV == true
			sErrMsg = (std::string)"Can't open DB: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
			fprintf(stderr, sErrMsg.c_str(), 0);
#endif
			bIsOk = false;
		};
		
		//Populate Difficulty table with default values	
		if(bIsOk) {
			rc = sqlite3_prepare_v2(db, cSqlInsertTbl_Header, -1, &stmt_Header, NULL);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't Prepare SQL: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//sName
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt_Header, ":sName");
			
			rc = sqlite3_bind_text(stmt_Header, 
								   iParameterPosition,
								   sName.c_str(), 
								   -1,
								   SQLITE_TRANSIENT);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//sValue 
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt_Header, ":sValue");
			
			rc = sqlite3_bind_text(stmt_Header, 
								   iParameterPosition,
								   sValue.c_str(), 
								   -1,
								   SQLITE_TRANSIENT);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//sValue2 
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt_Header, ":sValue2");
			
			rc = sqlite3_bind_text(stmt_Header, 
								   iParameterPosition,
								   sValue2.c_str(), 
								   -1,
								   SQLITE_TRANSIENT);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//step (i.e. run the command)
		if(bIsOk){
			rc = sqlite3_step(stmt_Header);
			
			if(rc == SQLITE_ERROR){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't Step statment: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};//if bIsOk
		
		if(bIsOk){
			rc = sqlite3_reset(stmt_Header);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				fprintf(stderr, "Can't reset statment: %\n", sqlite3_errmsg(db));
#endif
				bIsOk = false;
			};
		};//if bIsOk

		
		sErrMsg.clear();
//		bool bIsOk;
		cSqlInsertTbl_Header = NULL;
		if (bIsOk) {
			sqlite3_finalize(stmt_Header);
		};
//		int iParameterPosition;
		cPath = NULL;
		
	} catch (...) {
		//Close database connection	
		rc = sqlite3_close(db);
		bDbIsOpen = false;
	};

//	int rc;

};

void ClsQuiz::insertQuestion(int iQuestionNo) {
	int rc;

	try {
		std::string sErrMsg;
		bool bIsOk;
		const char *cSqlInsertTbl_Question("insert into tblQuestions (fldQuestionNo, fldQuestionType, fldQuestionGroup, fldQuestionEncoded, fldDecodeText, fldQuestionText, fldGivenAnswerText, fldGivenAnswerMultipleChoice, fldDifficulty, fldCorrect, fldAnswerType, fldIsMultipleChoice, fldHasOnlyOneAnswer, fldIsDirty, fldPic, fldHasPic) values (:iQuestionNo, :sQuestionType, :sQuestionGroup, :sQuestionEncoded, :sDecodedText, :sQuestionText, :sGivenAnswerText, :iGivenAnswerMultipleChoice, :iDifficulty, :iCorrect, :sAnswerType, :iIsMultipleChoice, :iHasOnlyOneAnswer, :iIsDirty, :sPic, :iHasPic);");
		sqlite3_stmt *stmt_Question;
		int iParameterPosition;
		char *cPath;
		std::string sTemp;
		int iTemp;
		
		bIsOk = true;
		rc = SQLITE_OK;
		
		if (bDbIsOpen != true){
			cPath = (char*)this->sFilePath.c_str();
			
			//open database connection
			rc = sqlite3_open(cPath, &db);
		};
		
		if(rc != SQLITE_OK){
#if TEST_ENV == true
			sErrMsg = (std::string)"Can't open DB: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
			fprintf(stderr, sErrMsg.c_str(), 0);
#endif
			bIsOk = false;
		};
		
		//Populate Difficulty table with default values	
		if(bIsOk) {
			rc = sqlite3_prepare_v2(db, cSqlInsertTbl_Question, -1, &stmt_Question, NULL);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't Prepare SQL: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};

		//iQuestionNo
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt_Question, ":iQuestionNo");
			
			iTemp = iQuestionNo;
			
			rc = sqlite3_bind_int(stmt_Question, 
								  iParameterPosition,
								  iTemp);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//sQuestionType
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt_Question, ":sQuestionType");
			
			enumQuestionType eQuesTypeTemp;
			eQuesTypeTemp = vecQuestions[iQuestionNo].eQuestionType;
			
			sTemp = modMisc::convertEnumText_QuestionType(eQuesTypeTemp);
			
			rc = sqlite3_bind_text(stmt_Question, 
								   iParameterPosition,
								   sTemp.c_str(), 
								   -1,
								   SQLITE_TRANSIENT);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//sQuestionGroup
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt_Question, ":sQuestionGroup");
			
			enumQuestionGroup eQuesGroupTemp;
			eQuesGroupTemp = vecQuestions[iQuestionNo].eQuestionGroup;
			
			sTemp = modMisc::convertEnum_QuesTypeGroupText(eQuesGroupTemp);
			
			rc = sqlite3_bind_text(stmt_Question, 
								   iParameterPosition,
								   sTemp.c_str(), 
								   -1,
								   SQLITE_TRANSIENT);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//sQuestionEncoded 
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt_Question, ":sQuestionEncoded");
			
			sTemp = vecQuestions[iQuestionNo].sQuestionEncoded;
			
			rc = sqlite3_bind_text(stmt_Question, 
								   iParameterPosition,
								   sTemp.c_str(), 
								   -1,
								   SQLITE_TRANSIENT);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//sDecodedText
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt_Question, ":sDecodedText");
			
			sTemp = vecQuestions[iQuestionNo].sDecodeText;
			
			rc = sqlite3_bind_text(stmt_Question, 
								   iParameterPosition,
								   sTemp.c_str(), 
								   -1,
								   SQLITE_TRANSIENT);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//sQuestionText 
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt_Question, ":sQuestionText");
			
			sTemp = vecQuestions[iQuestionNo].sQuestionEncoded;
			
			rc = sqlite3_bind_text(stmt_Question, 
								   iParameterPosition,
								   sTemp.c_str(), 
								   -1,
								   SQLITE_TRANSIENT);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};

		
		//sGivenAnswerText 
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt_Question, ":sGivenAnswerText");
			
			sTemp = vecQuestions[iQuestionNo].sGivenAnswer;
			
			rc = sqlite3_bind_text(stmt_Question, 
								   iParameterPosition,
								   sTemp.c_str(), 
								   -1,
								   SQLITE_TRANSIENT);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};

		//iGivenAnswerMultipleChoice
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt_Question, ":iGivenAnswerMultipleChoice");
			
			iTemp = vecQuestions[iQuestionNo].iGivenAnswerMultipleChoice;
			
			rc = sqlite3_bind_int(stmt_Question, 
								  iParameterPosition,
								  iTemp);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		
		
		//iDifficulty
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt_Question, ":iDifficulty");
			
			iTemp = vecQuestions[iQuestionNo].iDifficulty;
			
			rc = sqlite3_bind_int(stmt_Question, 
								  iParameterPosition,
								  iTemp);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//iCorrect
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt_Question, ":iCorrect");
			
			if(vecQuestions[iQuestionNo].bCorrect == true) {
				iTemp = 1;
			}else{
				iTemp = 0;
			};
			
			rc = sqlite3_bind_int(stmt_Question, 
								  iParameterPosition,
								  iTemp);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//sAnswerType
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt_Question, ":sAnswerType");
			
			sTemp = modMisc::convertEnumText_AnswerType(vecQuestions[iQuestionNo].eAnswerType);
			
			rc = sqlite3_bind_text(stmt_Question, 
								   iParameterPosition,
								   sTemp.c_str(), 
								   -1,
								   SQLITE_TRANSIENT);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//:iIsMultipleChoice, 
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt_Question, ":iIsMultipleChoice");
			
			if(vecQuestions[iQuestionNo].bIsMultipleChoice == true) {
				iTemp = 1;
			}else{
				iTemp = 0;
			};
			
			rc = sqlite3_bind_int(stmt_Question, 
								  iParameterPosition,
								  iTemp);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//:iHasOnlyOneAnswer, 
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt_Question, ":iHasOnlyOneAnswer");
			
			if(vecQuestions[iQuestionNo].bOnlyOneAnswer == true) {
				iTemp = 1;
			}else{
				iTemp = 0;
			};
			
			rc = sqlite3_bind_int(stmt_Question, 
								  iParameterPosition,
								  iTemp);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//:iIsDirty, 
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt_Question, ":iIsDirty");
			
			if(vecQuestions[iQuestionNo].bIsDirty == true) {
				iTemp = 1;
			}else{
				iTemp = 0;
			};
			
			rc = sqlite3_bind_int(stmt_Question, 
								  iParameterPosition,
								  iTemp);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//:sPic, 
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt_Question, ":sPic");
			
			sTemp = modMisc::convertEnumText_Pic(vecQuestions[iQuestionNo].ePic);
			
			rc = sqlite3_bind_text(stmt_Question, 
								   iParameterPosition,
								   sTemp.c_str(), 
								   -1,
								   SQLITE_TRANSIENT);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//:iHasPic
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt_Question, ":iHasPic");
			
			if(vecQuestions[iQuestionNo].bHasPic == true) {
				iTemp = 1;
			}else{
				iTemp = 0;
			};
			
			rc = sqlite3_bind_int(stmt_Question, 
								  iParameterPosition,
								  iTemp);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		
		
		
		
		
		
		
		
		
		
		
		if(bIsOk){
			rc = sqlite3_step(stmt_Question);
			
			if(rc == SQLITE_ERROR){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't Step statment: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};//if bIsOk
		
		vecQuestions[iQuestionNo].iSqlite_Id = sqlite3_last_insert_rowid(db);
		vecQuestions[iQuestionNo].iId = sqlite3_last_insert_rowid(db);
		
		if(bIsOk){
			rc = sqlite3_reset(stmt_Question);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				fprintf(stderr, "Can't reset statment: %\n", sqlite3_errmsg(db));
#endif
				bIsOk = false;
			};
		};//if bIsOk
		
		
		
		sErrMsg.clear();
//		bool bIsOk;
		cSqlInsertTbl_Question = NULL;
		if (bIsOk) {
			sqlite3_finalize(stmt_Question);
		};
//		int iParameterPosition;
		cPath = NULL;
		sTemp.clear();
//		int iTemp;
		
	} catch (...) {
		//Close database connection	
		rc = sqlite3_close(db);
		bDbIsOpen = false;
	};

//	int rc;

};

void ClsQuiz::insertQuestionValue(int iQuestionNo, int iValueNo){
	int rc;
	
	try {
		std::string sErrMsg;
		bool bIsOk;
		const char *cSqlInsertTbl_QuestionValues("insert into tblQuestionsValues (fldAppId, fldQuestionID, fldValues, fldIsDirty) values (:iAppId, :iQuestionNo, :fValue, :iIsDirty);");
		sqlite3_stmt *stmt;
		int iParameterPosition;
		char *cPath;
		std::string sTemp;
		float fTemp;
		int iTemp;
		
		bIsOk = true;
		rc = SQLITE_OK;
		
		if (bDbIsOpen != true){
			cPath = (char*)this->sFilePath.c_str();
			
			//open database connection
			rc = sqlite3_open(cPath, &db);
		};
		
		if(rc != SQLITE_OK){
#if TEST_ENV == true
			sErrMsg = (std::string)"Can't open DB: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
			fprintf(stderr, sErrMsg.c_str(), 0);
#endif
			bIsOk = false;
		};
		
		//Populate Difficulty table with default values	
		if(bIsOk) {
			rc = sqlite3_prepare_v2(db, cSqlInsertTbl_QuestionValues, -1, &stmt, NULL);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't Prepare SQL: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//iQuestionNo
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt, ":iQuestionNo");
			
			iTemp = iQuestionNo;
			
			rc = sqlite3_bind_int(stmt, 
								  iParameterPosition,
								  iTemp);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//iAppId
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt, ":iAppId");
			
			iTemp = iValueNo;
			
			rc = sqlite3_bind_int(stmt, 
								  iParameterPosition,
								  iTemp);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//fValue
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt, ":fValue");
			
			fTemp = vecQuestions[iQuestionNo].vecValues[iValueNo].fValue;
			
			rc = sqlite3_bind_double(stmt, 
								     iParameterPosition,
								     fTemp);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//iIsDirty
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt, ":iIsDirty");
			
			fTemp = vecQuestions[iQuestionNo].vecValues[iValueNo].bIsDirty;
			
			rc = sqlite3_bind_int(stmt, 
								  iParameterPosition,
								  fTemp);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		if(bIsOk){
			rc = sqlite3_step(stmt);
			
			if(rc == SQLITE_ERROR){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't Step statment: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};//if bIsOk
		
		if(bIsOk){
			rc = sqlite3_reset(stmt);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				fprintf(stderr, "Can't reset statment: %\n", sqlite3_errmsg(db));
#endif
				bIsOk = false;
			};
		};//if bIsOk
		
		
		sErrMsg.clear();
//		bool bIsOk;
		cSqlInsertTbl_QuestionValues = NULL;
		if (bIsOk) {
			sqlite3_finalize(stmt);
		};
//		int iParameterPosition;
		cPath = NULL;
		sTemp.clear();
//		float fTemp;
//		int iTemp;
		
	} catch (...) {
		//Close database connection	
		rc = sqlite3_close(db);
		bDbIsOpen = false;
	};

//	int rc;

};


void ClsQuiz::insertQuestionChoice(int iQuestionNo, int iChoiceNo){
	int rc;
	
	try {
		std::string sErrMsg;
		bool bIsOk;
		const char *cSqlInsertTbl_QuestionChoices("insert into tblQuestionsChoices (fldAppId, fldQuestionID, fldValue, fldIsSelected, fldIsCorrect) values (:iAppId, :iQuestionNo, :sValue, :iIsSelected, :iIsCorrect) ;");
		sqlite3_stmt *stmt;
		int iParameterPosition;
		char *cPath;
		std::string sTemp;
		int iTemp;
		
		bIsOk = true;
		rc = SQLITE_OK;
		
		if (bDbIsOpen != true){
			cPath = (char*)this->sFilePath.c_str();
			
			//open database connection
			rc = sqlite3_open(cPath, &db);
		};
		
		if(rc != SQLITE_OK){
#if TEST_ENV == true
			sErrMsg = (std::string)"Can't open DB: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
			fprintf(stderr, sErrMsg.c_str(), 0);
#endif
			bIsOk = false;
		};
		
		//Populate Difficulty table with default values	
		if(bIsOk) {
			rc = sqlite3_prepare_v2(db, cSqlInsertTbl_QuestionChoices, -1, &stmt, NULL);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't Prepare SQL: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//iQuestionNo
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt, ":iQuestionNo");
			
			iTemp = iQuestionNo;
			
			rc = sqlite3_bind_int(stmt, 
								  iParameterPosition,
								  iTemp);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//iAppId
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt, ":iAppId");
			
			iTemp = iChoiceNo;
			
			rc = sqlite3_bind_int(stmt, 
								  iParameterPosition,
								  iTemp);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//:sValue
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt, ":sValue");

			sTemp = vecQuestions[iQuestionNo].vecChoices[iChoiceNo].sValue;

			rc = sqlite3_bind_text(stmt, 
								   iParameterPosition,
								   sTemp.c_str(), 
								   -1, 
								   SQLITE_TRANSIENT);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add double parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//:iIsSelected, :iIsCorrect) ;");
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt, ":iIsSelected");
			
			if (vecQuestions[iQuestionNo].vecChoices[iChoiceNo].bIsSelected == true) {
				iTemp = 1;
			} else {
				iTemp = 0;
			};

			
			rc = sqlite3_bind_int(stmt, 
								  iParameterPosition,
								  iTemp);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//:iIsCorrect) ;");
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt, ":iIsCorrect");
			
			if (vecQuestions[iQuestionNo].vecChoices[iChoiceNo].bIsCorrect == true) {
				iTemp = 1;
			} else {
				iTemp = 0;
			};
			
			rc = sqlite3_bind_int(stmt, 
								  iParameterPosition,
								  iTemp);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		if(bIsOk){
			rc = sqlite3_step(stmt);
			
			if(rc == SQLITE_ERROR){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't Step statment: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};//if bIsOk
		
		if(bIsOk){
			rc = sqlite3_reset(stmt);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				fprintf(stderr, "Can't reset statment: %\n", sqlite3_errmsg(db));
#endif
				bIsOk = false;
			};
		};//if bIsOk
		
		
		sErrMsg.clear();
//		bool bIsOk;
		cSqlInsertTbl_QuestionChoices = NULL;
		if (bIsOk) {
			sqlite3_finalize(stmt);
		};
//		int iParameterPosition;
		cPath = NULL;
		sTemp.clear();
//		int iTemp;
		
	} catch (...) {
		//Close database connection	
		rc = sqlite3_close(db);
		bDbIsOpen = false;
	};

//	int rc;
};

void ClsQuiz::insertQuestionAnswersWanted(int iQuestionNo, int iAnswerNo, float fAnswerValue){
	//	char *cSqlCreateTbl_QuestionAnswersWanted("create table tblQuestionsAnswersWanted (fldId integer primary key, fldQuestionID int, fldAnswer float);");
	
};

void ClsQuiz::insertQuestionAnswersGiven(int iQuestionNo, int iAnswerNo, float fAnswerValue){
	//	char *cSqlCreateTbl_QuestionAnswersWanted("create table tblQuestionsAnswersWanted (fldId integer primary key, fldQuestionID int, fldAnswer float);");
	
};

void ClsQuiz::selectHeader(){
	int rc;
	
	try {
		const char *cSqlSelectTbl_Header("select fldId, fldName, fldValue, fldValue2 from tblHeader ");
		bool bIsOk;
		sqlite3_stmt *stmt;
		std::string sErrMsg;
		char *cPath;
		std::stringstream sResult;

		strHeader sHeader;
		
		this->vecHeader.clear();
		
		bIsOk = true;
		rc = SQLITE_OK;
		
		if (bDbIsOpen != true){
			cPath = (char*)this->sFilePath.c_str();
			
			//open database connection
			rc = sqlite3_open(cPath, &db);
			bDbIsOpen = true;
		};
		
		if(rc != SQLITE_OK){
#if TEST_ENV == true
			sErrMsg = (std::string)"Can't open DB: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
			printf(sErrMsg.c_str());
#endif
			bIsOk = false;
			bDbIsOpen = false;
		};
		
		if(bIsOk) {
			rc = sqlite3_prepare_v2(db, cSqlSelectTbl_Header, -1, &stmt, NULL);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't Prepare SQL: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		if(bIsOk){
			sHeader.iId = 0; 
			sHeader.sName = ""; 
			sHeader.sValue = ""; 
			
			while(sqlite3_step(stmt) == SQLITE_ROW){
				sHeader.iId = 0; 
				sHeader.sName = ""; 
				sHeader.sValue = ""; 

				sHeader.iId = sqlite3_column_int(stmt, 0);
				
				sResult.str("");
				sResult << sqlite3_column_text(stmt, 1);
				sHeader.sName = sResult.str();
				
				sResult.str("");
				sResult << sqlite3_column_text(stmt, 2);
				sHeader.sValue = sResult.str();
				
				sResult.str("");
				sResult << sqlite3_column_text(stmt, 3);
				sHeader.sValue2 = sResult.str();
				
				this->vecHeader.push_back(sHeader);
			};//while
		};//if bIsOk
		
		if(bIsOk){
			rc = sqlite3_reset(stmt);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				fprintf(stderr, "Can't reset statment: %\n", sqlite3_errmsg(db));
#endif
				bIsOk = false;
			};
		};//if bIsOk
		
		
		cSqlSelectTbl_Header = NULL;
//		bool bIsOk;
		if (bIsOk) {
			sqlite3_finalize(stmt);
		};
		sErrMsg.clear();
		cPath = NULL;
//		std::stringstream sResult;
		
	} catch (...) {
		//Close database connection	
		rc = sqlite3_close(db);
		bDbIsOpen = false;
	};

//	int rc;
	
};

int ClsQuiz::secondSpent() {

	vector<strHeader>::iterator itHeader;
	int iTotalSeconds;
	int iValue;
	int iValue2;
	
	iTotalSeconds = 0;
	iValue = 0;
	iValue2 = 0;
	
	for (itHeader = this->vecHeader.begin(); itHeader < this->vecHeader.end(); itHeader++) {
		if (itHeader->sName.compare(modMisc::sHeader_QuizDateTimeSplit) == 0) {
			iValue = atoi(itHeader->sValue.c_str());
			iValue2 = atoi(itHeader->sValue2.c_str());
			
//			iValue = (int)itHeader->sValue;
//			iValue2 = (int)itHeader->sValue2;
			
			iTotalSeconds = iTotalSeconds + iValue2 - iValue;
		};
	};

	return iTotalSeconds;
};


void ClsQuiz::selectQuestion(int iQuestionNo){
	//select as in the SQL phrase meaning read the contence of the SQLite file

}; 

void ClsQuiz::selectQuestionValue(int iQuestionNo, int iValueNo){
	//select as in the SQL phrase meaning read the contence of the SQLite file

}; 

void ClsQuiz::selectQuestionAnswersWanted(int iQuestionNo, int iAnswerNo){
	//select as in the SQL phrase meaning read the contence of the SQLite file

};

void ClsQuiz::selectQuestionAnswersGiven(int iQuestionNo, int iAnswerNo){
	//select as in the SQL phrase meaning read the contence of the SQLite file

}; 

void ClsQuiz::saveQuiz(std::string sPath){
//	sqlite3 *db;
//	char *zErr;
	int rc;

	try {
		/*		
		std::string sErrMsg;
		bool bIsOk;
		char *cSqlCreateTbl_Header("create table tblHeader (fldId integer primary key, fldName varchar(255), fldValue varchar(255));");
		char *cSqlCreateTbl_Diff("create table tblDifficulty (fldId integer primary key, fldDifficulty double);");
		char *cSqlCreateTbl_Enabled("create table tblEnabled (fldId integer primary key, fldGroup varchar(255), fldName varchar(255), fldEnabled integer);");
		char *cSqlInsertTbl_Header("insert into tblHeader (fldName, fldValue) values (:sName, :sValue);");
		char *cSqlInsertTbl_Diff("insert into tblDifficulty (fldDifficulty) values (:dDifficulty);");
		char *cSqlInsertTbl_Enabled("insert into tblEnabled (fldGroup, fldName, fldEnabled) values (:sGroup, :sName, :bEnabled);");
		sqlite3_stmt *stmt_Diff;
		sqlite3_stmt *stmt_Enabled;
		int iParameterPosition;
		std::vector<strSubject> vecSubject;
		strSubject sSubject;
		std::string sName;
		int iCounterProperties;
		char *cPath;
		std::vector<strSubject>::iterator itProperty;
		float fDifficulty;

		sSubject.sGroup = "arithmetic";
		sSubject.sName = "add";
		sSubject.bEnabled = true;
		vecSubject.push_back(sSubject);
		
		sSubject.sGroup = "arithmetic";
		sSubject.sName = "subtract";
		sSubject.bEnabled = true;
		vecSubject.push_back(sSubject);
		
		sSubject.sGroup = "arithmetic";
		sSubject.sName = "multiple";
		sSubject.bEnabled = true;
		vecSubject.push_back(sSubject);
		
		sSubject.sGroup = "arithmetic";
		sSubject.sName = "divide";
		sSubject.bEnabled = true;
		vecSubject.push_back(sSubject);
		
		sSubject.sGroup = "stuff";
		sSubject.sName = "one";
		sSubject.bEnabled = true;
		vecSubject.push_back(sSubject);
		
		sSubject.sGroup = "stuff";
		sSubject.sName = "two";
		sSubject.bEnabled = true;
		vecSubject.push_back(sSubject);
		
		fDifficulty = 100.0f;
		
		bIsOk = true;
		rc = SQLITE_OK;
		
		if (bDbIsOpen != true){
			cPath = (char*)this->sFilePath.c_str();

			remove(cPath);
			
			//open database connection
			rc = sqlite3_open(cPath, &db);
		};
		
		if(rc != SQLITE_OK){
	#if TEST_ENV == true
			sErrMsg = (std::string)"Can't open DB: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
			fprintf(stderr, sErrMsg.c_str(), 0);
	#endif
			bIsOk = false;
		};
		
		//create difficulty table to store the level of difficulty
		if(bIsOk) {
			rc = sqlite3_exec(db, cSqlCreateTbl_Diff, NULL, NULL, &zErr);
			
			if(rc != SQLITE_OK){
	#if TEST_ENV == true
				sErrMsg = (std::string)"Can't open DB: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
	#endif
				bIsOk = false;
			};
		};
		
		//create enabled table to store which subjects are enabled
		if(bIsOk) {
			rc = sqlite3_exec(db, cSqlCreateTbl_Enabled, NULL, NULL, &zErr);
			
			if(rc != SQLITE_OK){
	#if TEST_ENV == true
				sErrMsg = (std::string)"Can't open DB: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
	#endif
				bIsOk = false;
			};
		};
		
		//Populate Difficulty table with default values	
		if(bIsOk) {
			rc = sqlite3_prepare_v2(db, cSqlInsertTbl_Diff, -1, &stmt_Diff, NULL);
			
			if(rc != SQLITE_OK){
	#if TEST_ENV == true
				sErrMsg = (std::string)"Can't Prepare SQL: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
	#endif
				bIsOk = false;
			};
		};
		
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt_Diff, ":dDifficulty");
			
			rc = sqlite3_bind_double(stmt_Diff, 
									 iParameterPosition,
									 (double)fDifficulty);
			
			if(rc != SQLITE_OK){
	#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add int parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
	#endif
				bIsOk = false;
			};
		};
		
		if(bIsOk){
			rc = sqlite3_step(stmt_Diff);
			
			if(rc == SQLITE_ERROR){
	#if TEST_ENV == true
				sErrMsg = (std::string)"Can't Step statment: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
	#endif
				bIsOk = false;
			};
		};
		
		//coffeeID = sqlite3_last_insert_rowid(database);
		
		if(bIsOk){
			rc = sqlite3_reset(stmt_Diff);
			
			if(rc != SQLITE_OK){
	#if TEST_ENV == true
				fprintf(stderr, "Can't reset statment: %\n", sqlite3_errmsg(db));
	#endif
				bIsOk = false;
			};
		};//if bIsOk
		
		//Populate Enabled table with default values	
		iCounterProperties = 0;
		
		for(itProperty=vecSubject.begin(); itProperty < vecSubject.end(); itProperty++){
			if(bIsOk) {
				rc = sqlite3_prepare_v2(db, cSqlInsertTbl_Enabled, -1, &stmt_Enabled, NULL);
				
				if(rc != SQLITE_OK){
	#if TEST_ENV == true
					sErrMsg = (std::string)"Can't Prepare SQL: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
					printf(sErrMsg.c_str());
	#endif
					bIsOk = false;
				};
			};//if bIsOk
			
			sSubject = vecSubject[iCounterProperties];
			
			if(bIsOk){
				iParameterPosition = sqlite3_bind_parameter_index(stmt_Enabled, ":sGroup");
				
				rc = sqlite3_bind_text(stmt_Enabled, 
									   iParameterPosition,
									   sSubject.sGroup.c_str(), 
									   -1,
									   SQLITE_TRANSIENT);
				
				if(rc != SQLITE_OK){
	#if TEST_ENV == true
					sErrMsg = (std::string)"Can't add string parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
					printf(sErrMsg.c_str());
	#endif
					bIsOk = false;
				};
			};//if bIsOk
			
			if(bIsOk){
				iParameterPosition = sqlite3_bind_parameter_index(stmt_Enabled, ":sName");
				
				rc = sqlite3_bind_text(stmt_Enabled, 
									   iParameterPosition,
									   sSubject.sName.c_str(), 
									   -1,
									   SQLITE_TRANSIENT);
				
				if(rc != SQLITE_OK){
	#if TEST_ENV == true
					sErrMsg = (std::string)"Can't add string parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
					printf(sErrMsg.c_str());
	#endif
					bIsOk = false;
				};
			};//if bIsOk
			
			if(bIsOk){
				iParameterPosition = sqlite3_bind_parameter_index(stmt_Enabled, ":bEnabled");
				
				if(sSubject.bEnabled){
					rc = sqlite3_bind_int(stmt_Enabled, 
										  iParameterPosition,
										  1); // 1 = true (enabled); 0 = false (disabled)
				}
				else
				{
					rc = sqlite3_bind_int(stmt_Enabled, 
										  iParameterPosition,
										  0); // 1 = true (enabled); 0 = false (disabled)
				};
				
				if(rc != SQLITE_OK){
	#if TEST_ENV == true
					sErrMsg = (std::string)"Can't add int parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
					printf(sErrMsg.c_str());
	#endif
					bIsOk = false;
				};
			};//if bIsOk
			
			if(bIsOk){
				rc = sqlite3_step(stmt_Enabled);
				
				if(rc == SQLITE_ERROR){
	#if TEST_ENV == true
					sErrMsg = (std::string)"Can't Step statment: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
					printf(sErrMsg.c_str());
	#endif
					bIsOk = false;
				};
			};//if bIsOk
			
			//coffeeID = sqlite3_last_insert_rowid(database);
			
			if(bIsOk){
				rc = sqlite3_reset(stmt_Enabled);
				
				if(rc != SQLITE_OK){
	#if TEST_ENV == true
					fprintf(stderr, "Can't reset statment: %\n", sqlite3_errmsg(db));
	#endif
					bIsOk = false;
				};
			};//if bIsOk
			
			iCounterProperties++;
		};//for
*/		
	}
	catch(...) {
		rc = sqlite3_close(db);
		bDbIsOpen = false;
	};
	//	this->load_subjects();
};

std::string ClsQuiz::sQuestionQuestion(int iQuestionNo){
	if (iQuestionNo >= 0 and iQuestionNo <= this->vecQuestions.size()) {
		strQuestion* sQues;
		
		sQues = &vecQuestions[iQuestionNo]; 
		
		return sQues->sQuestionText;
	}
	else {
		return "Error";
	};
};

void ClsQuiz::setGivenAnswer(std::string sAnswer){

}; 

void ClsQuiz::setGivenAnswer(std::string sAnswer, int iQuestionNo) {
	if (iQuestionNo >= 0 and iQuestionNo <= this->vecQuestions.size()) {
		this->vecQuestions[iQuestionNo].sGivenAnswer = sAnswer;
	};
}; 

std::string ClsQuiz::getGivenAnswer(int iQuestionNo) {
	std::string sResult;

	if (iQuestionNo >= 0 and iQuestionNo <= this->vecQuestions.size()) {
		sResult = this->vecQuestions[iQuestionNo].sGivenAnswer;
	} else {
		sResult = "";
	};

	return sResult;
}; 



bool ClsQuiz::questionIsMultipleChoice(int iQuestionNo) {
	if (iQuestionNo >= 0 and iQuestionNo <= this->vecQuestions.size()) {
		bool bTemp = this->vecQuestions[iQuestionNo].bIsMultipleChoice;
	
		return bTemp;
	} else {
		return false;
	};
};

bool ClsQuiz::questionIsPicture(int iQuestionNo) {
	if (iQuestionNo >= 0 and iQuestionNo <= this->vecQuestions.size()) {
		bool bTemp = this->vecQuestions[iQuestionNo].bHasPic;
		
		return bTemp;
	} else {
		return false;
	};
};

std::string ClsQuiz::picName(int iQuestionNo) {
	std::string sPicName;
	
	if (iQuestionNo >= 0 and iQuestionNo <= vecQuestions.size()) {
		sPicName = "";
		
		if (vecQuestions[iQuestionNo].ePic == ePic_None) {
			sPicName = "";
		};
		
		if (vecQuestions[iQuestionNo].ePic == ePicQues_TriangleRightAngle) {
			sPicName = "triangle right angle";
		};
		
		if (vecQuestions[iQuestionNo].ePic == ePicQues_TriangleNotRight) {
			sPicName = "";
		};
		
		if (vecQuestions[iQuestionNo].ePic == ePicQues_Parallelogram) {
			sPicName = "parallogram";
		};
		
		if (vecQuestions[iQuestionNo].ePic == ePicQues_Cylinder) {
			sPicName = "cylinder";
		};
		
		if (vecQuestions[iQuestionNo].ePic == ePicQues_Circle) {
			sPicName = "";
		};
		
		if (vecQuestions[iQuestionNo].ePic == ePicQues_Square) {
			sPicName = "";
		};
		
		if (vecQuestions[iQuestionNo].ePic == ePicQues_Cube) {
			sPicName = "";
		}; 
	} else {
		sPicName = "";
	};
	
	return sPicName;
}

enumPicQues ClsQuiz::picType(int iQuestionNo) {
	if (iQuestionNo >= 0 and iQuestionNo <= this->vecQuestions.size()) {
		 return this->vecQuestions[iQuestionNo].ePic; 
	} else {
		return ePic_None;
	};
}

std::string ClsQuiz::multipleChoiceAnswerText(int iQuestionNo, int iAnswerNo){
	std::string sAnswer;
	
	if (iQuestionNo >= 0 and iQuestionNo < this->vecQuestions.size()) {
		if (iAnswerNo >= 0 and iAnswerNo < this->vecQuestions[iQuestionNo].vecChoices.size()) {
			sAnswer = this->vecQuestions[iQuestionNo].vecChoices[iAnswerNo].sValue;
		} else {
			sAnswer = "";
		};
	} else {
		sAnswer = "";
	};
	
	return sAnswer;
};

bool ClsQuiz::multipleChoiceAnswerCorrect(int iQuestionNo, int iAnswerNo){
	bool bCorrect;
	
	if (iQuestionNo >= 0 and iQuestionNo < this->vecQuestions.size()) {
		if (iAnswerNo >= 0 and iAnswerNo < this->vecQuestions[iQuestionNo].vecChoices.size()) {
			bCorrect = this->vecQuestions[iQuestionNo].vecChoices[iAnswerNo].bIsCorrect;
		} else {
			bCorrect = false;
		};
	} else {
		bCorrect = false;
	};
	
	return bCorrect;
};

bool ClsQuiz::multipleChoiceAnswerSelected(int iQuestionNo, int iAnswerNo){
	bool bSelected;
	
	if (iQuestionNo >= 0 and iQuestionNo < this->vecQuestions.size()) {
		if (iAnswerNo >= 0 and iAnswerNo < this->vecQuestions[iQuestionNo].vecChoices.size()) {
			bSelected = this->vecQuestions[iQuestionNo].vecChoices[iAnswerNo].bIsSelected;
		} else {
			bSelected = false;
		};
	} else {
		bSelected = false;
	};
	
	return bSelected;
};

void ClsQuiz::multipleChoiceAnswerSelect(int iQuestionNo, int iChoiceNo, bool bSelect){
	if (iQuestionNo >= 0 and iQuestionNo < this->vecQuestions.size()) {
		if (iChoiceNo >= 0 and iChoiceNo < this->vecQuestions[iQuestionNo].vecChoices.size()) {
			this->vecQuestions[iQuestionNo].vecChoices[iChoiceNo].bIsSelected = bSelect;
			this->updateQuestionChoices(iQuestionNo, iChoiceNo);
		};
	};
};


bool ClsQuiz::multipleChoiceInOnlyOneAnswer(int iQuestionNo){
	if (iQuestionNo >= 0 and iQuestionNo <= vecQuestions.size()) {
		bool bTemp;
		
		bTemp = vecQuestions[iQuestionNo].bOnlyOneAnswer;
		
		return bTemp;
	} else {
		return false;
	};
	
};

#endif