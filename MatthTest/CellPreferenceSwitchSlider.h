//
//  CellPreferenceSwitchSlider.h
//  Hello World Table View
//
//  Created by Matthew Baynham on 12/9/10.
//  Copyright 2010 Baynham Coding. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "ClsPreferences.h"

@interface CellPreferenceSwitchSlider : UITableViewCell {
	UILabel *lblTitle;
	UISwitch *swhEnabled;
	UISlider *sldAmount;
	UIButton *btnInfo;
	NSString *sPrefix;
	ClsPreferences *cPreferences;
	std::string sStdGroup;
	int iMaxSlider;
	
	UITableViewController *vCtrlParent;
}

-(void)groupEnabled:(NSString*)sGroup bEnabled:(NSNumber*)bEnabled; 
-(NSString*)generateTitle;
-(void)updateTitle;

@property (nonatomic,retain) UITableViewController *vCtrlParent;
@property (nonatomic,retain) UILabel *lblTitle;
@property (nonatomic,retain) UISwitch *swhEnabled;
@property (nonatomic,retain) UISlider *sldAmount;
@property (nonatomic,retain) UIButton *btnInfo;

@end
