//
//  RootViewController.h
//  MatthTest
//
//  Created by Matthew Baynham on 1/22/11.
//  Copyright 2011 Baynham Coding. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "modMisc.h"
#import "ViewControllerPreferencesBySubject.h"
#import "ViewControllerPreferencesByGroup.h"
#import "ViewControllerPreferencesSimple.h"
#import "ViewControllerFiles.h"
#import "ViewControllerTestFunction.h"
#import "AdWhirlDelegateProtocol.h"
#import "ClsPreferences.h"
#import "CellBaynhamCoding.h"
//#import "ClsHelp.h"

@interface RootViewController : UIViewController <AdWhirlDelegate, UITableViewDelegate, UITableViewDataSource> {
    
    //@interface RootViewController : UITableViewController <UITableViewDelegate, UITableViewDataSource> {
	AdWhirlView *adView;
	ClsPreferences* cPref;
    //	ClsHelp cHelp;
	UIActivityIndicatorView* vwActiv;
	ViewControllerFiles *viewControllerFiles;
	ViewControllerPreferencesSimple *viewControllerPreferencesSimple;
	ViewControllerPreferencesByGroup *viewControllerPreferencesByGroup;
	ViewControllerPreferencesBySubject *viewControllerPreferencesBySubject;
	
}

@property (nonatomic,retain) ViewControllerFiles *viewControllerFiles;
@property (nonatomic,retain) ViewControllerPreferencesSimple *viewControllerPreferencesSimple;
@property (nonatomic,retain) ViewControllerPreferencesByGroup *viewControllerPreferencesByGroup;
@property (nonatomic,retain) ViewControllerPreferencesBySubject *viewControllerPreferencesBySubject;

@property (nonatomic,retain) AdWhirlView *adView;
@property (nonatomic,retain) UIActivityIndicatorView* vwActiv;
@property (nonatomic,readonly) UILabel *label;
@property (nonatomic,readonly) UITableView *table;

-(void) CheckPreference;
-(void) configMenu;
- (void)adjustAdSize;

@end
