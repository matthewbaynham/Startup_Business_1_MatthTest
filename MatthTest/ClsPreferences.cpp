/*  Preferences.cpp
 *  Command Line C++ App
 *
 *  Created by Matthew Baynham on 29/08/2010.
 *  Copyright 2010 Baynham Coding. All rights reserved.
 */

#include <iostream>
#include <string>
#include <cstddef>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdio.h>
#import <sqlite3.h>
#include <algorithm>
#include <sys/stat.h>

#include "modMisc.h"
#include "ClsPreferences.h"

#ifndef CLS_PREFERENCES_CPP
#define CLS_PREFERENCES_CPP

using namespace std;
using namespace modMisc;

ClsPreferences::ClsPreferences(){
	int rc;
	
	try {
//		if (bDbIsOpen != false) { 
//			rc = sqlite3_close(db);
//		};
		bDbIsOpen = false;
	} catch (...) {
		rc = sqlite3_close(db);
		bDbIsOpen = false;
	};
//	delete rc;
};	

ClsPreferences::~ClsPreferences(){
	int rc;
	
	try {
		std::vector<strSubjectGroup>::iterator itGroup;
		std::vector<strSubjectName>::iterator itSubjects;		

		for (itGroup = this->vecSubAll.begin(); itGroup < this->vecSubAll.end(); itGroup++) {
			for (itSubjects = itGroup->vecSubjects.begin(); itSubjects < itGroup->vecSubjects.end(); itSubjects++) {
				itSubjects->sName.clear();
			};
			itGroup->sGroup.clear();
			itGroup->vecSubjects.clear();
		};
		
		this->sPathPreference.clear();
		this->vecSubAll.clear();

		rc = sqlite3_close(db);
		bDbIsOpen = false;
	} catch (...) {
		rc = sqlite3_close(db);
		bDbIsOpen = false;
	};
	
//	delete rc;
};

void ClsPreferences::setPath(std::string sHomePath){
//	char *cPath;
	
	//if last char is not / then add / between directory and filename
	size_t iPos;
	std::string sLastChar;
	
	iPos = sHomePath.size() - 1;
	sLastChar = sHomePath.substr(iPos, 1);
	
	if (sLastChar.compare("/") == 0) {
		char cTemp[(sHomePath.size() + modMisc::sFileNamePreferences.size() + 2)];
		
		strcpy (cTemp,sHomePath.c_str());
		strcat (cTemp,modMisc::sFileNamePreferences.c_str());
		
		std::string sTempPath = (std::string)cTemp;

		this->sPathPreference = sTempPath;

		sTempPath.clear();
//		cTemp = NULL;
	} else {
		char cTemp[(sHomePath.size() + modMisc::sFileNamePreferences.size() + 1)];
		
		strcpy (cTemp,sHomePath.c_str());
		strcat (cTemp,"/");
		strcat (cTemp,modMisc::sFileNamePreferences.c_str());

		sPathPreference.clear();

		std::string sTempPath;
		sTempPath =  (std::string)cTemp;
		
		sPathPreference = sTempPath;
		
		sTempPath.clear();
//		cTemp = NULL;
	};

	sLastChar.clear();
	
//	cPath = (char*)this->sPathPreference.c_str();
//	delete iPos;
//	cPath = NULL;
//	delete cPath;
//	delete bIsOk;
//	delete rc;
	
//	this->CheckPreferenceFile();
	this->load_subjects();
};


void ClsPreferences::changePath(std::string* sPathNew){
	//take all the data in this class and save it in a new file location 
	//so that in can bacome a quiz file and contain all the questions etc...
	
	
	
	
}

std::string* ClsPreferences::getPath(){
	return &(sPathPreference);
};

void ClsPreferences::setTypeSimple(){
	ePrefType = ePrefSimple;
};

void ClsPreferences::setTypeBySubject(){
	ePrefType = ePrefBySubject;
};

void ClsPreferences::setTypeByGroup(){
	ePrefType = ePrefByGroup;
};

void ClsPreferences::setType(enumPreferenceType eType){
	ePrefType = eType;
};

enumPreferenceType ClsPreferences::getType(){
	return ePrefType;
};



void ClsPreferences::setNoOfQuestions(int iNoOfQuestions){
	int rc;
	
	try {
		char *zErr;
		std::string sErrMsg;
		bool bIsOk;
		const char *cSqlUpdateTbl_Diff("update tblNoOfQuestions set fldNoOfQuestions = :iNoOfQuestions ;");
		sqlite3_stmt *stmt_Diff;
		int iParameterPosition;
		char *cPath;
		
		bIsOk = true;
		rc = SQLITE_OK;
		
		if (bDbIsOpen != true){
			cPath = (char*)this->sPathPreference.c_str();
			
			//open database connection
			rc = sqlite3_open(cPath, &db);
			bDbIsOpen = true;
		};
		
		
		if(rc != SQLITE_OK){
#if TEST_ENV == true
			sErrMsg = (std::string)"Can't open DB: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
			fprintf(stderr, sErrMsg.c_str(), 0);
#endif
			bIsOk = false;
			bDbIsOpen = false;
		};

		//Populate Difficulty table with default values	
		if(bIsOk) {
			rc = sqlite3_prepare_v2(db, cSqlUpdateTbl_Diff, -1, &stmt_Diff, NULL);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't Prepare SQL: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt_Diff, ":iNoOfQuestions");
			
			rc = sqlite3_bind_int(stmt_Diff, 
								  iParameterPosition,
								  (int)iNoOfQuestions);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
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
		
		if(bIsOk){
			rc = sqlite3_reset(stmt_Diff);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				fprintf(stderr, "Can't reset statment: %\n", sqlite3_errmsg(db));
#endif
				bIsOk = false;
			};
		};
		
		zErr = NULL;
		sErrMsg.clear();
		cSqlUpdateTbl_Diff = NULL;
		if (bIsOk) {
			modMisc::sqliteFinalizeRapper(stmt_Diff);
//			sqlite3_finalize(stmt_Diff);
		};
//		delete stmt_Diff;
		cPath = NULL;
	} catch (...) {
		rc = sqlite3_close(db);
		bDbIsOpen = false;
	};
};

float ClsPreferences::getNoOfQuestions(){
	int rc;
	
	try {
		const char *cSql("select fldNoOfQuestions from tblNoOfQuestions where fldId = (select max(fldId) from tblNoOfQuestions);");
		bool bIsOk;
		sqlite3_stmt *stmt;
		std::string sErrMsg;
		char *cPath;
		int iResult;
		int iResultCounter;

		bIsOk = true;
		rc = SQLITE_OK;
		
//		int iDbStatus;
//		iDbStatus = sqlite3_db_status(db, int op, int *pCur, int *pHiwtr, int resetFlg);

		
//		if (bDbIsOpen != true){
			cPath = (char*)this->sPathPreference.c_str();
//			printf(this->sPathPreference.c_str());
			
			//open database connection
			rc = sqlite3_open(cPath, &db);
			bDbIsOpen = true;
//		};
		
		if(rc != SQLITE_OK){
#if TEST_ENV == true
			sErrMsg = (std::string)"Can't open DB: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
			printf(sErrMsg.c_str());
#endif
			bIsOk = false;
			bDbIsOpen = false;
		};

		
		
		if(bIsOk) {
			rc = sqlite3_prepare_v2(db, cSql, -1, &stmt, NULL);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't Prepare SQL: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		iResultCounter = 0;
		if(bIsOk){
			while(sqlite3_step(stmt) == SQLITE_ROW){
				iResult = sqlite3_column_int(stmt, 0);
				iResultCounter++;
			};
		};
		
		if(bIsOk){
			rc = sqlite3_reset(stmt);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				fprintf(stderr, "Can't reset statment: %\n", sqlite3_errmsg(db));
#endif
				bIsOk = false;
			};
		};
		
		
		cSql = NULL;
		if (bIsOk) {
			modMisc::sqliteFinalizeRapper(stmt);
//			sqlite3_finalize(stmt);
		};
		
		sErrMsg.clear();
		cPath = NULL;

		if(iResultCounter==1){
			return iResult;
		}
		else{
			return -1;
		};

	} catch (...) {
		rc = sqlite3_close(db);
		bDbIsOpen = false;
	};
};



void ClsPreferences::CheckPreferenceFile(){
	bool bIsOk;
	struct stat stFileInfo;
	int intStat;
	
	bIsOk = true;

	// Attempt to get the file attributes
//	intStat = stat(gcsFullPath.c_str(),&stFileInfo);
	const char* cPath = this->sPathPreference.c_str();
	
	intStat = stat(cPath,&stFileInfo);
//	intStat = stat(this->sPathPreference.c_str(),&stFileInfo);

	if(intStat == 0) {
		// We were able to get the file attributes
		// so the file obviously exists.
		bIsOk = true;
	} else {
		// We were not able to get the file attributes.
		// This may mean that we don't have permission to
		// access the folder which contains this file. If you
		// need to do that level of checking, lookup the
		// return values of stat which will give you
		// more details on why stat failed.
		bIsOk = false;
	};
		
	
	if (not bIsOk) {
		//the previous code with the stat() fn doesn't work on a iPhone
		//this->ResetPreferences();
	};
	
	delete cPath;
};

void ClsPreferences::FillDefaultPreferences(){
    strSubjectGroup eSubjectGroup;
    strSubjectName eSubjectName;

    //Arithmetic Basics
    eSubjectGroup.sGroup = "Arithmetic Basics";
    eSubjectGroup.eQuestionGroup = modMisc::convertEnum_QuesTypeGroupText(eSubjectGroup.sGroup);
    eSubjectGroup.iDifficulty = 1;
    eSubjectGroup.iDiffMax = 3;
    eSubjectGroup.bEnabled = true;
    eSubjectGroup.vecSubjects.clear();
    
    
    eSubjectName.sName = "Adding";
    eSubjectName.eQuestionType = eQuesAddingNumbers;
    eSubjectName.bEnabled = true;
    eSubjectGroup.vecSubjects.push_back(eSubjectName);

    eSubjectName.sName = "Subtracting";
    eSubjectName.eQuestionType = eQuesSubtractingNumbers;
    eSubjectName.bEnabled = true;
    eSubjectGroup.vecSubjects.push_back(eSubjectName);

    eSubjectName.sName = "Multiplication";
    eSubjectName.eQuestionType = eQuesMultiplingNumbers;
    eSubjectName.bEnabled = true;
    eSubjectGroup.vecSubjects.push_back(eSubjectName);
    
    eSubjectName.sName = "Division";
    eSubjectName.eQuestionType = eQuesDividingNumbers;
    eSubjectName.bEnabled = true;
    eSubjectGroup.vecSubjects.push_back(eSubjectName);
    
    vecSubAll.push_back(eSubjectGroup);
    

    //Arithmetic Advanced
    eSubjectGroup.sGroup = "Arithmetic Advanced";
    eSubjectGroup.eQuestionGroup = modMisc::convertEnum_QuesTypeGroupText(eSubjectGroup.sGroup);
    eSubjectGroup.iDifficulty = 1;
    eSubjectGroup.iDiffMax = 3;
    eSubjectGroup.bEnabled = true;
    eSubjectGroup.vecSubjects.clear();

    eSubjectName.sName = "Squared";
    eSubjectName.eQuestionType = eQuesSquare;
    eSubjectName.bEnabled = true;
    eSubjectGroup.vecSubjects.push_back(eSubjectName);

    eSubjectName.sName = "Cubed";
    eSubjectName.eQuestionType = eQuesCubed;
    eSubjectName.bEnabled = true;
    eSubjectGroup.vecSubjects.push_back(eSubjectName);
    
    eSubjectName.sName = "Squared Root";
    eSubjectName.eQuestionType = eQuesSquareRoot;
    eSubjectName.bEnabled = true;
    eSubjectGroup.vecSubjects.push_back(eSubjectName);
    
    eSubjectName.sName = "Cubed Root";
    eSubjectName.eQuestionType = eQuesCubedRoot;
    eSubjectName.bEnabled = true;
    eSubjectGroup.vecSubjects.push_back(eSubjectName);
    
    eSubjectName.sName = "Misc";
    eSubjectName.eQuestionType = eQuesArithmetic;
    eSubjectName.bEnabled = true;
    eSubjectGroup.vecSubjects.push_back(eSubjectName);
    
    vecSubAll.push_back(eSubjectGroup);
    
    
    //Shapes
    eSubjectGroup.sGroup = "Shapes";
    eSubjectGroup.eQuestionGroup = modMisc::convertEnum_QuesTypeGroupText(eSubjectGroup.sGroup);
    eSubjectGroup.iDifficulty = 1;
    eSubjectGroup.iDiffMax = 5;
    eSubjectGroup.bEnabled = true;
    eSubjectGroup.vecSubjects.clear();
    
    eSubjectName.sName = "Parallogram";
    eSubjectName.eQuestionType = eQuesParallogram;
    eSubjectName.bEnabled = true;
    eSubjectGroup.vecSubjects.push_back(eSubjectName);
    
    eSubjectName.sName = "Cylinder";
    eSubjectName.eQuestionType = eQuesCylinder;
    eSubjectName.bEnabled = true;
    eSubjectGroup.vecSubjects.push_back(eSubjectName);
    
    eSubjectName.sName = "Triangle";
    eSubjectName.eQuestionType = eQuesPythagorean;
    eSubjectName.bEnabled = true;
    eSubjectGroup.vecSubjects.push_back(eSubjectName);
    
    eSubjectName.sName = "Trigonometry";
    eSubjectName.eQuestionType = eQuesTrigonometry;
    eSubjectName.bEnabled = true;
    eSubjectGroup.vecSubjects.push_back(eSubjectName);
    
    vecSubAll.push_back(eSubjectGroup);
    
    //Statistics
    eSubjectGroup.sGroup = "Averages";
    eSubjectGroup.eQuestionGroup = modMisc::convertEnum_QuesTypeGroupText(eSubjectGroup.sGroup);
    eSubjectGroup.iDifficulty = 1;
    eSubjectGroup.iDiffMax = 5;
    eSubjectGroup.bEnabled = true;
    eSubjectGroup.vecSubjects.clear();
    
    eSubjectName.sName = "Average Mean";
    eSubjectName.eQuestionType = eQuesAverageMean;
    eSubjectName.bEnabled = true;
    eSubjectGroup.vecSubjects.push_back(eSubjectName);
    
    eSubjectName.sName = "Average Median";
    eSubjectName.eQuestionType = eQuesAverageMedian;
    eSubjectName.bEnabled = true;
    eSubjectGroup.vecSubjects.push_back(eSubjectName);
    
    eSubjectName.sName = "Average Mode";
    eSubjectName.eQuestionType = eQuesAverageMode;
    eSubjectName.bEnabled = true;
    eSubjectGroup.vecSubjects.push_back(eSubjectName);
    
    vecSubAll.push_back(eSubjectGroup);
    
    //fractions
    eSubjectGroup.sGroup = "Fractions";
    eSubjectGroup.eQuestionGroup = modMisc::convertEnum_QuesTypeGroupText(eSubjectGroup.sGroup);
    eSubjectGroup.iDifficulty = 1;
    eSubjectGroup.iDiffMax = 5;
    eSubjectGroup.bEnabled = true;
    eSubjectGroup.vecSubjects.clear();
    
    eSubjectName.sName = "Fractions Add Subtract";
    eSubjectName.eQuestionType = eQuesFractionsAddSub;
    eSubjectName.bEnabled = true;
    eSubjectGroup.vecSubjects.push_back(eSubjectName);
    
    eSubjectName.sName = "Fractions Multiply Divide";
    eSubjectName.eQuestionType = eQuesFractionsMultiDiv;
    eSubjectName.bEnabled = true;
    eSubjectGroup.vecSubjects.push_back(eSubjectName);
    
    vecSubAll.push_back(eSubjectGroup);
    
    //equations
    eSubjectGroup.sGroup = "Equations";
    eSubjectGroup.eQuestionGroup = modMisc::convertEnum_QuesTypeGroupText(eSubjectGroup.sGroup);
    eSubjectGroup.iDifficulty = 1;
    eSubjectGroup.iDiffMax = 5;
    eSubjectGroup.bEnabled = true;
    eSubjectGroup.vecSubjects.clear();
    
    eSubjectName.sName = "Quadratic Equations";
    eSubjectName.eQuestionType = eQuesEquationsQuadratic;
    eSubjectName.bEnabled = true;
    eSubjectGroup.vecSubjects.push_back(eSubjectName);
    
    eSubjectName.sName = "Simultaneous Linear Equations";
    eSubjectName.eQuestionType = eQuesEquationsSimultaneousLinear;
    eSubjectName.bEnabled = true;
    eSubjectGroup.vecSubjects.push_back(eSubjectName);
    
    vecSubAll.push_back(eSubjectGroup);
}

void ClsPreferences::ResetPreferences(){
	int rc;
	
	try {
		char *zErr;
		std::string sErrMsg;
		bool bIsOk;

		const char *cSqlDropTbl_Header("drop table if exists tblHeader ;");
		const char *cSqlDropTbl_Diff("drop table if exists tblDifficulty ;");
		const char *cSqlDropTbl_NoOfQuestions("drop table if exists tblNoOfQuestions ;");
		const char *cSqlDropTbl_EnabledGroup("drop table if exists tblEnabledGroup ;");
		const char *cSqlDropTbl_EnabledSubject("drop table if exists tblEnabledSubject ;");
		
		const char *cSqlCreateTbl_Header("create table tblHeader (fldId integer primary key, fldName varchar(255), fldValue varchar(255), fldValue2 varchar(255));");
		const char *cSqlCreateTbl_Diff("create table tblDifficulty (fldId integer primary key, fldDifficulty double, fldGroup varchar(255));");
		const char *cSqlCreateTbl_NoOfQuestions("create table tblNoOfQuestions (fldId integer primary key, fldNoOfQuestions int);");
		const char *cSqlCreateTbl_EnabledGroup("create table tblEnabledGroup (fldId integer primary key, fldGroup varchar(255), fldEnabled integer, fldDifficulty integer);");
		const char *cSqlCreateTbl_EnabledSubject("create table tblEnabledSubject (fldId integer primary key, fldGroup varchar(255), fldName varchar(255), fldEnabled integer);");

		const char *cSqlInsertTbl_Header("insert into tblHeader (fldName, fldValue) values (:sName, :sValue);");
		const char *cSqlInsertTbl_Diff("insert into tblDifficulty (fldDifficulty) values (:dDifficulty);");
		const char *cSqlInsertTbl_NoOfQuestions("insert into tblNoOfQuestions (fldNoOfQuestions) values (:iNoOfQuestions);");
		const char *cSqlInsertTbl_EnabledGroup("insert into tblEnabledGroup (fldGroup, fldEnabled, fldDifficulty) values (:sGroup, :bEnabled, :iDifficulty );");
		const char *cSqlInsertTbl_EnabledSubject("insert into tblEnabledSubject (fldGroup, fldName, fldEnabled) values (:sGroup, :sName, :bEnabled );");
//		sqlite3_stmt *stmt_Header;
		sqlite3_stmt *stmt_Diff;
		sqlite3_stmt *stmt_NoOfQuestions;
		sqlite3_stmt *stmt_EnabledGroup;
		sqlite3_stmt *stmt_EnabledSubject;
		int iParameterPosition;
		std::string sName;
		char *cPath;
		std::vector<strSubjectGroup>::iterator itGroup;
		std::vector<strSubjectName>::iterator itName;
		float fDifficulty;
		int iNoOfQuestions;
		
		dropAllTables();
//		this->vecSubAll = {};
		this->vecSubAll.clear();
		
		FillDefaultPreferences();
		
		
		fDifficulty = 200.0f;
		iNoOfQuestions = 10;
		
		rc = sqlite3_close(db);
		bDbIsOpen = false;
		
		bIsOk = true;
		rc = SQLITE_OK;
		
		if (bDbIsOpen != true){
			cPath = (char*)this->sPathPreference.c_str();
		};

		remove(cPath);
	
		if (bDbIsOpen != true){
			//open database connection
			rc = sqlite3_open(cPath, &db);
			bDbIsOpen = true;
		};
		
		if(rc != SQLITE_OK){
#if TEST_ENV == true
			sErrMsg = (std::string)"Can't open DB: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
			fprintf(stderr, sErrMsg.c_str(), 0);
#endif
			bIsOk = false;
			bDbIsOpen = false;
		};
		
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		
		//create difficulty table to store the level of difficulty
		if(bIsOk) {
			rc = sqlite3_exec(db, cSqlDropTbl_Diff, NULL, NULL, &zErr);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't open DB: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//create No Of Questions table to store the number of questions
		if(bIsOk) {
			rc = sqlite3_exec(db, cSqlDropTbl_NoOfQuestions, NULL, NULL, &zErr);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't open DB: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//create enabled table to store which subjects are enabled Group
		if(bIsOk) {
			rc = sqlite3_exec(db, cSqlDropTbl_EnabledGroup, NULL, NULL, &zErr);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't open DB: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//create enabled table to store which subjects are enabled Subject
		if(bIsOk) {
			rc = sqlite3_exec(db, cSqlDropTbl_EnabledSubject, NULL, NULL, &zErr);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't open DB: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		
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
		
		//create No Of Questions table to store the number of questions
		if(bIsOk) {
			rc = sqlite3_exec(db, cSqlCreateTbl_NoOfQuestions, NULL, NULL, &zErr);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't open DB: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//create enabled table to store which subjects are enabled Group
		if(bIsOk) {
			rc = sqlite3_exec(db, cSqlCreateTbl_EnabledGroup, NULL, NULL, &zErr);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't open DB: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		//create enabled table to store which subjects are enabled Subject
		if(bIsOk) {
			rc = sqlite3_exec(db, cSqlCreateTbl_EnabledSubject, NULL, NULL, &zErr);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't open DB: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
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
		
		if(bIsOk){
			rc = sqlite3_reset(stmt_Diff);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				fprintf(stderr, "Can't reset statment: %\n", sqlite3_errmsg(db));
#endif
				bIsOk = false;
			};
		};//if bIsOk
		
		
	//Populate No of Questions table with default values	
		if(bIsOk) {
			rc = sqlite3_prepare_v2(db, cSqlInsertTbl_NoOfQuestions, -1, &stmt_NoOfQuestions, NULL);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't Prepare SQL: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt_NoOfQuestions, ":iNoOfQuestions");
			
			rc = sqlite3_bind_int(stmt_NoOfQuestions, 
								  iParameterPosition,
								  (int)iNoOfQuestions);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add int parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		if(bIsOk){
			rc = sqlite3_step(stmt_NoOfQuestions);
			
			if(rc == SQLITE_ERROR){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't Step statment: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		if(bIsOk){
			rc = sqlite3_reset(stmt_NoOfQuestions);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				fprintf(stderr, "Can't reset statment: %\n", sqlite3_errmsg(db));
#endif
				bIsOk = false;
			};
		};//if bIsOk
		
	//Populate Enabled tables with default values	
		
		for(itGroup=vecSubAll.begin(); itGroup < vecSubAll.end(); itGroup++){
			//enabled group level
			
			if(bIsOk) {
				rc = sqlite3_prepare_v2(db, cSqlInsertTbl_EnabledGroup, -1, &stmt_EnabledGroup, NULL);
				
				if(rc != SQLITE_OK){
#if TEST_ENV == true
					sErrMsg = (std::string)"Can't Prepare SQL: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
					printf(sErrMsg.c_str());
#endif
					bIsOk = false;
				};
			};//if bIsOk
			
			if(bIsOk){
				iParameterPosition = sqlite3_bind_parameter_index(stmt_EnabledGroup, ":sGroup");
				
				rc = sqlite3_bind_text(stmt_EnabledGroup, 
									   iParameterPosition,
									   itGroup->sGroup.c_str(), 
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
				iParameterPosition = sqlite3_bind_parameter_index(stmt_EnabledGroup, ":iDifficulty");
				
				rc = sqlite3_bind_int(stmt_EnabledGroup, 
									  iParameterPosition,
									  1);
				
				if(rc != SQLITE_OK){
#if TEST_ENV == true
					sErrMsg = (std::string)"Can't add string parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
					printf(sErrMsg.c_str());
#endif
					bIsOk = false;
				};
			};//if bIsOk
			
			if(bIsOk){
				iParameterPosition = sqlite3_bind_parameter_index(stmt_EnabledGroup, ":bEnabled");
				
				if(itGroup->bEnabled){
					rc = sqlite3_bind_int(stmt_EnabledGroup, 
										  iParameterPosition,
										  1); // 1 = true (enabled); 0 = false (disabled)
				}
				else
				{
					rc = sqlite3_bind_int(stmt_EnabledGroup, 
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
				rc = sqlite3_step(stmt_EnabledGroup);
				
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
				rc = sqlite3_reset(stmt_EnabledGroup);
				
				if(rc != SQLITE_OK){
#if TEST_ENV == true
					fprintf(stderr, "Can't reset statment: %\n", sqlite3_errmsg(db));
#endif
					bIsOk = false;
				};
			};//if bIsOk

			
			
			
			
			
			
			for(itName=itGroup->vecSubjects.begin(); itName < itGroup->vecSubjects.end(); itName++){
				//enabled subject level
				if(bIsOk) {
					rc = sqlite3_prepare_v2(db, cSqlInsertTbl_EnabledSubject, -1, &stmt_EnabledSubject, NULL);
					
					if(rc != SQLITE_OK){
#if TEST_ENV == true
						sErrMsg = (std::string)"Can't Prepare SQL: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
						printf(sErrMsg.c_str());
#endif
						bIsOk = false;
					};
				};//if bIsOk
				
				if(bIsOk){
					iParameterPosition = sqlite3_bind_parameter_index(stmt_EnabledSubject, ":sGroup");
					
					rc = sqlite3_bind_text(stmt_EnabledSubject, 
										   iParameterPosition,
										   itGroup->sGroup.c_str(), 
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
					iParameterPosition = sqlite3_bind_parameter_index(stmt_EnabledSubject, ":sName");
					
					rc = sqlite3_bind_text(stmt_EnabledSubject, 
										   iParameterPosition,
										   itName->sName.c_str(), 
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
					iParameterPosition = sqlite3_bind_parameter_index(stmt_EnabledSubject, ":bEnabled");
					
					if(itName->bEnabled){
						rc = sqlite3_bind_int(stmt_EnabledSubject, 
											  iParameterPosition,
											  1); // 1 = true (enabled); 0 = false (disabled)
					}
					else
					{
						rc = sqlite3_bind_int(stmt_EnabledSubject, 
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
					rc = sqlite3_step(stmt_EnabledSubject);
					
					if(rc == SQLITE_ERROR){
#if TEST_ENV == true
						sErrMsg = (std::string)"Can't Step statment: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
						printf(sErrMsg.c_str());
#endif
						bIsOk = false;
					};
				};//if bIsOk
				
				if(bIsOk){
					rc = sqlite3_reset(stmt_EnabledSubject);
					
					if(rc != SQLITE_OK){
#if TEST_ENV == true
						fprintf(stderr, "Can't reset statment: %\n", sqlite3_errmsg(db));
#endif
						bIsOk = false;
					};
				};//if bIsOk
			};//for Name
		};//for group 

		if (bIsOk) {
//			modMisc::sqliteFinalizeRapper(stmt_Header);
//			modMisc::sqliteFinalizeRapper(stmt_Diff);
//			modMisc::sqliteFinalizeRapper(stmt_NoOfQuestions);
//			modMisc::sqliteFinalizeRapper(stmt_EnabledGroup);
//			modMisc::sqliteFinalizeRapper(stmt_EnabledSubject);
			
//			sqlite3_finalize(stmt_Header);
			sqlite3_finalize(stmt_Diff);
			sqlite3_finalize(stmt_NoOfQuestions);
			sqlite3_finalize(stmt_EnabledGroup);
			sqlite3_finalize(stmt_EnabledSubject);
		};
		
		zErr = NULL;
		sErrMsg.clear();
		
		cSqlDropTbl_Header = NULL;
		cSqlDropTbl_Diff = NULL;
		cSqlDropTbl_NoOfQuestions = NULL;
		cSqlDropTbl_EnabledGroup = NULL;
		cSqlDropTbl_EnabledSubject = NULL;
		
		cSqlCreateTbl_Header = NULL;
		cSqlCreateTbl_Diff = NULL;
		cSqlCreateTbl_NoOfQuestions = NULL;
		cSqlCreateTbl_EnabledGroup = NULL;
		cSqlCreateTbl_EnabledSubject = NULL;
		
		cSqlInsertTbl_Header = NULL;
		cSqlInsertTbl_Diff = NULL;
		cSqlInsertTbl_NoOfQuestions = NULL;
		cSqlInsertTbl_EnabledGroup = NULL;
		cSqlInsertTbl_EnabledSubject = NULL;

		sName.clear();
		cPath = NULL;
		
		this->load_subjects();
	} catch(...) {
		//Close database connection	
		rc = sqlite3_close(db);
		bDbIsOpen = false;
	};
};


void ClsPreferences::CheckPreferencesSubjectGroups(){
	int rc;
	
	try {
		char *zErr;
		std::string sErrMsg;
		bool bIsOk;
        
		std::string sName;
		char *cPath;
		std::vector<strSubjectGroup>::iterator itGroup;
//		std::vector<strSubjectName>::iterator itName;
		std::vector<strSubjectGroup>::iterator itFileGroup;
		std::vector<strSubjectName>::iterator itFileSubject;
		std::vector<strSubjectGroup>::iterator itDefaultGroup;
		std::vector<strSubjectName>::iterator itDefaultSubject;
        std::vector<strSubjectGroup> vecFile;
        bool bIsfoundGroup;
        bool bIsfoundSubject;
        
        ////////////////////////////////////////////////////////////////////////////////////////////
        //1) Fill this->vecSubAll with the default setttings

//        if (vecSubAll.size() != 0) {
        this->vecSubAll.clear();
//		};
        
		FillDefaultPreferences();
	
        ////////////////////////////////////////////////////////////////////////////////////////////
        //2) Load the setting in the Preference file into a tempary vector vecFile
        
        vecFile.clear();
        
        const char *cSqlSelectGroup("select fldGroup, fldEnabled, fldDifficulty from tblEnabledGroup order by fldGroup ;");
		const char *cSqlSelectSubject("select fldGroup, fldName, fldEnabled from tblEnabledSubject order by fldGroup, fldName ;");
		sqlite3_stmt *stmt_SelectGroup;
		sqlite3_stmt *stmt_SelectSubject;
		std::stringstream sResult;
		std::string sGroupPrevious;
		strSubjectGroup sSubjectGroup;
		strSubjectName sSubjectName;
		std::string sGroup;
		
		
		bIsOk = true;
		rc = SQLITE_OK;
		
		if (bDbIsOpen != true){
			cPath = (char*)this->sPathPreference.c_str();
			
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
		
		
		//Group Level
		if(bIsOk) {
			rc = sqlite3_prepare_v2(db, cSqlSelectGroup, -1, &stmt_SelectGroup, NULL);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't Prepare SQL: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		if(bIsOk){
			sGroupPrevious = ""; 
			
			while(sqlite3_step(stmt_SelectGroup) == SQLITE_ROW){
                
				sResult.str("");
				sResult << sqlite3_column_text(stmt_SelectGroup, 0);
				sSubjectGroup.sGroup = sResult.str();
				
				// 1 = true (enabled); 0 = false (disabled)
				if (sqlite3_column_int(stmt_SelectGroup, 1) == 1) {
					sSubjectGroup.bEnabled = true;
				} else {
					sSubjectGroup.bEnabled = false;
				};
				sSubjectGroup.iDifficulty = sqlite3_column_int(stmt_SelectGroup, 2);
				sSubjectGroup.eQuestionGroup = modMisc::convertEnum_QuesTypeGroupText(sSubjectGroup.sGroup);
				
				vecFile.push_back(sSubjectGroup);
			};//while
		};//if bIsOk
		
		if(bIsOk){
			rc = sqlite3_reset(stmt_SelectGroup);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				fprintf(stderr, "Can't reset statment: %\n", sqlite3_errmsg(db));
#endif
				bIsOk = false;
			};
		};//if bIsOk
		
		
		
		
		
		//Subject Level
		if(bIsOk) {
			rc = sqlite3_prepare_v2(db, cSqlSelectSubject, -1, &stmt_SelectSubject, NULL);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't Prepare SQL: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		if(bIsOk){
			while(sqlite3_step(stmt_SelectSubject) == SQLITE_ROW){
				sResult.str("");
				sResult << sqlite3_column_text(stmt_SelectSubject, 0);
				sGroup = sResult.str();
				
				sResult.str("");
				sResult << sqlite3_column_text(stmt_SelectSubject, 1);
				sSubjectName.sName = sResult.str();
                
				// 1 = true (enabled); 0 = false (disabled)
				if (sqlite3_column_int(stmt_SelectSubject, 2) == 1) {
					sSubjectName.bEnabled = true;
				} else {
					sSubjectName.bEnabled = false;
				};
				
				sSubjectName.eQuestionType = modMisc::convertEnumText_QuestionType(sSubjectName.sName);
				
				//loop through the vecSubAll to find the right place to add these subjects stuff
				for (itGroup = vecFile.begin(); itGroup != vecFile.end(); itGroup++) {
					if (itGroup->sGroup.compare(sGroup) == 0) {
						itGroup->vecSubjects.push_back(sSubjectName);
					};
				};
			};//while
		};//if bIsOk
		
		if(bIsOk){
			rc = sqlite3_reset(stmt_SelectSubject);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				fprintf(stderr, "Can't reset statment: %\n", sqlite3_errmsg(db));
#endif
				bIsOk = false;
			};
		};//if bIsOk
		
		
		
		cSqlSelectGroup = NULL;
		cSqlSelectSubject = NULL;


/////////////////////////////////////////////////////////////////////////////////////////////////////////        
        //3) loop through the vectors to find the entries that do not exist and correct them
        
        //3a) Groups
        for (itDefaultGroup = this->vecSubAll.begin(); itDefaultGroup != this->vecSubAll.end(); itDefaultGroup++) {
            bIsfoundGroup = false;
            for (itFileGroup = vecFile.begin(); itFileGroup != vecFile.end(); itFileGroup++) {
                if (itDefaultGroup->sGroup.compare(itFileGroup->sGroup) == 0) {
                    bIsfoundGroup = true;
                };
                
            };//File Group
            
            if (bIsfoundGroup == false) {
                strSubjectGroup sSubGroup = this->vecSubAll[distance(this->vecSubAll.begin(), itDefaultGroup) - 1];
                
                PreferencesInsertGroup(sSubGroup);
            };
        };//Default Group
        
        //3a) Subjects
        for (itDefaultGroup = this->vecSubAll.begin(); itDefaultGroup != this->vecSubAll.end(); itDefaultGroup++) {
            for (itFileGroup = vecFile.begin(); itFileGroup != vecFile.end(); itFileGroup++) {
                if (itDefaultGroup->sGroup.compare(itFileGroup->sGroup) == 0) {
                    for (itDefaultSubject = itDefaultGroup->vecSubjects.begin(); itDefaultSubject != itDefaultGroup->vecSubjects.end(); itDefaultSubject++) {
                        bIsfoundSubject = false;
                        for (itFileSubject = itFileGroup->vecSubjects.begin(); itFileSubject != itFileGroup->vecSubjects.end(); itFileSubject++) {
                            if (itDefaultSubject->sName.compare(itFileSubject->sName) == 0) {
                                bIsfoundSubject = true;
                            };
                        };//File Subject

                        if (bIsfoundSubject == false) {
                            strSubjectName sSubSubject = itDefaultGroup->vecSubjects[distance(itDefaultGroup->vecSubjects.begin(), itDefaultSubject)];
                            
                            PreferencesInsertSubject(itDefaultGroup->sGroup, sSubSubject);
                        };
                    };//Default Subject
                };
            };//File Group
        };//Default Group
        
		zErr = NULL;
		sErrMsg.clear();
		
		
//		cSqlInsertTbl_EnabledGroup = NULL;
//		cSqlInsertTbl_EnabledSubject = NULL;
        
		sName.clear();
		cPath = NULL;

        this->vecSubAll.clear();
	} catch(...) {
		//Close database connection	
		rc = sqlite3_close(db);
		bDbIsOpen = false;
	};
};

void ClsPreferences::PreferencesInsertGroup(strSubjectGroup sGroup){
	int rc;
	
	try {
		char *zErr;
		std::string sErrMsg;
		bool bIsOk;
        
		const char *cSqlInsertTbl_EnabledGroup("insert into tblEnabledGroup (fldGroup, fldEnabled, fldDifficulty) values (:sGroup, :bEnabled, :iDifficulty );");
		const char *cSqlInsertTbl_EnabledSubject("insert into tblEnabledSubject (fldGroup, fldName, fldEnabled) values (:sGroup, :sName, :bEnabled );");
		sqlite3_stmt *stmt_EnabledGroup;
		sqlite3_stmt *stmt_EnabledSubject;
		int iParameterPosition;
		std::string sName;
		char *cPath;
		std::vector<strSubjectName>::iterator itName;

		rc = sqlite3_close(db);
		bDbIsOpen = false;
		
		bIsOk = true;
		rc = SQLITE_OK;
		
		if (bDbIsOpen != true){
			cPath = (char*)this->sPathPreference.c_str();
		};
        
		if (bDbIsOpen != true){
			//open database connection
			rc = sqlite3_open(cPath, &db);
			bDbIsOpen = true;
		};
		
		if(rc != SQLITE_OK){
#if TEST_ENV == true
			sErrMsg = (std::string)"Can't open DB: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
			fprintf(stderr, sErrMsg.c_str(), 0);
#endif
			bIsOk = false;
			bDbIsOpen = false;
		};
		
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		
        //Populate Enabled tables with default values	
		
//		for(itGroup=vecSubAll.begin(); itGroup < vecSubAll.end(); itGroup++){
			//enabled group level
			
			if(bIsOk) {
				rc = sqlite3_prepare_v2(db, cSqlInsertTbl_EnabledGroup, -1, &stmt_EnabledGroup, NULL);
				
				if(rc != SQLITE_OK){
#if TEST_ENV == true
					sErrMsg = (std::string)"Can't Prepare SQL: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
					printf(sErrMsg.c_str());
#endif
					bIsOk = false;
				};
			};//if bIsOk
			
			if(bIsOk){
				iParameterPosition = sqlite3_bind_parameter_index(stmt_EnabledGroup, ":sGroup");
				
				rc = sqlite3_bind_text(stmt_EnabledGroup, 
									   iParameterPosition,
									   sGroup.sGroup.c_str(), 
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
				iParameterPosition = sqlite3_bind_parameter_index(stmt_EnabledGroup, ":iDifficulty");
				
				rc = sqlite3_bind_int(stmt_EnabledGroup, 
									  iParameterPosition,
									  1);
				
				if(rc != SQLITE_OK){
#if TEST_ENV == true
					sErrMsg = (std::string)"Can't add string parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
					printf(sErrMsg.c_str());
#endif
					bIsOk = false;
				};
			};//if bIsOk
			
			if(bIsOk){
				iParameterPosition = sqlite3_bind_parameter_index(stmt_EnabledGroup, ":bEnabled");
				
				if(sGroup.bEnabled){
					rc = sqlite3_bind_int(stmt_EnabledGroup, 
										  iParameterPosition,
										  1); // 1 = true (enabled); 0 = false (disabled)
				}
				else
				{
					rc = sqlite3_bind_int(stmt_EnabledGroup, 
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
				rc = sqlite3_step(stmt_EnabledGroup);
				
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
				rc = sqlite3_reset(stmt_EnabledGroup);
				
				if(rc != SQLITE_OK){
#if TEST_ENV == true
					fprintf(stderr, "Can't reset statment: %\n", sqlite3_errmsg(db));
#endif
					bIsOk = false;
				};
			};//if bIsOk
            
			
			
			
			
			
			
			for(itName=sGroup.vecSubjects.begin(); itName < sGroup.vecSubjects.end(); itName++){
				//enabled subject level
				if(bIsOk) {
					rc = sqlite3_prepare_v2(db, cSqlInsertTbl_EnabledSubject, -1, &stmt_EnabledSubject, NULL);
					
					if(rc != SQLITE_OK){
#if TEST_ENV == true
						sErrMsg = (std::string)"Can't Prepare SQL: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
						printf(sErrMsg.c_str());
#endif
						bIsOk = false;
					};
				};//if bIsOk
				
				if(bIsOk){
					iParameterPosition = sqlite3_bind_parameter_index(stmt_EnabledSubject, ":sGroup");
					
					rc = sqlite3_bind_text(stmt_EnabledSubject, 
										   iParameterPosition,
										   sGroup.sGroup.c_str(), 
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
					iParameterPosition = sqlite3_bind_parameter_index(stmt_EnabledSubject, ":sName");
					
					rc = sqlite3_bind_text(stmt_EnabledSubject, 
										   iParameterPosition,
										   itName->sName.c_str(), 
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
					iParameterPosition = sqlite3_bind_parameter_index(stmt_EnabledSubject, ":bEnabled");
					
					if(itName->bEnabled){
						rc = sqlite3_bind_int(stmt_EnabledSubject, 
											  iParameterPosition,
											  1); // 1 = true (enabled); 0 = false (disabled)
					}
					else
					{
						rc = sqlite3_bind_int(stmt_EnabledSubject, 
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
					rc = sqlite3_step(stmt_EnabledSubject);
					
					if(rc == SQLITE_ERROR){
#if TEST_ENV == true
						sErrMsg = (std::string)"Can't Step statment: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
						printf(sErrMsg.c_str());
#endif
						bIsOk = false;
					};
				};//if bIsOk
				
				if(bIsOk){
					rc = sqlite3_reset(stmt_EnabledSubject);
					
					if(rc != SQLITE_OK){
#if TEST_ENV == true
						fprintf(stderr, "Can't reset statment: %\n", sqlite3_errmsg(db));
#endif
						bIsOk = false;
					};
				};//if bIsOk
			};//for Name
        
		zErr = NULL;
		sErrMsg.clear();
		
		cSqlInsertTbl_EnabledGroup = NULL;
		cSqlInsertTbl_EnabledSubject = NULL;
        
		sName.clear();
		cPath = NULL;
		
	} catch(...) {
		//Close database connection	
		rc = sqlite3_close(db);
		bDbIsOpen = false;
	};
};

void ClsPreferences::PreferencesInsertSubject(std::string sGroup, strSubjectName sSubject) {
	int rc;
	
	try {
		char *zErr;
		std::string sErrMsg;
		bool bIsOk;
        
		const char *cSqlInsertTbl_EnabledSubject("insert into tblEnabledSubject (fldGroup, fldName, fldEnabled) values (:sGroup, :sName, :bEnabled );");
		sqlite3_stmt *stmt_EnabledSubject;
		int iParameterPosition;
		std::string sName;
		char *cPath;
		std::vector<strSubjectName>::iterator itName;
        
		rc = sqlite3_close(db);
		bDbIsOpen = false;
		
		bIsOk = true;
		rc = SQLITE_OK;
		
		if (bDbIsOpen != true){
			cPath = (char*)this->sPathPreference.c_str();
		};
        
		if (bDbIsOpen != true){
			//open database connection
			rc = sqlite3_open(cPath, &db);
			bDbIsOpen = true;
		};
		
		if(rc != SQLITE_OK){
#if TEST_ENV == true
			sErrMsg = (std::string)"Can't open DB: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
			fprintf(stderr, sErrMsg.c_str(), 0);
#endif
			bIsOk = false;
			bDbIsOpen = false;
		};
		
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		
        //Populate Enabled tables with default values	
		
            //enabled subject level
            if(bIsOk) {
                rc = sqlite3_prepare_v2(db, cSqlInsertTbl_EnabledSubject, -1, &stmt_EnabledSubject, NULL);
                
                if(rc != SQLITE_OK){
#if TEST_ENV == true
                    sErrMsg = (std::string)"Can't Prepare SQL: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
                    printf(sErrMsg.c_str());
#endif
                    bIsOk = false;
                };
            };//if bIsOk
            
            if(bIsOk){
                iParameterPosition = sqlite3_bind_parameter_index(stmt_EnabledSubject, ":sGroup");
                
                rc = sqlite3_bind_text(stmt_EnabledSubject, 
                                       iParameterPosition,
                                       sGroup.c_str(), 
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
                iParameterPosition = sqlite3_bind_parameter_index(stmt_EnabledSubject, ":sName");
                
                rc = sqlite3_bind_text(stmt_EnabledSubject, 
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
                iParameterPosition = sqlite3_bind_parameter_index(stmt_EnabledSubject, ":bEnabled");
                
                if(sSubject.bEnabled){
                    rc = sqlite3_bind_int(stmt_EnabledSubject, 
                                          iParameterPosition,
                                          1); // 1 = true (enabled); 0 = false (disabled)
                }
                else
                {
                    rc = sqlite3_bind_int(stmt_EnabledSubject, 
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
                rc = sqlite3_step(stmt_EnabledSubject);
                
                if(rc == SQLITE_ERROR){
#if TEST_ENV == true
                    sErrMsg = (std::string)"Can't Step statment: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
                    printf(sErrMsg.c_str());
#endif
                    bIsOk = false;
                };
            };//if bIsOk
            
            if(bIsOk){
                rc = sqlite3_reset(stmt_EnabledSubject);
                
                if(rc != SQLITE_OK){
#if TEST_ENV == true
                    fprintf(stderr, "Can't reset statment: %\n", sqlite3_errmsg(db));
#endif
                    bIsOk = false;
                };
            };//if bIsOk
        
		zErr = NULL;
		sErrMsg.clear();
		
		cSqlInsertTbl_EnabledSubject = NULL;
        
		sName.clear();
		cPath = NULL;

	} catch(...) {
		//Close database connection	
		rc = sqlite3_close(db);
		bDbIsOpen = false;
	};
};


void ClsPreferences::dropAllTables(){
	int rc;
	
	try {
		const char *cSqlSelectAllTables("SELECT name FROM sqlite_master WHERE type = 'table' ;");
		bool bIsOk;
		sqlite3_stmt *stmt_SelectAllTables;
		sqlite3_stmt *stmt_DeleteAll;
		sqlite3_stmt *stmt_DropTable;
		std::string sErrMsg;
		char *cPath;
		std::stringstream sResult;
		std::string sTableName;
		int iTblCounter = 0;
		
//		modMisc::sqliteStmtStatusRest(stmt_SelectAllTables);
//		modMisc::sqliteStmtStatusRest(stmt_DeleteAll);
//		modMisc::sqliteStmtStatusRest(stmt_DropTable);
		
		bIsOk = true;
		rc = SQLITE_OK;
		
		if (bDbIsOpen != true){
			cPath = (char*)this->sPathPreference.c_str();
			
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
			rc = sqlite3_prepare_v2(db, cSqlSelectAllTables, -1, &stmt_SelectAllTables, NULL);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't Prepare SQL: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		if(bIsOk){
			while(sqlite3_step(stmt_SelectAllTables) == SQLITE_ROW){
				//		sqlite3_stmt *stmt_DeleteAll;

				//delete all data
				sResult.str("");
				sResult << "delete from " << sqlite3_column_text(stmt_SelectAllTables, 0) << " ;";
				sTableName = sResult.str();
				
				if(bIsOk) {
					
					const char* cSqlDeleteAllData = (char*)sTableName.c_str();
					rc = sqlite3_prepare_v2(db, cSqlDeleteAllData, -1, &stmt_DeleteAll, NULL);
					
					if(rc != SQLITE_OK){
#if TEST_ENV == true
						sErrMsg = (std::string)"Can't Prepare SQL: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
						printf(sErrMsg.c_str());
#endif
						bIsOk = false;
					};
				};
				
				if(bIsOk){
					rc = sqlite3_step(stmt_DeleteAll);
					
					if(rc == SQLITE_ERROR){
#if TEST_ENV == true
						sErrMsg = (std::string)"Can't Step statment: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
						printf(sErrMsg.c_str());
#endif
						bIsOk = false;
					};
				};//if bIsOk
				
				
				
				
				
				//Drop table
				sResult.str("");
				sResult << "drop table " << sqlite3_column_text(stmt_SelectAllTables, 0);
				sTableName = sResult.str();
				
				if(bIsOk) {

					char* cSqlDropTable = (char*)sTableName.c_str();
					rc = sqlite3_prepare_v2(db, cSqlDropTable, -1, &stmt_DropTable, NULL);
					
					if(rc != SQLITE_OK){
#if TEST_ENV == true
						sErrMsg = (std::string)"Can't Prepare SQL: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
						printf(sErrMsg.c_str());
#endif
						bIsOk = false;
					};
				};

				if(bIsOk){
					rc = sqlite3_step(stmt_DropTable);
					
					if(rc == SQLITE_ERROR){
#if TEST_ENV == true
						sErrMsg = (std::string)"Can't Step statment: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
						printf(sErrMsg.c_str());
#endif
						bIsOk = false;
					};
				};//if bIsOk
				
				iTblCounter++;
			};//while
		};//if bIsOk
		
		if(bIsOk){
			rc = sqlite3_reset(stmt_SelectAllTables);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				fprintf(stderr, "Can't reset statment: %\n", sqlite3_errmsg(db));
#endif
				bIsOk = false;
			};
		};//if bIsOk
		
		cSqlSelectAllTables = NULL;
		if (bIsOk) {
//			modMisc::sqliteFinalizeRapper(stmt_SelectAllTables);
//			modMisc::sqliteFinalizeRapper(stmt_DeleteAll);
//			modMisc::sqliteFinalizeRapper(stmt_DropTable);
			
			
			
//			sqlite3_finalize(stmt_SelectAllTables);
//			sqlite3_finalize(stmt_DeleteAll);
//			sqlite3_finalize(stmt_DropTable);

//			if (<#condition#>) {
//				<#statements#>
//			}
//			sqlite3_finalize(stmt_SelectAllTables);
			//			sqlite3_finalize(stmt_DeleteAll);
			//			sqlite3_finalize(stmt_DropTable);

			
			sqlite3_finalize(stmt_SelectAllTables);
			if (iTblCounter != 0) {
				sqlite3_finalize(stmt_DeleteAll);
				sqlite3_finalize(stmt_DropTable);
			};
		};
		sErrMsg = "";
		sErrMsg.clear();
		cPath = NULL;
//		std::stringstream sResult;
		sTableName = "";
		sTableName.clear();
		
	} catch (...) {
		//Close database connection	
		rc = sqlite3_close(db);
		bDbIsOpen = false;
	};
};


void ClsPreferences::load_subjects(){
	int rc;

	try {
		const char *cSqlSelectEnabledGroup("select fldGroup, fldEnabled, fldDifficulty from tblEnabledGroup order by fldGroup ;");
		const char *cSqlSelectEnabledSubject("select fldGroup, fldName, fldEnabled from tblEnabledSubject order by fldGroup, fldName ;");
		bool bIsOk;
		sqlite3_stmt *stmt_EnabledGroup;
		sqlite3_stmt *stmt_EnabledSubject;
		std::string sErrMsg;
		char *cPath;
		std::stringstream sResult;
		std::string sGroupPrevious;
		strSubjectGroup sSubjectGroup;
		strSubjectName sSubjectName;
		std::string sGroup;
		vector<strSubjectGroup>::iterator itGroup;
		
		vecSubAll.clear();
		
		bIsOk = true;
		rc = SQLITE_OK;
		
		if (bDbIsOpen != true){
			cPath = (char*)this->sPathPreference.c_str();
			
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
		
		
		//Group Level
		if(bIsOk) {
			rc = sqlite3_prepare_v2(db, cSqlSelectEnabledGroup, -1, &stmt_EnabledGroup, NULL);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't Prepare SQL: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		if(bIsOk){
			sGroupPrevious = ""; 
			
			while(sqlite3_step(stmt_EnabledGroup) == SQLITE_ROW){

				sResult.str("");
				sResult << sqlite3_column_text(stmt_EnabledGroup, 0);
				sSubjectGroup.sGroup = sResult.str();
				
				// 1 = true (enabled); 0 = false (disabled)
				if (sqlite3_column_int(stmt_EnabledGroup, 1) == 1) {
					sSubjectGroup.bEnabled = true;
				} else {
					sSubjectGroup.bEnabled = false;
				};
				sSubjectGroup.iDifficulty = sqlite3_column_int(stmt_EnabledGroup, 2);
				sSubjectGroup.eQuestionGroup = modMisc::convertEnum_QuesTypeGroupText(sSubjectGroup.sGroup);
				
				vecSubAll.push_back(sSubjectGroup);
			};//while
		};//if bIsOk
		
		if(bIsOk){
			rc = sqlite3_reset(stmt_EnabledGroup);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				fprintf(stderr, "Can't reset statment: %\n", sqlite3_errmsg(db));
#endif
				bIsOk = false;
			};
		};//if bIsOk
		
		
		
		
		
		//Subject Level
		if(bIsOk) {
			rc = sqlite3_prepare_v2(db, cSqlSelectEnabledSubject, -1, &stmt_EnabledSubject, NULL);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't Prepare SQL: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		if(bIsOk){
			while(sqlite3_step(stmt_EnabledSubject) == SQLITE_ROW){
				sResult.str("");
				sResult << sqlite3_column_text(stmt_EnabledSubject, 0);
				sGroup = sResult.str();
				
				sResult.str("");
				sResult << sqlite3_column_text(stmt_EnabledSubject, 1);
				sSubjectName.sName = sResult.str();

				// 1 = true (enabled); 0 = false (disabled)
				if (sqlite3_column_int(stmt_EnabledSubject, 2) == 1) {
					sSubjectName.bEnabled = true;
				} else {
					sSubjectName.bEnabled = false;
				};
				
//				std::string sABC;
				
				sSubjectName.eQuestionType = modMisc::convertEnumText_QuestionType(sSubjectName.sName);
				
				//loop through the vecSubAll to find the right place to add these subjects stuff
				for (itGroup = vecSubAll.begin(); itGroup != vecSubAll.end(); itGroup++) {
					if (itGroup->sGroup.compare(sGroup) == 0) {
						itGroup->vecSubjects.push_back(sSubjectName);
					};
				};
			};//while
		};//if bIsOk
		
		if(bIsOk){
			rc = sqlite3_reset(stmt_EnabledSubject);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				fprintf(stderr, "Can't reset statment: %\n", sqlite3_errmsg(db));
#endif
				bIsOk = false;
			};
		};//if bIsOk
		
		
		
		cSqlSelectEnabledGroup = NULL;
		cSqlSelectEnabledSubject = NULL;
		if (bIsOk == true) {
			modMisc::sqliteFinalizeRapper(stmt_EnabledGroup);			
			modMisc::sqliteFinalizeRapper(stmt_EnabledSubject);
			
//			rc = sqlite3_finalize(stmt_EnabledGroup);
//			rc = sqlite3_finalize(stmt_EnabledSubject);
		};
		sErrMsg.clear();
		cPath = NULL;
//		std::stringstream sResult;
		sGroupPrevious.clear();
//		strSubjectGroup sSubjectGroup;
//		strSubjectName sSubjectName;
		sGroup.clear();
		
		
	} catch (...) {
		//Close database connection	
		rc = sqlite3_close(db);
		bDbIsOpen = false;
	};
};

#pragma mark -
#pragma mark Difficulty

void ClsPreferences::setDifficulty(float fDifficulty){
	int rc;
	
	try {
		char *zErr;
		std::string sErrMsg;
		bool bIsOk;
		const char *cSqlUpdateTbl_Diff("update tblDifficulty set fldDifficulty = :dDifficulty where fldGroup is Null;");
		sqlite3_stmt *stmt_Diff;
		int iParameterPosition;
		char *cPath;
		
		bIsOk = true;
		rc = SQLITE_OK;
		
		if (bDbIsOpen != true){
			cPath = (char*)this->sPathPreference.c_str();
			
			//open database connection
			rc = sqlite3_open(cPath, &db);
			bDbIsOpen = true;
		};
		
		if(rc != SQLITE_OK){
#if TEST_ENV == true
			sErrMsg = (std::string)"Can't open DB: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
			fprintf(stderr, sErrMsg.c_str(), 0);
#endif
			bIsOk = false;
			bDbIsOpen = false;
		};
		
		//Populate Difficulty table with default values	
		if(bIsOk) {
			rc = sqlite3_prepare_v2(db, cSqlUpdateTbl_Diff, -1, &stmt_Diff, NULL);
			
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
				sErrMsg = (std::string)"Can't add double parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
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
		
		if(bIsOk){
			rc = sqlite3_reset(stmt_Diff);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				fprintf(stderr, "Can't reset statment: %\n", sqlite3_errmsg(db));
#endif
				bIsOk = false;
			};
		};
	
		zErr = NULL;
		sErrMsg = "";
		sErrMsg.clear();
		cSqlUpdateTbl_Diff = NULL;

		if (bIsOk == true) {
			sqlite3_finalize(stmt_Diff);
		};
		cPath = NULL;
		
	} catch(...) {
		//Close database connection	
		rc = sqlite3_close(db);
		bDbIsOpen = false;
	};
};

void ClsPreferences::setDifficulty(float fDifficulty, std::string* sGroup){
	int rc;
	
	try {
		char *zErr;
		std::string sErrMsg;
		bool bIsOk;
		const char *cSqlUpdateTbl_Diff("update tblEnabledGroup set fldDifficulty = :dDifficulty where fldGroup = :sGroup ;");
		sqlite3_stmt *stmt_Diff;
		int iParameterPosition;
		char *cPath;
		
		bIsOk = true;
		rc = SQLITE_OK;
		
		if (bDbIsOpen != true){
			cPath = (char*)this->sPathPreference.c_str();
			
			//open database connection
			rc = sqlite3_open(cPath, &db);
			bDbIsOpen = true;
		};
		
		createDifficultyGroup(sGroup);//checks if a difficulty value has been created for a group and if not creates it
		
		if(rc != SQLITE_OK){
#if TEST_ENV == true
			sErrMsg = (std::string)"Can't open DB: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
			fprintf(stderr, sErrMsg.c_str(), 0);
#endif
			bIsOk = false;
			bDbIsOpen = false;
		};
		
		//Populate Difficulty table with default values	
		if(bIsOk) {
			rc = sqlite3_prepare_v2(db, cSqlUpdateTbl_Diff, -1, &stmt_Diff, NULL);
			
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
				sErrMsg = (std::string)"Can't add double parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt_Diff, ":sGroup");
			
			rc = sqlite3_bind_text(stmt_Diff, 
								   iParameterPosition,
								   sGroup->c_str(), 
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
		
		if(bIsOk){
			rc = sqlite3_reset(stmt_Diff);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				fprintf(stderr, "Can't reset statment: %\n", sqlite3_errmsg(db));
#endif
				bIsOk = false;
			};
		};
		
		zErr = NULL;
		sErrMsg.clear();
		cSqlUpdateTbl_Diff = NULL;

		if (bIsOk == true) {
			sqlite3_finalize(stmt_Diff);
		};
		cPath = NULL;
		
	} catch(...) {
		//Close database connection	
		rc = sqlite3_close(db);
		bDbIsOpen = false;
	};
};

float ClsPreferences::getDifficulty() {
	int rc;
	
	try {
		const char *cSql("select fldDifficulty from tblDifficulty where fldId = (select max(fldId) from tblDifficulty  where fldGroup is null) and fldGroup is null;");
		bool bIsOk;
		sqlite3_stmt *stmt;
		std::string sErrMsg;
		char *cPath;
		float fResult;
		int iResultCounter;
		
		bIsOk = true;
		rc = SQLITE_OK;
		
		if (bDbIsOpen != true){
			cPath = (char*)this->sPathPreference.c_str();
			
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
			rc = sqlite3_prepare_v2(db, cSql, -1, &stmt, NULL);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't Prepare SQL: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		iResultCounter = 0;
		if(bIsOk){
			while(sqlite3_step(stmt) == SQLITE_ROW){
				fResult = sqlite3_column_double(stmt, 0);
				iResultCounter++;
			};
		};
		
		if(bIsOk){
			rc = sqlite3_reset(stmt);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				fprintf(stderr, "Can't reset statment: %\n", sqlite3_errmsg(db));
#endif
				bIsOk = false;
			};
		};
		
	
		cSql = NULL;
		if (bIsOk) {
			sqlite3_finalize(stmt);
		};
		sErrMsg.clear();
		cPath = NULL;
		
		if (iResultCounter == 1) {
			return fResult;
		} else {
			return -1;
		};

	} catch(...) {
		rc = sqlite3_close(db);
		bDbIsOpen = false;
	};
};


float ClsPreferences::getDifficulty(std::string* sGroup) {
	int rc;
	
	try {
		const char *cSql("select fldDifficulty from tblEnabledGroup where fldGroup = :sGroup ;");
		bool bIsOk;
		sqlite3_stmt *stmt;
		std::string sErrMsg;
		char *cPath;
		float fResult;
		int iResultCounter;
		int iParameterPosition;
		
		bIsOk = true;
		rc = SQLITE_OK;
		
		if (bDbIsOpen != true){
			cPath = (char*)this->sPathPreference.c_str();
			
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
			rc = sqlite3_prepare_v2(db, cSql, -1, &stmt, NULL);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't Prepare SQL: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt, ":sGroup");
			
			rc = sqlite3_bind_text(stmt, 
								   iParameterPosition,
								   sGroup->c_str(), 
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
		
		iResultCounter = 0;
		if(bIsOk){
			while(sqlite3_step(stmt) == SQLITE_ROW){
				fResult = sqlite3_column_double(stmt, 0);
				iResultCounter++;
			};
		};
		
		if(bIsOk){
			rc = sqlite3_reset(stmt);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				fprintf(stderr, "Can't reset statment: %\n", sqlite3_errmsg(db));
#endif
				bIsOk = false;
			};
		};
		
	
		cSql = NULL;
		if (bIsOk) {
			sqlite3_finalize(stmt);
		};
		sErrMsg.clear();
		cPath = NULL;

		if(iResultCounter==1){
			return fResult;
		} else {
			return -1;
		};

	} catch(...) {
		rc = sqlite3_close(db);
		bDbIsOpen = false;
	};
};

float ClsPreferences::getSubjectGroupDiffMax(enumQuestionGroup eGroup) {
	int iResult;
	
	if (eGroup == eQuesGrpArithmeticBasics) {
		iResult = 3;
	} else if (eGroup == eQuesGrpArithmeticAdvanced) {
		iResult = 3;
	} else if (eGroup == eQuesGrpShapes) {
		iResult = 5;
	} else if (eGroup == eQuesGrpEquations) {
		iResult = 5;
	} else if (eGroup == eQuesGrpAverages) {
		iResult = 5;
	} else if (eGroup == eQuesGrpFractions) {
		iResult = 5;
	} else if (eGroup == eQuesGrpStatistics) {
		iResult = 5;
	} else {
		iResult = -1;
	};
	
	return iResult;
	
};

enumQuestionGroup ClsPreferences::getGroupEnum(std::string* sGroup) {
	std::vector<strSubjectGroup>::iterator itGroup;
	enumQuestionGroup eResult;
	
	eResult = eQuesGrpError;
	
	for(itGroup=vecSubAll.begin(); itGroup < vecSubAll.end(); itGroup++){
		if(itGroup->sGroup.compare(*sGroup) == 0){
			eResult = itGroup->eQuestionGroup;
		};
	};
	
	return eResult;
};

#pragma mark -
#pragma mark Groups and Subjects

int ClsPreferences::countGroups(){
	int iTemp;
	
	iTemp = (int)vecSubAll.size();
	
	return iTemp;
};

int ClsPreferences::countNames(std::string sGroup){
	std::vector<strSubjectGroup>::iterator itSubject;
	int iCounter;
	
	iCounter = 0;
	for(itSubject=vecSubAll.begin(); itSubject < vecSubAll.end(); itSubject++){
		if(sGroup==itSubject->sGroup){
			iCounter = itSubject->vecSubjects.size();
		};
	};
	
	return iCounter;
};

int ClsPreferences::countNames(int iGroup){
	int iResult;
	
	if(iGroup >= 0 and iGroup < vecSubAll.size()){
		iResult = vecSubAll[iGroup].vecSubjects.size();
	} else {
		iResult = 0;
	};
	return iResult;
};

std::string ClsPreferences::getSubjectGroup(int iGroup){
	std::string sResult;
	
	if(iGroup >= 0 and iGroup < vecSubAll.size()){
		sResult = vecSubAll[iGroup].sGroup;
	} else {
		sResult = "";
	};
	
	return sResult;
};

std::string ClsPreferences::getSubjectName(int iGroup, int iName){
	std::string sResult;
	
	if(iGroup >= 0 and iGroup < vecSubAll.size()){
		if(iName >= 0 and iName < vecSubAll[iGroup].vecSubjects.size()){
			sResult = vecSubAll[iGroup].vecSubjects[iName].sName;
		} else {
			sResult = "";
		};
	} else {
		sResult = "";
	};
	
	return sResult;
};

void ClsPreferences::createDifficultyGroup(std::string* sGroup) {
	int rc;
	
	try {
		//check if group exists
		char *zErr;
		const char *cSql_Check("select fldDifficulty from tblDifficulty where fldGroup = :sGroup ;");
		bool bIsOk;
		sqlite3_stmt *stmt_Check;
		std::string sErrMsg;
		char *cPath;
		float fResult;
		int iResultCounter;
		int iParameterPosition;
		
		bIsOk = true;
		rc = SQLITE_OK;
		
		if (bDbIsOpen != true){
			cPath = (char*)this->sPathPreference.c_str();
			
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
			rc = sqlite3_prepare_v2(db, cSql_Check, -1, &stmt_Check, NULL);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't Prepare SQL: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt_Check, ":sGroup");
			
			rc = sqlite3_bind_text(stmt_Check, 
								   iParameterPosition,
								   sGroup->c_str(), 
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
		
		iResultCounter = 0;
		if(bIsOk){
			while(sqlite3_step(stmt_Check) == SQLITE_ROW){
				fResult = sqlite3_column_double(stmt_Check, 0);
				iResultCounter++;
			};
		};
		
		if(bIsOk){
			rc = sqlite3_reset(stmt_Check);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				fprintf(stderr, "Can't reset statment: %\n", sqlite3_errmsg(db));
#endif
				bIsOk = false;
			};
		};
		
		if(iResultCounter == 0){
			//add a new group in the difficulty table
			const char *cSql_Insert("insert into tblDifficulty (fldDifficulty, fldGroup) values (:dDifficulty , :sGroup )");
			sqlite3_stmt *stmt_Insert;
			
			//Populate Difficulty table with default values	
			if(bIsOk) {
				rc = sqlite3_prepare_v2(db, cSql_Insert, -1, &stmt_Insert, NULL);
				
				if(rc != SQLITE_OK){
#if TEST_ENV == true
					sErrMsg = (std::string)"Can't Prepare SQL: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
					printf(sErrMsg.c_str());
#endif
					bIsOk = false;
				};
			};
			
			if(bIsOk){
				iParameterPosition = sqlite3_bind_parameter_index(stmt_Insert, ":dDifficulty");
				
				rc = sqlite3_bind_double(stmt_Insert, 
										 iParameterPosition,
										 (double)1.0);
				
				if(rc != SQLITE_OK){
#if TEST_ENV == true
					sErrMsg = (std::string)"Can't add double parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
					printf(sErrMsg.c_str());
#endif
					bIsOk = false;
				};
			};
			
			if(bIsOk){
				iParameterPosition = sqlite3_bind_parameter_index(stmt_Insert, ":sGroup");
				
				rc = sqlite3_bind_text(stmt_Insert, 
									   iParameterPosition,
									   sGroup->c_str(), 
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
			
			if(bIsOk){
				rc = sqlite3_step(stmt_Insert);
				
				if(rc == SQLITE_ERROR){
#if TEST_ENV == true
					sErrMsg = (std::string)"Can't Step statment: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
					printf(sErrMsg.c_str());
#endif
					bIsOk = false;
				};
			};
			
			if(bIsOk){
				rc = sqlite3_reset(stmt_Insert);
				
				if(rc != SQLITE_OK){
#if TEST_ENV == true
					fprintf(stderr, "Can't reset statment: %\n", sqlite3_errmsg(db));
#endif
					bIsOk = false;
				};
			};
		};
		
		zErr = NULL;
		cSql_Check = NULL;
		if (bIsOk) {
			sqlite3_finalize(stmt_Check);
		};
		sErrMsg.clear();
		cPath = NULL;
		
	} catch (...) {
		rc = sqlite3_close(db);
		bDbIsOpen = false;
	};
	
};

void ClsPreferences::setGroupEnabled(bool bEnabled, std::string* sGroup) {
	int rc;
	
	try {
		char *zErr;
		std::string sErrMsg;
		bool bIsOk;
		const char *cSqlUpdateTbl_Diff("update tblEnabledGroup set fldEnabled = :bEnabled where fldGroup = :sGroup ;");
		sqlite3_stmt *stmt_Diff;
		int iParameterPosition;
		char *cPath;
		vector<strSubjectGroup>::iterator itGroup;
		
		bIsOk = true;
		rc = SQLITE_OK;
		
		if (bDbIsOpen != true){
			cPath = (char*)this->sPathPreference.c_str();
			
			//open database connection
			rc = sqlite3_open(cPath, &db);
			bDbIsOpen = true;
		};
		
		createDifficultyGroup(sGroup);//checks if a difficulty value has been created for a group and if not creates it
		
		if(rc != SQLITE_OK){
#if TEST_ENV == true
			sErrMsg = (std::string)"Can't open DB: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
			fprintf(stderr, sErrMsg.c_str(), 0);
#endif
			bIsOk = false;
			bDbIsOpen = false;
		};
		
		//Populate Difficulty table with default values	
		if(bIsOk) {
			rc = sqlite3_prepare_v2(db, cSqlUpdateTbl_Diff, -1, &stmt_Diff, NULL);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't Prepare SQL: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt_Diff, ":bEnabled");
			
			if (bEnabled == true) {
				rc = sqlite3_bind_int(stmt_Diff, 
									  iParameterPosition,
									  1);
			} else {
				rc = sqlite3_bind_int(stmt_Diff, 
									  iParameterPosition,
									  0);
			};
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add double parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt_Diff, ":sGroup");
			
			rc = sqlite3_bind_text(stmt_Diff, 
								   iParameterPosition,
								   sGroup->c_str(), 
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
		
		if(bIsOk){
			rc = sqlite3_reset(stmt_Diff);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				fprintf(stderr, "Can't reset statment: %\n", sqlite3_errmsg(db));
#endif
				bIsOk = false;
			};
		};
		
		for (itGroup = vecSubAll.begin(); itGroup != vecSubAll.end(); itGroup++) {
			if (itGroup->sGroup.compare(*sGroup) == 0) {
				itGroup->bEnabled = bEnabled;
			};
		};
		
		zErr = NULL;
		sErrMsg.clear();
//		bool bIsOk;
		cSqlUpdateTbl_Diff = NULL;
		if (bIsOk) {
			sqlite3_finalize(stmt_Diff);
		};
//		int iParameterPosition;
		cPath = NULL;
//		vector<strSubjectGroup>::iterator itGroup;
		
		
		
	} catch(...) {
		//Close database connection	
		rc = sqlite3_close(db);
		bDbIsOpen = false;
	};
};

bool ClsPreferences::getGroupEnabled(std::string* sGroup) {
	int rc;
	
	try {
		const char *cSql("select fldEnabled from tblEnabledGroup where fldGroup = :sGroup ;");
		bool bIsOk;
		sqlite3_stmt *stmt;
		std::string sErrMsg;
		char *cPath;
		int iResult;
		bool bResult;
		int iResultCounter;
		int iParameterPosition;
		
		bIsOk = true;
		rc = SQLITE_OK;
		
		if (bDbIsOpen != true){
			cPath = (char*)this->sPathPreference.c_str();
			
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
			rc = sqlite3_prepare_v2(db, cSql, -1, &stmt, NULL);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't Prepare SQL: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt, ":sGroup");
			
			rc = sqlite3_bind_text(stmt, 
								   iParameterPosition,
								   sGroup->c_str(), 
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
		
		iResultCounter = 0;
		if(bIsOk){
			while(sqlite3_step(stmt) == SQLITE_ROW){
				iResult = sqlite3_column_int(stmt, 0);
				
				if (iResult == 1) {
					bResult = true;
				} else {
					bResult = false;
				};
				
				iResultCounter++;
			};
		};
		
		if(bIsOk){
			rc = sqlite3_reset(stmt);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				fprintf(stderr, "Can't reset statment: %\n", sqlite3_errmsg(db));
#endif
				bIsOk = false;
			};
		};
		

		cSql = NULL;
//		bool bIsOk;
		if (bIsOk) {
			sqlite3_finalize(stmt);
		};
		sErrMsg.clear();
		cPath = NULL;
//		int iResult;
//		bool bResult;
//		int iResultCounter;
//		int iParameterPosition;
		
	
		if(iResultCounter == 1){
			return bResult;
		}
		else{
			return true;
		};

	} catch(...) {
		rc = sqlite3_close(db);
		bDbIsOpen = false;
	};
};

void ClsPreferences::setSubjectEnabled(std::string *sGroup, std::string *sName, bool bIsEnabled){
	int rc;
	
	try {
		char *zErr;
		std::string sErrMsg;
		bool bIsOk;
		const char *cSqlUpdateTbl_Enabled("update tblEnabledSubject set fldEnabled = :bEnabled where fldName = :sName and fldGroup = :sGroup ;");
		sqlite3_stmt *stmt_Enabled;
		int iParameterPosition;
		char *cPath;
		std::vector<strSubjectGroup>::iterator itSubjectGroup;
		std::vector<strSubjectName>::iterator itSubjectName;
		
		bIsOk = true;
		rc = SQLITE_OK;
		
		if (bDbIsOpen != true){
			cPath = (char*)this->sPathPreference.c_str();
			
			//open database connection
			rc = sqlite3_open(cPath, &db);
			bDbIsOpen = true;
		};
		
		if(rc != SQLITE_OK){
#if TEST_ENV == true
			sErrMsg = (std::string)"Can't open DB: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
			fprintf(stderr, sErrMsg.c_str(), 0);
#endif
			bIsOk = false;
			bDbIsOpen = false;
		};
		
		//Populate Difficulty table with default values	
		if(bIsOk) {
			rc = sqlite3_prepare_v2(db, cSqlUpdateTbl_Enabled, -1, &stmt_Enabled, NULL);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't Prepare SQL: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt_Enabled, ":bEnabled");
			
			if(bIsEnabled == true) {
				rc = sqlite3_bind_int(stmt_Enabled, 
									  iParameterPosition,
									  1);
			}
			else
			{
				rc = sqlite3_bind_int(stmt_Enabled, 
									  iParameterPosition,
									  0);
			};
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add Enabled parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt_Enabled, ":sName");
			
			rc = sqlite3_bind_text(stmt_Enabled, 
								   iParameterPosition,
								   sName->c_str(), 
								   -1,
								   SQLITE_TRANSIENT);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add Name parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt_Enabled, ":sGroup");
			
			rc = sqlite3_bind_text(stmt_Enabled, 
								   iParameterPosition,
								   sGroup->c_str(), 
								   -1,
								   SQLITE_TRANSIENT);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add Group parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		if(bIsOk){
			rc = sqlite3_step(stmt_Enabled);
			
			if(rc == SQLITE_ERROR){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't Step statment: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		if(bIsOk){
			rc = sqlite3_reset(stmt_Enabled);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				fprintf(stderr, "Can't reset statment: %\n", sqlite3_errmsg(db));
#endif
				bIsOk = false;
			};
		};
		
		if(!vecSubAll.empty()){
			for(itSubjectGroup=vecSubAll.begin(); itSubjectGroup < vecSubAll.end(); itSubjectGroup++){
				if(itSubjectGroup->sGroup.c_str() == sGroup->c_str()){
					for(itSubjectName=itSubjectGroup->vecSubjects.begin(); itSubjectName < itSubjectGroup->vecSubjects.end(); itSubjectName++){
						if(itSubjectName->sName.c_str() == sName->c_str()){
							itSubjectName->bEnabled = bIsEnabled;
						};
					};
				};
			};
		};
	
		
		zErr = NULL;
		sErrMsg.clear();
//		bool bIsOk;
		cSqlUpdateTbl_Enabled = NULL;
		if (bIsOk) {
			sqlite3_finalize(stmt_Enabled);
		};
//		int iParameterPosition;
		cPath = NULL;
//		std::vector<strSubjectGroup>::iterator itSubjectGroup;
//		std::vector<strSubjectName>::iterator itSubjectName;
		
	} catch(...) {
		rc = sqlite3_close(db);
		bDbIsOpen = false;
	};
};

void ClsPreferences::setSubjectEnabled(std::string *sGroup, bool bIsEnabled){
	int rc;
	
	try {
		char *zErr;
		std::string sErrMsg;
		bool bIsOk;
		const char *cSqlUpdateTbl_Enabled("update tblEnabledGroup set fldEnabled = :bEnabled where fldGroup = :sGroup ;");
		sqlite3_stmt *stmt_Enabled;
		int iParameterPosition;
		char *cPath;
		std::vector<strSubjectGroup>::iterator itSubjectGroup;
		std::vector<strSubjectName>::iterator itSubjectName;
		
		bIsOk = true;
		rc = SQLITE_OK;
		
		if (bDbIsOpen != true){
			cPath = (char*)this->sPathPreference.c_str();
			
			//open database connection
			rc = sqlite3_open(cPath, &db);
			bDbIsOpen = true;
		};
		
		if(rc != SQLITE_OK){
#if TEST_ENV == true
			sErrMsg = (std::string)"Can't open DB: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
			fprintf(stderr, sErrMsg.c_str(), 0);
#endif
			bIsOk = false;
			bDbIsOpen = false;
		};
		
		//Populate Difficulty table with default values	
		if(bIsOk) {
			rc = sqlite3_prepare_v2(db, cSqlUpdateTbl_Enabled, -1, &stmt_Enabled, NULL);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't Prepare SQL: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt_Enabled, ":bEnabled");
			
			if(bIsEnabled == true) {
				rc = sqlite3_bind_int(stmt_Enabled, 
									  iParameterPosition,
									  1);
			}
			else
			{
				rc = sqlite3_bind_int(stmt_Enabled, 
									  iParameterPosition,
									  0);
			};
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add Enabled parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt_Enabled, ":sGroup");
			
			rc = sqlite3_bind_text(stmt_Enabled, 
								   iParameterPosition,
								   sGroup->c_str(), 
								   -1,
								   SQLITE_TRANSIENT);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add Group parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		if(bIsOk){
			rc = sqlite3_step(stmt_Enabled);
			
			if(rc == SQLITE_ERROR){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't Step statment: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		if(bIsOk){
			rc = sqlite3_reset(stmt_Enabled);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				fprintf(stderr, "Can't reset statment: %\n", sqlite3_errmsg(db));
#endif
				bIsOk = false;
			};
		};
		
		if(!vecSubAll.empty()){
			for(itSubjectGroup=vecSubAll.begin(); itSubjectGroup <= vecSubAll.end(); itSubjectGroup++){
				if(itSubjectGroup->sGroup.c_str() == sGroup->c_str()){
					for(itSubjectName=itSubjectGroup->vecSubjects.begin(); itSubjectName <= itSubjectGroup->vecSubjects.end(); itSubjectName++){
						itSubjectName->bEnabled = bIsEnabled;
					};
				};
			};
		};
	
		zErr = NULL;
		sErrMsg.clear();
//		bool bIsOk;
		cSqlUpdateTbl_Enabled = NULL;
		if (bIsOk) {
			sqlite3_finalize(stmt_Enabled);
		};
//		int iParameterPosition;
		cPath = NULL;
//		std::vector<strSubjectGroup>::iterator itSubjectGroup;
//		std::vector<strSubjectName>::iterator itSubjectName;
		
	} catch(...) {
		rc = sqlite3_close(db);
		bDbIsOpen = false;
	};
};

bool ClsPreferences::getSubjectEnabled(std::string *sGroup, std::string *sName){
	int rc;
	
	try {
		const char *cSqlSelectEnabled("select fldEnabled from tblEnabledSubject where fldName = :sName and fldGroup = :sGroup ;");
		bool bIsOk;
		sqlite3_stmt *stmt_Enabled;
		std::string sErrMsg;
		int iParameterPosition;
		char *cPath;
		int iResult;
		int iResultCounter;
		
		bIsOk = true;
		rc = SQLITE_OK;
		
		if (bDbIsOpen != true){
			cPath = (char*)this->sPathPreference.c_str();
			
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
			rc = sqlite3_prepare_v2(db, cSqlSelectEnabled, -1, &stmt_Enabled, NULL);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't Prepare SQL: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt_Enabled, ":sName");
			
			rc = sqlite3_bind_text(stmt_Enabled, 
								   iParameterPosition,
								   sName->c_str(), 
								   -1,
								   SQLITE_TRANSIENT);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add Name parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt_Enabled, ":sGroup");
			
			rc = sqlite3_bind_text(stmt_Enabled, 
								   iParameterPosition,
								   sGroup->c_str(), 
								   -1,
								   SQLITE_TRANSIENT);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add Group parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		iResultCounter = 0;
		if(bIsOk){
			while(sqlite3_step(stmt_Enabled) == SQLITE_ROW){
				iResult = sqlite3_column_int(stmt_Enabled, 0);
				iResultCounter++;
			};
		};
		
		if(bIsOk){
			rc = sqlite3_reset(stmt_Enabled);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				fprintf(stderr, "Can't reset statment: %\n", sqlite3_errmsg(db));
#endif
				bIsOk = false;
			};
		};

		cSqlSelectEnabled = NULL;
		//		bool bIsOk;
		if (bIsOk) {
			sqlite3_finalize(stmt_Enabled);
		}
		sErrMsg.clear();
		//		int iParameterPosition;
		cPath = NULL;
		//		int iResult;
		//		int iResultCounter;
		
		if(iResultCounter==1){
			if(iResult==1){
				return true;
			} else{
				return false;
			};
		} else{
			return true;
		};
		
		
		
	} catch(...) {
		rc = sqlite3_close(db);
		bDbIsOpen = false;
	};
};

bool ClsPreferences::getSubjectEnabled(std::string *sGroup){
	int rc;
	
	try {
		const char *cSqlSelectEnabled("select fldEnabledGroup from tblEnabled where fldGroup = :sGroup;");
		bool bIsOk;
		sqlite3_stmt *stmt_Enabled;
		std::string sErrMsg;
		int iParameterPosition;
		char *cPath;
		int iResult;
		int iResultCounter;
		bool bIsEnabled;
		
		bIsOk = true;
		rc = SQLITE_OK;
		
		if (bDbIsOpen != true){
			cPath = (char*)this->sPathPreference.c_str();
			
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
			rc = sqlite3_prepare_v2(db, cSqlSelectEnabled, -1, &stmt_Enabled, NULL);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't Prepare SQL: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		if(bIsOk){
			iParameterPosition = sqlite3_bind_parameter_index(stmt_Enabled, ":sGroup");
			
			rc = sqlite3_bind_text(stmt_Enabled, 
								   iParameterPosition,
								   sGroup->c_str(), 
								   -1,
								   SQLITE_TRANSIENT);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				sErrMsg = (std::string)"Can't add Group parameter: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
#endif
				bIsOk = false;
			};
		};
		
		bIsEnabled = false;
		
		iResultCounter = 0;
		if(bIsOk){
			
			while(sqlite3_step(stmt_Enabled) == SQLITE_ROW){
				iResult = sqlite3_column_int(stmt_Enabled, 0);
				
				if (iResult == 1) {
					bIsEnabled = true;
				};
				
				iResultCounter++;
			};
		};
		
		if(bIsOk){
			rc = sqlite3_reset(stmt_Enabled);
			
			if(rc != SQLITE_OK){
#if TEST_ENV == true
				fprintf(stderr, "Can't reset statment: %\n", sqlite3_errmsg(db));
#endif
				bIsOk = false;
			};
		};
		
		
		
		cSqlSelectEnabled = NULL;
//		bool bIsOk;
		if (bIsOk) {
			sqlite3_finalize(stmt_Enabled);
		};
		sErrMsg.clear();
//		int iParameterPosition;
		cPath = NULL;
//		int iResult;
//		int iResultCounter;
//		bool bIsEnabled;

		if(iResultCounter == 1){
			if(bIsEnabled == true){
				return true;
			}else{
				return false;
			};
		} else {
			return true;
		};
		
	} catch(...) {
		rc = sqlite3_close(db);
		bDbIsOpen = false;
	}
};

enumQuestionGroup ClsPreferences::getRandomQuestionGroup() {
	int iChoice;
	enumQuestionGroup eChoice;
	int iChoiceStartPos;
	int iChoiceEndPos;
	int iRepetitions;
//	bool bIsFound;
	bool bIsEnabled;
	std::vector<strSubjectName>::iterator itName;
	
	if (this->ePrefType == ePrefSimple) {
		iChoice = modMisc::iRandomNumber(0, this->vecSubAll.size() - 1);
		
		eChoice = this->vecSubAll[iChoice].eQuestionGroup;
	} else if (this->ePrefType == ePrefBySubject or this->ePrefType == ePrefByGroup) {
		iChoiceStartPos = modMisc::iRandomNumber(0, this->vecSubAll.size() - 1);
		
		if (iChoiceStartPos == 0) {
			iChoiceEndPos = this->vecSubAll.size() - 1;
		} else {
			iChoiceEndPos = iChoiceStartPos - 1;
		};
		
		iRepetitions = modMisc::iRandomNumber(1, 2 * (this->vecSubAll.size()));
		
		iChoice = iChoiceStartPos;
		
		do {
			do {
				if (iChoice == this->vecSubAll.size() - 1) {
					iChoice = 0;
				} else {
					iChoice++;
				};
				
				if (this->vecSubAll[iChoice].bEnabled == true) {
					if (this->ePrefType == ePrefBySubject) {
						//if all of the subjects in the group are disabled the the group is disabled even if it isn't flag disabled
						bIsEnabled = false;
						
						//if just one of the subjects is flagged as enabled then the group can also be enabled
						for (itName = this->vecSubAll[iChoice].vecSubjects.begin(); itName < this->vecSubAll[iChoice].vecSubjects.end(); itName++) {
							if (itName->bEnabled == true) {
								bIsEnabled = true;
							};
						};
					} else {
						bIsEnabled = true;
					};
				} else {
					bIsEnabled = false;
				};
			} while (bIsEnabled == false);

			iRepetitions--;
		} while (iRepetitions > 0);

		eChoice = this->vecSubAll[iChoice].eQuestionGroup;
	} else {
		eChoice = eQuesGrpError;
	};
	
	return eChoice;
};

enumQuestionType ClsPreferences::getRandomQuestionType(enumQuestionGroup eGroup) {
	int iChoice;
	enumQuestionType eChoice;
	int iChoiceStartPos;
	int iChoiceEndPos;
	int iRepetitions;
	bool bIsEnabled;
	bool bIsFoundGroup;
	std::vector<strSubjectGroup>::iterator itGroup;
	std::vector<strSubjectName>::iterator itName;

	bIsFoundGroup = false;
	if (eGroup == eQuesGrpError) {
		eChoice = eQuesError;
	} else {
		for (itGroup = this->vecSubAll.begin(); itGroup < this->vecSubAll.end(); itGroup++) {
			if (itGroup->eQuestionGroup == eGroup) {
				bIsFoundGroup = true;
				
				if (this->ePrefType == ePrefSimple) {
					iChoice = modMisc::iRandomNumber(0, itGroup->vecSubjects.size() - 1);
					
					eChoice = itGroup->vecSubjects[iChoice].eQuestionType; 
				} else if (this->ePrefType == ePrefByGroup) {
					iChoice = modMisc::iRandomNumber(0, itGroup->vecSubjects.size() - 1);

					eChoice = itGroup->vecSubjects[iChoice].eQuestionType;
				} else if (this->ePrefType == ePrefBySubject) {
					iChoiceStartPos = modMisc::iRandomNumber(0, itGroup->vecSubjects.size() - 1);
					
					if (iChoiceStartPos == 0) {
						iChoiceEndPos = itGroup->vecSubjects.size() - 1;
					} else {
						iChoiceEndPos = iChoiceStartPos - 1;
					};
					
					iRepetitions = modMisc::iRandomNumber(1, 2 * (itGroup->vecSubjects.size()));
					
					iChoice = iChoiceStartPos;
					
					do {
						do {
							if (iChoice == itGroup->vecSubjects.size() - 1) {
								iChoice = 0;
							} else {
								iChoice++;
							};
							
							if (itGroup->vecSubjects[iChoice].bEnabled == true) {
								bIsEnabled = true;
							} else {
								bIsEnabled = false;
							};
						} while (bIsEnabled == false);
						
						iRepetitions--;
					} while (iRepetitions > 0);
					
					eChoice = itGroup->vecSubjects[iChoice].eQuestionType;
				} else {
					eChoice = eQuesError;
				};
			};
		};
	};
	
	if (bIsFoundGroup == false) {
		eChoice = eQuesError;
	};
	
	return eChoice;
};

bool ClsPreferences::isQuestionPossible() {
	bool bResult;
	std::vector<strSubjectGroup>::iterator itGroup;
	std::vector<strSubjectName>::iterator itSubject;
	
	if (this->ePrefType == ePrefSimple) {
		bResult = true;
	} else if (this->ePrefType == ePrefByGroup) {
		bResult = false;
		for (itGroup = this->vecSubAll.begin(); itGroup < this->vecSubAll.end(); itGroup++) {
			if (itGroup->bEnabled == true) {
				bResult = true;
			};
		};
	} else if (this->ePrefType == ePrefBySubject) {
		bResult = false;
		for (itGroup = this->vecSubAll.begin(); itGroup < this->vecSubAll.end(); itGroup++) {
			if (itGroup->bEnabled == true) {
				for (itSubject = itGroup->vecSubjects.begin(); itSubject < itGroup->vecSubjects.end(); itSubject++) {
					if (itSubject->bEnabled == true) { 
						bResult = true;
					};
				};
			};
		};
	} else {
		bResult = false;
	};
	
	return bResult;
};

#pragma mark -
#pragma mark Check Question

bool ClsPreferences::isQuestionOk(strQuestion sQuestion) {
	bool bIsOk;
	
	switch (this->ePrefType) {
		case ePrefSimple:
			float fUpperLimit;
			float fLowerLimit;
			
			fUpperLimit = this->fDiff * (((float)modMisc::iRandomNumber(1100, 1000)) / 1000);
			fLowerLimit = this->fDiff * (((float)modMisc::iRandomNumber(900, 1000)) / 1000);
			
			if (sQuestion.iDifficulty >= fLowerLimit and sQuestion.iDifficulty <= fUpperLimit) {
				bIsOk = true;
			} else {
				bIsOk = false;
			};
			break;
		case ePrefBySubject:
			bIsOk = true;
			break;
		default:
			break;
	};
	
	return bIsOk;
};

bool ClsPreferences::isQuestionOk(enumQuestionType eQuesType, enumQuestionGroup eQuesGroup) {
	bool bIsOk;
//	vector<strSubjectGroup>::iterator itGroup;
//	vector<strSubjectName>::iterator itName;
	
	if (ePrefType == ePrefSimple) {
		bIsOk = true;
	} else if (ePrefType == ePrefBySubject) {
		bIsOk = isQuestionOkBySubject(eQuesType, eQuesGroup);
	} else if (ePrefType == ePrefByGroup) {
		bIsOk = isQuestionOkByGroup(eQuesType, eQuesGroup);
	} else {
		//error this should never be selected
		bIsOk = false;
	};
	
	return bIsOk;
};

bool ClsPreferences::isQuestionOkBySubject(enumQuestionType eQuesType, enumQuestionGroup eQuesGroup) {
	bool bIsOk;
	vector<strSubjectGroup>::iterator itGroup;
	vector<strSubjectName>::iterator itName;
	bool bIsFound;
	
	if (ePrefType == ePrefSimple) {
		bIsOk = true;
	} else if (ePrefType == ePrefBySubject) {
		bIsOk = false;
		bIsFound = false;
		
		for(itGroup=this->vecSubAll.begin(); itGroup < this->vecSubAll.end(); itGroup++){

			if (itGroup->eQuestionGroup == eQuesGroup) {
				if (itGroup->bEnabled == true) {
					for(itName=itGroup->vecSubjects.begin(); itName < itGroup->vecSubjects.end(); itName++){
						if (questionTypeIsEqual(itName->eQuestionType, eQuesType) == true) {
							bIsFound = true;
							if (itName->bEnabled == true) {
								bIsOk = true;
							} else {
								bIsOk = false;
							}; //if itName enabled
						}; //if itName Question Type
					};//for itName
				} else {
					bIsOk = false;
				};//if group enabled
			};//if group name...
		};//for itGroup
		
		if (bIsFound == false) {
			//think of an error routine
			bIsOk = false;
		};
	} else if (ePrefType == ePrefByGroup) {
		bIsOk = true;
	} else {
		//error this should never be selected
		bIsOk = false;
	};
	
	return bIsOk;
};


bool ClsPreferences::isQuestionOkByGroup(enumQuestionType eQuesType, enumQuestionGroup eQuesGroup) {
	bool bIsOk;
	vector<strSubjectGroup>::iterator itGroup;
	vector<strSubjectName>::iterator itName;
	bool bIsFound;
	
	if (ePrefType == ePrefSimple) {
		bIsOk = true;
	} else if (ePrefType == ePrefBySubject) {
		bIsOk = true;
	} else if (ePrefType == ePrefByGroup) {
		bIsOk = false;
		bIsFound = false;
		
		for(itGroup=this->vecSubAll.begin(); itGroup <= this->vecSubAll.end(); itGroup++){
			if (itGroup->eQuestionGroup == eQuesGroup) {
				bIsFound = true;
				if (itGroup->bEnabled == true) {
					bIsOk = true;
				};
			};
		};
		if (bIsFound == false) {
			//think of an error routine
			bIsOk = false;
		};
	} else {
		//error this should never be selected
		bIsOk = false;
	};
	
	return bIsOk;
};

#pragma mark -
#pragma mark Check Preference File

void ClsPreferences::CheckEnabledTblUnique() {
	
	
	
};

#endif