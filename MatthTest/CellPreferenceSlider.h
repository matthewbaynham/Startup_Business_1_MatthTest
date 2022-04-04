//
//  CellPreferenceSlider.h
//  Hello World Table View
//
//  Created by Matthew Baynham on 8/31/10.
//  Copyright 2010 Baynham Coding. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "ClsPreferences.h"


@interface CellPreferenceSlider : UITableViewCell {
	UILabel *lblTitle;
	UISlider *sldAmount;
	NSString *sPrefix;
	ClsPreferences *cPreferences;
	
	UITableViewController *vCtrlParent;
}

-(void)groupEnabled:(NSString*)sGroup bEnabled:(NSNumber*)bEnabled; 
-(NSString*)generateTitle;
-(void)setValues:(float)fLimitMin
	   fLimitMax:(float)fLimitMax;

@property (nonatomic,retain) UITableViewController *vCtrlParent;
@property (nonatomic,retain) UILabel *lblTitle;
@property (nonatomic,retain) UISlider *sldAmount;

@end
