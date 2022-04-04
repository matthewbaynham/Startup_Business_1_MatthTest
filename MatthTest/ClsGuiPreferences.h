//
//  ClsGuiPreferences.h
//  Hello World Table View
//
//  Created by Matthew Baynham on 11/8/10.
//  Copyright 2010 Baynham Coding. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "modMisc.h"
#import "ClsQuiz.h"

using namespace modMisc;

@interface ClsGuiPreferences : NSObject {
	UIColor *colText; //colour of the text
	UIColor *colCell; //colour of the cells 
	UIColor *colBoarder; //colour of the boarder of the cells
	UIColor *colDelete; //colour of the text related to delete
	UIColor *colHighlighted; //colour of things that should be highlighted (in yellow)
	
	UIFont *fntQuestions; //font for all the questions
	UIFont *fntAnswers; //font for all the answers
	UIFont *fntFileDetails; //font for the file Details
	UIFont *fntMenu; //font for all the menus
	UIFont *fntMisc; //font for everything else
	UIFont *fntTitle; //font for Titles
	UIFont *fntBaynhamCoding; //font for Baynham Coding
	
	UIImage *imgRoundSelectorCyan;

	UIImage *imgBtnAnswerMultiChoiceBlack;
	UIImage *imgBtnAnswerMultiChoiceBlackGreenTick;
	UIImage *imgBtnAnswerMultiChoiceBlackRedCross;
	UIImage *imgBtnAnswerMultiChoiceBlackYellowArrow;
	UIImage *imgBtnAnswerMultiChoiceBlue;
	UIImage *imgBtnAnswerMultiChoiceCyan;
	UIImage *imgBtnAnswerMultiChoiceGreen;
	UIImage *imgBtnAnswerMultiChoiceRed;
	UIImage *imgBtnAnswerMultiChoiceYellow;

	UIImage *imgPicQuestionYellowTriangle;
	UIImage *imgPicQuestionYellowCylinder;
	UIImage *imgPicQuestionYellowParallogram;
	
	UTF8Char *mathUtf8_Infinity;
	UTF8Char *mathUtf8_PlusMinus;
	UTF8Char *mathUtf8_LessThanEqualTo;
	UTF8Char *mathUtf8_GreaterThanEqualTo;
	
	UTF8Char *mathUtf8_DoesNotEquals;
	UTF8Char *mathUtf8_Sum;
	
	UTF8Char *mathUtf8_Intergral;
	UTF8Char *mathUtf8_Theta;
	UTF8Char *mathUtf8_Pi;
	UTF8Char *mathUtf8_Delta;
	
	UTF8Char *mathUtf8_SubscriptOne;
	UTF8Char *mathUtf8_SubscriptTwo;
	UTF8Char *mathUtf8_SubscriptThree;
	
	UTF8Char *mathUtf8_Half;
	
	UTF8Char *mathUtf8_SquareRoot;
	UTF8Char *mathUtf8_CubeRoot;
	
	
	const char* mathCChr_Infinity;// = "\xe2\x88\x9e";
	const char* mathCChr_PlusMinus;// = "\xc2\xb1";
	const char* mathCChr_LessThanEqualTo;// = "\xe2\x89\xa4";
	const char* mathCChr_GreaterThanEqualTo;// = "\xe2\x89\xa5";
	const char* mathCChr_DoesNotEquals;// = "\xe2\x89\xa0";
	const char* mathCChr_Sum;// = "\xe2\x88\x91";
	
	const char* mathCChr_Intergral;// = "\xe2\x88\xab";
	const char* mathCChr_Theta;// = "\xce\x98";
	const char* mathCChr_Pi;// = "\xce\xa0";
	const char* mathCChr_Delta;// = "\xce\x94";
	
	const char* mathCChr_SubscriptOne;// = "\xc2\xb9";
	const char* mathCChr_SubscriptTwo;// = "\xc2\xb2";
	const char* mathCChr_SubscriptThree;// = "\xc2\xb3";
	
	const char* mathCChr_Half;// = "\xc2\xbd";
	
	const char* mathCChr_SquareRoot;// = "\xe2\x88\x9a";
	
	
	
	
	NSString *mathChr_Infinity;
	NSString *mathChr_PlusMinus;
	NSString *mathChr_LessThanEqualTo;
	NSString *mathChr_GreaterThanEqualTo;
	
	NSString *mathChr_DoesNotEquals;
	NSString *mathChr_Sum;
	
	NSString *mathChr_Intergral;
	NSString *mathChr_Theta;
	NSString *mathChr_Pi;
	NSString *mathChr_Delta;
	
	NSString *mathChr_SubscriptOne;
	NSString *mathChr_SubscriptTwo;
	NSString *mathChr_SubscriptThree;
	
	NSString *mathChr_Half;
	
	NSString *mathChr_SquareRoot;
	NSString *mathChr_CubeRoot;
	
}

-(id)init;
-(id)init:(bool)bIsDefault;
-(void)resetToDefault;
-(NSString*) decodeQuestion:(ClsQuiz*)cQuiz 
				iQuestionNo:(int)iQuestionNo;
//-(UIImage*) picture:(NSString*)sPictureName;
//-(UIImage*) picture:(const char*)sPictureName;
-(UIImage*) picture:(enumPicQues)ePictureType;

@property (readonly) UIColor *colText; //colour of the text
@property (readonly) UIColor *colCell; //colour of the cells 
@property (readonly) UIColor *colBoarder; //colour of the boarder of the cells
@property (readonly) UIColor *colDelete; //colour of the text related to delete
@property (readonly) UIColor *colHighlighted; //colour of things that should be highlighted (in yellow)

@property (readonly) UIFont *fntQuestions; //font for all the questions
@property (readonly) UIFont *fntAnswers; //font for all the answers
@property (readonly) UIFont *fntFileDetails; //font for the file Details
@property (readonly) UIFont *fntMenu; //font for all the menus
@property (readonly) UIFont *fntMisc; //font for everything else
@property (readonly) UIFont *fntTitle; //font for Titles
@property (readonly) UIFont *fntBaynhamCoding; //font for fntBaynham Coding

@property (readonly) UIImage *imgRoundSelectorCyan;

@property (readonly) UIImage *imgBtnAnswerMultiChoiceBlack;
@property (readonly) UIImage *imgBtnAnswerMultiChoiceBlackGreenTick;
@property (readonly) UIImage *imgBtnAnswerMultiChoiceBlackRedCross;
@property (readonly) UIImage *imgBtnAnswerMultiChoiceBlackYellowArrow;

@property (readonly) UIImage *imgBtnAnswerMultiChoiceBlue;
@property (readonly) UIImage *imgBtnAnswerMultiChoiceCyan;
@property (readonly) UIImage *imgBtnAnswerMultiChoiceGreen;
@property (readonly) UIImage *imgBtnAnswerMultiChoiceRed;
@property (readonly) UIImage *imgBtnAnswerMultiChoiceYellow;

@property (readonly) UIImage *imgPicQuestionYellowTriangle;
@property (readonly) UIImage *imgPicQuestionYellowCylinder;
@property (readonly) UIImage *imgPicQuestionYellowParallogram;

@property (readonly) NSString *mathChr_Infinity;
@property (readonly) NSString *mathChr_PlusMinus;
@property (readonly) NSString *mathChr_LessThanEqualTo;
@property (readonly) NSString *mathChr_GreaterThanEqualTo;

@property (readonly) NSString *mathChr_DoesNotEquals;
@property (readonly) NSString *mathChr_Sum;

@property (readonly) NSString *mathChr_Intergral;
@property (readonly) NSString *mathChr_Theta;
@property (readonly) NSString *mathChr_Pi;
@property (readonly) NSString *mathChr_Delta;

@property (readonly) NSString *mathChr_SubscriptOne;
@property (readonly) NSString *mathChr_SubscriptTwo;
@property (readonly) NSString *mathChr_SubscriptThree;

@property (readonly) NSString *mathChr_Half;

@property (readonly) NSString *mathChr_SquareRoot;
@property (readonly) NSString *mathChr_CubeRoot;

@end
