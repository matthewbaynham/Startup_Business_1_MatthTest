//  TextFieldRounded.mm
//  Hello World Table View
//
//  Created by Matthew Baynham on 12/5/10.
//  Copyright 2010 Baynham Coding. All rights reserved.

#import "TextFieldRounded.h"

@implementation TextFieldRounded

- (CGRect)editingRectForBounds:(CGRect)rect{
	CGFloat newX = rect.size.width * 0.05;    
    CGFloat newY = rect.size.height * 0.15; 
    CGFloat newWidth = rect.size.width * 0.95;    
    CGFloat newHeight = rect.size.height * 0.85; 
	
    CGRect newRect = CGRectMake(newX, newY, newWidth, newHeight);
	return newRect;
};

- (CGRect)textRectForBounds:(CGRect)bounds {
	CGFloat newX = bounds.size.width * 0.05;    
	CGFloat newY = bounds.size.height * 0.15; 
	CGFloat newWidth = bounds.size.width * 0.95;    
	CGFloat newHeight = bounds.size.height * 0.85; 

	CGRect newRect = CGRectMake(newX, newY, newWidth, newHeight);
	return newRect;
};

- (void)dealloc {
    [super dealloc];
}


@end
