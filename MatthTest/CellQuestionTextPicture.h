//
//  CellQuestionTextPicture.h
//  Hello World Table View
//
//  Created by Matthew Baynham on 12/2/10.
//  Copyright 2010 Baynham Coding. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "MatthTestAppDelegate.h"
//#import "ClsGuiPreferences.h"
//#import "ClsQuiz.h"
#import "TextFieldRounded.h"
#import "modMisc.h"


@interface CellQuestionTextPicture : UITableViewCell {
	UILabel *lblQuestion;
//	UITextField *txtAnswer;
	TextFieldRounded *txtAnswer;
//	UIImage *imgPic;
	UIImageView *imgVwPic;
//	ClsQuiz* cQuiz;
	int iQuestionNo;
//	ClsGuiPreferences *cGuiPreferences; 

	UITableViewController *vCtrlParent;
}

@property (nonatomic,retain) UITableViewController *vCtrlParent;

//@property (nonatomic,retain) UIImage *imgPic;
@property (nonatomic,retain) UIImageView *imgVwPic;
@property (nonatomic,retain) UILabel *lblQuestion;
//@property (nonatomic,retain) UITextField *txtAnswer;
@property (nonatomic,retain) TextFieldRounded *txtAnswer;

@end
