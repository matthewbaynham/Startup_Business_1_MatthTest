//
//  CellSectionTitle.mm
//  Hello World Table View
//
//  Created by Matthew Baynham on 9/12/10.
//  Copyright 2010 Baynham Coding. All rights reserved.
//

#import "CellSectionTitle.h"
#import "ClsPreferences.h"


@implementation CellSectionTitle

@synthesize lblTitle;

const float fTitle_PosX = 0.08;
const float fTitle_PosY = 0.08;
const float fTitle_Width = 0.85;
const float fTitle_Height = 0.4;


- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier 
													 sGroupName:(NSString*)sGroupName 
														  cPref:(ClsPreferences*)cPref { 
	float fWidth;
	float fHeight;

	if ((self = [super initWithStyle:style reuseIdentifier:reuseIdentifier])) {
        // Initialization code
		fWidth = self.contentView.bounds.size.width;
		fHeight = self.contentView.bounds.size.height;
		
		// Initialization code
		CGRect frmTitle = CGRectMake(fWidth * fTitle_PosX, fHeight * fTitle_PosY, fWidth * fTitle_Width, fHeight * fTitle_Height);
		
		self.lblTitle = [[UILabel alloc] initWithFrame:frmTitle];
		self.lblTitle.text = sGroupName;
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
	
	self.lblTitle.frame = frmTitle;
	
	[self.lblTitle setTextAlignment:UITextAlignmentCenter];
	[self.lblTitle setBaselineAdjustment:UIBaselineAdjustmentAlignCenters];

}


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
//	cPreferences = NULL;
	//	std::string sStdGroup;
//	[vCtrlParent release];
	
    [super dealloc];
}

- (void)setParent:(UITableViewController*) vCtrlParentTemp {
	vCtrlParent = vCtrlParentTemp;
}

//-(void)groupEnabled:(NSString*)sGroup bEnabled:(NSNumber*)bEnabled {
	
//}; 


@end
