/*
 *  ClsQuizGuiLayout.h
 *  MatthTest
 *
 *  Created by Matthew Baynham on 2/5/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef CLS_QUIZ_GUI_LAYOUT_H
#define CLS_QUIZ_GUI_LAYOUT_H

#include <string>;
#include <cstddef>//std array rapper
#include <vector>
//#include <cstdlib>;
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "modMisc.h"

using namespace std;
using namespace modMisc;

class ClsQuizGuiLayout //: clsQuestion
{
public:
	//	ClsQuiz(std::string*);
	ClsQuizGuiLayout();
	ClsQuizGuiLayout(int);
	~ClsQuizGuiLayout();

	void reInitialize(int);
	enumQuizGuiLayout cellLayout(int);
	int questionNo(int);
	int totalCells();

private:
	std::vector<strQuizGuiLayout> vecLayout;
};

#endif
