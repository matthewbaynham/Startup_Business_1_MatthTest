//  ViewControllerQuiz.h
//
//  Created by Matthew Baynham on 9/26/10.
//  Copyright 2010 Baynham Coding. All rights reserved.

#import <UIKit/UIKit.h>
#import "ClsQuiz.h"
#import "ClsPreferences.h"
#import "ClsQuizGuiLayout.h"
#import "AdWhirlDelegateProtocol.h"

@interface ViewControllerQuiz : UIViewController <AdWhirlDelegate, UITableViewDelegate, UITableViewDataSource> {

//@interface ViewControllerQuiz : UITableViewController {
	ClsQuiz cQuiz;
	ClsPreferences cPreferences;
	ClsQuizGuiLayout cQuizGuiLayout;
	
	int iStartSecond;
	int iPreviousSecond;
	int iQuestionHeightMenu;
	int iQuestionHeightText;
	int iQuestionHeightMultipleChoice;
	int iQuestionHeightTextPic;
	int iQuestionHeightMultipleChoicePic;
	int iQuestionHeightAdvert;
	NSString* sStatus;
	int iMsgBoxMode;
	
	NSTimer* tTimer;
	NSDate* dteTimeStamp;
	
	AdWhirlView *adView;
	UIActivityIndicatorView* vwActiv;
};

@property(assign, readonly) int iQuestionHeightMenu;
@property(assign, readonly) int iQuestionHeightText;
@property(assign, readonly) int iQuestionHeightMultipleChoice;
@property(assign, readonly) int iQuestionHeightTextPic;
@property(assign, readonly) int iQuestionHeightMultipleChoicePic;
@property(assign, readonly) int iQuestionHeightAdvert;

@property (nonatomic,retain) AdWhirlView *adView;
@property (nonatomic,retain) UIActivityIndicatorView* vwActiv;

@property (nonatomic,readonly) UILabel *label;
@property (nonatomic,readonly) UITableView *table;
@property (nonatomic,retain) NSTimer* tTimer;
@property (nonatomic,retain) NSDate* dteTimeStamp;
@property (nonatomic,retain) NSString* sStatus;

-(void) setPref:(ClsPreferences*) cPref;
-(void) setQuiz:(NSString*) sPath;
-(void) closeTableView;
-(void) markQuiz;
-(void) refreshQuiz;

-(void) btnMultipleChoiceSelected:(int)iQuestionNo 
						  iChoice:(int)iChoice 
						bSelected:(bool)bSelected;

-(bool) btnMultipleChoiceIsSelected:(int)iQuestionNo 
 						    iChoice:(int)iChoice;

-(bool) btnMultipleChoiceIsCorrect:(int)iQuestionNo 
						   iChoice:(int)iChoice;

-(NSString*) sQuestionTextDecoded:(int)iQuestionNo;

-(NSString*) sAnswerMulipleChoiceDecoded:(int)iQuestionNo
								 iChoice:(int)iChoice;

-(NSString*) sQuizStatus;
-(NSString*) sQuestionGroup:(int)iQuestionNo;
-(NSString*) sQuestionSubject:(int)iQuestionNo;

-(bool) isOnlyOneAnswer:(int)iQuestionNo;

//-(void)startThreadUpdateHeaderTimeSplit;
-(void)updateHeaderTimeSplit;
-(void)startThreadCheckHeaderTimeSplit;
-(void)checkHeaderTimeSplit;

-(void) clickedBtnDelete;
-(void) clickedBtnMark;
-(void) clickedBtnSave;
-(void) configMenu;

- (void)adjustAdSize;

@end
