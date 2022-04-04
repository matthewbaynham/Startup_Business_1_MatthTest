/*
 *  ClsHelp.cpp
 *  MatthTest
 *
 *  Created by Matthew Baynham on 3/11/11.
 *  Copyright 2011 Baynham Coding. All rights reserved.
 *
 */


#ifndef CLS_HELP_CPP
#define CLS_HELP_CPP

#include <vector>
#include <string>
#include <cstddef>//std array rapper
#include <sqlite3.h>;
#include <stdlib.h>;
#include <time.h>
#include <sstream>
#include <math.h>
#include "modMisc.h"
#include "ClsHelp.h"

using namespace std;
using namespace modMisc;

ClsHelp::ClsHelp() {

};

ClsHelp::~ClsHelp() {

};

void ClsHelp::setPath(std::string sPath) {
	
	this->sPathHelp = sPath;

	this->loadData();

}
/*
void ClsHelp::setPath(std::string sHomePath) {
	char *cPath;
	
	//if last char is not / then add / between directory and filename
	size_t iPos;
	std::string sLastChar;
	
	iPos = sHomePath.size() - 1;
	sLastChar = sHomePath.substr(iPos, 1);

	this->sPathHelp.clear();
//	sPathHelp.clear();

	if (sLastChar.compare("/") == 0) {
		char cTemp[(sHomePath.size() + modMisc::sFileNameHelp.size() + 2)];
		
		strcpy (cTemp,sHomePath.c_str());
		strcat (cTemp,modMisc::sFileNameHelp.c_str());
		
		std::string sTempPath = (std::string)cTemp;
		
		this->sPathHelp = sTempPath;
//		sPathHelp.c_str(cTemp);
//		this->sPathHelp = new std::string(sTempPath.c_str());
//		sPathHelp.assign(sTempPath.c_str());
//		sPathHelp.c_str(cTemp);
//		sPathHelp = (std::string)cTemp;
//		sPathHelp = sTempPath;
		
//		sTempPath = "";
		sTempPath.clear();
//		cTemp = NULL;
	} else {
		char cTemp[(sHomePath.size() + modMisc::sFileNameHelp.size() + 1)];
		
		strcpy (cTemp,sHomePath.c_str());
		strcat (cTemp,"/");
		strcat (cTemp,modMisc::sFileNameHelp.c_str());
		
		std::string sTempPath;
		sTempPath =  (std::string)cTemp;
		
		this->sPathHelp = sTempPath;
		
		sTempPath = "";
//		sTempPath.clear();
		//		cTemp = NULL;
	};
	
	sLastChar.clear();
	
	this->loadData();
};
*/

void ClsHelp::loadData() {
	this->loadDataMenu();
	this->loadDataGroups();
	this->loadDataSubject();
};


void ClsHelp::loadDataGroups(){
	int rc;
	
	try {
		const char *cSqlSelectGroup("select tblGroups.fldGroup, tblDescription.fldDescription from tblGroups inner join tblDescription on tblGroups.fldDescId = tblDescription.fldId ;");
		bool bIsOk;
		sqlite3_stmt *stmt;
		std::string sErrMsg;
		char *cPath;
		std::stringstream sResult;
		strHelpGroup sHelpGroup;
		
		this->vecGroup.clear();
		
		bIsOk = true;
		rc = SQLITE_OK;
		
		if (bDbIsOpen != true){
			cPath = (char*)this->sPathHelp.c_str();
//			printf(cPath);
			
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
			rc = sqlite3_prepare_v2(db, cSqlSelectGroup, -1, &stmt, NULL);
			
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
				
				sResult.str("");
				sResult << sqlite3_column_text(stmt, 0);
				sHelpGroup.sGroup = sResult.str();
				
				sResult.str("");
				sResult << sqlite3_column_text(stmt, 1);
				sHelpGroup.sDesc = sResult.str();
				
				this->vecGroup.push_back(sHelpGroup);
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
		
		
		
		
		
		cSqlSelectGroup = NULL;
		if (bIsOk == true) {
			modMisc::sqliteFinalizeRapper(stmt);
		};
		sErrMsg.clear();
		cPath = NULL;
		
		
	} catch (...) {
		//Close database connection	
		rc = sqlite3_close(db);
		bDbIsOpen = false;
	};
};

void ClsHelp::loadDataSubject(){
	int rc;
	
	try {
		const char *cSqlSelectSubject("select tblSubject.fldGroup, tblSubject.fldSubject, tblDescription.fldDescription from tblSubject inner join tblDescription on tblSubject.fldDescId = tblDescription.fldId ;");
		bool bIsOk;
		sqlite3_stmt *stmt;
		std::string sErrMsg;
		char *cPath;
		std::stringstream sResult;
		strHelpSubject sHelpSubject;
		
		this->vecSubject.clear();
		
		bIsOk = true;
		rc = SQLITE_OK;
		
		if (bDbIsOpen != true){
			cPath = (char*)this->sPathHelp.c_str();
//			printf(cPath);
			
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
			rc = sqlite3_prepare_v2(db, cSqlSelectSubject, -1, &stmt, NULL);
			
			if(rc != SQLITE_OK){
				//#if TEST_ENV == true
				sErrMsg = (std::string)"Can't Prepare SQL: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
				//#endif
				bIsOk = false;
			};
		};
		
		if(bIsOk){
			while(sqlite3_step(stmt) == SQLITE_ROW){
				
				sResult.str("");
				sResult << sqlite3_column_text(stmt, 0);
				sHelpSubject.sGroup = sResult.str();
				
				sResult.str("");
				sResult << sqlite3_column_text(stmt, 1);
				sHelpSubject.sSubject = sResult.str();
				
				sResult.str("");
				sResult << sqlite3_column_text(stmt, 2);
				sHelpSubject.sDesc = sResult.str();
				
				this->vecSubject.push_back(sHelpSubject);
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
		
		
		
		
		
		cSqlSelectSubject = NULL;
		if (bIsOk == true) {
			modMisc::sqliteFinalizeRapper(stmt);
		};
		sErrMsg.clear();
		cPath = NULL;
		
		
	} catch (...) {
		//Close database connection	
		rc = sqlite3_close(db);
		bDbIsOpen = false;
	};
};

void ClsHelp::loadDataMenu(){
	int rc;
	
	try {
		const char *cSqlSelectMenu("select tblMenu.fldMenu, tblDescription.fldDescription from tblMenu inner join tblDescription on tblMenu.fldDescId = tblDescription.fldId ;");
		bool bIsOk;
		sqlite3_stmt *stmt;
		std::string sErrMsg;
		char *cPath;
		std::stringstream sResult;
		strHelpMenu sHelpMenu;
		
		this->vecMenu.clear();
		
		bIsOk = true;
		rc = SQLITE_OK;
		
//		if (bDbIsOpen != true){
			cPath = (char*)this->sPathHelp.c_str();
//			printf(cPath);
			
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
			rc = sqlite3_prepare_v2(db, cSqlSelectMenu, -1, &stmt, NULL);
			
			if(rc != SQLITE_OK){
				//#if TEST_ENV == true
				sErrMsg = (std::string)"Can't Prepare SQL: " + (std::string)sqlite3_errmsg(db) + (std::string)"\n";
				printf(sErrMsg.c_str());
				//#endif
				bIsOk = false;
			};
		};
		
		if(bIsOk){
			while(sqlite3_step(stmt) == SQLITE_ROW){
				
				sResult.str("");
				sResult << sqlite3_column_text(stmt, 0);
				sHelpMenu.sMenu = sResult.str();
				
				sResult.str("");
				sResult << sqlite3_column_text(stmt, 1);
				sHelpMenu.sDesc = sResult.str();
				
				this->vecMenu.push_back(sHelpMenu);
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
		
		
		
		
		
		cSqlSelectMenu = NULL;
		if (bIsOk == true) {
			modMisc::sqliteFinalizeRapper(stmt);
		};
		sErrMsg.clear();
		cPath = NULL;
		
		
	} catch (...) {
		//Close database connection	
		rc = sqlite3_close(db);
		bDbIsOpen = false;
	};
};

std::string ClsHelp::menuHelp(std::string sGroup){
	std::vector<strHelpMenu>::iterator itMenu;
	bool bIsFound;
	std::string sResult;
	
	bIsFound = false;
	for (itMenu = this->vecMenu.begin(); itMenu < this->vecMenu.end(); itMenu++) {
		if (strcmp(itMenu->sMenu.c_str(), sGroup.c_str()) == 0) {
			sResult = itMenu->sDesc;
			bIsFound = true;
		};
	};
	
	if (bIsFound == false) {
		sResult = "";
	};
	
	return sResult;
};

bool ClsHelp::menuHelpIsFound(std::string sMenu){
	std::vector<strHelpMenu>::iterator itMenu;
	bool bIsFound;
	
	bIsFound = false;
	for (itMenu = this->vecMenu.begin(); itMenu < this->vecMenu.end(); itMenu++) {
		if (strcmp(itMenu->sMenu.c_str(), sMenu.c_str()) == 0) {
			bIsFound = true;
		};
	};
	
	return bIsFound;
};

std::string ClsHelp::groupHelp(std::string sGroup){
	std::vector<strHelpGroup>::iterator itGroup;
	bool bIsFound;
	std::string sResult;
	
	bIsFound = false;
	for (itGroup = this->vecGroup.begin(); itGroup < this->vecGroup.end(); itGroup++) {
		if (strcmp(itGroup->sGroup.c_str(), sGroup.c_str()) == 0) {
			sResult = itGroup->sDesc;
			bIsFound = true;
		};
	};
	
	if (bIsFound == false) {
		sResult = "";
	};
	
	return sResult;
};

bool ClsHelp::groupHelpIsFound(std::string sGroup){
	std::vector<strHelpGroup>::iterator itGroup;
	bool bIsFound;
	
	bIsFound = false;
	for (itGroup = this->vecGroup.begin(); itGroup < this->vecGroup.end(); itGroup++) {
		if (strcmp(itGroup->sGroup.c_str(), sGroup.c_str()) == 0) {
			bIsFound = true;
		};
	};
	
	return bIsFound;
};

std::string ClsHelp::subjectHelp(std::string sGroup, std::string sSubject) {
	std::vector<strHelpSubject>::iterator itSubject;
	bool bIsFound;
	std::string sResult;
	
	bIsFound = false;
	for (itSubject = this->vecSubject.begin(); itSubject < this->vecSubject.end(); itSubject++) {
		if (strcmp(itSubject->sGroup.c_str(), sGroup.c_str()) == 0) {
			if (strcmp(itSubject->sSubject.c_str(), sSubject.c_str()) == 0) {
				sResult = itSubject->sDesc;
				bIsFound = true;
			};
		};
	};
	
	if (bIsFound == false) {
		sResult = "";
	};
	
	return sResult;
};

bool ClsHelp::subjectHelpIsFound(std::string sGroup, std::string sSubject) {
	std::vector<strHelpSubject>::iterator itSubject;
	bool bIsFound;
	
	bIsFound = false;
	for (itSubject = this->vecSubject.begin(); itSubject < this->vecSubject.end(); itSubject++) {
		if (strcmp(itSubject->sGroup.c_str(), sGroup.c_str()) == 0) {
			if (strcmp(itSubject->sSubject.c_str(), sSubject.c_str()) == 0) {
				bIsFound = true;
			};
		};
	};
	
	return bIsFound;
};

#endif
