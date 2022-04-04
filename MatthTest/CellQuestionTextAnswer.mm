//
//  CellQuestionTextAnswer.mm
//  Hello World Table View
//
//  Created by Matthew Baynham on 10/3/10.
//  Copyright 2010 Baynham Coding. All rights reserved.
//

#import "CellQuestionTextAnswer.h"

using namespace modMisc;

@implementation CellQuestionTextAnswer

@synthesize lblQuestion, txtAnswer;

const float fQuestion_PosX = 0.05;
const float fQuestion_PosY = 0.03;
const float fQuestion_Width = 0.9;
const float fQuestion_Height = 0.58;

const float fAnswer_PosX = 0.05;
const float fAnswer_PosY = 0.63;
const float fAnswer_Width = 0.9;
const float fAnswer_Height = 0.3;

- (id)initWithStyle:(UITableViewCellStyle)style 
	reuseIdentifier:(NSString *)reuseIdentifier 
//			 cpQuiz:(ClsQuiz*)cpQuiz
	iQuestionNumber:(int)iQuestionNumber {
    if ((self = [super initWithStyle:style reuseIdentifier:reuseIdentifier])) {
//		cQuiz = cpQuiz;
		iQuestionNo = iQuestionNumber;
//		cQuiz->iQuestionCurrent(iQuestionNo);
		
//		if (cGuiPreferences == Nil) {
//			cGuiPreferences = [[ClsGuiPreferences alloc] init];
//		}
		
		
		float fWidth;
		float fHeight;
		
		fWidth = self.contentView.bounds.size.width;
		fHeight = self.contentView.bounds.size.height;
		
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
//		self.txtAnswer = [[UITextField alloc] initWithFrame:frmAnswer];
		self.txtAnswer = [[TextFieldRounded alloc] initWithFrame:frmAnswer];
		self.txtAnswer.borderStyle = UITextBorderStyleNone;
		self.txtAnswer.font = [[[UIApplication sharedApplication] delegate] GuiFont:@"answers"];
		self.txtAnswer.returnKeyType = UIReturnKeyDone;
		self.txtAnswer.keyboardType = UIKeyboardTypeNumbersAndPunctuation;
		[self.txtAnswer addTarget:self action:@selector(answerCompleted) forControlEvents: UIControlEventEditingDidEndOnExit];
//		[self.txtAnswer addTarget:self action:@selector(answerCompleted) forControlEvents: UIControlEventTouchUpOutside];
		
		self.txtAnswer.textColor = [[[UIApplication sharedApplication] delegate] GuiColour:@"text"];
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

	CGRect frmQuestion = CGRectMake(fWidth * fQuestion_PosX, fHeight * fQuestion_PosY, fWidth * fQuestion_Width, fHeight * fQuestion_Height);
	CGRect frmAnswer = CGRectMake(fWidth * fAnswer_PosX, fHeight * fAnswer_PosY, fWidth * fAnswer_Width, fHeight * fAnswer_Height);

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
