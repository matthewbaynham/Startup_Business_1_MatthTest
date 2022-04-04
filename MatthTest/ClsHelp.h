/*
 *  ClsHelp.h
 *  MatthTest
 *
 *  Created by Matthew Baynham on 3/11/11.
 *  Copyright 2011 Baynham Coding. All rights reserved.
 *
 */

#ifndef CLS_HELP_H
#define CLS_HELP_H

#include <vector>
#include <string>
#include <cstddef>//std array rapper
#include <sqlite3.h>;
#include <stdlib.h>;
#include <time.h>
#include <sstream>
#include <math.h>
#include "modMisc.h"

using namespace std;
using namespace modMisc;

//const std::string gcsFullPath = "/Code/xCode/Hello World Table View/Hello World Table View/Preferences.db";

class ClsHelp {
private:
	std::string sPathHelp;
	std::vector<strHelpMenu> vecMenu;
	std::vector<strHelpGroup> vecGroup;
	std::vector<strHelpSubject> vecSubject;
	sqlite3 *db;
	bool bDbIsOpen;
	
public:
	ClsHelp();
	~ClsHelp();
	void setPath(std::string);
	void loadData();
	void loadDataGroups();
	void loadDataMenu();
	void loadDataSubject();
	std::string groupHelp(std::string);
	bool groupHelpIsFound(std::string);
	std::string subjectHelp(std::string, std::string);
	bool subjectHelpIsFound(std::string, std::string);
	std::string menuHelp(std::string);
	bool menuHelpIsFound(std::string);
};

#endif
