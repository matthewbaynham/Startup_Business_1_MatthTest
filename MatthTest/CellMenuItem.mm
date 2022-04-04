//
//  CellMenuItem.mm
//  MatthTest
//
//  Created by Matthew Baynham on 2/7/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "modMisc.h"
#import "CellMenuItem.h"


@implementation CellMenuItem

@synthesize imgVwPic;
@synthesize lblText;

const float fPic_PosX = 0.88;
const float fPic_PosY = 0.20;
//const float fPic_Width = 0.08; //has to be a circle so width is based on height
const float fPic_Height = 0.60;

const float fText_PosX = 0.035;
const float fText_PosY = 0.02;
const float fText_Width = 0.83;
const float fText_Height = 0.96;


- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier {
    
    self = [super initWithStyle:style reuseIdentifier:reuseIdentifier];
    if (self) {
        // Initialization code.
		float fWidth;
		float fHeight;
		
		fWidth = self.contentView.bounds.size.width;
		fHeight = self.contentView.bounds.size.height;

//		CGRect frmPic = CGRectMake(fWidth * fPic_PosX, fHeight * fPic_PosY, fWidth * fPic_Width, fHeight * fPic_Height);
		CGRect frmPic = CGRectMake(fWidth * fPic_PosX, fHeight * fPic_PosY, fHeight * fPic_Height, fHeight * fPic_Height); //has to be a circle so width is based on height
		self.imgVwPic = [[UIImageView alloc] initWithFrame:frmPic]; 
		[self.imgVwPic setImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicSelector_Cyan]]; 
		self.imgVwPic.opaque = YES; // explicitly opaque for performance 
		[self.contentView addSubview:self.imgVwPic]; 

		//Add Question Label
		CGRect frmText = CGRectMake(fWidth * fText_PosX, fHeight * fText_PosY, fWidth * fText_Width, fHeight * fText_Height);
		self.lblText = [[UILabel alloc] initWithFrame:frmText];
		
		self.lblText.numberOfLines = 0;
		self.lblText.textColor = [[[UIApplication sharedApplication] delegate] GuiColour:@"text"];
		self.lblText.font = [[[UIApplication sharedApplication] delegate] GuiFont:@"menu"];
		[self.lblText setBackgroundColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"cell"]];
		[self.contentView addSubview:self.lblText];
	}
    return self;
}

- (void)drawRect:(CGRect)rect
{
	float fWidth;
	float fHeight;
	
	fWidth = self.contentView.bounds.size.width;
	fHeight = self.contentView.bounds.size.height;

//	CGRect frmPic = CGRectMake(fWidth * fPic_PosX, fHeight * fPic_PosY, fWidth * fPic_Width, fHeight * fPic_Height);
	CGRect frmPic = CGRectMake(fWidth * fPic_PosX, fHeight * fPic_PosY, fHeight * fPic_Height, fHeight * fPic_Height); //has to be a circle so width is based on height
	CGRect frmText = CGRectMake(fWidth * fText_PosX, fHeight * fText_PosY, fWidth * fText_Width, fHeight * fText_Height);

	self.imgVwPic.frame = frmPic; 
	self.lblText.frame = frmText;
	
	self.lblText.baselineAdjustment = UIBaselineAdjustmentAlignCenters;

}	

- (void)setSelected:(BOOL)selected animated:(BOOL)animated {
    
    [super setSelected:selected animated:animated];
    
    // Configure the view for the selected state.
}


- (void)dealloc {
    [super dealloc];
}

-(void)setFont:(UIFont *)fntFont {
	[self.lblText setFont:fntFont];
};

-(void)setText:(NSString *)txtText {
	[self.lblText setText:txtText];
};

@end
