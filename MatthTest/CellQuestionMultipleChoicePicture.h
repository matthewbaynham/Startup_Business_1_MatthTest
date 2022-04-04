//
//  CellQuestionMultipleChoicePicture.h
//  Hello World Table View
//
//  Created by Matthew Baynham on 12/6/10.
//  Copyright 2010 Baynham Coding. All rights reserved.
//

#import "MatthTestAppDelegate.h"
#import <UIKit/UIKit.h>
#import "modMisc.h"
//#import "ClsQuiz.h"
//#import "ClsQuiz.h"
//#import "ClsGuiPreferences.h"
//#import "ButtonAnswer.h"


@interface CellQuestionMultipleChoicePicture : UITableViewCell {
//	ClsGuiPreferences* cGuiPreferences; 
	//	ClsQuiz* cQuiz;
	
	int iQuestionNo;
	
	bool bOnlyOneAnswer;
	bool bBtnSelected1;
	bool bBtnSelected2;
	bool bBtnSelected3;
	bool bBtnSelected4;
	bool bBtnSelected5;
	bool bBtnCorrect1;
	bool bBtnCorrect2;
	bool bBtnCorrect3;
	bool bBtnCorrect4;
	bool bBtnCorrect5;
	
//	UIImage *imgPic;
	UIImageView *imgVwPic;
	UILabel *lblQuestion;
	
	UIButton *btnAnswer1;
	UIButton *btnAnswer2;
	UIButton *btnAnswer3;
	UIButton *btnAnswer4;
	UIButton *btnAnswer5;
	
	UITableViewController *vCtrlParent;

	std::string stdGroup;
	std::string stdSubject;
}

-(void)buttonPressed1;
-(void)buttonPressed2;
-(void)buttonPressed3;
-(void)buttonPressed4;
-(void)buttonPressed5;

@property (nonatomic,retain) UITableViewController *vCtrlParent;

//@property (nonatomic,retain) UIImage *imgPic;
@property (nonatomic,retain) UIImageView *imgVwPic;
@property (nonatomic,retain) UILabel *lblQuestion;
/*
 @property (nonatomic,retain) UILabel *lblAnswer1;
 @property (nonatomic,retain) UILabel *lblAnswer2;
 @property (nonatomic,retain) UILabel *lblAnswer3;
 @property (nonatomic,retain) UILabel *lblAnswer4;
 @property (nonatomic,retain) UILabel *lblAnswer5;
 */
@property (nonatomic,retain) UIButton *btnAnswer1;
@property (nonatomic,retain) UIButton *btnAnswer2;
@property (nonatomic,retain) UIButton *btnAnswer3;
@property (nonatomic,retain) UIButton *btnAnswer4;
@property (nonatomic,retain) UIButton *btnAnswer5;
@property (nonatomic,retain) UIButton *btnInfo;

@property (nonatomic, readonly) int iQuestionNo;
@property (nonatomic, readonly) bool bOnlyOneAnswer;
@property (nonatomic, readonly) bool bBtnSelected1;
@property (nonatomic, readonly) bool bBtnSelected2;
@property (nonatomic, readonly) bool bBtnSelected3;
@property (nonatomic, readonly) bool bBtnSelected4;
@property (nonatomic, readonly) bool bBtnSelected5;

@property (nonatomic, readonly) bool bBtnCorrect1;
@property (nonatomic, readonly) bool bBtnCorrect2;
@property (nonatomic, readonly) bool bBtnCorrect3;
@property (nonatomic, readonly) bool bBtnCorrect4;
@property (nonatomic, readonly) bool bBtnCorrect5;

-(void)infoButtonPressed;

@end
