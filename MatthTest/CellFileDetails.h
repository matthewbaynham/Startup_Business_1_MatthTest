//
//  CellFileDetails.h
//  Hello World Table View
//
//  Created by Matthew Baynham on 10/16/10.
//  Copyright 2010 Baynham Coding. All rights reserved.


#import <UIKit/UIKit.h>
#import "ClsQuiz.h"

@interface CellFileDetails : UITableViewCell {
	UILabel *lblDateTime;
	ClsQuiz cQuiz;
	int iFileNo;
	bool bIsComplete;
	NSString *sFullPath;
	
	UIButton *btnDelete;
	UIButton *btnView;
	UILabel *lblTimeTaken;
//	UILabel *lblDateTimeStart;//date time quiz started
	UILabel *lblStatus;//Status of the quiz i.e. finished, started 
	UILabel *lblQuestionsTotal;//total number of questions
	UILabel *lblQuestionsResults;//if status = finished this is the no of correct question, if status = not finished this is the no of question completed

	UILabel *lblTimeTakenValue;
	UILabel *lblDateTimeStartValue;//date time quiz started
	UILabel *lblStatusValue;//Status of the quiz i.e. finished, started 
	UILabel *lblQuestionsTotalValue;//total number of questions
	UILabel *lblQuestionsResultsValue;//if status = finished this is the no of correct question, if status = not finished this is the no of question completed
	
	UITableViewController *vCtrlParent;
}


-(void)setParent:(UITableViewController*)vCtrlParentTemp;
-(NSString*)sQuizStatus;
-(void)setFile:(NSString*)sPath
   iFileNumber:(int)iFileNumber;

@property (nonatomic,retain) UILabel *lblDateTime;
@property (nonatomic,retain) UIButton *btnDelete;
@property (nonatomic,retain) UIButton *btnView;

@property (nonatomic,retain) UILabel *lblTimeTaken;
@property (nonatomic,retain) UILabel *lblStatus; //Status of the quiz i.e. finished, started 
@property (nonatomic,retain) UILabel *lblQuestionsTotal; //total number of questions
@property (nonatomic,retain) UILabel *lblQuestionsResults; //if status = finished this is the no of correct question, if status = not finished this is the no of question completed

@property (nonatomic,retain) UILabel *lblTimeTakenValue;
@property (nonatomic,retain) UILabel *lblDateTimeStartValue; //date time quiz started
@property (nonatomic,retain) UILabel *lblStatusValue; //Status of the quiz i.e. finished, started 
@property (nonatomic,retain) UILabel *lblQuestionsTotalValue; //total number of questions
@property (nonatomic,retain) UILabel *lblQuestionsResultsValue; //if status = finished this is the no of correct question, if status = not finished this is the no of question completed

//@property(nonatomic,retain) ViewControllerQuiz *viewControllerQuiz;
@property (nonatomic,retain) UITableViewController *vCtrlParent;


@end

//add buttons 
//restart / view details (depending on if the quiz is finished)
//Delete results (follwed by the "Are you sure?" message)
//Total time taken so far (can come in different segements, viewing the quiz will show the detail)
//Questions answered
//Total number of Questions
//Questions correct
//date started.
//name??? not sure have to change the quiz so that the name is entered, definitely do this for the iPad
