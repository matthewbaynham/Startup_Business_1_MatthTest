//
//  CellSectionTitle.h
//  Hello World Table View
//
//  Created by Matthew Baynham on 9/12/10.
//  Copyright 2010 Baynham Coding. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "ClsPreferences.h"

@interface CellSectionTitle : UITableViewCell {
	UILabel *lblTitle;
	UITableViewController *vCtrlParent;
}

@property (nonatomic,retain) UITableViewController *vCtrlParent;
@property (nonatomic,retain) UILabel *lblTitle;

@end
