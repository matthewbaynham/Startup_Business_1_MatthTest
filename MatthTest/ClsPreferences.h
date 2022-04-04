/*
 *  Preferences.h
 *  Command Line C++ App
 *
 *  Created by Matthew Baynham on 29/08/2010.
 *  Copyright 2010 Baynham Coding. All rights reserved.
 *
 */

#ifndef CLS_PREFERENCES_H
#define CLS_PREFERENCES_H
#include <vector>
#include <string>
#include "modMisc.h"

using namespace std;
using namespace modMisc;

//const std::string gcsFullPath = "/Code/xCode/Hello World Table View/Hello World Table View/Preferences.db";

class ClsPreferences {
private:
	std::vector<strSubjectGroup> vecSubAll;
	std::string sPathPreference;
	float fDiff;
	enumPreferenceType ePrefType;
	sqlite3 *db;
	bool bDbIsOpen;

	
public:
	ClsPreferences();
	~ClsPreferences();
	void setPath(std::string);
	void changePath(std::string*);
	std::string* getPath();
	
	void setTypeSimple();
	void setTypeBySubject();
	void setTypeByGroup();
	void setType(enumPreferenceType);
	enumPreferenceType getType();
		
	void setDifficulty(float);
	void setDifficulty(float, std::string*);
	float getDifficulty();
	float getDifficulty(std::string*);
	float getSubjectGroupDiffMax(enumQuestionGroup);
	
	void createDifficultyGroup(std::string*);

	void setGroupEnabled(bool, std::string*);
	bool getGroupEnabled(std::string*);
	enumQuestionGroup getGroupEnum(std::string*);
	
	void setNoOfQuestions(int);
	float getNoOfQuestions();
	void setSubjectEnabled(std::string*, std::string*, bool);
	void setSubjectEnabled(std::string*, bool);
	bool getSubjectEnabled(std::string*, std::string*);
	bool getSubjectEnabled(std::string*);
	void CheckPreferenceFile();
    void FillDefaultPreferences();
	void ResetPreferences();
    void CheckPreferencesSubjectGroups();
    void PreferencesInsertGroup(strSubjectGroup);
    void PreferencesInsertSubject(std::string, strSubjectName);
	void dropAllTables();
	void load_subjects();
	int countGroups();
	int countNames(std::string);
	int countNames(int);
	std::string getSubjectGroup(int);
	std::string getSubjectName(int, int);

	enumQuestionGroup getRandomQuestionGroup();
	enumQuestionType getRandomQuestionType(enumQuestionGroup);
//	enumQuestionType getRandomQuestionType();
	bool isQuestionPossible();

	bool isQuestionOk(strQuestion);
	bool isQuestionOk(enumQuestionType, enumQuestionGroup);
	bool isQuestionOkBySubject(enumQuestionType, enumQuestionGroup);
	bool isQuestionOkByGroup(enumQuestionType, enumQuestionGroup);
//	bool questionOK_GroupLevel(enumQuestionType, enumQuestionGroup);
//	bool questionOK_SubjectLevel(enumQuestionType, enumQuestionGroup);

//	bool questionTypeOK(enumQuestionType, enumQuestionGroup);
	void CheckEnabledTblUnique();
	
};

#endif