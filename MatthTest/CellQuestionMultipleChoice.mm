//
//  CellQuestionMultipleChoice.mm
//  Hello World Table View
//
//  Created by Matthew Baynham on 10/5/10.
//  Copyright 2010 Baynham Coding. All rights reserved.


#import "CellQuestionMultipleChoice.h"

@implementation CellQuestionMultipleChoice

@synthesize lblQuestion;
@synthesize btnAnswer1, btnAnswer2, btnAnswer3, btnAnswer4, btnAnswer5;
@synthesize btnInfo;
@synthesize iQuestionNo, bOnlyOneAnswer;
@synthesize bBtnSelected1, bBtnSelected2, bBtnSelected3, bBtnSelected4, bBtnSelected5;
//@synthesize sStatus;
@synthesize bBtnCorrect1, bBtnCorrect2, bBtnCorrect3, bBtnCorrect4, bBtnCorrect5;

const float fQuestion_PosX = 0.05;
const float fQuestion_PosY = 0.01;
const float fQuestion_Width = 0.9;
const float fQuestion_Height = 0.38;

const float fAnswerBtn1_PosX = 0.05;
const float fAnswerBtn1_PosY = 0.40;
const float fAnswerBtn1_Width = 0.9;
const float fAnswerBtn1_Height = 0.10;

const float fAnswerBtn2_PosX = 0.05;
const float fAnswerBtn2_PosY = 0.52;
const float fAnswerBtn2_Width = 0.9;
const float fAnswerBtn2_Height = 0.10;

const float fAnswerBtn3_PosX = 0.05;
const float fAnswerBtn3_PosY = 0.64;
const float fAnswerBtn3_Width = 0.9;
const float fAnswerBtn3_Height = 0.10;

const float fAnswerBtn4_PosX = 0.05;
const float fAnswerBtn4_PosY = 0.76;
const float fAnswerBtn4_Width = 0.9;
const float fAnswerBtn4_Height = 0.10;

const float fAnswerBtn5_PosX = 0.05;
const float fAnswerBtn5_PosY = 0.88;
const float fAnswerBtn5_Width = 0.9;
const float fAnswerBtn5_Height = 0.10;

const float fInfo_PosX = 0.9;
const float fInfo_PosY = 0.01;
const float fInfo_Width = 0.1;
const float fInfo_Height = 0.05;


- (id)initWithStyle:(UITableViewCellStyle)style 
	reuseIdentifier:(NSString *)reuseIdentifier 
	iQuestionNumber:(int)iQuestionNumber {
	CGRect frmQuestion;
	CGRect frmAnswerBtn1;
	CGRect frmAnswerBtn2;
	CGRect frmAnswerBtn3;
	CGRect frmAnswerBtn4;
	CGRect frmAnswerBtn5;
	
	if ((self = [super initWithStyle:style reuseIdentifier:reuseIdentifier])) {
		iQuestionNo = iQuestionNumber;

		float fWidth;
		float fHeight;

		fWidth = self.contentView.bounds.size.width;
		fHeight = self.contentView.bounds.size.height;
		
		
		//Add Question Label
		frmQuestion = CGRectMake(fWidth * fQuestion_PosX, fHeight * fQuestion_PosY, fWidth * fQuestion_Width, fHeight * fQuestion_Height);
		self.lblQuestion = [[UILabel alloc] initWithFrame:frmQuestion];

		self.lblQuestion.numberOfLines = 0;
		self.lblQuestion.textColor = [[[UIApplication sharedApplication] delegate] GuiColour:@"text"];
		self.lblQuestion.font = [[[UIApplication sharedApplication] delegate] GuiFont:@"questions"];
		[self.lblQuestion setBackgroundColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"cell"]];
		[self.contentView addSubview:self.lblQuestion];
		
		
        //Add Answer Button 1
		frmAnswerBtn1 = CGRectMake(fWidth * fAnswerBtn1_PosX, fHeight * fAnswerBtn1_PosY, fWidth * fAnswerBtn1_Width, fHeight * fAnswerBtn1_Height);
		self.btnAnswer1 = [[UIButton alloc] initWithFrame:frmAnswerBtn1];
		[self.btnAnswer1 setBackgroundColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"cell"]];
		[self.btnAnswer1 setTitleColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"text"] forState:UIControlStateNormal];
		self.btnAnswer1.titleLabel.font = [[[UIApplication sharedApplication] delegate] GuiFont:@"answers"];
		[self.btnAnswer1 setEnabled:YES];
		[self.btnAnswer1 setSelected:NO];
		[self.btnAnswer1 setHighlighted:NO];

		[self.btnAnswer1 addTarget:self action:@selector(buttonPressed1) forControlEvents:UIControlEventTouchUpInside];
		[self.contentView addSubview:self.btnAnswer1];
		
        //Add Answer Button 2
		frmAnswerBtn2 = CGRectMake(fWidth * fAnswerBtn2_PosX, fHeight * fAnswerBtn2_PosY, fWidth * fAnswerBtn2_Width, fHeight * fAnswerBtn2_Height);
		self.btnAnswer2 = [[UIButton alloc] initWithFrame:frmAnswerBtn2];
		[self.btnAnswer2 setBackgroundColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"cell"]];
		[self.btnAnswer2 setTitleColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"text"] forState:UIControlStateNormal];
		self.btnAnswer2.titleLabel.font = [[[UIApplication sharedApplication] delegate] GuiFont:@"answers"];
		[self.btnAnswer2 setEnabled:YES];
		[self.btnAnswer2 setSelected:NO];
		[self.btnAnswer2 setHighlighted:NO];

		[self.btnAnswer2 addTarget:self action:@selector(buttonPressed2) forControlEvents:UIControlEventTouchUpInside];
		[self.contentView addSubview:self.btnAnswer2];
		
        //Add Answer Button 3
		frmAnswerBtn3 = CGRectMake(fWidth * fAnswerBtn3_PosX, fHeight * fAnswerBtn3_PosY, fWidth * fAnswerBtn3_Width, fHeight * fAnswerBtn3_Height);
		self.btnAnswer3 = [[[UIButton alloc] initWithFrame:frmAnswerBtn3] autorelease];
		[self.btnAnswer3 setTitleColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"text"] forState:UIControlStateNormal];
		self.btnAnswer3.titleLabel.font = [[[UIApplication sharedApplication] delegate] GuiFont:@"answers"];
		[self.btnAnswer3 setEnabled:YES];
		[self.btnAnswer3 setSelected:NO];
		[self.btnAnswer3 setHighlighted:NO];

		[self.btnAnswer3 addTarget:self action:@selector(buttonPressed3) forControlEvents:UIControlEventTouchUpInside];
		[self.contentView addSubview:self.btnAnswer3];
		
        //Add Answer Button 4
		frmAnswerBtn4 = CGRectMake(fWidth * fAnswerBtn4_PosX, fHeight * fAnswerBtn4_PosY, fWidth * fAnswerBtn4_Width, fHeight * fAnswerBtn4_Height);
		self.btnAnswer4 = [[UIButton alloc] initWithFrame:frmAnswerBtn4];
		[self.btnAnswer4 setBackgroundColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"cell"]];
		[self.btnAnswer4 setTitleColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"text"] forState:UIControlStateNormal];
		self.btnAnswer4.titleLabel.font = [[[UIApplication sharedApplication] delegate] GuiFont:@"answers"];
		[self.btnAnswer4 setEnabled:YES];
		[self.btnAnswer4 setSelected:NO];
		[self.btnAnswer4 setHighlighted:NO];

		[self.btnAnswer4 addTarget:self action:@selector(buttonPressed4) forControlEvents:UIControlEventTouchUpInside];
		[self.contentView addSubview:self.btnAnswer4];
		
        //Add Answer Button 5
		frmAnswerBtn5 = CGRectMake(fWidth * fAnswerBtn5_PosX, fHeight * fAnswerBtn5_PosY, fWidth * fAnswerBtn5_Width, fHeight * fAnswerBtn5_Height);
		self.btnAnswer5 = [[UIButton alloc] initWithFrame:frmAnswerBtn5];
		[self.btnAnswer5 setTitleColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"text"] forState:UIControlStateNormal];
		self.btnAnswer5.titleLabel.font = [[[UIApplication sharedApplication] delegate] GuiFont:@"answers"];
		[self.btnAnswer5 setEnabled:YES];
		[self.btnAnswer5 setSelected:NO];
		[self.btnAnswer5 setHighlighted:NO];
		
		[self.btnAnswer5 addTarget:self action:@selector(buttonPressed5) forControlEvents:UIControlEventTouchUpInside];
		[self.contentView addSubview:self.btnAnswer5];
		
		CGRect frmInfo = CGRectMake(fWidth * fInfo_PosX, fHeight * fInfo_PosY, fWidth * fInfo_Width, fHeight * fInfo_Height);
		self.btnInfo = [UIButton buttonWithType:UIButtonTypeInfoLight];
		self.btnInfo.frame = frmInfo;
		[self.btnInfo addTarget:self action:@selector(infoButtonPressed) 
			   forControlEvents:UIControlEventTouchUpInside];    
		[self.btnInfo setHidden:YES];
		[self.contentView addSubview:self.btnInfo];
		
	};

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
	
	bBtnSelected1 = [vCtrlParent btnMultipleChoiceIsSelected:self.iQuestionNo iChoice:0];
	bBtnSelected2 = [vCtrlParent btnMultipleChoiceIsSelected:self.iQuestionNo iChoice:1];
	bBtnSelected3 = [vCtrlParent btnMultipleChoiceIsSelected:self.iQuestionNo iChoice:2];
	bBtnSelected4 = [vCtrlParent btnMultipleChoiceIsSelected:self.iQuestionNo iChoice:3];
	bBtnSelected5 = [vCtrlParent btnMultipleChoiceIsSelected:self.iQuestionNo iChoice:4];

	// Initialization code
	CGRect frmQuestion = CGRectMake(fWidth * fQuestion_PosX, fHeight * fQuestion_PosY, fWidth * fQuestion_Width, fHeight * fQuestion_Height);
	CGRect frmAnswerBtn1 = CGRectMake(fWidth * fAnswerBtn1_PosX, fHeight * fAnswerBtn1_PosY, fWidth * fAnswerBtn1_Width, fHeight * fAnswerBtn1_Height);
	CGRect frmAnswerBtn2 = CGRectMake(fWidth * fAnswerBtn2_PosX, fHeight * fAnswerBtn2_PosY, fWidth * fAnswerBtn2_Width, fHeight * fAnswerBtn2_Height);
	CGRect frmAnswerBtn3 = CGRectMake(fWidth * fAnswerBtn3_PosX, fHeight * fAnswerBtn3_PosY, fWidth * fAnswerBtn3_Width, fHeight * fAnswerBtn3_Height);
	CGRect frmAnswerBtn4 = CGRectMake(fWidth * fAnswerBtn4_PosX, fHeight * fAnswerBtn4_PosY, fWidth * fAnswerBtn4_Width, fHeight * fAnswerBtn4_Height);
	CGRect frmAnswerBtn5 = CGRectMake(fWidth * fAnswerBtn5_PosX, fHeight * fAnswerBtn5_PosY, fWidth * fAnswerBtn5_Width, fHeight * fAnswerBtn5_Height);

	if ([sStatus isEqualToString:@"Finished"] == YES) {
		bBtnCorrect1 = [vCtrlParent btnMultipleChoiceIsCorrect:self.iQuestionNo iChoice:0];
		bBtnCorrect2 = [vCtrlParent btnMultipleChoiceIsCorrect:self.iQuestionNo iChoice:1];
		bBtnCorrect3 = [vCtrlParent btnMultipleChoiceIsCorrect:self.iQuestionNo iChoice:2];
		bBtnCorrect4 = [vCtrlParent btnMultipleChoiceIsCorrect:self.iQuestionNo iChoice:3];
		bBtnCorrect5 = [vCtrlParent btnMultipleChoiceIsCorrect:self.iQuestionNo iChoice:4];
		
		
		if (bBtnSelected1 == true) {
			if (bBtnCorrect1 == true) {
				[btnAnswer1 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_BlackGreenTick] forState:UIControlStateNormal];
				[btnAnswer1 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_BlackGreenTick] forState:UIControlStateSelected];
				[btnAnswer1 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_BlackGreenTick] forState:UIControlStateHighlighted];
			} else {
				[btnAnswer1 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_BlackRedCross] forState:UIControlStateNormal];
				[btnAnswer1 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_BlackRedCross] forState:UIControlStateSelected];
				[btnAnswer1 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_BlackRedCross] forState:UIControlStateHighlighted];
			}; 
		} else {
			if (bBtnCorrect1 == true) {
				[btnAnswer1 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_BlackYellowArrow] forState:UIControlStateNormal];
				[btnAnswer1 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_BlackYellowArrow] forState:UIControlStateSelected];
				[btnAnswer1 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_BlackYellowArrow] forState:UIControlStateHighlighted];
			} else {
				[btnAnswer1 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Black] forState:UIControlStateNormal];
				[btnAnswer1 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Black] forState:UIControlStateSelected];
				[btnAnswer1 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Black] forState:UIControlStateHighlighted];
			}; 
		};
		
		if (bBtnSelected2 == true) {
			if (bBtnCorrect2 == true) {
				[btnAnswer2 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_BlackGreenTick] forState:UIControlStateNormal];
				[btnAnswer2 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_BlackGreenTick] forState:UIControlStateSelected];
				[btnAnswer2 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_BlackGreenTick] forState:UIControlStateHighlighted];
			} else {
				[btnAnswer2 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_BlackRedCross] forState:UIControlStateNormal];
				[btnAnswer2 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_BlackRedCross] forState:UIControlStateSelected];
				[btnAnswer2 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_BlackRedCross] forState:UIControlStateHighlighted];
			}; 
		} else {
			if (bBtnCorrect2 == true) {
				[btnAnswer2 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_BlackYellowArrow] forState:UIControlStateNormal];
				[btnAnswer2 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_BlackYellowArrow] forState:UIControlStateSelected];
				[btnAnswer2 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_BlackYellowArrow] forState:UIControlStateHighlighted];
			} else {
				[btnAnswer2 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Black] forState:UIControlStateNormal];
				[btnAnswer2 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Black] forState:UIControlStateSelected];
				[btnAnswer2 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Black] forState:UIControlStateHighlighted];
			}; 
		};
		
		if (bBtnSelected3 == true) {
			if (bBtnCorrect3 == true) {
				[btnAnswer3 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_BlackGreenTick] forState:UIControlStateNormal];
				[btnAnswer3 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_BlackGreenTick] forState:UIControlStateSelected];
				[btnAnswer3 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_BlackGreenTick] forState:UIControlStateHighlighted];
			} else {
				[btnAnswer3 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_BlackRedCross] forState:UIControlStateNormal];
				[btnAnswer3 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_BlackRedCross] forState:UIControlStateSelected];
				[btnAnswer3 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_BlackRedCross] forState:UIControlStateHighlighted];
			}; 
		} else {
			if (bBtnCorrect3 == true) {
				[btnAnswer3 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_BlackYellowArrow] forState:UIControlStateNormal];
				[btnAnswer3 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_BlackYellowArrow] forState:UIControlStateSelected];
				[btnAnswer3 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_BlackYellowArrow] forState:UIControlStateHighlighted];
			} else {
				[btnAnswer3 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Black] forState:UIControlStateNormal];
				[btnAnswer3 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Black] forState:UIControlStateSelected];
				[btnAnswer3 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Black] forState:UIControlStateHighlighted];
			}; 
		};
		
		if (bBtnSelected4 == true) {
			if (bBtnCorrect4 == true) {
				[btnAnswer4 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_BlackGreenTick] forState:UIControlStateNormal];
				[btnAnswer4 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_BlackGreenTick] forState:UIControlStateSelected];
				[btnAnswer4 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_BlackGreenTick] forState:UIControlStateHighlighted];
			} else {
				[btnAnswer4 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_BlackRedCross] forState:UIControlStateNormal];
				[btnAnswer4 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_BlackRedCross] forState:UIControlStateSelected];
				[btnAnswer4 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_BlackRedCross] forState:UIControlStateHighlighted];
			}; 
		} else {
			if (bBtnCorrect4 == true) {
				[btnAnswer4 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_BlackYellowArrow] forState:UIControlStateNormal];
				[btnAnswer4 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_BlackYellowArrow] forState:UIControlStateSelected];
				[btnAnswer4 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_BlackYellowArrow] forState:UIControlStateHighlighted];
			} else {
				[btnAnswer4 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Black] forState:UIControlStateNormal];
				[btnAnswer4 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Black] forState:UIControlStateSelected];
				[btnAnswer4 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Black] forState:UIControlStateHighlighted];
			}; 
		};
		
		if (bBtnSelected5 == true) {
			if (bBtnCorrect5 == true) {
				[btnAnswer5 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_BlackGreenTick] forState:UIControlStateNormal];
				[btnAnswer5 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_BlackGreenTick] forState:UIControlStateSelected];
				[btnAnswer5 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_BlackGreenTick] forState:UIControlStateHighlighted];
			} else {
				[btnAnswer5 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_BlackRedCross] forState:UIControlStateNormal];
				[btnAnswer5 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_BlackRedCross] forState:UIControlStateSelected];
				[btnAnswer5 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_BlackRedCross] forState:UIControlStateHighlighted];
			}; 
		} else {
			if (bBtnCorrect5 == true) {
				[btnAnswer5 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_BlackYellowArrow] forState:UIControlStateNormal];
				[btnAnswer5 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_BlackYellowArrow] forState:UIControlStateSelected];
				[btnAnswer5 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_BlackYellowArrow] forState:UIControlStateHighlighted];
			} else {
				[btnAnswer5 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Black] forState:UIControlStateNormal];
				[btnAnswer5 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Black] forState:UIControlStateSelected];
				[btnAnswer5 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Black] forState:UIControlStateHighlighted];
			}; 
		};

	} else {
		if (self.bBtnSelected1 == true) {
			[btnAnswer1 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Red] forState:UIControlStateNormal];
		} else {
			[btnAnswer1 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Black] forState:UIControlStateNormal];
		};
		
		if (self.bBtnSelected2 == true) {
			[btnAnswer2 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Red] forState:UIControlStateNormal];
		} else {
			[btnAnswer2 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Black] forState:UIControlStateNormal];
		};
		
		if (self.bBtnSelected3 == true) {
			[btnAnswer3 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Red] forState:UIControlStateNormal];
		} else {
			[btnAnswer3 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Black] forState:UIControlStateNormal];
		};
		
		if (self.bBtnSelected4 == true) {
			[btnAnswer4 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Red] forState:UIControlStateNormal];
		} else {
			[btnAnswer4 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Black] forState:UIControlStateNormal];
		};
		
		if (self.bBtnSelected5 == true) {
			[btnAnswer5 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Red] forState:UIControlStateNormal];		
		} else {
			[btnAnswer5 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Black] forState:UIControlStateNormal];		
		};

		[btnAnswer1 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Red] forState:UIControlStateSelected];
		[btnAnswer1 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Blue] forState:UIControlStateHighlighted];
		[btnAnswer2 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Red] forState:UIControlStateSelected];
		[btnAnswer2 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Blue] forState:UIControlStateHighlighted];
		[btnAnswer3 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Red] forState:UIControlStateSelected];
		[btnAnswer3 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Blue] forState:UIControlStateHighlighted];
		[btnAnswer4 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Red] forState:UIControlStateSelected];
		[btnAnswer4 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Blue] forState:UIControlStateHighlighted];
		[btnAnswer5 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Red] forState:UIControlStateSelected];
		[btnAnswer5 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Blue] forState:UIControlStateHighlighted];

	};
	
	self.lblQuestion.frame = frmQuestion;
	self.btnAnswer1.frame = frmAnswerBtn1;
	self.btnAnswer2.frame = frmAnswerBtn2;
	self.btnAnswer3.frame = frmAnswerBtn3;
	self.btnAnswer4.frame = frmAnswerBtn4;
	self.btnAnswer5.frame = frmAnswerBtn5;

	bOnlyOneAnswer = [vCtrlParent isOnlyOneAnswer:iQuestionNo];
	
	self.lblQuestion.text = [vCtrlParent sQuestionTextDecoded:iQuestionNo];
	[self.btnAnswer1 setTitle:[vCtrlParent sAnswerMulipleChoiceDecoded:iQuestionNo iChoice:0] forState:UIControlStateNormal];
	[self.btnAnswer2 setTitle:[vCtrlParent sAnswerMulipleChoiceDecoded:iQuestionNo iChoice:1] forState:UIControlStateNormal];
	[self.btnAnswer3 setTitle:[vCtrlParent sAnswerMulipleChoiceDecoded:iQuestionNo iChoice:2] forState:UIControlStateNormal];
	[self.btnAnswer4 setTitle:[vCtrlParent sAnswerMulipleChoiceDecoded:iQuestionNo iChoice:3] forState:UIControlStateNormal];
	[self.btnAnswer5 setTitle:[vCtrlParent sAnswerMulipleChoiceDecoded:iQuestionNo iChoice:4] forState:UIControlStateNormal];

	stdGroup = [[vCtrlParent sQuestionGroup:self.iQuestionNo] UTF8String];
	stdSubject = [[vCtrlParent sQuestionSubject:self.iQuestionNo] UTF8String];
	
	NSString* sGroup = [[[NSString alloc] initWithCString:stdGroup.c_str()] autorelease];
	NSString* sSubject = [[[NSString alloc] initWithCString:stdSubject.c_str()] autorelease];
	
	bool bHasHelp = [[[UIApplication sharedApplication] delegate] helpSubjectIsFound:sGroup
 																			sSubject:sSubject];
	if (bHasHelp == true) {
		CGRect frmInfo = CGRectMake(fWidth * fInfo_PosX, fHeight * fInfo_PosY, fWidth * fInfo_Width, fHeight * fInfo_Height);
		self.btnInfo.frame = frmInfo;
		[self.btnInfo setHidden:NO];
	} else {
		[self.btnInfo setHidden:YES];
	};
	
};


//-(void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation {
//	[[[UIApplication sharedApplication] delegate] popUp:@"Rotated"];
//};

- (void)setSelected:(BOOL)selected animated:(BOOL)animated {

    [super setSelected:selected animated:animated];

};

- (void)viewDidUnload {
	[lblQuestion release];
	
	[btnAnswer1 release];
	[btnAnswer2 release];
	[btnAnswer3 release];
	[btnAnswer4 release];
	[btnAnswer5 release];

	[self release];
}

- (void)dealloc {
//	int iQuestionNo;
	
//	bool bOnlyOneAnswer;
//	bool bBtnSelected1;
//	bool bBtnSelected2;
//	bool bBtnSelected3;
//	bool bBtnSelected4;
//	bool bBtnSelected5;
//	bool bBtnCorrect1;
//	bool bBtnCorrect2;
//	bool bBtnCorrect3;
//	bool bBtnCorrect4;
//	bool bBtnCorrect5;
	//	NSString* sStatus;
	stdGroup.clear();
	stdSubject.clear();
	
	[lblQuestion release];
	
	[btnAnswer1 release];
	[btnAnswer2 release];
	[btnAnswer3 release];
	[btnAnswer4 release];
	[btnAnswer5 release];
	[btnInfo release];
	
//	UITableViewController *vCtrlParent;
	
    [super dealloc];
};

-(void) buttonPressed1 {
	NSString* sStatus = [[[NSString alloc] initWithString:[vCtrlParent sQuizStatus]] autorelease];
	
	if ([sStatus isEqualToString:@"Finished"] != YES) {
		if (self.bOnlyOneAnswer) {
			bBtnSelected1 = true;
			bBtnSelected2 = false;
			bBtnSelected3 = false; 
			bBtnSelected4 = false;
			bBtnSelected5 = false;
			
			[vCtrlParent btnMultipleChoiceSelected:iQuestionNo iChoice:0 bSelected:true];
			[vCtrlParent btnMultipleChoiceSelected:iQuestionNo iChoice:1 bSelected:false];
			[vCtrlParent btnMultipleChoiceSelected:iQuestionNo iChoice:2 bSelected:false];
			[vCtrlParent btnMultipleChoiceSelected:iQuestionNo iChoice:3 bSelected:false];
			[vCtrlParent btnMultipleChoiceSelected:iQuestionNo iChoice:4 bSelected:false];
			
			[self.btnAnswer1 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Red] forState:UIControlStateNormal];
			[self.btnAnswer2 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Black] forState:UIControlStateNormal];
			[self.btnAnswer3 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Black] forState:UIControlStateNormal];
			[self.btnAnswer4 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Black] forState:UIControlStateNormal];
			[self.btnAnswer5 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Black] forState:UIControlStateNormal];
			
		} else {
			if (self.bBtnSelected1 == true) {
				bBtnSelected1 = false;
				[self.btnAnswer1 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Black] forState:UIControlStateNormal];
			} else {
				bBtnSelected1 = true;
				[self.btnAnswer1 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Red] forState:UIControlStateNormal];
			};
			[vCtrlParent btnMultipleChoiceSelected:iQuestionNo iChoice:0 bSelected:bBtnSelected1];
		};
	};
};

-(void) buttonPressed2 {
	NSString* sStatus = [[[NSString alloc] initWithString:[vCtrlParent sQuizStatus]] autorelease];

	if ([sStatus isEqualToString:@"Finished"] != YES) {
		if (self.bOnlyOneAnswer) {
			bBtnSelected1 = false;
			bBtnSelected2 = true;
			bBtnSelected3 = false; 
			bBtnSelected4 = false;
			bBtnSelected5 = false;
			
			[vCtrlParent btnMultipleChoiceSelected:self.iQuestionNo iChoice:0 bSelected:false];
			[vCtrlParent btnMultipleChoiceSelected:self.iQuestionNo iChoice:1 bSelected:true];
			[vCtrlParent btnMultipleChoiceSelected:self.iQuestionNo iChoice:2 bSelected:false];
			[vCtrlParent btnMultipleChoiceSelected:self.iQuestionNo iChoice:3 bSelected:false];
			[vCtrlParent btnMultipleChoiceSelected:self.iQuestionNo iChoice:4 bSelected:false];
			
			[self.btnAnswer1 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Black] forState:UIControlStateNormal];
			[self.btnAnswer2 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Red] forState:UIControlStateNormal];
			[self.btnAnswer3 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Black] forState:UIControlStateNormal];
			[self.btnAnswer4 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Black] forState:UIControlStateNormal];
			[self.btnAnswer5 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Black] forState:UIControlStateNormal];		
			
		} else {
			if (self.bBtnSelected2 == true) {
				bBtnSelected2 = false;
				[self.btnAnswer2 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Black] forState:UIControlStateNormal];
			} else {
				bBtnSelected2 = true;
				[self.btnAnswer2 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Red] forState:UIControlStateNormal];
			};
			[vCtrlParent btnMultipleChoiceSelected:self.iQuestionNo iChoice:1 bSelected:bBtnSelected2];
		};
	};
};

-(void)buttonPressed3 {
	NSString* sStatus = [[[NSString alloc] initWithString:[vCtrlParent sQuizStatus]] autorelease];

	if ([sStatus isEqualToString:@"Finished"] != YES) {
		if (self.bOnlyOneAnswer) {
			bBtnSelected1 = false;
			bBtnSelected2 = false;
			bBtnSelected3 = true; 
			bBtnSelected4 = false;
			bBtnSelected5 = false;
			
			[vCtrlParent btnMultipleChoiceSelected:self.iQuestionNo iChoice:0 bSelected:false];
			[vCtrlParent btnMultipleChoiceSelected:self.iQuestionNo iChoice:1 bSelected:false];
			[vCtrlParent btnMultipleChoiceSelected:self.iQuestionNo iChoice:2 bSelected:true];
			[vCtrlParent btnMultipleChoiceSelected:self.iQuestionNo iChoice:3 bSelected:false];
			[vCtrlParent btnMultipleChoiceSelected:self.iQuestionNo iChoice:4 bSelected:false];
			
			[self.btnAnswer1 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Black] forState:UIControlStateNormal];
			[self.btnAnswer2 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Black] forState:UIControlStateNormal];
			[self.btnAnswer3 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Red] forState:UIControlStateNormal];
			[self.btnAnswer4 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Black] forState:UIControlStateNormal];
			[self.btnAnswer5 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Black] forState:UIControlStateNormal];
		} else {
			if (self.bBtnSelected3 == true) {
				bBtnSelected3 = false;
				[self.btnAnswer3 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Black] forState:UIControlStateNormal];
			} else {
				bBtnSelected3 = true;
				[self.btnAnswer3 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Red] forState:UIControlStateNormal];
			};
			[vCtrlParent btnMultipleChoiceSelected:self.iQuestionNo iChoice:2 bSelected:bBtnSelected3];
		};
	};
};

-(void)buttonPressed4 {
	NSString* sStatus = [[[NSString alloc] initWithString:[vCtrlParent sQuizStatus]] autorelease];

	if ([sStatus isEqualToString:@"Finished"] != YES) {
		if (self.bOnlyOneAnswer) {
			bBtnSelected1 = false;
			bBtnSelected2 = false;
			bBtnSelected3 = false; 
			bBtnSelected4 = true;
			bBtnSelected5 = false;
			
			[vCtrlParent btnMultipleChoiceSelected:self.iQuestionNo iChoice:0 bSelected:false];
			[vCtrlParent btnMultipleChoiceSelected:self.iQuestionNo iChoice:1 bSelected:false];
			[vCtrlParent btnMultipleChoiceSelected:self.iQuestionNo iChoice:2 bSelected:false];
			[vCtrlParent btnMultipleChoiceSelected:self.iQuestionNo iChoice:3 bSelected:true];
			[vCtrlParent btnMultipleChoiceSelected:self.iQuestionNo iChoice:4 bSelected:false];
			
			[self.btnAnswer1 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Black] forState:UIControlStateNormal];
			[self.btnAnswer2 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Black] forState:UIControlStateNormal];
			[self.btnAnswer3 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Black] forState:UIControlStateNormal];
			[self.btnAnswer4 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Red] forState:UIControlStateNormal];
			[self.btnAnswer5 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Black] forState:UIControlStateNormal];
		} else {
			if (self.bBtnSelected4 == true) {
				bBtnSelected4 = false;
				[self.btnAnswer4 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Black] forState:UIControlStateNormal];
			} else {
				bBtnSelected4 = true;
				[self.btnAnswer4 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Red] forState:UIControlStateNormal];
			};
			[vCtrlParent btnMultipleChoiceSelected:self.iQuestionNo iChoice:3 bSelected:bBtnSelected4];
		};
	};
};

-(void)buttonPressed5 {
	NSString* sStatus = [[[NSString alloc] initWithString:[vCtrlParent sQuizStatus]] autorelease];

	if ([sStatus isEqualToString:@"Finished"] != YES) {
		if (self.bOnlyOneAnswer) {
			bBtnSelected1 = false;
			bBtnSelected2 = false;
			bBtnSelected3 = false; 
			bBtnSelected4 = false;
			bBtnSelected5 = true;
			
			[vCtrlParent btnMultipleChoiceSelected:self.iQuestionNo iChoice:0 bSelected:false];
			[vCtrlParent btnMultipleChoiceSelected:self.iQuestionNo iChoice:1 bSelected:false];
			[vCtrlParent btnMultipleChoiceSelected:self.iQuestionNo iChoice:2 bSelected:false];
			[vCtrlParent btnMultipleChoiceSelected:self.iQuestionNo iChoice:3 bSelected:false];
			[vCtrlParent btnMultipleChoiceSelected:self.iQuestionNo iChoice:4 bSelected:true];
			
			[self.btnAnswer1 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Black] forState:UIControlStateNormal];
			[self.btnAnswer2 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Black] forState:UIControlStateNormal];
			[self.btnAnswer3 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Black] forState:UIControlStateNormal];
			[self.btnAnswer4 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Black] forState:UIControlStateNormal];
			[self.btnAnswer5 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Red] forState:UIControlStateNormal];
		} else {
			if (self.bBtnSelected5 == true) {
				bBtnSelected5 = false;
				[self.btnAnswer5 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Black] forState:UIControlStateNormal];
			} else {
				bBtnSelected5 = true;
				[self.btnAnswer5 setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Red] forState:UIControlStateNormal];
			};
			[vCtrlParent btnMultipleChoiceSelected:self.iQuestionNo iChoice:4 bSelected:bBtnSelected5];
		};
	};
};

-(void)infoButtonPressed {
	NSString* sGroup = [[[NSString alloc] initWithCString:stdGroup.c_str()] autorelease];
	NSString* sSubject = [[[NSString alloc] initWithCString:stdSubject.c_str()] autorelease];
	[[[UIApplication sharedApplication] delegate] helpSubject:sGroup 
													 sSubject:sSubject];
}

@end
