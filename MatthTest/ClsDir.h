//  ClsDir.h
//  File Test
//
//  Created by Matthew Baynham on 11/10/2010.
//  Copyright 2010 Baynham Coding. All rights reserved.

#import <UIKit/UIKit.h>

#ifndef CLS_DIR_H
#define CLS_DIR_H

@interface ClsDir : NSObject {
	NSArray *arrFiles;
	NSString *sPathResults;
	NSString *sPathHome;
	NSString *sPathPreference;
	NSString *sPathHelp;
	int iCurrentFile;
}

-(void) setHomePath;
-(NSString*) getPathResults;
-(NSString*) getPathHome;
-(int) getFileCounter;
-(void) setFileCounter: (int) iCounter;
-(int) getFileMax;
-(NSString*) getFileName: (int) iFileNo;
-(NSString*) generateQuizPath;
-(NSString*) generateQuizPath:(NSDate*)dteTimeStamp;
-(bool) isFoundPreferenceFile;
-(void) deleteFile: (NSString*)sPath;
-(void) cleanupFiles;
-(NSString*)documentsPath;
//-(bool)PrefExists;
-(NSString*)helpPath;

@property(retain) NSString *sPathResults;
@property(retain) NSString *sPathHome;
@property(retain) NSString *sPathPreference;
@property(retain) NSString *sPathHelp;


@end


#endif