//
//  CellPreferenceSwitch.mm
//  Hello World Table View
//
//  Created by Matthew Baynham on 8/31/10.
//  Copyright 2010 Baynham Coding. All rights reserved.
//

#import "CellPreferenceSwitch.h"
#import "ClsPreferences.h"


@implementation CellPreferenceSwitch

@synthesize lblTitle, swhEnabled;

const float fTitle_PosX = 0.035;
const float fTitle_PosY = 0.04;
const float fTitle_Width = 0.55;
const float fTitle_Height = 0.8;

const float fSwitch_PosX = 0.65;
const float fSwitch_PosY = 0.1;
const float fSwitch_Width = 0.75;
const float fSwitch_Height = 0.8;

- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier 
													 sGroupName:(NSString*)sGroupName 
														  sName:(NSString*)sName 
														  cPref:(ClsPreferences*)cPref { 

	float fWidth;
	float fHeight;
	bool bIsEnabled;

	cPreferences = cPref;
	
    if ((self = [super initWithStyle:style reuseIdentifier:reuseIdentifier])) {
		fWidth = self.contentView.bounds.size.width;
		fHeight = self.contentView.bounds.size.height;

		// Initialization code
		CGRect frmSwitch = CGRectMake(fWidth * fSwitch_PosX, fHeight * fSwitch_PosY, fWidth * fSwitch_Width, fHeight * fSwitch_Height);
		
		self.swhEnabled = [[UISwitch alloc] initWithFrame:frmSwitch];
		[self.swhEnabled addTarget:self action:@selector(updateTitle) forControlEvents:UIControlEventValueChanged];
		
		// in case the parent view draws with a custom color or gradient, use a transparent color
		self.swhEnabled.backgroundColor = [UIColor clearColor];

		sStdGroup = [sGroupName UTF8String];
		sStdName = [sName UTF8String];

		bIsEnabled = cPreferences->getSubjectEnabled(&sStdGroup,&sStdName);
		
		if (bIsEnabled == true) {
			self.swhEnabled.on = YES;
		} else {
			self.swhEnabled.on = NO;
		};

		self.swhEnabled.enabled = cPreferences->getGroupEnabled(&sStdGroup);
		
		// Add an accessibility label that describes the slider.
		[self.contentView addSubview:self.swhEnabled];

		CGRect frmTitle = CGRectMake(fWidth * fTitle_PosX, fHeight * fTitle_PosY, fWidth * fTitle_Width, fHeight * fTitle_Height);
		
		self.lblTitle = [[UILabel alloc] initWithFrame:frmTitle];
		self.lblTitle.text = sName;
		self.lblTitle.textColor = [[[UIApplication sharedApplication] delegate] GuiColour:@"text"];
		self.lblTitle.font = [[[UIApplication sharedApplication] delegate] GuiFont:@"menu"];
		[self.lblTitle setBackgroundColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"cell"]];
		[self.contentView addSubview:self.lblTitle];
    }
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
	CGRect frmSwitch = CGRectMake(fWidth * fSwitch_PosX, fHeight * fSwitch_PosY, fWidth * fSwitch_Width, fHeight * fSwitch_Height);
	CGRect frmTitle = CGRectMake(fWidth * fTitle_PosX, fHeight * fTitle_PosY, fWidth * fTitle_Width, fHeight * fTitle_Height);

	self.swhEnabled.frame = frmSwitch;
	self.swhEnabled.contentVerticalAlignment = UIControlContentVerticalAlignmentCenter;
	self.lblTitle.numberOfLines = 0;
//	self.lblTitle.contentVerticalAlignment = UIControlContentVerticalAlignmentCenter;
	self.lblTitle.frame = frmTitle;

	bIsEnabled = cPreferences->getSubjectEnabled(&sStdGroup,&sStdName);
	
	if (bIsEnabled == true) {
		self.swhEnabled.on = YES;
	}
	else {
		self.swhEnabled.on = NO;
	};
	
	self.swhEnabled.enabled = cPreferences->getGroupEnabled(&sStdGroup);
	
}

- (void)setSelected:(BOOL)selected animated:(BOOL)animated {

    [super setSelected:selected animated:animated];

    // Configure the view for the selected state
	
	
}

-(void)updateTitle{
	bool bIsEnabled;

	if (self.swhEnabled.on == YES) {
		bIsEnabled = true;
	}
	else {
		bIsEnabled = false;
	};
	cPreferences->setSubjectEnabled(&sStdGroup,&sStdName, bIsEnabled);
	
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
	cPreferences = NULL;
	sStdGroup.clear();
	sStdName.clear();
	
//	UITableViewController *vCtrlParent;
	
    [super dealloc];
}

- (void)setParent:(UITableViewController*) vCtrlParentTemp {
	vCtrlParent = vCtrlParentTemp;
}

-(void)groupEnabled:(NSString*)sGroup bEnabled:(NSNumber*)bEnabled {
	if (sStdGroup == [sGroup UTF8String]) {
		self.swhEnabled.enabled = bEnabled.boolValue;
	};

}; 


@end
