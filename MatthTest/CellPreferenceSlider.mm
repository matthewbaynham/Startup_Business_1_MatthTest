//
//  CellPreferenceSlider.mm
//  Hello World Table View
//
//  Created by Matthew Baynham on 8/31/10.
//  Copyright 2010 Baynham Coding. All rights reserved.
//

#import "CellPreferenceSlider.h"
#import "ClsPreferences.h"


@implementation CellPreferenceSlider

//@synthesize lblTitle, btnOK, sldAmount;
@synthesize lblTitle, sldAmount;

const NSString* cPrefix_Difficulty = @"Difficulty";
const NSString* cPrefix_NoOfQuestions = @"No. of Questions";

const float fTitle_PosX = 0.035;
const float fTitle_PosY = 0.1;
const float fTitle_Width = 0.9;
const float fTitle_Height = 0.4;

const float fSlider_PosX = 0.035;
const float fSlider_PosY = 0.6;
const float fSlider_Width = 0.90;
const float fSlider_Height = 0.4;

-(id)initWithStyle:(UITableViewCellStyle)style 
   reuseIdentifier:(NSString*)reuseIdentifier
	  sTitlePrefix:(NSString*)sTitlePrefix  
			 cPref:(ClsPreferences*)cPref  
		 nLimitMin:(NSNumber*)nLimitMin
		 nLimitMax:(NSNumber*)nLimitMax {

    if ((self = [super initWithStyle:style reuseIdentifier:reuseIdentifier])) {

		cPreferences = cPref;
		sPrefix = sTitlePrefix;
		
        // Initialization code
		float fWidth;
		float fHeight;

		fWidth = self.contentView.bounds.size.width;
		fHeight = self.contentView.bounds.size.height;

		CGRect frmSlider = CGRectMake(fWidth * fSlider_PosX, fHeight * fSlider_PosY, fWidth * fSlider_Width, fHeight * fSlider_Height);
		self.sldAmount = [[UISlider alloc] initWithFrame:frmSlider];
		[self.sldAmount addTarget:self action:@selector(updateTitle) forControlEvents:UIControlEventValueChanged];
			
		// in case the parent view draws with a custom color or gradient, use a transparent color
		self.sldAmount.backgroundColor = [UIColor clearColor];
		self.sldAmount.minimumValue = [nLimitMin floatValue];
		self.sldAmount.maximumValue = [nLimitMax floatValue];
		self.sldAmount.continuous = YES;


		if (sTitlePrefix == cPrefix_Difficulty) {
			self.sldAmount.value = cPreferences->getDifficulty();
		} else if (sTitlePrefix == cPrefix_NoOfQuestions) {
			self.sldAmount.value = cPreferences->getNoOfQuestions();
		} else {
			self.sldAmount.value = 0;
		};

		// Add an accessibility label that describes the slider.
		[self.sldAmount setAccessibilityLabel:NSLocalizedString(@"StandardSlider", @"")];
		[self.contentView addSubview:self.sldAmount];
		
		CGRect frmTitle = CGRectMake(fWidth * fTitle_PosX, fHeight * fTitle_PosY, fWidth * fTitle_Width, fHeight * fTitle_Height);
		self.lblTitle = [[UILabel alloc] initWithFrame:frmTitle];
		self.lblTitle.text = [self generateTitle];
		self.lblTitle.textColor = [[[UIApplication sharedApplication] delegate] GuiColour:@"text"];
		self.lblTitle.font = [[[UIApplication sharedApplication] delegate] GuiFont:@"title"];
		[self.lblTitle setBackgroundColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"cell"]];
		[self.contentView addSubview:self.lblTitle];
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
	CGRect frmTitle = CGRectMake(fWidth * fTitle_PosX, fHeight * fTitle_PosY, fWidth * fTitle_Width, fHeight * fTitle_Height);
	CGRect frmSlider = CGRectMake(fWidth * fSlider_PosX, fHeight * fSlider_PosY, fWidth * fSlider_Width, fHeight * fSlider_Height);
	
	self.lblTitle.frame = frmTitle;
	self.sldAmount.frame = frmSlider;
}

- (void)setSelected:(BOOL)selected animated:(BOOL)animated {

    [super setSelected:selected animated:animated];

    // Configure the view for the selected state
}

-(NSString*)generateTitle {
	CGFloat fAmount;
	
	fAmount = self.sldAmount.value;
	
	NSString *sText = [[NSString alloc] initWithString:[NSString stringWithFormat:@"%@ : %i", sPrefix, (int)fAmount]];
	
	return sText;
}

-(void)updateTitle {
	if (self->sPrefix == cPrefix_Difficulty){
		float fDifficulty;
		
		fDifficulty = self.sldAmount.value;
		
		cPreferences->setDifficulty(fDifficulty);
		
		self.lblTitle.text = [self generateTitle];
	}else if (self->sPrefix == cPrefix_NoOfQuestions) {
		int iNoOfQuestions;
		
		iNoOfQuestions = self.sldAmount.value;
		
		cPreferences->setNoOfQuestions(iNoOfQuestions);
		
		self.lblTitle.text = [self generateTitle];
	} else {
		self.sldAmount.value = 0;
	};
	
}

- (void)viewDidUnload {
    // Relinquish ownership of anything that can be recreated in viewDidLoad or on demand.
    // For example: self.myOutlet = nil;
	//	[tableview autorelease];
	[self release];
}

- (void)dealloc {
	[lblTitle release];
	[sldAmount release];
	[sPrefix release];
	cPreferences = NULL;
	
//	[vCtrlParent release];
	
	
    [super dealloc];
};

- (void)setParent:(UITableViewController*) vCtrlParentTemp {
	vCtrlParent = vCtrlParentTemp;
};

-(void)groupEnabled:(NSString*)sGroup bEnabled:(NSNumber*)bEnabled {
	
};

@end
