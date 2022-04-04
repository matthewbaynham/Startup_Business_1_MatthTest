//
//  CellQuizMenu.mm
//  Hello World Table View
//
//  Created by Matthew Baynham on 11/19/10.
//  Copyright 2010 Baynham Coding. All rights reserved.
//

#import "CellQuizMenu.h"

using namespace modMisc;

@implementation CellQuizMenu

//@synthesize root;
@synthesize sFullPath;
@synthesize vCtrlParent;
@synthesize lblTotalTimeTaken;//need to have other smaller cells giving a breakdow of all the time periods
@synthesize lblScore;
@synthesize btnFinished;
@synthesize btnSave;
@synthesize btnDelete;

const float fBtnFinished_PosX = 0.08;
const float fBtnFinished_PosY = 0.59;
const float fBtnFinished_Width = 0.84;
const float fBtnFinished_Height = 0.26;

const float fBtnSave_PosX = 0.52;
const float fBtnSave_PosY = 0.19;
const float fBtnSave_Width = 0.4;
const float fBtnSave_Height = 0.24;

const float fBtnDelete_PosX = 0.08;
const float fBtnDelete_PosY = 0.19;
const float fBtnDelete_Width = 0.4;
const float fBtnDelete_Height = 0.26;


/*
- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString*)reuseIdentifier 
													   cGuiPref:(ClsGuiPreferences*)cGuiPref {
*/
- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString*)reuseIdentifier 
														  sPath:(NSString*)sPath{
    if ((self = [super initWithStyle:style reuseIdentifier:reuseIdentifier])) {
        // Initialization code
		
		sFullPath = sPath;
		[sFullPath retain];
		
		float fWidth;
		float fHeight;
			
		fWidth = self.contentView.bounds.size.width;
		fHeight = self.contentView.bounds.size.height;
		
        //Add Button Finished
		NSString *sBtnFinished = [[[NSString alloc] initWithString:@"Finished"] autorelease];
		
		CGRect frmBtnFinished = CGRectMake(fWidth * fBtnFinished_PosX, fHeight * fBtnFinished_PosY, fWidth * fBtnFinished_Width, fHeight * fBtnFinished_Height);
		self.btnFinished = [[UIButton alloc] initWithFrame:frmBtnFinished];
		[self.btnFinished setTitle:sBtnFinished forState:UIControlStateNormal];
		[self.btnFinished setBackgroundColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"cell"]];
		[self.btnFinished setTitleColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"text"] forState:UIControlStateNormal];
		self.btnFinished.titleLabel.font = [[[UIApplication sharedApplication] delegate] GuiFont:@"menu"];
		[self.btnFinished setEnabled:YES];
		[self.btnFinished setSelected:NO];
		[self.btnFinished setHighlighted:NO];
		
		[self.btnFinished setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Black] forState:UIControlStateNormal];
		[self.btnFinished setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Black] forState:UIControlStateDisabled];
		[self.btnFinished setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Red] forState:UIControlStateSelected];
		[self.btnFinished setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Blue] forState:UIControlStateHighlighted];
		
		[self.btnFinished addTarget:self action:@selector(buttonPressedFinished) forControlEvents:UIControlEventTouchUpInside];
		[self.contentView addSubview:self.btnFinished];
		
		
		
        //Add Button Save
		NSString *sBtnSave = [[[NSString alloc] initWithString:@"Save"] autorelease];
		
		CGRect frmBtnSave = CGRectMake(fWidth * fBtnSave_PosX, fHeight * fBtnSave_PosY, fWidth * fBtnSave_Width, fHeight * fBtnSave_Height);
		self.btnSave = [[UIButton alloc] initWithFrame:frmBtnSave];
		[self.btnSave setTitle:sBtnSave forState:UIControlStateNormal];
		[self.btnSave setBackgroundColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"cell"]];
		[self.btnSave setTitleColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"text"] forState:UIControlStateNormal];
		self.btnSave.titleLabel.font = [[[UIApplication sharedApplication] delegate] GuiFont:@"menu"];
		[self.btnSave setEnabled:YES];
		[self.btnSave setSelected:NO];
		[self.btnSave setHighlighted:NO];
		
		[self.btnSave setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Black] forState:UIControlStateNormal];
		[self.btnSave setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Black] forState:UIControlStateDisabled];
		[self.btnSave setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Red] forState:UIControlStateSelected];
		[self.btnSave setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Blue] forState:UIControlStateHighlighted];
		
		[self.btnSave addTarget:self action:@selector(buttonPressedSave) forControlEvents:UIControlEventTouchUpInside];
		[self.contentView addSubview:self.btnSave];
		
		
		
        //Add Button Delete
		NSString *sBtnDelete = [[[NSString alloc] initWithString:@"Delete"] autorelease];
		
		CGRect frmBtnDelete = CGRectMake(fWidth * fBtnDelete_PosX, fHeight * fBtnDelete_PosY, fWidth * fBtnDelete_Width, fHeight * fBtnDelete_Height);
		self.btnDelete = [[UIButton alloc] initWithFrame:frmBtnDelete];
		[self.btnDelete setTitle:sBtnDelete forState:UIControlStateNormal];
		[self.btnDelete setBackgroundColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"cell"]];
		[self.btnDelete setTitleColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"delete"] forState:UIControlStateNormal];
		self.btnDelete.titleLabel.font = [[[UIApplication sharedApplication] delegate] GuiFont:@"menu"];
		[self.btnDelete setEnabled:YES];
		[self.btnDelete setSelected:NO];
		[self.btnDelete setHighlighted:NO];
		
		[self.btnDelete setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Black] forState:UIControlStateNormal];
		[self.btnDelete setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Black] forState:UIControlStateDisabled];
		[self.btnDelete setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Red] forState:UIControlStateSelected];
		[self.btnDelete setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Blue] forState:UIControlStateHighlighted];
		
		[self.btnDelete addTarget:self action:@selector(buttonPressedDelete) forControlEvents:UIControlEventTouchUpInside];
		[self.contentView addSubview:self.btnDelete];
    }
    return self;
}

- (void)setParent:(UITableViewController*) vCtrlParentTemp {
	vCtrlParent = vCtrlParentTemp;
}

- (void)drawRect:(CGRect)rect
{
	float fWidth;
	float fHeight;
	NSString* sStatus = [[[NSString alloc] initWithString:[vCtrlParent sQuizStatus]] autorelease];
	
	fWidth = self.contentView.bounds.size.width;
	fHeight = self.contentView.bounds.size.height;
		
	CGRect frmbtnFinished = CGRectMake(fWidth * fBtnFinished_PosX, fHeight * fBtnFinished_PosY, fWidth * fBtnFinished_Width, fHeight * fBtnFinished_Height);
	self.btnFinished.frame = frmbtnFinished;

	CGRect frmbtnSave = CGRectMake(fWidth * fBtnSave_PosX, fHeight * fBtnSave_PosY, fWidth * fBtnSave_Width, fHeight * fBtnSave_Height);
	self.btnSave.frame = frmbtnSave;
	
	CGRect frmBtnDelete = CGRectMake(fWidth * fBtnDelete_PosX, fHeight * fBtnDelete_PosY, fWidth * fBtnDelete_Width, fHeight * fBtnDelete_Height);
	self.btnDelete.frame = frmBtnDelete;

	if ([sStatus isEqualToString:@"Finished"] == YES) {
		NSString *sBtnSave = [[[NSString alloc] initWithString:@"Exit"] autorelease];
		NSString *sBtnDelete = [[[NSString alloc] initWithString:@"Delete"] autorelease];

		btnFinished.hidden = YES;
		
		[btnSave setTitle:sBtnSave forState:UIControlStateNormal];
		[btnSave setTitle:sBtnSave forState:UIControlStateSelected];
		[btnSave setTitle:sBtnSave forState:UIControlStateHighlighted];
		[btnDelete setTitle:sBtnDelete forState:UIControlStateNormal];
		[btnDelete setTitle:sBtnDelete forState:UIControlStateSelected];
		[btnDelete setTitle:sBtnDelete forState:UIControlStateHighlighted];
	} else {
		NSString *sBtnFinished = [[[NSString alloc] initWithString:@"Mark"] autorelease];
		NSString *sBtnSave = [[[NSString alloc] initWithString:@"Save"] autorelease];
		NSString *sBtnDelete = [[[NSString alloc] initWithString:@"Delete"] autorelease];

		[btnSave setTitle:sBtnSave forState:UIControlStateNormal];
		[btnSave setTitle:sBtnSave forState:UIControlStateSelected];
		[btnSave setTitle:sBtnSave forState:UIControlStateHighlighted];
		[btnDelete setTitle:sBtnDelete forState:UIControlStateNormal];
		[btnDelete setTitle:sBtnDelete forState:UIControlStateSelected];
		[btnDelete setTitle:sBtnDelete forState:UIControlStateHighlighted];
		[btnFinished setTitle:sBtnFinished forState:UIControlStateNormal];
		[btnFinished setTitle:sBtnFinished forState:UIControlStateSelected];
		[btnFinished setTitle:sBtnFinished forState:UIControlStateHighlighted];
	};
}	

- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex {
	//the delete pop up
	if (buttonIndex == 1) {
		[[[UIApplication sharedApplication] delegate] DirDeleteFile:self.sFullPath];

		[vCtrlParent closeTableView];
	}
}

- (void)setSelected:(BOOL)selected animated:(BOOL)animated {

    [super setSelected:selected animated:animated];

    // Configure the view for the selected state
}

- (void)dealloc {
	[sFullPath release];
	[lblTotalTimeTaken release];//need to have other smaller cells giving a breakdow of all the time periods
	[lblScore release];
	[btnFinished release];
	[btnSave release];
	[btnDelete release];
//	UITableViewController *vCtrlParent;

//	[cGuiPreferences dealloc];
    [super dealloc];
}

- (void)buttonPressedFinished {
	
	//	cQuiz->mark();
	
	[vCtrlParent markQuiz];
//	[vCtrlParent closeTableView];
	[vCtrlParent refreshQuiz];
	
}

- (void)buttonPressedSave {
	[vCtrlParent closeTableView];
}

- (void)buttonPressedDelete {
	NSString* sStatus = [[[NSString alloc] initWithString:[vCtrlParent sQuizStatus]] autorelease];
	
	if ([sStatus isEqualToString:@"Finished"] != YES) {
		NSString *sTitle = [[[NSString alloc] initWithString:@"Delete?"] autorelease];
		NSString *sMessage = [[[NSString alloc] initWithString:@"Do you want to delete the test results?"] autorelease];
		NSString *sBtnCancel = [[[NSString alloc] initWithString:@"Cancel"] autorelease];
		NSString *sBtnDelete = [[[NSString alloc] initWithString:@"Delete"] autorelease];

		UIAlertView *msgDelete = [[UIAlertView alloc] initWithTitle:sTitle
															message:sMessage
														   delegate:self 
												  cancelButtonTitle:sBtnCancel 
												  otherButtonTitles:sBtnDelete, nil];
		[msgDelete show];
		[msgDelete release];
	};
}



@end
