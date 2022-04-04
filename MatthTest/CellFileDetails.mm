//
//  CellFileDetails.mm
//  Hello World Table View
//
//  Created by Matthew Baynham on 10/16/10.
//  Copyright 2010 Baynham Coding. All rights reserved.
//

#import "modMisc.h"
#import "CellFileDetails.h"
#import "ViewControllerQuiz.h"


@implementation CellFileDetails

@synthesize lblDateTime;

@synthesize btnDelete;
@synthesize btnView;

@synthesize lblTimeTaken;
//@synthesize lblDateTimeStart;//date time quiz started
@synthesize lblStatus;//Status of the quiz i.e. finished, started 
@synthesize lblQuestionsTotal;//total number of questions
@synthesize lblQuestionsResults;//if status = finished this is the no of correct question, if status = not finished this is the no of question completed

@synthesize lblTimeTakenValue;
@synthesize lblDateTimeStartValue; //date time quiz started
@synthesize lblStatusValue; //Status of the quiz i.e. finished, started 
@synthesize lblQuestionsTotalValue; //total number of questions
@synthesize lblQuestionsResultsValue; //if status = finished this is the no of correct question, if status = not finished this is the no of question completed

//@synthesize viewControllerQuiz;

//const float fDateTime_PosX = 0.05;
//const float fDateTime_PosY = 0.03;
//const float fDateTime_Width = 0.9;
//const float fDateTime_Height = 0.11;

//const float fLblDateTimeStarted_PosX = 0.05;
//const float fLblDateTimeStarted_PosY = 0.02;
//const float fLblDateTimeStarted_Width = 0.9;
//const float fLblDateTimeStarted_Height = 0.11;

const float fLblDateTimeStartedValue_PosX = 0.05;
const float fLblDateTimeStartedValue_PosY = 0.15;
const float fLblDateTimeStartedValue_Width = 0.9;
const float fLblDateTimeStartedValue_Height = 0.11;

const float fLblTimeTaken_PosX = 0.05;
const float fLblTimeTaken_PosY = 0.28;
const float fLblTimeTaken_Width = 0.45;
const float fLblTimeTaken_Height = 0.11;

const float fLblTimeTakenValue_PosX = 0.45;
const float fLblTimeTakenValue_PosY = 0.28;
const float fLblTimeTakenValue_Width = 0.45;
const float fLblTimeTakenValue_Height = 0.11;

const float fLblStatus_PosX = 0.05;
const float fLblStatus_PosY = 0.41;
const float fLblStatus_Width = 0.45;
const float fLblStatus_Height = 0.11;

const float fLblStatusValue_PosX = 0.50;
const float fLblStatusValue_PosY = 0.41;
const float fLblStatusValue_Width = 0.45;
const float fLblStatusValue_Height = 0.11;

const float fLblQuestionsTotal_PosX = 0.05;
const float fLblQuestionsTotal_PosY = 0.54;
const float fLblQuestionsTotal_Width = 0.7;
const float fLblQuestionsTotal_Height = 0.11;

const float fLblQuestionsTotalValue_PosX = 0.80;
const float fLblQuestionsTotalValue_PosY = 0.54;
const float fLblQuestionsTotalValue_Width = 0.15;
const float fLblQuestionsTotalValue_Height = 0.11;

const float fLblQuestionsResults_PosX = 0.05;
const float fLblQuestionsResults_PosY = 0.67;
const float fLblQuestionsResults_Width = 0.7;
const float fLblQuestionsResults_Height = 0.11;

const float fLblQuestionsResultsValue_PosX = 0.80;
const float fLblQuestionsResultsValue_PosY = 0.67;
const float fLblQuestionsResultsValue_Width = 0.15;
const float fLblQuestionsResultsValue_Height = 0.11;

const float fBtnDelete_PosX = 0.05;
const float fBtnDelete_PosY = 0.83;
const float fBtnDelete_Width = 0.43;
const float fBtnDelete_Height = 0.14;

const float fBtnView_PosX = 0.52;
const float fBtnView_PosY = 0.83;
const float fBtnView_Width = 0.43;
const float fBtnView_Height = 0.14;


/* 
 - (id)initWithStyle:(UITableViewCellStyle)style 
 reuseIdentifier:(NSString *)reuseIdentifier {
 */
- (id)initWithStyle:(UITableViewCellStyle)style 
	reuseIdentifier:(NSString *)reuseIdentifier 
			  sPath:(NSString*)sPath
		iFileNumber:(int)iFileNumber {
	if ((self = [super initWithStyle:style reuseIdentifier:reuseIdentifier])) {
        // Initialization code
		iFileNo = iFileNumber;
		sFullPath = sPath;
		[sFullPath retain];
		
		float fWidth;
		float fHeight;
		
		fWidth = self.contentView.bounds.size.width;
		fHeight = self.contentView.bounds.size.height;
		
		std::string stdPath;
		stdPath = [sPath UTF8String];
		
		cQuiz.setPath(stdPath);
		cQuiz.selectHeader();//load the header info into the class ready

		
        //Add Button View

		NSString *sBtnViewText = [[NSString alloc] initWithString:@""];
		
		if (bIsComplete == true) {
			sBtnViewText = @"View";
		} else {
			sBtnViewText = @"Cont...";
		};

		CGRect frmBtnView = CGRectMake(fWidth * fBtnView_PosX, fHeight * fBtnView_PosY, fWidth * fBtnView_Width, fHeight * fBtnView_Height);
		self.btnView = [[UIButton alloc] initWithFrame:frmBtnView];
		[self.btnView setTitle:sBtnViewText forState:UIControlStateNormal];
		[self.btnView setBackgroundColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"cell"]];
		[self.btnView setTitleColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"text"] forState:UIControlStateNormal];
		self.btnView.titleLabel.font = [[[UIApplication sharedApplication] delegate] GuiFont:@"filedetails"];//Try the Gui Thing
		[self.btnView setEnabled:YES];
		[self.btnView setSelected:NO];
		[self.btnView setHighlighted:NO];
		
		[self.btnView setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Black] forState:UIControlStateNormal];//Try the Gui Thing
		[self.btnView setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Black] forState:UIControlStateDisabled];//Try the Gui Thing
		[self.btnView setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Red] forState:UIControlStateSelected];//Try the Gui Thing
		[self.btnView setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Blue] forState:UIControlStateHighlighted];//Try the Gui Thing
		
		[self.btnView addTarget:self action:@selector(buttonPressedView) forControlEvents:UIControlEventTouchUpInside];
		[self.contentView addSubview:self.btnView];

		
        //Add Button Delete
		NSString *sBtnDeleteText = [[NSString alloc] initWithString:@"Delete"];
		
		CGRect frmBtnDelete = CGRectMake(fWidth * fBtnDelete_PosX, fHeight * fBtnDelete_PosY, fWidth * fBtnDelete_Width, fHeight * fBtnDelete_Height);
		self.btnDelete = [[UIButton alloc] initWithFrame:frmBtnDelete];
		[self.btnDelete setTitle:sBtnDeleteText forState:UIControlStateNormal];
		[self.btnDelete setBackgroundColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"cell"]];//Try the Gui Thing
		[self.btnDelete setTitleColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"delete"] forState:UIControlStateNormal];//Try the Gui Thing
		self.btnDelete.titleLabel.font = [[[UIApplication sharedApplication] delegate] GuiFont:@"filedetails"];//Try the Gui Thing
		[self.btnDelete setEnabled:YES];
		[self.btnDelete setSelected:NO];
		[self.btnDelete setHighlighted:NO];
		
		[self.btnDelete setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Black] forState:UIControlStateNormal];//Try the Gui Thing
		[self.btnDelete setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Black] forState:UIControlStateDisabled];//Try the Gui Thing
		[self.btnDelete setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Red] forState:UIControlStateSelected];//Try the Gui Thing
		[self.btnDelete setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Blue] forState:UIControlStateHighlighted];//Try the Gui Thing
		
		[self.btnDelete addTarget:self action:@selector(buttonPressedDelete) forControlEvents:UIControlEventTouchUpInside];
		[self.contentView addSubview:self.btnDelete];
		
		
		
	//add buttons 
	//restart / view details (depending on if the quiz is finished)
	//Delete results (follwed by the "Are you sure?" message)
	//Total time taken so far (can come in different segements, viewing the quiz will show the detail)
	//Questions answered
	//Total number of Questions
	//Questions correct
	//date started.
	//name??? not sure have to change the quiz so that the name is entered, definitely do this for the iPad

		
		
		//Add Time Taken Label
		CGRect frmTimeTaken = CGRectMake(fWidth * fLblTimeTaken_PosX, fHeight * fLblTimeTaken_PosY, fWidth * fLblTimeTaken_Width, fHeight * fLblTimeTaken_Height);
		self.lblTimeTaken = [[UILabel alloc] initWithFrame:frmTimeTaken];
		
		NSString *sTimeTaken = [[[NSString alloc] initWithString:@"Time taken"] autorelease];
		
		self.lblTimeTaken.text = sTimeTaken;
		self.lblTimeTaken.textColor = [[[UIApplication sharedApplication] delegate] GuiColour:@"text"];
		self.lblTimeTaken.font = [[[UIApplication sharedApplication] delegate] GuiFont:@"filedetails"];//Try the Gui Thing
		[self.lblTimeTaken setBackgroundColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"cell"]];
		[self.contentView addSubview:self.lblTimeTaken];
		
		
		//Add Time Taken Label Value
		CGRect frmTimeTakenValue = CGRectMake(fWidth * fLblTimeTakenValue_PosX, fHeight * fLblTimeTakenValue_PosY, fWidth * fLblTimeTakenValue_Width, fHeight * fLblTimeTakenValue_Height);
		self.lblTimeTakenValue = [[UILabel alloc] initWithFrame:frmTimeTakenValue];
		
		int iTimeTakenValue = cQuiz.secondSpent();
		const char* cTimeTakenValue;
		cTimeTakenValue = convertSecondsIntoTimeString(iTimeTakenValue).c_str();
		NSString *sTimeTakenValue = [[[NSString alloc] initWithString:[NSString stringWithCString:cTimeTakenValue length:strlen(cTimeTakenValue)]] autorelease];			
		
		self.lblTimeTakenValue.text = sTimeTakenValue;
		self.lblTimeTakenValue.textColor = [[[UIApplication sharedApplication] delegate] GuiColour:@"text"];
		self.lblTimeTakenValue.font = [[[UIApplication sharedApplication] delegate] GuiFont:@"filedetails"];//Try the Gui Thing
		[self.lblTimeTakenValue setBackgroundColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"cell"]];
		[self.contentView addSubview:self.lblTimeTakenValue];
		
		
		
		
		//Add Date Time Started Label Value
		CGRect frmDateTimeStartedValue = CGRectMake(fWidth * fLblDateTimeStartedValue_PosX, fHeight * fLblDateTimeStartedValue_PosY, fWidth * fLblDateTimeStartedValue_Width, fHeight * fLblDateTimeStartedValue_Height);
		self.lblDateTimeStartValue = [[UILabel alloc] initWithFrame:frmDateTimeStartedValue];
		
		const char* cDateTimeStartedValue;
		cDateTimeStartedValue = cQuiz.headerValue(modMisc::sHeader_QuizCreatedDate).c_str();
		NSString *sDateTimeStartedValue = [[[NSString alloc] initWithString:[NSString stringWithCString:cDateTimeStartedValue length:strlen(cDateTimeStartedValue)]] autorelease];			
		
		self.lblDateTimeStartValue.text = sDateTimeStartedValue;
		self.lblDateTimeStartValue.textColor = [[[UIApplication sharedApplication] delegate] GuiColour:@"text"];//Try the Gui Thing
		self.lblDateTimeStartValue.font = [[[UIApplication sharedApplication] delegate] GuiFont:@"filedetails"];//Try the Gui Thing
		[self.lblDateTimeStartValue setBackgroundColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"cell"]];//Try the Gui Thing
		[self.contentView addSubview:self.lblDateTimeStartValue];
		
		
		
		//Add Status Label
		CGRect frmStatus = CGRectMake(fWidth * fLblStatus_PosX, fHeight * fLblStatus_PosY, fWidth * fLblStatus_Width, fHeight * fLblStatus_Height);
		self.lblStatus = [[UILabel alloc] initWithFrame:frmStatus];
		
		NSString *sStatus = [[[NSString alloc] initWithString:@"Status"] autorelease];
		
		self.lblStatus.text = sStatus;
		self.lblStatus.textColor = [[[UIApplication sharedApplication] delegate] GuiColour:@"text"];
		self.lblStatus.font = [[[UIApplication sharedApplication] delegate] GuiFont:@"filedetails"];//Try the Gui Thing
		[self.lblStatus setBackgroundColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"cell"]];
		[self.contentView addSubview:self.lblStatus];
		
		
		//Add Status Label Value
		CGRect frmStatusValue = CGRectMake(fWidth * fLblStatusValue_PosX, fHeight * fLblStatusValue_PosY, fWidth * fLblStatusValue_Width, fHeight * fLblStatusValue_Height);
		self.lblStatusValue = [[UILabel alloc] initWithFrame:frmStatusValue];
		
		const char* cStatusValue;
		cStatusValue = cQuiz.headerValue(modMisc::sHeader_QuizStatus).c_str();
		NSString *sStatusValue = [[[NSString alloc] initWithString:[NSString stringWithCString:cStatusValue length:strlen(cStatusValue)]] autorelease];			
		
		self.lblStatusValue.text = sStatusValue;
		self.lblStatusValue.textColor = [[[UIApplication sharedApplication] delegate] GuiColour:@"highlight"];
		self.lblStatusValue.font = [[[UIApplication sharedApplication] delegate] GuiFont:@"filedetails"];//Try the Gui Thing
		[self.lblStatusValue setBackgroundColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"cell"]];
		[self.contentView addSubview:self.lblStatusValue];
		
		
		
		//Add Questions Total Label
		CGRect frmQuestionsTotal = CGRectMake(fWidth * fLblQuestionsTotal_PosX, fHeight * fLblQuestionsTotal_PosY, fWidth * fLblQuestionsTotal_Width, fHeight * fLblQuestionsTotal_Height);
		self.lblQuestionsTotal = [[UILabel alloc] initWithFrame:frmQuestionsTotal];
		
		NSString *sQuestionsTotal = [[[NSString alloc] initWithString:@"No. of Questions"] autorelease];
		
		self.lblQuestionsTotal.text = sQuestionsTotal;
		self.lblQuestionsTotal.textColor = [[[UIApplication sharedApplication] delegate] GuiColour:@"text"];
		self.lblQuestionsTotal.font = [[[UIApplication sharedApplication] delegate] GuiFont:@"filedetails"];//Try the Gui Thing
		[self.lblQuestionsTotal setBackgroundColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"cell"]];
		[self.contentView addSubview:self.lblQuestionsTotal];
		
		
		//Add Questions Total Label Value
		CGRect frmQuestionsTotalValue = CGRectMake(fWidth * fLblQuestionsTotalValue_PosX, fHeight * fLblQuestionsTotalValue_PosY, fWidth * fLblQuestionsTotalValue_Width, fHeight * fLblQuestionsTotalValue_Height);
		self.lblQuestionsTotalValue = [[UILabel alloc] initWithFrame:frmQuestionsTotalValue];
		
		const char* cQuestionsTotalValue;
		cQuestionsTotalValue = cQuiz.headerValue(modMisc::sHeader_QuestionTotal).c_str();
		NSString *sQuestionsTotalValue = [[[NSString alloc] initWithString:[NSString stringWithCString:cQuestionsTotalValue length:strlen(cQuestionsTotalValue)]] autorelease];			
		
		self.lblQuestionsTotalValue.text = sQuestionsTotalValue;
		self.lblQuestionsTotalValue.textColor = [[[UIApplication sharedApplication] delegate] GuiColour:@"text"];
		self.lblQuestionsTotalValue.font = [[[UIApplication sharedApplication] delegate] GuiFont:@"filedetails"];//Try the Gui Thing
		[self.lblQuestionsTotalValue setBackgroundColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"cell"]];
		[self.contentView addSubview:self.lblQuestionsTotalValue];
		
		
		//Add Questions Results Label
		CGRect frmQuestionsResults = CGRectMake(fWidth * fLblQuestionsResults_PosX, fHeight * fLblQuestionsResults_PosY, fWidth * fLblQuestionsResults_Width, fHeight * fLblQuestionsResults_Height);
		self.lblQuestionsResults = [[UILabel alloc] initWithFrame:frmQuestionsResults];
		
		NSString *sQuestionsResults = [[[NSString alloc] initWithString:@"Correct Answers"] autorelease];
		
		self.lblQuestionsResults.text = sQuestionsResults;
		self.lblQuestionsResults.textColor = [[[UIApplication sharedApplication] delegate] GuiColour:@"text"];
		self.lblQuestionsResults.font = [[[UIApplication sharedApplication] delegate] GuiFont:@"filedetails"];//Try the Gui Thing
		[self.lblQuestionsResults setBackgroundColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"cell"]];
		[self.contentView addSubview:self.lblQuestionsResults];
		
		
		//Add Questions Results Label Value
		CGRect frmQuestionsResultsValue = CGRectMake(fWidth * fLblQuestionsResultsValue_PosX, fHeight * fLblQuestionsResultsValue_PosY, fWidth * fLblQuestionsResultsValue_Width, fHeight * fLblQuestionsResultsValue_Height);
		self.lblQuestionsResultsValue = [[UILabel alloc] initWithFrame:frmQuestionsResultsValue];
		
		const char* cQuestionsResultsValue;
		cQuestionsResultsValue = cQuiz.headerValue(modMisc::sHeader_QuestionCount).c_str();
		NSString *sQuestionsResultsValue = [[[NSString alloc] initWithString:[NSString stringWithCString:cQuestionsResultsValue length:strlen(cQuestionsResultsValue)]] autorelease];			
		
		self.lblQuestionsResultsValue.text = sQuestionsResultsValue;
		self.lblQuestionsResultsValue.textColor = [[[UIApplication sharedApplication] delegate] GuiColour:@"text"];
		self.lblQuestionsResultsValue.font = [[[UIApplication sharedApplication] delegate] GuiFont:@"filedetails"];//Try the Gui Thing
		[self.lblQuestionsResultsValue setBackgroundColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"cell"]];
		[self.contentView addSubview:self.lblQuestionsResultsValue];
		
//		[sBtnViewText release];
//		[frmBtnView release];
//		[sBtnDeleteText release];
//		[frmBtnDelete release];
//		[frmTimeTaken release];
//		[sTimeTaken release];
//		[frmTimeTakenValue release];
//		int iTimeTakenValue;
//		const char* cTimeTakenValue;
//		cTimeTakenValue = NULL;
//		[sTimeTakenValue release];			
//		[frmDateTimeStartedValue release];
//		const char* cDateTimeStartedValue;
//		cDateTimeStartedValue = NULL;
//		[sDateTimeStartedValue release];			
//		[frmStatus release];
//		[sStatus release];
//		[frmStatusValue release];
//		const char* cStatusValue;
//		cStatusValue = NULL;
//		[sStatusValue release];			
//		[frmQuestionsTotal release];
//		[sQuestionsTotal release];
//		[frmQuestionsTotalValue release];
//		const char* cQuestionsTotalValue;
//		cQuestionsTotalValue = NULL;
//		[sQuestionsTotalValue release];			
//		[frmQuestionsResults release];
//		[sQuestionsResults release];
//		[frmQuestionsResultsValue release];
//		const char* cQuestionsResultsValue;
//		cQuestionsResultsValue = NULL;
//		[sQuestionsResultsValue release];			
		

//		stdPath.clear();

	};
	
    return self;
	
};

-(void)setFile:(NSString*)sPath
   iFileNumber:(int)iFileNumber {
	
	//Add Status Label
	
	NSString *sStatus = [[[NSString alloc] initWithString:@"Status"] autorelease];
	
	[self.lblStatus setText:sStatus];
	
	
	//Add Status Label Value
	const char* cStatusValue;
	cStatusValue = cQuiz.headerValue(modMisc::sHeader_QuizStatus).c_str();
	NSString *sStatusValue = [[[NSString alloc] initWithString:[NSString stringWithCString:cStatusValue length:strlen(cStatusValue)]] autorelease];			
	
	[self.lblStatusValue setText:sStatusValue];
	
	//Add Questions Total Label
	NSString *sQuestionsTotal = [[[NSString alloc] initWithString:@"QuestionsTotal"] autorelease];
	
	[self.lblQuestionsTotal setText:sQuestionsTotal];
	
	//Add Questions Total Label Value
	const char* cQuestionsTotalValue;
	cQuestionsTotalValue = cQuiz.headerValue(modMisc::sHeader_QuestionTotal).c_str();
	NSString *sQuestionsTotalValue = [[[NSString alloc] initWithString:[NSString stringWithCString:cQuestionsTotalValue length:strlen(cQuestionsTotalValue)]] autorelease];
	
	[self.lblQuestionsTotalValue setText:sQuestionsTotalValue];
	
	
	//Add Questions Results Label
	NSString *sQuestionsResults = [[[NSString alloc] initWithString:@"QuestionsResults"] autorelease];
	
	[self.lblQuestionsResults setText:sQuestionsResults];
	
	
	//Add Questions Results Label Value
	const char* cQuestionsResultsValue;
	cQuestionsResultsValue = cQuiz.headerValue(modMisc::sHeader_QuestionCount).c_str();
	NSString *sQuestionsResultsValue = [[[NSString alloc] initWithString:[NSString stringWithCString:cQuestionsResultsValue length:strlen(cQuestionsResultsValue)]] autorelease];
	
	[self.lblQuestionsResultsValue setText:sQuestionsResultsValue];

//	NSString *sStatus;
//	cStatusValue = NULL;
//	NSString *sStatusValue;			
//	NSString *sQuestionsTotal;
//	cQuestionsTotalValue = NULL;
//	NSString *sQuestionsTotalValue;			
//	NSString *sQuestionsResults;
//	cQuestionsResultsValue = NULL;
//	NSString *sQuestionsResultsValue;			
	
}


- (void)drawRect:(CGRect)rect
{
	float fWidth;
	float fHeight;
	NSString* sStatus = [[[NSString alloc] initWithString:[self sQuizStatus]] autorelease];
	
//	sStatus = [self sQuizStatus];
	
	fWidth = self.contentView.bounds.size.width;
	fHeight = self.contentView.bounds.size.height;

	CGRect frmBtnView = CGRectMake(fWidth * fBtnView_PosX, fHeight * fBtnView_PosY, fWidth * fBtnView_Width, fHeight * fBtnView_Height);
	self.btnView.frame = frmBtnView;

	CGRect frmBtnDelete = CGRectMake(fWidth * fBtnDelete_PosX, fHeight * fBtnDelete_PosY, fWidth * fBtnDelete_Width, fHeight * fBtnDelete_Height);
	self.btnDelete.frame = frmBtnDelete;

	CGRect frmTimeTaken = CGRectMake(fWidth * fLblTimeTaken_PosX, fHeight * fLblTimeTaken_PosY, fWidth * fLblTimeTaken_Width, fHeight * fLblTimeTaken_Height);
	self.lblTimeTaken.frame = frmTimeTaken;
	self.lblTimeTaken.textAlignment = UITextAlignmentLeft;
	
	CGRect frmTimeTakenValue = CGRectMake(fWidth * fLblTimeTakenValue_PosX, fHeight * fLblTimeTakenValue_PosY, fWidth * fLblTimeTakenValue_Width, fHeight * fLblTimeTakenValue_Height);
	self.lblTimeTakenValue.frame = frmTimeTakenValue;
	self.lblTimeTakenValue.textAlignment = UITextAlignmentRight;
	
//	CGRect frmDateTimeStarted = CGRectMake(fWidth * fLblDateTimeStarted_PosX, fHeight * fLblDateTimeStarted_PosY, fWidth * fLblDateTimeStarted_Width, fHeight * fLblDateTimeStarted_Height);
//	self.lblDateTimeStart.frame = frmDateTimeStarted;
	
	CGRect frmDateTimeStartedValue = CGRectMake(fWidth * fLblDateTimeStartedValue_PosX, fHeight * fLblDateTimeStartedValue_PosY, fWidth * fLblDateTimeStartedValue_Width, fHeight * fLblDateTimeStartedValue_Height);
	self.lblDateTimeStartValue.frame = frmDateTimeStartedValue;
	self.lblDateTimeStartValue.textAlignment = UITextAlignmentCenter;

	CGRect frmStatus = CGRectMake(fWidth * fLblStatus_PosX, fHeight * fLblStatus_PosY, fWidth * fLblStatus_Width, fHeight * fLblStatus_Height);
	self.lblStatus.frame = frmStatus;
	self.lblStatus.textAlignment = UITextAlignmentLeft;
	
	CGRect frmStatusValue = CGRectMake(fWidth * fLblStatusValue_PosX, fHeight * fLblStatusValue_PosY, fWidth * fLblStatusValue_Width, fHeight * fLblStatusValue_Height);
	self.lblStatusValue.frame = frmStatusValue;
	self.lblStatusValue.textAlignment = UITextAlignmentRight;
	
	CGRect frmQuestionsTotal = CGRectMake(fWidth * fLblQuestionsTotal_PosX, fHeight * fLblQuestionsTotal_PosY, fWidth * fLblQuestionsTotal_Width, fHeight * fLblQuestionsTotal_Height);
	self.lblQuestionsTotal.frame = frmQuestionsTotal;
	self.lblQuestionsTotal.textAlignment = UITextAlignmentLeft;
	
	CGRect frmQuestionsTotalValue = CGRectMake(fWidth * fLblQuestionsTotalValue_PosX, fHeight * fLblQuestionsTotalValue_PosY, fWidth * fLblQuestionsTotalValue_Width, fHeight * fLblQuestionsTotalValue_Height);
	self.lblQuestionsTotalValue.frame = frmQuestionsTotalValue;
	self.lblQuestionsTotalValue.textAlignment = UITextAlignmentRight;
	
	CGRect frmQuestionsResults = CGRectMake(fWidth * fLblQuestionsResults_PosX, fHeight * fLblQuestionsResults_PosY, fWidth * fLblQuestionsResults_Width, fHeight * fLblQuestionsResults_Height);
	self.lblQuestionsResults.frame = frmQuestionsResults;
	self.lblQuestionsResults.textAlignment = UITextAlignmentLeft;
	
	CGRect frmQuestionsResultsValue = CGRectMake(fWidth * fLblQuestionsResultsValue_PosX, fHeight * fLblQuestionsResultsValue_PosY, fWidth * fLblQuestionsResultsValue_Width, fHeight * fLblQuestionsResultsValue_Height);
	self.lblQuestionsResultsValue.frame = frmQuestionsResultsValue;
	self.lblQuestionsResultsValue.textAlignment = UITextAlignmentRight;
	
	NSString *sBtnView = [[NSString alloc] initWithString:@""];
	
	if ([sStatus isEqualToString:@"Finished"] == YES) {
		sBtnView = @"View";
	} else {
		sBtnView = @"Continue";
	};
	
	[btnView setTitle:sBtnView forState:UIControlStateNormal];
	[btnView setTitle:sBtnView forState:UIControlStateSelected];
	[btnView setTitle:sBtnView forState:UIControlStateHighlighted];

//	float fWidth;
//	float fHeight;
//	NSString* sStatus;
//	CGRect frmBtnView;
//	CGRect frmBtnDelete;
//	CGRect frmTimeTaken;
//	CGRect frmTimeTakenValue;
//	CGRect frmDateTimeStartedValue;
//	CGRect frmStatus;
//	CGRect frmStatusValue;
//	CGRect frmQuestionsTotal;
//	CGRect frmQuestionsTotalValue;
//	CGRect frmQuestionsResults;
//	CGRect frmQuestionsResultsValue;
//	NSString *sBtnView;
	
}

- (void)setParent:(UITableViewController*) vCtrlParentTemp {
	vCtrlParent = vCtrlParentTemp;
}

-(void)buttonPressedView {
	[vCtrlParent openQuiz:sFullPath];
}

-(void)buttonPressedDelete {
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

//	NSString *sTitle;
//	NSString *sMessage;
//	NSString *sBtnCancel;
//	NSString *sBtnDelete;
	
}

- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex {
	//the delete pop up
	if (buttonIndex == 1) {
		
		[[[UIApplication sharedApplication] delegate] DirDeleteFile:sFullPath];
		
		self.lblStatusValue.text = @"Deleted";
		self.lblStatusValue.textColor = [[[UIApplication sharedApplication] delegate] GuiColour:@"delete"];
		
		[btnView setHidden:YES];
		[btnDelete setHidden:YES];
	}
}

- (void)setSelected:(BOOL)selected animated:(BOOL)animated {

    [super setSelected:selected animated:animated];

    // Configure the view for the selected state
}
/*
- (void)viewDidUnload {

	[lblDateTime release];
	//	cQuiz = NULL;
	//	int iFileNo;
	//	bool bIsComplete;
	
	[btnDelete release];
	[btnView release];
	[lblTimeTaken release];
	//	[lblDateTimeStart release];//date time quiz started
	[lblStatus release];//Status of the quiz i.e. finished, started 
	[lblQuestionsTotal release];//total number of questions
	[lblQuestionsResults release];//if status = finished this is the no of correct question, if status = not finished this is the no of question completed
	
	[lblTimeTakenValue release];
	[lblDateTimeStartValue release];//date time quiz started
	[lblStatusValue release];//Status of the quiz i.e. finished, started 
	[lblQuestionsTotalValue release];//total number of questions
	[lblQuestionsResultsValue release];//if status = finished this is the no of correct question, if status = not finished this is the no of question completed
	
	//	UITableViewController *vCtrlParent;
	
	[sFullPath release];
	
};
*/
- (void)dealloc {
    [super dealloc];
}

-(void)readHeaderTable {

}

-(NSString*) sQuizStatus {
	const char* cStatus;
	cStatus = cQuiz.headerValue(modMisc::sHeader_QuizStatus).c_str();
//	NSString *sStatus = [NSString stringWithCString:cStatus length:strlen(cStatus)];
	NSString *sStatus = [[[NSString alloc] initWithString:[NSString stringWithCString:cStatus length:strlen(cStatus)]] autorelease];
//	cStatus = NULL;
	
	return sStatus;
	
//	NSString *sStatus;
};


@end
