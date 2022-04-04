/*
 *  ClsQuiz.h
 *  
 *
 *  Created by Matthew Baynham on 18/06/2010.
 *  Copyright 2010 Baynham Coding. All rights reserved.
 *
 */

#ifndef CLS_QUIZ_H
#define CLS_QUIZ_H

#include <string>;
#include <cstddef>//std array rapper
#include <vector>
//#include <cstdlib>;
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "modMisc.h"
#include "ClsPreferences.h"
#include "ClsPrimeNumbers.h"
//#include "clsQuestion.h"
//#include "clsMySQLite.h"
//#include "ClsDir.h"

using namespace std;
using namespace modMisc;

class ClsQuiz //: clsQuestion
{
public:
//	ClsQuiz(std::string*);
	ClsQuiz();
	~ClsQuiz();
	void setPath(std::string);
	std::string getPath();
	void setPreferences(ClsPreferences*);
	int iQuestionCurrent();
	void iQuestionCurrent(int);
	int iQuestionMax();
	std::string sQuestionText();
	std::string sQuestionText(int);
	std::string sQuestionTextEncoded();
	std::string sQuestionTextEncoded(int);
	int iQuestionValuesCount(int);
	float fQuestionValue(int, int);
	bool bQuestionIsRight();
	std::string sQuestionGroup(int);
	std::string sQuestionSubject(int);
	
	void loadQuiz();
	void emptyQuiz();
	void loadHeader();
	void loadQuestions();//if the quiz is being reopenned then this opens the db file and loads all the questions from it
//	void loadValues();
//	void loadAnswers();
//	void loadChoices();
	strQuestion loadValues(strQuestion*);
	strQuestion loadAnswers(strQuestion*);
	strQuestion loadChoices(strQuestion*);
	
	void createQuiz();
//	void addQuestion(int);
//	void addQuestion(int, enumQuestionType);
	void addQuestion(ClsPreferences*);
	int getNextQuestionId();

//	void addQuestions(int, ClsPreferences*);
//	enumQuestionType randomQuestionType();
	strQuestion randomQuestionType();
	strQuestion randomQuestionTypeRaw(enumQuestionGroup, enumQuestionType);
	
	//Arithmetic
	strQuestion randomQuestionType_AddingTwoNumbers();
	strQuestion randomQuestionType_SubtractTwoNumbers();
	strQuestion randomQuestionType_MultiplyTwoNumbers();
	strQuestion randomQuestionType_DivideTwoNumbers();
	strQuestion randomQuestionType_Square();
	strQuestion randomQuestionType_Cubed();
	strQuestion randomQuestionType_SquareRoot();
	strQuestion randomQuestionType_CubedRoot();
	
	//Shapes
	strQuestion randomQuestionType_Pythagorean();
    strQuestion randomQuestionType_Trigonometry();
	strQuestion randomQuestionType_parallogram();
	strQuestion randomQuestionType_Cylinder();
	
	//
	strQuestion randomQuestionType_Differentiation();
	strQuestion randomQuestionType_Polynomial();
	strQuestion randomQuestionType_ArithmeticMisc3Figures();
	
	//Statistics
	strQuestion randomQuestionType_AverageMean();
	strQuestion randomQuestionType_AverageMedian();
	strQuestion randomQuestionType_AverageMode();
	
	//Fractions
	strQuestion randomQuestionType_FractionsAddSub();
	strQuestion randomQuestionType_FractionsMulitpleDivide();
	
	//Equations
	strQuestion randomQuestionType_EquationsQuadratic();
    strQuestion randomQuestionType_EquationsSimultaneousLinear();
	
	bool QuestionsSame(strQuestion, strQuestion);
	std::string sQuestionQuestion(int);
	void setGivenAnswer(std::string); 
	void setGivenAnswer(std::string, int); 
	std::string getGivenAnswer(int); 
	bool questionIsMultipleChoice(int);
	bool questionIsPicture(int);
	std::string picName(int);
	enumPicQues picType(int);
	std::string multipleChoiceAnswerText(int, int);
	bool multipleChoiceAnswerCorrect(int, int);
	bool multipleChoiceAnswerSelected(int, int);
	void multipleChoiceAnswerSelect(int, int, bool);
	bool multipleChoiceInOnlyOneAnswer(int);
	std::string headerValue(std::string);//if there is only one value for that header name
	std::string headerValue(std::string, int); //if there are many header values for that header name return the Nth
	int headerValueCount(std::string); //if there are many header values for that header name return a count of them
	void mark();
	bool isAnswerCorrectMultipleChoice(int);
	bool isAnswerCorrectText(int);

////////////////////////////////////////////////////////////////////////////////////////////////////////
//first sort out the structure of the quiz and questions before we start saving anything to files	
	void insertQuestionAll(int);
	void insertHeader(int);
	void insertHeader(std::string, std::string);
	void insertHeader(std::string, std::string, std::string);
	void insertQuestion(int);
	void insertQuestionValue(int, int);
	void insertQuestionAnswersWanted(int, int, float);
	void insertQuestionAnswersGiven(int, int, float);
	void insertQuestionAnswer(int, int, float);
	void insertQuestionChoice(int, int);
	
	void updateHeader(std::string, std::string);
	void updateHeader(std::string, std::string, std::string);
	void updateHeaderValue2(std::string, std::string, std::string);
	void updateQuestion(int); //just update the question without updating the vectors in the question
	void updateQuestionValue(int, int, float); //Values related to the question
	void updateQuestionChoices(int, int);
	void updateQuestionAnswersWanted(int, int, float); //the correct answers to the questions
	void updateQuestionAnswersGiven(int, int, float); //the answers entered 
	void updateQuestionAnswer(int, int, float); //???? might not keep
	
	void selectHeader();
	void selectQuestion(int); //select as in the SQL phrase meaning read the contence of the SQLite file
	void selectQuestionValue(int, int); //select as in the SQL phrase meaning read the contence of the SQLite file
	void selectQuestionAnswersWanted(int, int); //select as in the SQL phrase meaning read the contence of the SQLite file
	void selectQuestionAnswersGiven(int, int); //select as in the SQL phrase meaning read the contence of the SQLite file
	void saveQuestion(); //decide whether to insert or update the question and all the values and answers
	void saveQuiz(std::string); //initial save but also with each action the information is saved as we go
//	void loadQuiz(std::string);
//	void saveAnswer
	void createEmptyQuizTables();
	int secondSpent();

	void changeHeader(std::string, std::string); //don't know if we should insert or update
	void changeHeader(std::string, std::string, std::string); //don't know if we should insert or update
	
	
private:
//	strQuizDetails* oQuizDetails;
	int* iQuizId;
//	std::string sTestString;
//	std::string *sFilePath;
	std::string sFilePath;
//	clsMySQLite *cMySQLite;
	int ilQuestionCurrent;
	int ilQuestionMax;
//	long iRandomNumber(int, int);
//	long iRandomNumberNonZero(int, int);
	vector<strHeader> vecHeader;
	vector<strQuestion> vecQuestions;
//	vector<strInterests> vecInterests;
	std::string decodeText(string);
	enumFnTrig generateRandomFnTrig(bool);
	void generateQuestion(int);
	void generateQuestion(int, enumQuestionType);
	void generateNewQuizFilePath();
	ClsPreferences* cPreferences;
	ClsPrimeNumbers* cPrimeNumbers;
	sqlite3 *db;
	bool bDbIsOpen;
//	time_t tmTimestarted;

//	int rc;

//	enumQuestionType randomQuestionType();

};
#endif