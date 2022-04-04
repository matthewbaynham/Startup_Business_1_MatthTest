//
//  CellPreferenceSwitchSlider.mm
//  Hello World Table View
//
//  Created by Matthew Baynham on 12/9/10.
//  Copyright 2010 Baynham Coding. All rights reserved.
//

#import "CellPreferenceSwitchSlider.h"
#import "ClsPreferences.h"


@implementation CellPreferenceSwitchSlider

@synthesize lblTitle, swhEnabled, sldAmount, btnInfo;

const float fTitle_PosX = 0.035;
const float fTitle_PosY = 0.03;
const float fTitle_Width = 0.96;
const float fTitle_Height = 0.55;

const float fSwitch_PosX = 0.035;
const float fSwitch_PosY = 0.6;
const float fSwitch_Width = 0.40;
const float fSwitch_Height = 0.40;

const float fSlider_PosX = 0.45;
const float fSlider_PosY = 0.6;
const float fSlider_Width = 0.50;
const float fSlider_Height = 0.4;

const float fInfo_PosX = 0.35;
const float fInfo_PosY = 0.6;
const float fInfo_Width = 0.1;
const float fInfo_Height = 0.4;

- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier 
		 sGroupName:(NSString*)sGroupName 
			  cPref:(ClsPreferences*)cPref  
		  nLimitMin:(NSNumber*)nLimitMin
		  nLimitMax:(NSNumber*)nLimitMax { 
	
	bool bIsEnabled;
	
	cPreferences = cPref;
	
    if ((self = [super initWithStyle:style reuseIdentifier:reuseIdentifier])) {
        // Initialization code
		
		cPreferences = cPref;
		sPrefix = [[[NSString alloc] initWithString:sGroupName] retain];
		
        // Initialization code
		float fWidth;
		float fHeight;
		
		fWidth = self.contentView.bounds.size.width;
		fHeight = self.contentView.bounds.size.height;
		
		CGRect frmSwitch = CGRectMake(fWidth * fSwitch_PosX, fHeight * fSwitch_PosY, fWidth * fSwitch_Width, fHeight * fSwitch_Height);
		
		self.swhEnabled = [[UISwitch alloc] initWithFrame:frmSwitch];
		[self.swhEnabled addTarget:self action:@selector(updateSwitch) forControlEvents:UIControlEventValueChanged];
		
		// in case the parent view draws with a custom color or gradient, use a transparent color
		self.swhEnabled.backgroundColor = [UIColor clearColor];
		
		sStdGroup = [sGroupName UTF8String];
		
		bIsEnabled = cPreferences->getSubjectEnabled(&sStdGroup);
		
		if (bIsEnabled) {
			self.swhEnabled.on = YES;
		} else {
			self.swhEnabled.on = NO;
		};
		
		
		// Add an accessibility label that describes the slider.
		[self.contentView addSubview:self.swhEnabled];
		
		
		
		CGRect frmSlider = CGRectMake(fWidth * fSlider_PosX, fHeight * fSlider_PosY, fWidth * fSlider_Width, fHeight * fSlider_Height);
		self.sldAmount = [[UISlider alloc] initWithFrame:frmSlider];
		[self.sldAmount addTarget:self action:@selector(updateSlider) forControlEvents:UIControlEventValueChanged];
		
		// in case the parent view draws with a custom color or gradient, use a transparent color
		self.sldAmount.backgroundColor = [UIColor clearColor];
		self.sldAmount.minimumValue = [nLimitMin floatValue];
		self.sldAmount.maximumValue = [nLimitMax floatValue];
		iMaxSlider = [nLimitMax intValue];
		self.sldAmount.continuous = NO;
		
		self.sldAmount.value = cPreferences->getDifficulty(&sStdGroup);
		
		// Add an accessibility label that describes the slider.
		[self.sldAmount setAccessibilityLabel:NSLocalizedString(@"StandardSlider", @"")];
		[self.contentView addSubview:self.sldAmount];
		

		
		//Title
		CGRect frmTitle = CGRectMake(fWidth * fTitle_PosX, fHeight * fTitle_PosY, fWidth * fTitle_Width, fHeight * fTitle_Height);
		self.lblTitle = [[UILabel alloc] initWithFrame:frmTitle];
//		self.lblTitle.text = [self generateTitle];//????????????????????????????????
		self.lblTitle.textColor = [[[UIApplication sharedApplication] delegate] GuiColour:@"text"];
		self.lblTitle.font = [[[UIApplication sharedApplication] delegate] GuiFont:@"title"];
		[self.lblTitle setBackgroundColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"cell"]];
		[self.contentView addSubview:self.lblTitle];

		NSString* sGroup = [[[NSString alloc] initWithCString:sStdGroup.c_str()] autorelease];
		bool bHasHelp = [[[UIApplication sharedApplication] delegate] helpGroupIsFound:sGroup];
		if (bHasHelp == true) {
			CGRect frmInfo = CGRectMake(fWidth * fInfo_PosX, fHeight * fInfo_PosY, fWidth * fInfo_Width, fHeight * fInfo_Height);
			self.btnInfo = [UIButton buttonWithType:UIButtonTypeInfoLight];
			self.btnInfo.frame = frmInfo;
			[self.btnInfo addTarget:self action:@selector(infoButtonPressed) 
				   forControlEvents:UIControlEventTouchUpInside];    
 			[self.contentView addSubview:self.btnInfo];
		};
	};
	
    return self;
}

- (void)drawRect:(CGRect)rect
{
	float fWidth;
	float fHeight;
	bool bIsEnabled;

	fWidth = self.contentView.bounds.size.width;
	fHeight = self.contentView.bounds.size.height;
	
	// Initialization code
	CGRect frmTitle = CGRectMake(fWidth * fTitle_PosX, fHeight * fTitle_PosY, fWidth * fTitle_Width, fHeight * fTitle_Height);
	CGRect frmSlider = CGRectMake(fWidth * fSlider_PosX, fHeight * fSlider_PosY, fWidth * fSlider_Width, fHeight * fSlider_Height);
	CGRect frmSwitch = CGRectMake(fWidth * fSwitch_PosX, fHeight * fSwitch_PosY, fWidth * fSwitch_Width, fHeight * fSwitch_Height);

	self.lblTitle.frame = frmTitle;
	self.lblTitle.numberOfLines = 0;

	self.sldAmount.frame = frmSlider;
	self.swhEnabled.frame = frmSwitch;
	
	self.sldAmount.value = cPreferences->getDifficulty(&sStdGroup);

	bIsEnabled = cPreferences->getGroupEnabled(&sStdGroup);
	
	if (bIsEnabled == true) {
		self.swhEnabled.on = YES;
	} else {
		self.swhEnabled.on = NO;
	};

	self.lblTitle.text = [self generateTitle];//????????????????????????????????

	NSString* sGroup = [[[NSString alloc] initWithCString:sStdGroup.c_str()] autorelease];
	bool bHasHelp = [[[UIApplication sharedApplication] delegate] helpGroupIsFound:sGroup];
	if (bHasHelp == true) {
		CGRect frmInfo = CGRectMake(fWidth * fInfo_PosX, fHeight * fInfo_PosY, fWidth * fInfo_Width, fHeight * fInfo_Height);
		self.btnInfo.frame = frmInfo;
	};
	
};

- (void)setSelected:(BOOL)selected animated:(BOOL)animated {

    [super setSelected:selected animated:animated];

    // Configure the view for the selected state
}

- (void)viewDidUnload {
    // Relinquish ownership of anything that can be recreated in viewDidLoad or on demand.
    // For example: self.myOutlet = nil;
	//	[tableview autorelease];
	[self release];
}

- (void)dealloc {
	[lblTitle release];
	[swhEnabled release];
	[sldAmount release];
	[sPrefix release];
	cPreferences = NULL;
	sStdGroup.clear();
//	int iMaxSlider;
	
//	UITableViewController *vCtrlParent;

    [super dealloc];
}

- (void)setParent:(UITableViewController*) vCtrlParentTemp {
	vCtrlParent = vCtrlParentTemp;
}

-(NSString*)generateTitle {
	NSString* sText = [[NSString alloc] initWithString:[sPrefix stringByAppendingString:@": "]];
	
	if (self.swhEnabled.on == NO) {
		sText = [sText stringByAppendingString: @"Disabled"];
	} else {
		switch (iMaxSlider) {
			case 3:
				switch ((int)self.sldAmount.value) {
					case 0:
						sText = [sText stringByAppendingString: @"Disabled"];
						break;
					case 1:
						sText = [sText stringByAppendingString: @"Easy"];
						break;
					case 2:
						sText = [sText stringByAppendingString: @"Medium"];
						break;
					case 3:
						sText = [sText stringByAppendingString: @"Hard"];
						break;
					default:
						break;
				};
				break;
			case 5:
				switch ((int)self.sldAmount.value) {
					case 0:
						sText = [sText stringByAppendingString: @"Disabled"];
						break;
					case 1:
						sText = [sText stringByAppendingString: @"Very Easy"];
						break;
					case 2:
						sText = [sText stringByAppendingString: @"Easy"];
						break;
					case 3:
						sText = [sText stringByAppendingString: @"Medium"];
						break;
					case 4:
						sText = [sText stringByAppendingString: @"Hard"];
						break;
					case 5:
						sText = [sText stringByAppendingString: @"Very Hard"];
						break;
					default:
						break;
				};
				break;
			default:
				break;
		};
	};

	return sText;
};

-(void)updateTitle{
	bool bIsEnabled;

	bIsEnabled = cPreferences->getGroupEnabled(&sStdGroup);

	if (bIsEnabled == true) {
		self.swhEnabled.on = YES;
	} else {
		self.swhEnabled.on = NO;
	};
	
	self.lblTitle.text = [self generateTitle];
}


-(void)updateSwitch {
	bool bIsEnabled;

	if (self.swhEnabled.on == YES) {
		bIsEnabled = true;
		self.sldAmount.enabled = YES; 
	} else {
		bIsEnabled = false;
		self.sldAmount.enabled = NO; 
	};

	self.lblTitle.text = [self generateTitle];
	
	cPreferences->setGroupEnabled(bIsEnabled, &sStdGroup);
	
	const char* cGroup = sStdGroup.c_str();
	NSString* sGroup = [[NSString alloc] initWithCString:cGroup];
	NSNumber* nEnabled = [[NSNumber alloc] initWithBool:bIsEnabled];
	
	[vCtrlParent groupEnabled:sGroup bEnabled:nEnabled];
	
	cGroup = NULL;
	[sGroup release];
	[nEnabled autorelease];
}

-(void)updateSlider {
	NSString* sTemp = [[[NSString alloc] initWithString:[self generateTitle]] autorelease];
		
	self.lblTitle.text = sTemp;

	cPreferences->setDifficulty(self.sldAmount.value, &sStdGroup);
	
//	[sTemp autorelease];
}

-(void)groupEnabled:(NSString*)sGroup bEnabled:(NSNumber*)bEnabled {

}; 

-(void)infoButtonPressed {
	NSString* sGroup = [[[NSString alloc] initWithCString:sStdGroup.c_str()] autorelease];
	[[[UIApplication sharedApplication] delegate] helpGroup:sGroup];
}

@end
