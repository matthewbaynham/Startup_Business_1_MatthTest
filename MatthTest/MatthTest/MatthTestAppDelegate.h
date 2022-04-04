//
//  MatthTestAppDelegate.h
//  MatthTest
//
//  Created by Matthew Baynham on 1/22/11.
//  Copyright 2011 Baynham Coding. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "ClsGuiPreferences.h"
#import "ClsQuiz.h"
#import "ClsDir.h"
#import "ClsHelp.h"

@interface MatthTestAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    UINavigationController *navigationController;
	ClsGuiPreferences *cGuiPreferences;
	ClsDir *cDir;
	ClsHelp *cHelp;
    //	UIActivityIndicatorView* vwActiv;
}

//ClsGuiPreferences Methods
-(UIColor*) GuiColour:(NSString*)sDesc;
//-(id) GuiColour:(NSString*)sDesc;
-(UIImage*) GuiPicture:(enumPicQues)ePictureType;
//-(id) GuiPicture:(enumPicQues)ePictureType;
-(UIFont*) GuiFont:(NSString*)sDesc;
//-(id) GuiFont:(NSString*)sDesc;

-(NSString*) sQuestionTextDecoded:(int)iQuestionNo 
							cQuiz:(ClsQuiz*)cQuiz;

-(NSString*) sAnswerMulipleChoiceDecoded:(int)iQuestionNo
								 iChoice:(int)iChoice
								   cQuiz:(ClsQuiz*)cQuiz;

//ClsDir Methods
-(void) DirReInitialise;
-(void) DirCheckPreference;
-(int) DirGetFileMax;
-(NSString*) DirGetFileName:(int)iFileNo;
-(NSString*) DirGenerateQuizPath;
-(bool) DirIsFoundPreferenceFile;
-(void) DirDeleteFile:(NSString*)sFullPath;
-(NSString*) DirDocumentsPath;

-(bool)helpMenuIsFound:(NSString*)sMenu;
-(void)helpMenu:(NSString*)sMenu;

-(bool)helpGroupIsFound:(NSString*)sGroup;
-(void)helpGroup:(NSString*)sGroup;

-(bool)helpSubjectIsFound:(NSString*)sGroup
				 sSubject:(NSString*)sSubject;
-(void)helpSubject:(NSString*)sGroup
		  sSubject:(NSString*)sSubject;

//Misc Methods
-(void)popUp:(NSString*)sMessage;

@property (nonatomic, retain) UIWindow *window;
@property (nonatomic, retain) UINavigationController *navigationController;
//@property (nonatomic, retain) UIActivityIndicatorView* vwActiv;
@property (retain) ClsDir *cDir;
//@property (retain) ClsHelp *cHelp;

@end

