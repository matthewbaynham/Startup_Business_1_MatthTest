//
//  CellQuestionTextAnswer.h
//  Hello World Table View
//
//  Created by Matthew Baynham on 10/3/10.
//  Copyright 2010 Baynham Coding. All rights reserved.
//

#import <UIKit/UIKit.h>
//#import "ClsGuiPreferences.h"
//#import "ClsQuiz.h"
#import "TextFieldRounded.h"
#import "modMisc.h"

@interface CellQuestionTextAnswer : UITableViewCell {
	UILabel *lblQuestion;
//	UITextField *txtAnswer;
	TextFieldRounded *txtAnswer;
	
//	ClsQuiz* cQuiz;
	int iQuestionNo;
//	ClsGuiPreferences *cGuiPreferences; 

	UITableViewController *vCtrlParent;
}

@property (nonatomic,retain) UITableViewController *vCtrlParent;

@property (nonatomic,retain) UILabel *lblQuestion;
//@property (nonatomic,retain) UITextField *txtAnswer;
@property (nonatomic,retain) TextFieldRounded *txtAnswer;

@end
