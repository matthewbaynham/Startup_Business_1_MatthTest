/*
 *  modMisc.h
 *  Command Line C++ App
 *
 *  Created by Matthew Baynham on 02/07/2010.
 *  Copyright 2010 Baynham Coding. All rights reserved.
 *
 */
#include <string>;
#include <cstddef>//std array rapper
#include <vector>;
//#include <cstdlib>;
#include <sqlite3.h>;
#include <stdlib.h>;
#include <time.h>
#include <sstream>
#include <math.h>

#ifndef MOD_MISC_H
#define MOD_MISC_H

using namespace std;

enum enumPreferenceType {
	ePrefSimple,
	ePrefBySubject,
	ePrefByGroup
};

enum enumQuestionType {
	eQuesAddingNumbers,
	eQuesAddingNumbersEasy,
	eQuesAddingNumbersMedium,
	eQuesAddingNumbersHard,
	
	eQuesSubtractingNumbers,
	eQuesSubtractingNumbersEasy,
	eQuesSubtractingNumbersMedium,
	eQuesSubtractingNumbersHard,
	
	eQuesMultiplingNumbers,
	eQuesMultiplingNumbersEasy,
	eQuesMultiplingNumbersMedium,
	eQuesMultiplingNumbersHard,
	
	eQuesDividingNumbers,
	eQuesDividingNumbersEasy,
	eQuesDividingNumbersMedium,
	eQuesDividingNumbersHard,
	
//	eQuesDifferentiation,
	eQuesDifferentiation2BasePoly,
	eQuesDifferentiation2BasePolyVeryEasy,
	eQuesDifferentiation2BasePolyEasy,
	eQuesDifferentiation2BasePolyMedium,
	eQuesDifferentiation2BasePolyHard,
	eQuesDifferentiation2BasePolyVeryHard,

	eQuesDifferentiation3BasePoly,

	eQuesDifferentiationExpFn,
	eQuesDifferentiationTrigFn,
	eQuesDifferentiationChainRule,
	eQuesDifferentiationEverything, 
	eQuesError,
	
	eQuesTrig,
	eQuesTrigFromHypAdjCalcOpp,
	eQuesTrigFromHypOppCalcAdj,
	eQuesTrigFromOppAdjCalcHyp,
	
	eQuesSquare,
	eQuesSquare1Digit,
	eQuesSquare2Digit,
	eQuesSquare3Digit,
	
	eQuesCubed,
	eQuesCubed1Digit,
	eQuesCubed2Digit,
	eQuesCubed3Digit,

	eQuesSquareRoot,
	eQuesSquareRoot1Digit,
	eQuesSquareRoot2Digit,
	eQuesSquareRoot3Digit,

	eQuesCubedRoot,
	eQuesCubedRoot1Digit,
	eQuesCubedRoot2Digit,
	eQuesCubedRoot3Digit,
	
	eQuesArithmetic,
	eQuesArithmeticEasy,
	eQuesArithmeticMedium,
	eQuesArithmeticHard,
	
	eQuesPolynomial,
	eQuesPolynomialEasy,
	eQuesPolynomialMedium,
	eQuesPolynomialHard,
	    
	eQuesPythagorean,
	eQuesPythagoreanVeryEasy,
	eQuesPythagoreanEasy,
	eQuesPythagoreanMedium,
	eQuesPythagoreanHard,
	eQuesPythagoreanVeryHard,
	
    eQuesTrigonometry,
    eQuesTrigonometryVeryEasy,
    eQuesTrigonometryEasy,
    eQuesTrigonometryMedium,
    eQuesTrigonometryHard,
    eQuesTrigonometryVeryHard,

	eQuesParallogram,
	eQuesParallogramVeryEasy,
	eQuesParallogramEasy,
	eQuesParallogramMedium,
	eQuesParallogramHard,
	eQuesParallogramVeryHard,
	
	eQuesCylinder,
	eQuesCylinderVeryEasy,
	eQuesCylinderEasy,
	eQuesCylinderMedium,
	eQuesCylinderHard,
	eQuesCylinderVeryHard,
	
	eQuesAverageMean,
	eQuesAverageMeanVeryEasy,
	eQuesAverageMeanEasy,
	eQuesAverageMeanMedium,
	eQuesAverageMeanHard,
	eQuesAverageMeanVeryHard,
	
	eQuesAverageMedian,
	eQuesAverageMedianVeryEasy,
	eQuesAverageMedianEasy,
	eQuesAverageMedianMedium,
	eQuesAverageMedianHard,
	eQuesAverageMedianVeryHard,
	
	eQuesAverageMode,
	eQuesAverageModeVeryEasy,
	eQuesAverageModeEasy,
	eQuesAverageModeMedium,
	eQuesAverageModeHard,
	eQuesAverageModeVeryHard,
	
	eQuesFractionsAddSub,
	eQuesFractionsAddSubVeryEasy,
	eQuesFractionsAddSubEasy,
	eQuesFractionsAddSubMedium,
	eQuesFractionsAddSubHard,
	eQuesFractionsAddSubVeryHard,
	
	eQuesFractionsMultiDiv,
	eQuesFractionsMultiDivVeryEasy,
	eQuesFractionsMultiDivEasy,
	eQuesFractionsMultiDivMedium,
	eQuesFractionsMultiDivHard,
	eQuesFractionsMultiDivVeryHard,
	
	eQuesEquationsQuadratic,
	eQuesEquationsQuadraticVeryEasy,
	eQuesEquationsQuadraticEasy,
	eQuesEquationsQuadraticMedium,
	eQuesEquationsQuadraticHard,
	eQuesEquationsQuadraticVeryHard,
	
	eQuesEquationsSimultaneousLinear,
	eQuesEquationsSimultaneousLinearVeryEasy,
	eQuesEquationsSimultaneousLinearEasy,
	eQuesEquationsSimultaneousLinearMedium,
	eQuesEquationsSimultaneousLinearHard,
	eQuesEquationsSimultaneousLinearVeryHard
	
    
    
};

enum enumQuestionGroup {
	eQuesGrpArithmeticBasics,
	eQuesGrpArithmeticAdvanced,
	eQuesGrpShapes,
	eQuesGrpFractions,
	eQuesGrpEquations,
	eQuesGrpAverages,
	
	eQuesGrpDifferentiation, 
	eQuesGrpError,
	eQuesGrpTrig,
	eQuesGrpPolynomial,
	eQuesGrpStatistics
};

enum enumFnTrig {
	eFnTrigSin,
	eFnTrigCos,
	eFnTrigTan,
	eFnTrigSinInv,//Inv = Inverse
	eFnTrigCosInv,
	eFnTrigTanInv
};

enum enumAnswerType {
	eAnsTypNumeric,
	eAnsTypEquation,
	eAnsTypEquQuadratic,
	eAnsTypEquTrig,
	eAnsTypEquLog,
	eAnsTypEquExpo,
	eAnsTypMultipleNumeric, //i.e. solve X^2 + 5X + 6 = 0 (Answer: X = 2 and 3)
	eAnsTypMutipleChoice,
	eAnsTypError
	
};

enum enumPicQues {
//	ePicQues_None,
	ePic_None,
	ePicSelector_Cyan,
	ePicBtn_Black,
	ePicBtn_BlackGreenTick,
	ePicBtn_BlackRedCross,
	ePicBtn_BlackYellowArrow,
	ePicBtn_Blue,
	ePicBtn_Cyan,
	ePicBtn_Green,
	ePicBtn_Red,
	ePicBtn_Yellow,
	ePicQues_TriangleRightAngle,
	ePicQues_TriangleNotRight,
	ePicQues_Parallelogram,
	ePicQues_Circle,
	ePicQues_Cylinder,
	ePicQues_Square,
	ePicQues_Cube	
};

enum enumQuestionCellType {//not used
	eQuCell_TextAnswer,
	eQuCell_MultipleChoice,
	eQuCell_PictureMultipleChoice,
	eQuCell_PictureTextAnswer
};

enum enumQuizGuiLayout {
	eQuGui_Menu,
	eQuGui_Advert,
	eQuGui_Question
};

struct strQuizGuiLayout {
	enumQuizGuiLayout eCellType;
	int iQuestionNo;
	int iCellPos;
};

struct strTrigNumbers{
	float fAngle;
	float fHypotenuse;
	float fAdjacent;
	float fOpposite;
};

struct strQuizDetails {
	int iId;
	std::string sUserName;
	std::string sQuizName;
	int iStartTime;
	int iEndTime;
};

struct strSubjectName {
	enumQuestionType eQuestionType; 
	std::string sName;
	bool bEnabled;//if we use the "by subject" preferences
};

struct strSubjectGroup {
	std::string sGroup;
	enumQuestionGroup eQuestionGroup;
	int iDifficulty;
	int iDiffMax;
	bool bEnabled; //if we use the "by group" preferences
	std::vector<strSubjectName> vecSubjects;
};

//Old version disable later
//struct strSubject {
//	std::string sGroup;
//	std::string sName;
//	bool bEnabled;
//};

struct strHeader{
	int iId;
	std::string sName;
	std::string sValue;
	std::string sValue2;
};

struct strFloatValue{
	int iId; //need to store the ID used by the SQLite so that we can match things up again.
	float fValue;
	bool bIsDirty;
};

struct strStringAnswer{
	int iId; //need to store the ID used by the SQLite so that we can match things up again.
	std::string sValue;
	bool bIsSelected;
	bool bIsCorrect;
};

struct strHelpMenu {
	std::string sMenu;
	std::string sDesc;
};

struct strHelpGroup {
	std::string sGroup;
	std::string sDesc;
};

struct strHelpSubject {
	std::string sGroup;
	std::string sSubject;
	std::string sDesc;
};


struct strQuestion {
	int iId;
	int iSqlite_Id;
	enumQuestionType eQuestionType; 
	enumQuestionGroup eQuestionGroup;
	std::string sQuestionEncoded; //the values that need to be substuted into the question are encode in tags <0> = value 1 from array
//	std::string sDecodeText(std::string);
	std::string sDecodeText;//not sure if we need this
	std::string sQuestionText;//text typed in by user
	std::string sGivenAnswer; //the answer exactly how it has been type in.
	int iGivenAnswerMultipleChoice; //selected answer
	int iDifficulty;//difficulty
	bool bCorrect;// is correct or not
	std::vector<strFloatValue> vecValues; //values that go into generating the question
	std::vector<strFloatValue> vecAnswers; //all the given answers
	std::vector<enumFnTrig> eFnTrig;//if the question is a trig one then these are the trig funtions that are in the equation
	std::vector<strStringAnswer> vecChoices;//if the question is multiple choice these are the choices
//	int iCorrectChoice; each of the answers has a boolean attached to flag correct, because sometimes there can be more then one correct answer
	enumAnswerType eAnswerType;//is the answer a number or a function or what, so that we can process the answer given in different ways.
	bool bIsMultipleChoice;//is multiple choice or not
	bool bOnlyOneAnswer;//some questions have multiple correct answers
	bool bIsDirty; //true = question contence has changed sinse last saved to the file.
	enumPicQues ePic;//which pic to display
	bool bHasPic;//question cell has picture 
};

struct strInterests {
	enumQuestionType sQuestionType;
	int iDifficulty;
};

namespace modMisc {
	int addingNumbers(int, int);
	bool isSubStringInString(std::string, std::string); 
	std::string decodeText(std::string, std::vector<strFloatValue>);
	std::string replaceAll(std::string, std::string, std::string);
	std::string currentDateTime();
	long iRandomNumber(int, int);
	long iRandomNumberNonZero(int, int);
	strTrigNumbers friendlyTrigNumbers(int);
	const int iDifficultyMax = 1000;
	const std::string sVersion = "1.2";
	const std::string sFolderResults = "Results";
	const std::string sFileNamePreferences = "Preferences.db";
	const std::string sFileNameHelp = "help.db";
	const std::string sHeader_QuestionCount = "Question Count";
	const std::string sHeader_QuestionTotal = "Question Total";
	const std::string sHeader_QuizCreatedDate = "Quiz Created Date";
	const std::string sHeader_QuizStartDateTime = "Quiz Start Date Time";
	const std::string sHeader_QuizEndDateTime = "Quiz End Date Time";
	const std::string sHeader_QuizDateTimeSplit = "Quiz Date Time Split";
	const std::string sHeader_QuizStatus = "Quiz Status";
	const std::string sHeader_AppVersion = "App Version";
	const std::string sHeader_GeneratedBy = "Generated By";//Random or a lesson or assigned
	const wchar_t cMathChar_Infinite = 151;
	const int iCutOffTime = 30;
	const int iQuestionGroupMax = 7;
	bool stringEquals(std::string, std::string, bool, bool, bool);
	bool Sqlite_TableDB();
	bool Sqlite_TableExists(sqlite3, std::string);
	bool Sqlite_Populated();
	bool FileExists(std::string);
	bool MultipleChoiceAnswerFound(vector<strStringAnswer>, std::string);
	std::string numberInWords(int);
	std::string numberInWordsIst(int);
	std::string symbolsInWords(std::string);
	std::string convertSecondsIntoTimeString(int);
	
	std::string convertEnumText_QuestionType(enumQuestionType);
	enumQuestionType convertEnumText_QuestionType(std::string);

	std::string convertEnumText_AnswerType(enumAnswerType);
	enumAnswerType convertEnumText_AnswerType(std::string);

	std::string convertEnum_QuesTypeGroupText(enumQuestionGroup);//???
	enumQuestionGroup convertEnum_QuesTypeGroupText(std::string);//???

	std::string convertEnum_QuesTypeSubjectText(enumQuestionType);
	enumQuestionType convertEnum_QuesTypeSubjectText(std::string);
	
	std::string convertEnumText_Pic(enumPicQues);
	enumPicQues convertEnumText_Pic(std::string);

	
	bool questionTypeIsEqual(enumQuestionType, enumQuestionType);

	void sqliteFinalizeRapper(sqlite3_stmt*);
	void sqliteStmtStatusRest(sqlite3_stmt*);

    bool almostEquals(float, float, float);
}


#endif