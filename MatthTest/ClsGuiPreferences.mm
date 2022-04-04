//
//  ClsGuiPreferences.mm
//  Hello World Table View
//
//  Created by Matthew Baynham on 11/8/10.
//  Copyright 2010 Baynham Coding. All rights reserved.
//

#import "ClsGuiPreferences.h"

using namespace modMisc;

@implementation ClsGuiPreferences

@synthesize colText; //colour of the text
@synthesize colCell; //colour of the cells 
@synthesize colBoarder; //colour of the boarder of the cells
@synthesize colDelete; //colour of the text related to delete
@synthesize colHighlighted; //colour of things that should be highlighted (in yellow)

@synthesize fntQuestions; //font for all the questions
@synthesize fntAnswers; //font for all the answers
@synthesize fntFileDetails; //font for the file Details
@synthesize fntMenu; //font for all the menus
@synthesize fntMisc; //font for everything else
@synthesize fntTitle; //font for Title
@synthesize fntBaynhamCoding; //font for fntBaynham Coding

//@synthesize imgBtnAnswerMultiChoiceBlack;
//@synthesize imgBtnAnswerMultiChoiceBlackGreenTick;
//@synthesize imgBtnAnswerMultiChoiceBlackRedCross;

@synthesize imgRoundSelectorCyan;

@synthesize imgBtnAnswerMultiChoiceBlack;
@synthesize imgBtnAnswerMultiChoiceBlackGreenTick;
@synthesize imgBtnAnswerMultiChoiceBlackRedCross;
@synthesize imgBtnAnswerMultiChoiceBlackYellowArrow;


@synthesize imgBtnAnswerMultiChoiceBlue;
@synthesize imgBtnAnswerMultiChoiceCyan;
@synthesize imgBtnAnswerMultiChoiceGreen;
@synthesize imgBtnAnswerMultiChoiceRed;
@synthesize imgBtnAnswerMultiChoiceYellow;

@synthesize imgPicQuestionYellowTriangle;
@synthesize imgPicQuestionYellowCylinder;
@synthesize imgPicQuestionYellowParallogram;

@synthesize mathChr_Infinity;
@synthesize mathChr_PlusMinus;
@synthesize mathChr_LessThanEqualTo;
@synthesize mathChr_GreaterThanEqualTo;
@synthesize mathChr_DoesNotEquals;
@synthesize mathChr_Sum;
@synthesize mathChr_Intergral;
@synthesize mathChr_Theta;
@synthesize mathChr_Pi;
@synthesize mathChr_Delta;
@synthesize mathChr_SubscriptOne;
@synthesize mathChr_SubscriptTwo;
@synthesize mathChr_SubscriptThree;
@synthesize mathChr_Half;
@synthesize mathChr_SquareRoot;
@synthesize mathChr_CubeRoot;

-(id)init {
	[self resetToDefault];
	return self;
};

-(id)init:(bool)bIsDefault {
	if (bIsDefault == true) {
		[self resetToDefault];
	};
	
	return self;
};

-(void)resetToDefault {
	colBoarder = [[UIColor alloc] initWithRed:0.18 green:0.18 blue:0.18 alpha:1.0];
	colCell = [[UIColor alloc] initWithRed:0.0 green:0.0 blue:0.0 alpha:1.0];
	colText = [[UIColor alloc] initWithRed:1.0 green:1.0 blue:1.0 alpha:1.0];
	colDelete = [[UIColor alloc] initWithRed:1.0 green:0.0 blue:0.0 alpha:1.0];
	colHighlighted = [[UIColor alloc] initWithRed:1.0 green:1.0 blue:0.0 alpha:1.0];
	
	fntQuestions = [UIFont fontWithName:@"MarkerFelt-Thin" size:18.0]; //font for all the questions
	fntAnswers = [UIFont fontWithName:@"MarkerFelt-Thin" size:16.0]; //font for all the answers
	fntFileDetails = [UIFont fontWithName:@"MarkerFelt-Thin" size:12.0]; //font for everything else
	fntMenu = [UIFont fontWithName:@"MarkerFelt-Thin" size:18.0]; //font for all the menus
	fntMisc = [UIFont fontWithName:@"Zapfino" size:12.0]; //font for everything else
	fntTitle = [UIFont fontWithName:@"MarkerFelt-Thin" size:24.0]; //font for everything else
	fntBaynhamCoding = [UIFont fontWithName:@"Courier New" size:22.0]; //font for everything else


	imgRoundSelectorCyan = [[UIImage alloc] initWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"cyanRoundSelector" ofType:@"png"]];
	
	imgBtnAnswerMultiChoiceBlack = [[UIImage alloc] initWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"blackButton" ofType:@"jpg"]];
	imgBtnAnswerMultiChoiceBlackGreenTick = [[UIImage alloc] initWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"blackButtonGreenTick" ofType:@"jpg"]];
	imgBtnAnswerMultiChoiceBlackRedCross = [[UIImage alloc] initWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"blackButtonRedCross" ofType:@"jpg"]];
	imgBtnAnswerMultiChoiceBlackYellowArrow = [[UIImage alloc] initWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"blackButtonYellowArrow" ofType:@"jpg"]];
	
	imgBtnAnswerMultiChoiceBlue = [[UIImage alloc] initWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"blueButton" ofType:@"jpg"]];
	imgBtnAnswerMultiChoiceCyan = [[UIImage alloc] initWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"cyanButton" ofType:@"jpg"]];
	imgBtnAnswerMultiChoiceGreen = [[UIImage alloc] initWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"greenButton" ofType:@"jpg"]];
	imgBtnAnswerMultiChoiceRed   = [[UIImage alloc] initWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"redButton" ofType:@"jpg"]];
	imgBtnAnswerMultiChoiceYellow = [[UIImage alloc] initWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"yellowButton" ofType:@"png"]];

	imgPicQuestionYellowTriangle = [[UIImage alloc] initWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"yellowTriangle" ofType:@"jpg"]];
	imgPicQuestionYellowCylinder = [[UIImage alloc] initWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"yellowCylinder" ofType:@"jpg"]];
	imgPicQuestionYellowParallogram = [[UIImage alloc] initWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"yellowParallogram" ofType:@"jpg"]];
	
	
	mathCChr_Infinity = "\xe2\x88\x9e";
	mathCChr_PlusMinus = "\xc2\xb1";
	mathCChr_LessThanEqualTo = "\xe2\x89\xa4";
	mathCChr_GreaterThanEqualTo = "\xe2\x89\xa5";
	mathCChr_DoesNotEquals = "\xe2\x89\xa0";
	mathCChr_Sum = "\xe2\x88\x91";
	
	mathCChr_Intergral = "\xe2\x88\xab";
	mathCChr_Theta = "\xce\x98";
	mathCChr_Pi = "\xce\xa0";
	mathCChr_Delta = "\xce\x94";
	
	mathCChr_SubscriptOne = "\xc2\xb9";
	mathCChr_SubscriptTwo = "\xc2\xb2";
	mathCChr_SubscriptThree = "\xc2\xb3";
	
	mathCChr_Half = "\xc2\xbd";
	
	mathCChr_SquareRoot = "\xe2\x88\x9a";

	
	mathChr_Infinity = [NSString stringWithUTF8String:mathCChr_Infinity];
	mathChr_PlusMinus = [NSString stringWithUTF8String:mathCChr_PlusMinus];
	mathChr_LessThanEqualTo = [NSString stringWithUTF8String:mathCChr_LessThanEqualTo];
	mathChr_GreaterThanEqualTo = [NSString stringWithUTF8String:mathCChr_GreaterThanEqualTo];
	mathChr_DoesNotEquals = [NSString stringWithUTF8String:mathCChr_DoesNotEquals];
	mathChr_Sum = [NSString stringWithUTF8String:mathCChr_Sum];
	
	mathChr_Intergral = [NSString stringWithUTF8String:mathCChr_Intergral];
	mathChr_Theta = [NSString stringWithUTF8String:mathCChr_Theta];
	mathChr_Pi = [NSString stringWithUTF8String:mathCChr_Pi];
	mathChr_Delta = [NSString stringWithUTF8String:mathCChr_Delta];
	
	mathChr_SubscriptOne = [NSString stringWithUTF8String:mathCChr_SubscriptOne];
	mathChr_SubscriptTwo = [NSString stringWithUTF8String:mathCChr_SubscriptTwo];
	mathChr_SubscriptThree = [NSString stringWithUTF8String:mathCChr_SubscriptThree];
	
	mathChr_Half = [NSString stringWithUTF8String:mathCChr_Half];
	
	mathChr_SquareRoot = [NSString stringWithUTF8String:mathCChr_SquareRoot];
	mathChr_CubeRoot = [mathChr_SubscriptThree stringByAppendingString:mathChr_SquareRoot];
};


-(NSString*) decodeQuestion:(ClsQuiz*)cQuiz 
				iQuestionNo:(int)iQuestionNo {
	/*
	 Note: this sub does more advanced decoding because it also replaces numbers stored in the vecValues vector
	 */
	
	
	int iValuesCounter;
	int iValuesMax;
	float fValue;
	
	const char* cQuestion;
	cQuestion = cQuiz->sQuestionTextEncoded(iQuestionNo).c_str();
	NSString *sQuestion = [[NSString alloc] initWithString:[NSString stringWithCString:cQuestion length:strlen(cQuestion)]];			
	
	iValuesMax = cQuiz->iQuestionValuesCount(iQuestionNo);
	
	for (iValuesCounter=0; iValuesCounter<iValuesMax; iValuesCounter++) {
		fValue = cQuiz->fQuestionValue(iQuestionNo, iValuesCounter);
		
		NSString *sNumberNew = [[[NSString alloc] initWithString:[NSString stringWithFormat:@"%f", fValue]] autorelease];
		
		//if the sNumberNew ends in Zeros then we need go remove them
		if (@".000000" != [sNumberNew substringFromIndex:([sNumberNew length] - 7)]) {
			sNumberNew = [sNumberNew substringToIndex:([sNumberNew length] - 7)];
		};
		
		NSString *sSubString = [[[NSString alloc] initWithString:[NSString stringWithFormat:@"%i", iValuesCounter]] autorelease];
		sSubString = [@"<"  stringByAppendingString:sSubString];
		sSubString = [sSubString  stringByAppendingString:@">"];
		
		sQuestion = [sQuestion stringByReplacingOccurrencesOfString:sSubString 
											 withString:sNumberNew];
	};

	sQuestion = [self decodeText:sQuestion];
	
	return sQuestion;
};

-(NSString*) decodeAnswer:(ClsQuiz*)cQuiz 
			  iQuestionNo:(int)iQuestionNo
				  iChoice:(int)iChoice {
	
	const char* cAnswer;
	cAnswer = cQuiz->multipleChoiceAnswerText(iQuestionNo, iChoice).c_str();
	NSString *sAnswer = [[NSString alloc] initWithString:[NSString stringWithCString:cAnswer length:strlen(cAnswer)]];			
	
	sAnswer = [self decodeText:sAnswer];
	
	cAnswer = NULL;

	return sAnswer;
};

-(NSString*) decodeText:(NSString*)sText {
	
	sText = [sText stringByReplacingOccurrencesOfString:@"+-" 
													 withString:@" - "];
	sText = [sText stringByReplacingOccurrencesOfString:@"+ -" 
													 withString:@" - "];
	sText = [sText stringByReplacingOccurrencesOfString:@"  " 
													 withString:@" "];
	sText = [sText stringByReplacingOccurrencesOfString:@"  " 
													 withString:@" "];
	sText = [sText stringByReplacingOccurrencesOfString:@"  " 
													 withString:@" "];
	
	sText = [sText stringByReplacingOccurrencesOfString:@"<1/2>" 
											 withString:[NSString stringWithUTF8String:mathCChr_Half]];
	
	sText = [sText stringByReplacingOccurrencesOfString:@"<^1>" 
													 withString:[NSString stringWithUTF8String:mathCChr_SubscriptOne]];
	
	sText = [sText stringByReplacingOccurrencesOfString:@"<^2>"
													 withString:[NSString stringWithUTF8String:mathCChr_SubscriptTwo]];
	
	sText = [sText stringByReplacingOccurrencesOfString:@"<^3>" 
													 withString:[NSString stringWithUTF8String:mathCChr_SubscriptThree]];
	
	sText = [sText stringByReplacingOccurrencesOfString:@"<^1/2>" 
													 withString:[NSString stringWithUTF8String:mathCChr_SquareRoot]];
	
	sText = [sText stringByReplacingOccurrencesOfString:@"<^1/3>" 
													 withString:[[NSString stringWithUTF8String:mathCChr_SubscriptThree] stringByAppendingString:[NSString stringWithUTF8String:mathCChr_SquareRoot]]];
	
	sText = [sText stringByReplacingOccurrencesOfString:@"<pi>" 
											 withString:[NSString stringWithUTF8String:mathCChr_Pi]];
	
	return sText;
};

- (void)dealloc {
	[colText release];
	[colCell release]; 
	[colBoarder release]; 
	[colHighlighted release]; 
	
	[fntQuestions release];
	[fntAnswers release];
	[fntFileDetails release];
	[fntMenu release];
	[fntMisc release];
	[fntTitle release];
	[fntBaynhamCoding release];
	
	
	
	[imgRoundSelectorCyan release];

	[imgBtnAnswerMultiChoiceBlack release];
	[imgBtnAnswerMultiChoiceBlackGreenTick release];
	[imgBtnAnswerMultiChoiceBlackRedCross release];
	[imgBtnAnswerMultiChoiceBlackYellowArrow release];
	[imgBtnAnswerMultiChoiceBlue release];
	[imgBtnAnswerMultiChoiceCyan release];
	[imgBtnAnswerMultiChoiceGreen release];
	[imgBtnAnswerMultiChoiceRed release];
	[imgBtnAnswerMultiChoiceYellow release];

	[imgPicQuestionYellowTriangle release];
	[imgPicQuestionYellowCylinder release];
	[imgPicQuestionYellowParallogram release];
	
	[mathChr_Infinity release];
	[mathChr_PlusMinus release];
	[mathChr_LessThanEqualTo release];
	[mathChr_GreaterThanEqualTo release];
	
	[mathChr_DoesNotEquals release];
	[mathChr_Sum release];
	
	[mathChr_Intergral release];
	[mathChr_Theta release];
	[mathChr_Pi release];
	[mathChr_Delta release];
	
	[mathChr_SubscriptOne release];
	[mathChr_SubscriptTwo release];
	[mathChr_SubscriptThree release];
	
	[mathChr_Half release];
	
	[mathChr_SquareRoot release];
	[mathChr_CubeRoot release];
	
	[mathUtf8_Infinity release];
	[mathUtf8_PlusMinus release];
	[mathUtf8_LessThanEqualTo release];
	[mathUtf8_GreaterThanEqualTo release];
	
	[mathUtf8_DoesNotEquals release];
	[mathUtf8_Sum release];
	
	[mathUtf8_Intergral release];
	[mathUtf8_Theta release];
	[mathUtf8_Pi release];
	[mathUtf8_Delta release];
	
	[mathUtf8_SubscriptOne release];
	[mathUtf8_SubscriptTwo release];
	[mathUtf8_SubscriptThree release];
	
	[mathUtf8_Half release];
	
	[mathUtf8_SquareRoot release];
	[mathUtf8_CubeRoot release];
	
	
	mathCChr_Infinity = NULL;// = "\xe2\x88\x9e";
	mathCChr_PlusMinus = NULL;// = "\xc2\xb1";
	mathCChr_LessThanEqualTo = NULL;// = "\xe2\x89\xa4";
	mathCChr_GreaterThanEqualTo = NULL;// = "\xe2\x89\xa5";
	mathCChr_DoesNotEquals = NULL;// = "\xe2\x89\xa0";
	mathCChr_Sum = NULL;// = "\xe2\x88\x91";
	
	mathCChr_Intergral = NULL;// = "\xe2\x88\xab";
	mathCChr_Theta = NULL;// = "\xce\x98";
	mathCChr_Pi = NULL;// = "\xce\xa0";
	mathCChr_Delta = NULL;// = "\xce\x94";
	
	mathCChr_SubscriptOne = NULL;// = "\xc2\xb9";
	mathCChr_SubscriptTwo = NULL;// = "\xc2\xb2";
	mathCChr_SubscriptThree = NULL;// = "\xc2\xb3";
	
	mathCChr_Half = NULL;// = "\xc2\xbd";
	
	mathCChr_SquareRoot = NULL;// = "\xe2\x88\x9a";
	
	[super dealloc];
};

-(UIImage*) picture:(enumPicQues)ePictureType {
	bool bIsFound;
	
	bIsFound = false;
	
	if (ePictureType == ePicSelector_Cyan) {
		bIsFound = true;
		return imgRoundSelectorCyan;
	};
	
	if (ePictureType == ePicQues_TriangleRightAngle) {
		bIsFound = true;
		return imgPicQuestionYellowTriangle;
	};
	
	if (ePictureType == ePicQues_Cylinder) {
		bIsFound = true;
		return imgPicQuestionYellowCylinder;
	};
	
	if (ePictureType == ePicQues_Parallelogram) {
		bIsFound = true;
		return imgPicQuestionYellowParallogram;
	};
	
	if (ePictureType == ePicBtn_Black) {
		bIsFound = true;
		return imgBtnAnswerMultiChoiceBlack;
	};
	
	if (ePictureType == ePicBtn_Yellow) {
		bIsFound = true;
		return imgBtnAnswerMultiChoiceYellow;
	};
	
	if (ePictureType == ePicBtn_BlackYellowArrow) {
		bIsFound = true;
		return imgBtnAnswerMultiChoiceBlackYellowArrow;
	};
	
	if (ePictureType == ePicBtn_BlackGreenTick) {
		bIsFound = true;
		return imgBtnAnswerMultiChoiceBlackGreenTick;
	};
	
	if (ePictureType == ePicBtn_BlackRedCross) {
		bIsFound = true;
		return imgBtnAnswerMultiChoiceBlackRedCross;
	};
	
	if (ePictureType == ePicBtn_Blue) {
		bIsFound = true;
		return imgBtnAnswerMultiChoiceBlue;
	};
	
	if (ePictureType == ePicBtn_Cyan) {
		bIsFound = true;
		return imgBtnAnswerMultiChoiceCyan;
	};
	
	if (ePictureType == ePicBtn_Green) {
		bIsFound = true;
		return imgBtnAnswerMultiChoiceGreen;
	};
	
	if (ePictureType == ePicBtn_Red) {
		bIsFound = true;
		return imgBtnAnswerMultiChoiceRed;
	};
		
	if (bIsFound == false) {
		return nil;
	};
	
};


@end
