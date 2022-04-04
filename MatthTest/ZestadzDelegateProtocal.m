//
//  ZestadzDelegateProtocal.m
//  MatthTest
//
//  Created by Matthew Baynham on 2/4/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "ZestadzDelegateProtocal.h"


//@implementation ZestadzDelegateProtocal
//@implementation ZestadzView
@implementation ZestadzDelegate

- (NSString *)clientId {
    return @"14131C047A5040434757445C4154415F8F8A213F";//MatthTestProd
	//return @"14131C047A50414A4356415D4252465E889E29C7";//MatthTest
};

- (NSString *)keywords {  //keyword to target, e.g. "Proffessional,Doctor"
	return @"Education,Maths,Mathematics,School,Quiz,kids,learning";
};

- (UIColor *)adBackgroundColor {
	UIColor* colTemp;
	
	colTemp = [[[UIApplication sharedApplication] delegate] GuiColour:@"boarder"];
	
	return colTemp;
};

@end
