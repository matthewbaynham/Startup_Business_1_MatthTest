//
//  ViewControllerPreferencesBySubject.h
//  Hello World Table View
//
//  Created by Matthew Baynham on 10/18/10.
//  Copyright 2010 Baynham Coding. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "ClsPreferences.h"
#import "ViewControllerQuiz.h"

@interface ViewControllerPreferencesBySubject : UITableViewController {
	ClsPreferences *cPreferences;
    ViewControllerQuiz *viewControllerQuiz;
	UIActivityIndicatorView* vwActiv;
	NSTimer* tmrStartQuiz;
	UIView* vwSemiClear;
}

-(void)groupEnabled:(NSString*)sGroup bEnabled:(NSNumber*)bEnabled; 

@property(nonatomic,retain) ViewControllerQuiz *viewControllerQuiz;
@property(nonatomic,retain) UIActivityIndicatorView* vwActiv;
@property(nonatomic,retain) NSTimer* tmrStartQuiz;
@property(nonatomic,retain) UIView* vwSemiClear;

@end
