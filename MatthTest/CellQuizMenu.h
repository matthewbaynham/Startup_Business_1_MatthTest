//
//  CellQuizMenu.h
//  Hello World Table View
//
//  Created by Matthew Baynham on 11/19/10.
//  Copyright 2010 Baynham Coding. All rights reserved.
//

#import <UIKit/UIKit.h>
//#import "ClsDir.h"
//#import "ClsQuiz.h"
//#import "ClsGuiPreferences.h"
#import "modMisc.h"


@interface CellQuizMenu : UITableViewCell {
	// interface
//	ClsGuiPreferences *cGuiPreferences; 
//	ClsQuiz* cQuiz;

	NSString *sFullPath;
	UILabel *lblTotalTimeTaken;//need to have other smaller cells giving a breakdow of all the time periods
	UILabel *lblScore;
	UIButton *btnFinished;
	UIButton *btnSave;
	UIButton *btnDelete;
	UITableViewController *vCtrlParent;
}

- (void)setParent:(UITableView*) vCtrlParentTemp;
//- (void)setGuiPref:(ClsGuiPreferences*) cGuiPref;

@property (nonatomic,retain) UITableViewController *vCtrlParent;

@property (nonatomic,retain) NSString *sFullPath;
@property (nonatomic,retain) UILabel *lblTotalTimeTaken;//need to have other smaller cells giving a breakdow of all the time periods
@property (nonatomic,retain) UILabel *lblScore;
@property (nonatomic,retain) UIButton *btnFinished;
@property (nonatomic,retain) UIButton *btnSave;
@property (nonatomic,retain) UIButton *btnDelete;

@end
