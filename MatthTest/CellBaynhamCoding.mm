//
//  CellBaynhamCoding.mm
//  MatthTest
//
//  Created by Matthew Baynham on 3/18/11.
//  Copyright 2011 Baynham Coding. All rights reserved.
//

#import "CellBaynhamCoding.h"


@implementation CellBaynhamCoding

@synthesize lblBaynhamCoding, btnInfo;

const float fBaynhamCoding_PosX = 0.10;
const float fBaynhamCoding_PosY = 0.05;
const float fBaynhamCoding_Width = 0.70;
const float fBaynhamCoding_Height = 0.8;

const float fInfo_PosX = 0.85;
const float fInfo_PosY = 0.05;
const float fInfo_Width = 0.1;
const float fInfo_Height = 0.90;

- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier
{
    self = [super initWithStyle:style reuseIdentifier:reuseIdentifier];
    if (self) {
        float fWidth;
        float fHeight;
        
		fWidth = self.contentView.bounds.size.width;
		fHeight = self.contentView.bounds.size.height;
		
		// Initialization code
		CGRect frmBaynhamCoding = CGRectMake(fWidth * fBaynhamCoding_PosX, fHeight * fBaynhamCoding_PosY, fWidth * fBaynhamCoding_Width, fHeight * fBaynhamCoding_Height);
		
		self.lblBaynhamCoding = [[UILabel alloc] initWithFrame:frmBaynhamCoding];
		self.lblBaynhamCoding.text = @"baynham coding";
        [self.lblBaynhamCoding setTextAlignment:UITextAlignmentCenter]; 
		self.lblBaynhamCoding.textColor = [[[UIApplication sharedApplication] delegate] GuiColour:@"text"];
		self.lblBaynhamCoding.font = [[[UIApplication sharedApplication] delegate] GuiFont:@"baynhamcoding"];
		[self.lblBaynhamCoding setBackgroundColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"cell"]];
		[self.contentView addSubview:self.lblBaynhamCoding];

		NSString* sMenu = [[[NSString alloc] initWithString:@"baynham coding"] autorelease];
		bool bHasHelp = [[[UIApplication sharedApplication] delegate] helpMenuIsFound:sMenu];
		if (bHasHelp == true) {
			CGRect frmInfo = CGRectMake(fWidth * fInfo_PosX, fHeight * fInfo_PosY, fWidth * fInfo_Width, fHeight * fInfo_Height);
			self.btnInfo = [UIButton buttonWithType:UIButtonTypeInfoLight];
			self.btnInfo.frame = frmInfo;
			[self.btnInfo addTarget:self action:@selector(infoButtonPressed) 
				   forControlEvents:UIControlEventTouchUpInside];    
 			[self.contentView addSubview:self.btnInfo];
		};
    }
    return self;
}

- (void)drawRect:(CGRect)rect
{
	float fWidth;
	float fHeight;
	
	fWidth = self.contentView.bounds.size.width;
	fHeight = self.contentView.bounds.size.height;
	
	// Initialization code
	CGRect frmBaynhamCoding = CGRectMake(fWidth * fBaynhamCoding_PosX, fHeight * fBaynhamCoding_PosY, fWidth * fBaynhamCoding_Width, fHeight * fBaynhamCoding_Height);
	
	self.lblBaynhamCoding.frame = frmBaynhamCoding;
	
	[self.lblBaynhamCoding setTextAlignment:UITextAlignmentCenter];
	[self.lblBaynhamCoding setBaselineAdjustment:UIBaselineAdjustmentAlignCenters];
    
    CGRect frmInfo = CGRectMake(fWidth * fInfo_PosX, fHeight * fInfo_PosY, fWidth * fInfo_Width, fHeight * fInfo_Height);
    self.btnInfo.frame = frmInfo;

}

- (void)setSelected:(BOOL)selected animated:(BOOL)animated
{
    [super setSelected:selected animated:animated];

    // Configure the view for the selected state
}

- (void)dealloc
{
    [self.lblBaynhamCoding release];
    
    [super dealloc];
}

-(void)infoButtonPressed {
	NSString* sMenu = [[[NSString alloc] initWithString:@"baynham coding"] autorelease];
	[[[UIApplication sharedApplication] delegate] helpMenu:sMenu];
}

@end
