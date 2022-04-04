//
//  CellQuestionTextPicture.mm
//  Hello World Table View
//
//  Created by Matthew Baynham on 12/2/10.
//  Copyright 2010 Baynham Coding. All rights reserved.
//

#import "CellQuestionTextPicture.h"
#import "MatthTestAppDelegate.h"

using namespace modMisc;

@implementation CellQuestionTextPicture

//@synthesize lblQuestion, txtAnswer, imgPic, imgVwPic;
@synthesize lblQuestion, txtAnswer, imgVwPic;

const float fPic_PosX = 0.03;
const float fPic_PosY = 0.03;
const float fPic_Width = 0.94;
const float fPic_Height = 0.55;

const float fQuestion_PosX = 0.05;
const float fQuestion_PosY = 0.57;
const float fQuestion_Width = 0.9;
const float fQuestion_Height = 0.24;

const float fAnswer_PosX = 0.05;
const float fAnswer_PosY = 0.83;
const float fAnswer_Width = 0.9;
const float fAnswer_Height = 0.14;

- (id)initWithStyle:(UITableViewCellStyle)style 
	reuseIdentifier:(NSString *)reuseIdentifier 
//	   sPictureName:(const char*)sPictureName 
	   ePictureType:(enumPicQues)ePictureType
	iQuestionNumber:(int)iQuestionNumber {
    if ((self = [super initWithStyle:style reuseIdentifier:reuseIdentifier])) {
		iQuestionNo = iQuestionNumber;
		
//		if (cGuiPreferences == Nil) {
//			cGuiPreferences = [[ClsGuiPreferences alloc] init];
//		}
		
		float fWidth;
		float fHeight;
		
		fWidth = self.contentView.bounds.size.width;
		fHeight = self.contentView.bounds.size.height;
		
		//add Pic
		CGRect frmPic = CGRectMake(fWidth * fPic_PosX, fHeight * fPic_PosY, fWidth * fPic_Width, fHeight * fPic_Height);
//		self.imgPic = [cGuiPreferences imgPicQuestionYellowTriangle];
//		self.imgPic = imgPicture;
		
//		self.imgVwPic = [[UIImageView alloc] initWithImage:self.imgPic];
//		UIImage* imgPic = [cGuiPreferences picture:ePictureType];
		
//		self.imgVwPic = [[UIImageView alloc] initWithImage:imgPic];
		self.imgVwPic = [[[UIApplication sharedApplication] delegate] GuiPicture:ePictureType];
		
		self.imgVwPic.frame = frmPic; 
		[self.contentView addSubview:self.imgVwPic];
		
        //Add Question Label
		CGRect frmQuestion = CGRectMake(fWidth * fQuestion_PosX, fHeight * fQuestion_PosY, fWidth * fQuestion_Width, fHeight * fQuestion_Height);
		self.lblQuestion = [[UILabel alloc] initWithFrame:frmQuestion];
		
		self.lblQuestion.numberOfLines = 0;
		self.lblQuestion.textColor = [[[UIApplication sharedApplication] delegate] GuiColour:@"text"];
		self.lblQuestion.font = [[[UIApplication sharedApplication] delegate] GuiFont:@"questions"];
		[self.lblQuestion setBackgroundColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"cell"]];
		[self.contentView addSubview:self.lblQuestion];
		
		//Add answer textbox
		CGRect frmAnswer = CGRectMake(fWidth * fAnswer_PosX, fHeight * fAnswer_PosY, fWidth * fAnswer_Width, fHeight * fAnswer_Height);
		self.txtAnswer = [[TextFieldRounded alloc] initWithFrame:frmAnswer];
		self.txtAnswer.borderStyle = UITextBorderStyleNone;
		self.txtAnswer.font = [[[UIApplication sharedApplication] delegate] GuiFont:@"answers"];
		self.txtAnswer.returnKeyType = UIReturnKeyDone;
		self.txtAnswer.keyboardType = UIKeyboardTypeNumbersAndPunctuation;
		[self.txtAnswer addTarget:self action:@selector(answerCompleted) forControlEvents: UIControlEventEditingDidEndOnExit];
//		[self.txtAnswer addTarget:self action:@selector(answerCompleted) forControlEvents: UIControlEventTouchUpOutside];

		self.txtAnswer.textColor = [[[UIApplication sharedApplication] delegate] GuiColour:@"text"];
//		self.txtAnswer.background = [cGuiPreferences imgBtnAnswerMultiChoiceBlack];
		self.txtAnswer.background = [[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Black];
		
		[self.contentView addSubview:self.txtAnswer];
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
	
	fWidth = self.contentView.bounds.size.width;
	fHeight = self.contentView.bounds.size.height;
	
	CGRect frmPic = CGRectMake(fWidth * fPic_PosX, fHeight * fPic_PosY, fWidth * fPic_Width, fHeight * fPic_Height);
	CGRect frmQuestion = CGRectMake(fWidth * fQuestion_PosX, fHeight * fQuestion_PosY, fWidth * fQuestion_Width, fHeight * fQuestion_Height);
	CGRect frmAnswer = CGRectMake(fWidth * fAnswer_PosX, fHeight * fAnswer_PosY, fWidth * fAnswer_Width, fHeight * fAnswer_Height);

//	self.imgPic = [vCtrlParent sQuestionImage:iQuestionNo];
	self.imgVwPic.frame = frmPic; 
	self.lblQuestion.frame = frmQuestion;
	self.txtAnswer.text = [vCtrlParent answerText:iQuestionNo];
	self.txtAnswer.frame = frmAnswer;
	
	self.lblQuestion.text = [vCtrlParent sQuestionTextDecoded:iQuestionNo];
}


- (void)setSelected:(BOOL)selected animated:(BOOL)animated {

    [super setSelected:selected animated:animated];

    // Configure the view for the selected state
}


- (void)dealloc {
    [super dealloc];
}

-(void)answerCompleted {
	[vCtrlParent answerCompleted:self->iQuestionNo
					 sAnswerText:self.txtAnswer.text];

	[self.txtAnswer resignFirstResponder];
	
}

@end
