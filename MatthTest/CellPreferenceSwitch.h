//
//  CellPreferenceSwitch.h
//  Hello World Table View
//
//  Created by Matthew Baynham on 8/31/10.
//  Copyright 2010 Baynham Coding. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "ClsPreferences.h"

@interface CellPreferenceSwitch : UITableViewCell {
	UILabel *lblTitle;
	UISwitch *swhEnabled;
	ClsPreferences *cPreferences;
	std::string sStdGroup;
	std::string sStdName;
	
	UITableViewController *vCtrlParent;
}
-(void)groupEnabled:(NSString*)sGroup bEnabled:(NSNumber*)bEnabled; 

@property (nonatomic,retain) UITableViewController *vCtrlParent;
@property (nonatomic,retain) UILabel *lblTitle;
@property (nonatomic,retain) UISwitch *swhEnabled;

@end
