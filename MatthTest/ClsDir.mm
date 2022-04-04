//
//  ClsDir.mm
//  File Test
//
//  Created by Matthew Baynham on 11/10/2010.
//  Copyright 2010 Baynham Coding. All rights reserved.
//

#ifndef CLS_DIR_MM
#define CLS_DIR_MM

#import "ClsDir.h"
#import "modMisc.h"

@implementation ClsDir

using namespace modMisc;

@synthesize sPathResults, sPathHome, sPathPreference, sPathHelp;

- (id) init {
	if ( self = [super init] ) {
		iCurrentFile = 0;
		sPathResults = @"";
		sPathHome = @"";
		[self setHomePath];
//		[self performSelectorInBackground:@selector(cleanupFiles) withObject:nil];
//        [self cleanupFiles];
	} 
	return self; 
} 

- (void)dealloc {
//	[arrFiles release];
//	[sPathResults release];
//	[sPathHome release];
//	[sPathPreference release];
//	int iCurrentFile;

    [super dealloc];
}

-(void)setHomePath {
	NSString *sPath = [[[NSString alloc] initWithString:@""] autorelease];
//	NSString *sPath = [[NSString alloc] initWithString:@""];
	bool bExistsDirResults;
	
	sPathHome = [self documentsPath];
	[sPathHome retain];

	sPath = sPathHome;
	
	const char* cDirResults;
	cDirResults = modMisc::sFolderResults.c_str();
	NSString *sDirResults = [[[NSString alloc] initWithString:[NSString stringWithCString:cDirResults length:strlen(cDirResults)]] autorelease];			
		
	[[NSFileManager defaultManager] changeCurrentDirectoryPath:sPathHome];
	
	//if the last char in a director is a /
	NSString *sTempLastChar = [[[NSString alloc] initWithString:[sPathHome substringFromIndex:([sPathHome length] - 1)]] autorelease];
	
	const char* cFileNamePreferences;
	cFileNamePreferences = modMisc::sFileNamePreferences.c_str();
	NSString *sPreferencesFileName = [[[NSString alloc] initWithString:[NSString stringWithCString:cFileNamePreferences length:strlen(cFileNamePreferences)]] autorelease];			
	
	const char* cHelpFileName;
	cHelpFileName = modMisc::sFileNameHelp.c_str();
	sPathHelp = [[[NSString alloc] initWithString:[self helpPath]] retain];			
	
	if ([sTempLastChar compare:@"/"] ==  NSOrderedSame) {
		sPath = [sPathHome stringByAppendingString:sDirResults];
		sPath = [sPath stringByAppendingString:@"/"];
		
		sPathPreference = [sPathHome stringByAppendingString:sPreferencesFileName];
	} else {
		sPath = [sPathHome stringByAppendingString:@"/"];
		sPath = [sPath stringByAppendingString:sDirResults];
		
		sPathHome = [sPathHome stringByAppendingString:@"/"];
		sPathPreference = [sPathPreference stringByAppendingString:sPreferencesFileName];
	};
	[sPathPreference retain];
	
	bExistsDirResults = [[NSFileManager defaultManager] fileExistsAtPath:sPath];
	
	if (bExistsDirResults == NO) {
		//if the results folder does not exist then create it.
		[[NSFileManager defaultManager] createDirectoryAtPath:sPath
								  withIntermediateDirectories:NO
												   attributes:NULL
														error:NULL];
	};
	
	sPathResults = sPath;
	[sPathResults retain];
	
	arrFiles = [[[NSFileManager defaultManager] contentsOfDirectoryAtPath:sPathResults error:NULL] retain];
		
//	[fmFileManager drain];
//	[sPathResults drain];
//	[sPathHome drain];
//	[sPathPreference drain];
	
}

-(NSString*) getPathResults {
	return sPathResults;
}

-(NSString*) getPathHome {
	return sPathHome;
}

-(int) getFileCounter {
	return iCurrentFile;
}

-(void) setFileCounter: (int) iCounter {
	iCurrentFile = iCounter;
}

-(int) getFileMax {
	int iTemp;
	
	iTemp = [arrFiles count];
	
	return iTemp;
}

-(NSString*) getFileName: (int) iFileNo {
	NSString *sTemp = [[[NSString alloc] initWithString:@""] autorelease];
	int iNoFiles;
	
	iNoFiles = [self getFileMax];
	
	if (iNoFiles > iFileNo) {//note:base zero so is not equal to or greater than it's just greater than
		sTemp = [[[NSString alloc] initWithString:(NSString*)[arrFiles objectAtIndex:iFileNo]] autorelease];
	} else {
		sTemp = @"Error";
	};

	NSString *sPath = [[[NSString alloc] initWithString:@""] autorelease];
	NSString *sTempLastChar = [[[NSString alloc] initWithString:[sPathResults substringFromIndex:([sPathResults length] - 1)]] autorelease];

	if (sTempLastChar != @"/") {
		sPath = [sPathResults stringByAppendingString:@"/"];
	};

	sTemp = [sPath stringByAppendingString:sTemp];
	
//	[sPath release];
//	[sTempLastChar release];

	return sTemp;
};

-(NSString*) generateQuizPath {
	[self generateQuizPath:[[NSDate alloc] init]];
};

-(NSString*) generateQuizPath:(NSDate*)dteTimeStamp {
//	NSDate *dteNow = [[NSDate alloc] init];
	NSDateFormatter *dteFmt = [[NSDateFormatter alloc] init];
	[dteFmt setDateFormat:@"yyyyMMdd HHmmss"];
	
	//Optionally for time zone converstions
	[dteFmt setTimeZone:[NSTimeZone timeZoneWithName:@"..."]];
	
	NSString *sDate = [[NSString alloc] initWithString:[dteFmt stringFromDate:dteTimeStamp]];
	NSString *sTempLastChar = [[NSString alloc] initWithString:[sPathResults substringFromIndex:([sPathResults length] - 1)]];
	NSString *sPath = [[NSString alloc] initWithString:sPathResults];
    
	if ([sTempLastChar compare:@"/"] !=  NSOrderedSame) {
		sPath = [sPath stringByAppendingString:@"/"];
	};
    
	sPath = [sPath stringByAppendingString:sDate];
	sPath = [sPath stringByAppendingString:@".db"];
    

	[dteFmt release];
	[sDate release];
	[sTempLastChar release];
//	[sPath release];

	return sPath;
};

-(bool) isFoundPreferenceFile {
	bool bFileExists;
	
	bFileExists = [[NSFileManager defaultManager] fileExistsAtPath:sPathPreference];
	
	return bFileExists;
};

-(void) deleteFile: (NSString*)sPath {
    @try {
        [[NSFileManager defaultManager] removeItemAtPath:sPath error:NULL];
    }
    @catch (NSException *exception) {
        //do nothing if the file can't be deleted then nevermind
    }
    @finally {
        //do nothing if the file can't be deleted then nevermind
    }
};

-(void) cleanupFiles {
    @try {
        //remove any empty files.  These have been deleted with the quiz view controller locking the file, so they are empty and have no data
        /*
         1) loop through all the files in arrFiles
         2) get the size of each file
         3) if the size to to small then delete file
         */
        NSString* sFileName = [[NSString alloc] initWithString:@""];
        int iArrayPos = [arrFiles count];
        float fFileSize;
        bool bDelete;
        
        [[NSFileManager defaultManager] changeCurrentDirectoryPath:sPathResults];
        
        NSString *sFileSize = [[NSString alloc] initWithString:@""];
        NSString *sFullPath = [[NSString alloc] initWithString:@""];
        NSString *sTempLastChar = [[NSString alloc] initWithString:@""];
        
        while (iArrayPos--) {
            sFileName = [[arrFiles objectAtIndex:iArrayPos] retain];
            bDelete = false;
            
            NSDictionary *faFileAttributes = [[NSFileManager defaultManager] fileAttributesAtPath:sFileName traverseLink:YES];
            
            if(faFileAttributes == nil) {
                bDelete = true;
            } else {
                sFileSize = [faFileAttributes objectForKey:NSFileSize];
                
                fFileSize = [sFileSize floatValue]; 
                
                if (fFileSize <= 8192) {
                    bDelete = true;
                };
            };
            
            if (bDelete == true) {
                sTempLastChar = [sPathResults substringFromIndex:([sPathResults length] - 1)];
                
                if (sTempLastChar != @"/") {
                    sFullPath = [sPathResults stringByAppendingString:@"/"];
                } else {
                    sFullPath = sPathResults;
                };
                
                sFullPath = [sFullPath stringByAppendingString:sFileName];
                
                [self deleteFile:sFullPath];
                
            };
            
            [faFileAttributes release];
        };
        
        //NSDate dteEnd = [[NSDate alloc] init];
        //NSDate dtePeriod = [[NSDate alloc] initWithTimeIntervalSinceNow:(NSTimeInterval)dteStart];
        //NSDate dtePeriod = [[NSDate alloc] initWithTimeIntervalSinceReferenceDate:dteStart];
        
        //NSTimeInterval tiPeriod = [[NSDate alloc] initWithTimeIntervalSinceReferenceDate:dteStart];
        //NSLog(@"cleanupFiles");
        //NSLog([NSString stringWithFormat:@"%f", tiPeriod]);
        
        [sFileSize release];
        [sFullPath release];
        [sTempLastChar release];
        [sFileName release];
        
        
        
        //[faFileAttributes release];
        //[sFileName release];
    }
    @catch (NSException *exception) {
        //do nothing if the file can't be cleaned then nevermind
    }
    @finally {
        //do nothing if the file can't be cleaned then nevermind
    };
};

-(NSString*)documentsPath {
	NSString* sTemp = [[NSString alloc] initWithString:NSHomeDirectory()];
	NSString *sTempLastChar = [[[NSString alloc] initWithString:[sTemp substringFromIndex:([sTemp length] - 1)]] autorelease];
	
	if (sTempLastChar == @"/") {
		sTemp = [sTemp stringByAppendingString:@"Documents/"];
	} else {
		sTemp = [sTemp stringByAppendingString:@"/Documents/"];
	};
	
	return sTemp;
};

-(NSString*)helpPath {
	NSString* sPath = [[NSString alloc] initWithString:[[NSBundle mainBundle] pathForResource:@"help" ofType:@"db"]];
	
	return sPath;
}

@end

#endif
