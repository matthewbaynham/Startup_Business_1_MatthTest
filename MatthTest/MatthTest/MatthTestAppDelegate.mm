//
//  MatthTestAppDelegate.m
//  MatthTest
//
//  Created by Matthew Baynham on 1/22/11.
//  Copyright 2011 Baynham Coding. All rights reserved.
//

#import "RootViewController.h"
#import "MainViewController.h"
#import "ClsQuiz.h"
#import "ClsHelp.h"
#import "AdWhirlLog.h"
//#import "ClsPreferences.h"
#import "MatthTestAppDelegate.h"


@implementation MatthTestAppDelegate

@synthesize window;
@synthesize navigationController;
@synthesize cDir;
//@synthesize cHelp;


#pragma mark -
#pragma mark Application lifecycle

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {    
	cGuiPreferences = [[ClsGuiPreferences alloc] init];//replace GUI
	cDir = [[[ClsDir alloc] init] retain];
	[self DirCheckPreference];

	cHelp = new ClsHelp();
	std::string sPath = [[cDir helpPath] UTF8String];
	cHelp->setPath(sPath);
	
	
    // Override point for customization after application launch.
    
    // Add the navigation controller's view to the window and display.
    [self.window addSubview:navigationController.view];
    [self.window makeKeyAndVisible];
    
    //	vwActiv = [[[UIActivityIndicatorView alloc] initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleWhiteLarge] retain];
    //	vwActiv.center = (CGPoint)[self.window center];	
    //	[self.window addSubview:vwActiv];
	
    
	
    return YES;
}


- (void)applicationWillResignActive:(UIApplication *)application {
    /*
     Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
     Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
     */
}


- (void)applicationDidEnterBackground:(UIApplication *)application {
    /*
     Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
     If your application supports background execution, called instead of applicationWillTerminate: when the user quits.
     */
}


- (void)applicationWillEnterForeground:(UIApplication *)application {
    /*
     Called as part of  transition from the background to the inactive state: here you can undo many of the changes made on entering the background.
     */
}


- (void)applicationDidBecomeActive:(UIApplication *)application {
    /*
     Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
     */
}


- (void)applicationWillTerminate:(UIApplication *)application {
    /*
     Called when the application is about to terminate.
     See also applicationDidEnterBackground:.
     */
}


#pragma mark -
#pragma mark Memory management

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application {
    /*
     Free up as much memory as possible by purging cached data objects that can be recreated (or reloaded from disk) later.
     */
}


- (void)dealloc {
    //	[cDir release];
    //	cGuiPreferences = NULL;
	
	[navigationController release];
	[window release];
	[super dealloc];
}

#pragma mark -
#pragma mark ClsGuiPreferences delegate Methods

-(UIColor*) GuiColour:(NSString*)sDesc {
    //-(id) GuiColour:(NSString*)sDesc {
    
    UIColor *colTemp = [[[UIColor alloc] init] autorelease];
    
	if ([sDesc compare:@"text"] == NSOrderedSame) {
		colTemp = [cGuiPreferences colText];
	} else if ([sDesc compare:@"cell"] == NSOrderedSame) {
		colTemp = [cGuiPreferences colCell];
	} else if ([sDesc compare:@"boarder"] == NSOrderedSame) {
		colTemp = [cGuiPreferences colBoarder];
	} else if ([sDesc compare:@"delete"] == NSOrderedSame) {
		colTemp = [cGuiPreferences colDelete];
	} else if ([sDesc compare:@"highlight"] == NSOrderedSame) {
		colTemp = [cGuiPreferences colHighlighted];
	} else {
        colTemp = [UIColor clearColor];
    };
    
    return colTemp;
    
}

-(UIImage*) GuiPicture:(enumPicQues)ePictureType {
    //-(id) GuiPicture:(enumPicQues)ePictureType {
	return [cGuiPreferences picture:ePictureType];
}

-(UIFont*) GuiFont:(NSString*)sDesc {
    //-(id) GuiFont:(NSString*)sDesc {
	if ([sDesc compare:@"questions"] == NSOrderedSame) {
		return [cGuiPreferences fntQuestions];
	} else if ([sDesc compare:@"answers"] == NSOrderedSame) {
		return [cGuiPreferences fntAnswers];
	} else if ([sDesc compare:@"filedetails"] == NSOrderedSame) {
		return [cGuiPreferences fntFileDetails];
	} else if ([sDesc compare:@"menu"] == NSOrderedSame) {
		return [cGuiPreferences fntMenu];
	} else if ([sDesc compare:@"misc"] == NSOrderedSame) {
		return [cGuiPreferences fntMisc];
	} else if ([sDesc compare:@"title"] == NSOrderedSame) {
		return [cGuiPreferences fntTitle];
	} else if ([sDesc compare:@"baynhamcoding"] == NSOrderedSame) {
		return [cGuiPreferences fntBaynhamCoding];
	};
}

-(NSString*) sQuestionTextDecoded:(int)iQuestionNo 
							cQuiz:(ClsQuiz*)cQuiz {
	NSString *sQuestionTemp;
	sQuestionTemp = [cGuiPreferences decodeQuestion:cQuiz iQuestionNo:iQuestionNo];
	
	return sQuestionTemp;
}

-(NSString*) sAnswerMulipleChoiceDecoded:(int)iQuestionNo
								 iChoice:(int)iChoice
								   cQuiz:(ClsQuiz*)cQuiz {
	NSString *sAnswerTemp = [[[NSString alloc] initWithString:[cGuiPreferences decodeAnswer:cQuiz iQuestionNo:iQuestionNo iChoice:iChoice]] autorelease];
	
	return sAnswerTemp;
}


#pragma mark -
#pragma mark ClsDir Methods

-(void) DirReInitialise {
    //	bool bIsPrefExist;
    //	ClsPreferences* cPref;
	
    //	[cDir release];
	cDir = NULL;
	cDir = [[ClsDir alloc] init];
    //	cDir = [[[ClsDir alloc] init] retain];
	
    //	bIsPrefExist = [cDir isFoundPreferenceFile];
	
    //	if (bIsPrefExist == false) {
    //		cPref->ResetPreferences();
    //	}; 
	
    //	cPref = NULL;
};

-(void) DirCheckPreference {
	bool bIsPrefExist;
	ClsPreferences* cPref;
	
	if (cDir == NULL) {
		[self DirReInitialise];
	};
	
	bIsPrefExist = [cDir isFoundPreferenceFile];
	
	if (bIsPrefExist == false) {
		cPref = NULL;
		cPref = new ClsPreferences;
		cPref->ResetPreferences();
	} else { 
		cPref = NULL;
		cPref = new ClsPreferences;
        NSString *sPrefPath = [[NSString alloc] initWithString:[cDir documentsPath]];
        cPref->setPath([sPrefPath UTF8String]);
        cPref->CheckPreferencesSubjectGroups();
        [sPrefPath release];
    };

	cPref = NULL;
};

-(int) DirGetFileMax {
	int iResult;
    
	if (cDir == NULL) {
		[self DirReInitialise];
	};
	
	iResult = [cDir getFileMax];
	
	return iResult;
};

-(NSString*) DirGetFileName:(int)iFileNo {
	NSString* sResult = [[NSString alloc] initWithString:@""];
    
	if (cDir == NULL) {
		[self DirReInitialise];
	};
	
	sResult = [cDir getFileName:iFileNo];
	
	return sResult;
}

-(NSString*) DirGenerateQuizPath {
	NSString* sResult;
    
	if (cDir == NULL) {
		[self DirReInitialise];
	};
	
	sResult = [cDir generateQuizPath];
	
	return sResult;
}

-(NSString*) DirGenerateQuizPath:(NSDate*)dteTimeStamp {
    //	NSString* sResult = [[[NSString alloc] initWithString:@""] autorelease];
	NSString* sResult = [[NSString alloc] initWithString:@""];
    
	if (cDir == NULL) {
		[self DirReInitialise];
	};
	
	sResult = [cDir generateQuizPath:dteTimeStamp];
	
	return sResult;
}

-(bool) DirIsFoundPreferenceFile {
	bool bResult;
	
	if (cDir == NULL) {
		[self DirReInitialise];
	};
	
	bResult = [cDir isFoundPreferenceFile];
	
	return bResult;
}

-(void) DirDeleteFile:(NSString*)sFullPath {
	if (cDir == NULL) {
		[self DirReInitialise];
	};
    
	[cDir deleteFile:sFullPath];
};

-(NSString*) DirDocumentsPath {
	if (cDir == NULL) {
		[self DirReInitialise];
	};
	
	NSString* sReturn = [[NSString alloc] initWithString:[cDir documentsPath]];
	
	return sReturn;
};

#pragma mark -
#pragma mark Misc Methods

-(bool)helpMenuIsFound:(NSString*)sMenu {
	std::string stdMenu = [sMenu UTF8String];
	bool bResult = cHelp->menuHelpIsFound(stdMenu);
	return bResult;
}

-(void)helpMenu:(NSString*)sMenu {
	
	std::string stdMenu = [sMenu UTF8String];
	std::string stdMessage = cHelp->menuHelp(stdMenu);
	
	NSString *sMessage = [[[NSString alloc] initWithCString:stdMessage.c_str()] autorelease];
	NSString *sTitle = @"Info";
	NSString *sBtnCancel = @"Close";
	
	UIAlertView *msgPopUp = [[UIAlertView alloc] initWithTitle:sTitle
													   message:sMessage
													  delegate:self 
											 cancelButtonTitle:sBtnCancel 
											 otherButtonTitles:nil];
	[msgPopUp show];
	[msgPopUp release];
};

-(bool)helpGroupIsFound:(NSString*)sGroup {
	std::string stdGroup = [sGroup UTF8String];
	bool bResult = cHelp->groupHelpIsFound(stdGroup);
	return bResult;
}

-(void)helpGroup:(NSString*)sGroup {
	
	std::string stdGroup = [sGroup UTF8String];
	std::string stdMessage = cHelp->groupHelp(stdGroup);
	
	NSString *sMessage = [[[NSString alloc] initWithCString:stdMessage.c_str()] autorelease];
	NSString *sTitle = @"Info";
	NSString *sBtnCancel = @"Close";
	
	UIAlertView *msgPopUp = [[UIAlertView alloc] initWithTitle:sTitle
													   message:sMessage
													  delegate:self 
											 cancelButtonTitle:sBtnCancel 
											 otherButtonTitles:nil];
	[msgPopUp show];
	[msgPopUp release];
};

-(bool)helpSubjectIsFound:(NSString*)sGroup
				 sSubject:(NSString*)sSubject {
	std::string stdGroup = [sGroup UTF8String];
	std::string stdSubject = [sSubject UTF8String];
	bool bResult = cHelp->subjectHelpIsFound(stdGroup, stdSubject);
	return bResult;
}

-(void)helpSubject:(NSString*)sGroup
		  sSubject:(NSString*)sSubject {
	
	std::string stdGroup = [sGroup UTF8String];
	std::string stdSubject = [sSubject UTF8String];
	std::string stdMessage = cHelp->subjectHelp(stdGroup, stdSubject);
	
	NSString *sMessage = [[[NSString alloc] initWithCString:stdMessage.c_str()] autorelease];
	NSString *sTitle = @"Info";
	NSString *sBtnCancel = @"Close";
	
	UIAlertView *msgPopUp = [[UIAlertView alloc] initWithTitle:sTitle
													   message:sMessage
													  delegate:self 
											 cancelButtonTitle:sBtnCancel 
											 otherButtonTitles:nil];
	[msgPopUp show];
	[msgPopUp release];
};

-(void)popUp:(NSString*)sMessage {
	NSString *sTitle = @"Pop up";
	NSString *sBtnCancel = @"Close";
	
	UIAlertView *msgPopUp = [[UIAlertView alloc] initWithTitle:sTitle
													   message:sMessage
													  delegate:self 
											 cancelButtonTitle:sBtnCancel 
											 otherButtonTitles:nil];
	[msgPopUp show];
	[msgPopUp release];
};

@end

