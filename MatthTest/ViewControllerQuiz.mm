//  ViewControllerQuiz.mm
//
//  Created by Matthew Baynham on 9/26/10.
//  Copyright 2010 Baynham Coding. All rights reserved.

#import "MatthTestAppDelegate.h"
#import "modMisc.h"
#import "ViewControllerQuiz.h"
#import "CellQuizMenu.h"
#import "ClsPreferences.h"
#import "CellQuestionTextAnswer.h"
#import "CellQuestionMultipleChoice.h"
#import "CellQuestionTextPicture.h"
#import "CellQuestionMultipleChoicePicture.h"
#import "ClsQuiz.h"
#import "AdWhirlView.h"
#import "SampleConstants.h"

using namespace std;
using namespace modMisc;

@implementation ViewControllerQuiz

@synthesize adView;
@synthesize  iQuestionHeightText, iQuestionHeightMultipleChoice, iQuestionHeightTextPic, iQuestionHeightMultipleChoicePic, iQuestionHeightMenu, iQuestionHeightAdvert;
@synthesize tTimer, sStatus, dteTimeStamp, vwActiv;

const int iMsgBoxMode_Delete = 1;
const int iMsgBoxMode_Mark = 2;

#pragma mark -
#pragma mark Initialization

- (id)init {
//	if (self = [super initWithNibName:@"ViewControllerQuiz" bundle:nil]) {
//		self.title = @"Ad In Table";
//	}
	
	return self;
}

- (void)loadView
{
	iMsgBoxMode = 0;
	iPreviousSecond = 0;
	iStartSecond = 0;
//	tTimer = [NSTimer scheduledTimerWithTimeInterval:5.0 
//											  target:self 
//											selector:@selector(startThreadCheckHeaderTimeSplit) 
//											userInfo:nil 
//											 repeats:YES];
	//	cQuizGuiLayout = ;
  	int iNoQuestions;
	
	iNoQuestions = cQuiz.iQuestionMax();
	
	cQuizGuiLayout.reInitialize(iNoQuestions);
	
	//With a menu tableCell
	//[[UIApplication sharedApplication] setStatusBarHidden:YES animated:NO];
	//self.navigationController.navigationBar.hidden = YES;
	
	//With the status bar buttons
	[[UIApplication sharedApplication] setStatusBarHidden:YES animated:NO];
	self.navigationController.navigationBar.hidden = NO;
	
	
	
	
	
	iQuestionHeightMenu = 135;
	iQuestionHeightText = 100;
	iQuestionHeightMultipleChoice = 350;
	iQuestionHeightTextPic = 250;
	iQuestionHeightMultipleChoicePic = 450;
	iQuestionHeightAdvert = 120;
	
	UITableView *tableView = [[UITableView alloc] initWithFrame:[[UIScreen mainScreen] applicationFrame]
														  style:UITableViewStyleGrouped];
	tableView.autoresizingMask = UIViewAutoresizingFlexibleHeight|UIViewAutoresizingFlexibleWidth;
	tableView.delegate = self;
	tableView.dataSource = self;
	tableView.separatorColor = [[[UIApplication sharedApplication] delegate] GuiColour:@"boarder"];//replace GUI

	[tableView reloadData];
	self.view = tableView;
	[tableView release];
	
	[self.adView retain];
	
	[self updateHeaderTimeSplit];
	
}

/*
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
												cPref:(ClsPreferences *)cPref {


}
*/

//initWithStyle: or viewDidLoad
/*
//initWithFrame:style
- (id)initWithFrame:(UITableViewStyle)style bundle:(ClsPreferences *)cPref {
    // Override initWithStyle: if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
    if ((self = [super initWithStyle:style])) {
		
    }
    return self;
}
*/
/*
//- (id)initWithStyle:(UITableViewStyle)style cPref:(ClsPreferences *)cPref {
- (id)initWithStyle:(UITableViewStyle)style  {
    // Override initWithStyle: if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
    if ((self = [super initWithStyle:style])) {
		cGuiPreferences = [[ClsGuiPreferences alloc] init];
    }
    return self;
}
*/
#pragma mark -
#pragma mark View lifecycle


//set the quiz pointer here
-(void)viewDidLoad {
    [super viewDidLoad];
	self.view.backgroundColor = [[[UIApplication sharedApplication] delegate] GuiColour:@"boarder"];//replace GUI

	const char* cStatus;
	cStatus = cQuiz.headerValue(modMisc::sHeader_QuizStatus).c_str();
	sStatus = [[[NSString alloc] initWithString:[NSString stringWithCString:cStatus length:strlen(cStatus)]] retain];
	cStatus = NULL;
	
	self.adView = [AdWhirlView requestAdWhirlViewWithDelegate:self];
	self.adView.autoresizingMask = UIViewAutoresizingFlexibleLeftMargin|UIViewAutoresizingFlexibleRightMargin;
	
    // Uncomment the following line to preserve selection between presentations.
	//self.clearsSelectionOnViewWillAppear = NO;
	
    // Uncomment the following line to display an Edit button in the navigation bar for this view controller.
    // self.navigationItem.rightBarButtonItem = self.editButtonItem;
	
//	self.navigationItem.hidesBackButton = YES;
	
	
//	iStartSecond = 0;
//	[self updateHeaderTimeSplit];
	[self configMenu];
};




- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];

	[[UIApplication sharedApplication] setStatusBarHidden:YES animated:NO];
	self.navigationController.navigationBar.hidden = NO;
}

/* 
- (void)viewDidAppear:(BOOL)animated {
    [super viewDidAppear:animated];
}
*/

- (void)viewWillDisappear:(BOOL)animated {
//	iStartSecond = 0;
//	[tTimer invalidate];

    [super viewWillDisappear:animated];
}

- (void)viewDidDisappear:(BOOL)animated {
//	iStartSecond = 0;
//	[tTimer invalidate];
    [super viewDidDisappear:animated];
}

/*
- (void)drawRect:(CGRect)rect
{
	[[UIApplication sharedApplication] setStatusBarHidden:YES animated:NO];
	self.navigationController.navigationBar.hidden = YES;
}
*/
// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    return YES;
}

- (void)willAnimateRotationToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation duration:(NSTimeInterval)duration
{
	[self.adView rotateToOrientation:toInterfaceOrientation];
	[self adjustAdSize];
}

/*
-(void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation {
	
};
*/


#pragma mark -
#pragma mark Table view data source

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    // Return the number of sections.
	NSInteger iQuestionCount = cQuizGuiLayout.totalCells();
	
	return iQuestionCount;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    // Return the number of rows in the section.
    return 1;
}


// Customize the appearance of table view cells.
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    int iQuestionNo;
	int iQuestionMax;
	enumQuizGuiLayout eCellGuiLayout;
	int iSection;
	
	iSection = indexPath.section;
	
	eCellGuiLayout = cQuizGuiLayout.cellLayout(iSection);
	iQuestionNo = cQuizGuiLayout.questionNo(iSection);
	iQuestionMax = cQuiz.iQuestionMax();

	if (eCellGuiLayout == eQuGui_Question) {
		if (cQuiz.questionIsPicture(iQuestionNo)) {
			enumPicQues ePicture;
			
			ePicture = cQuiz.picType(iQuestionNo);

			if (cQuiz.questionIsMultipleChoice(iQuestionNo) == true) {
				static NSString *CellIdentifier = [[[NSString alloc] initWithString:@"CellQuestionMultipleChoicePicture"] autorelease];

				UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
				
				if (cell == nil) {
					cell = [[[CellQuestionMultipleChoicePicture alloc] initWithStyle:UITableViewCellStyleSubtitle 
																	 reuseIdentifier:nil
																		ePictureType:ePicture
																	 iQuestionNumber:iQuestionNo]
								autorelease];
				};

				[cell setBackgroundColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"cell"]];
				cell.selectionStyle = UITableViewCellSelectionStyleNone;
				[cell setParent:self];
				
				return cell;
			} else {
				static NSString *CellIdentifier = [[[NSString alloc] initWithString:@"CellQuestionTextPicture"] autorelease];
				
				UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
				
				if (cell == nil) {
					cell = [[[CellQuestionTextPicture alloc] initWithStyle:UITableViewCellStyleSubtitle 
														  reuseIdentifier:nil
															  ePictureType:ePicture
														  iQuestionNumber:iQuestionNo]
							autorelease];
				};
				
				[cell setBackgroundColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"cell"]];
				cell.selectionStyle = UITableViewCellSelectionStyleNone;
				[cell setParent:self];

				return cell;
			};
		} else {
			if (cQuiz.questionIsMultipleChoice(iQuestionNo) == true) {
				static NSString *CellIdentifier = [[[NSString alloc] initWithString:@"CellQuestionMultipleChoice"] autorelease];
				
				UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
				
				if (cell == nil) {
					cell = [[[CellQuestionMultipleChoice alloc] initWithStyle:UITableViewCellStyleSubtitle 
															  reuseIdentifier:nil
															  iQuestionNumber:iQuestionNo]
							autorelease];
				};
				
				[cell setBackgroundColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"cell"]];
				cell.selectionStyle = UITableViewCellSelectionStyleNone;
				[cell setParent:self];
				
				return cell;
			} else {
				static NSString *CellIdentifier = [[[NSString alloc] initWithString:@"CellQuestionTextAnswer"] autorelease];
				
				UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
				
				if (cell == nil) {
					cell = [[[CellQuestionTextAnswer alloc] initWithStyle:UITableViewCellStyleSubtitle 
														  reuseIdentifier:nil
														  iQuestionNumber:iQuestionNo]
							autorelease];
				};
				
				[cell setBackgroundColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"cell"]];
				cell.selectionStyle = UITableViewCellSelectionStyleNone;
				[cell setParent:self];

				return cell;
			};
		};
	} else if (eCellGuiLayout == eQuGui_Menu) {
		static NSString *CellIdentifier = [[[NSString alloc] initWithString:@"CellQuizMenu"] autorelease];
		
		UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
		
		if (cell == nil) {
			const char* cPathQuiz;
			cPathQuiz = cQuiz.getPath().c_str();
			NSString *sPathQuiz =[[[NSString alloc] initWithString:[NSString stringWithCString:cPathQuiz length:strlen(cPathQuiz)]] autorelease];			
			
			cell = [[[CellQuizMenu alloc] initWithStyle:UITableViewCellStyleSubtitle 
										reuseIdentifier:nil
												  sPath:sPathQuiz]
					autorelease];
		};
		
		[cell setBackgroundColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"cell"]];
		cell.selectionStyle = UITableViewCellSelectionStyleNone;
		[cell setParent:self];

		return cell;
	} else if (eCellGuiLayout == eQuGui_Advert) {
		static NSString *CellIdentifier = [[[NSString alloc] initWithString:@"Cell"] autorelease];
		
		UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
		if (cell == nil) {
			cell = [[[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault 
										   reuseIdentifier:CellIdentifier] 
					autorelease];
		}
		
        [cell setText:@"fun with maths..."];
        [cell setBackgroundColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"cell"]];
        [cell.detailTextLabel setTextAlignment:UITextAlignmentCenter];
        [cell.detailTextLabel setTextColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"cell"]];
        [cell.detailTextLabel setTextHighlightedTextColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"highlight"]];
        [cell.textLabel setFont:[[[UIApplication sharedApplication] delegate] GuiFont:@"answers"]];
        [cell.textLabel setTextColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"highlight"]];
        [cell.textLabel setTextAlignment:UITextAlignmentCenter];
		cell.selectionStyle = UITableViewCellSelectionStyleNone;
		[cell.contentView addSubview:adView];
		
		[adView requestFreshAd];
		[self checkHeaderTimeSplit];
		return cell;
	};
}

/*
// Override to support conditional editing of the table view.
- (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath {
    // Return NO if you do not want the specified item to be editable.
    return YES;
}
*/
/*
// Override to support editing the table view.
- (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath {
    
    if (editingStyle == UITableViewCellEditingStyleDelete) {
        // Delete the row from the data source
        [tableView deleteRowsAtIndexPaths:[NSArray arrayWithObject:indexPath] withRowAnimation:YES];
    }   
    else if (editingStyle == UITableViewCellEditingStyleInsert) {
        // Create a new instance of the appropriate class, insert it into the array, and add a new row to the table view
    }   
}
*/
/*
// Override to support rearranging the table view.
- (void)tableView:(UITableView *)tableView moveRowAtIndexPath:(NSIndexPath *)fromIndexPath toIndexPath:(NSIndexPath *)toIndexPath {
}
*/
/*
// Override to support conditional rearranging of the table view.
- (BOOL)tableView:(UITableView *)tableView canMoveRowAtIndexPath:(NSIndexPath *)indexPath {
    // Return NO if you do not want the item to be re-orderable.
    return YES;
}
*/

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath: (NSIndexPath *)indexPath 
{ 
	CGFloat fheight; 
	int iQuestionNo;
	int iQuestionMax;
	enumQuizGuiLayout eCellGuiLayout;
	int iSection;
	
	iSection = indexPath.section;
	
	//	iQuestionNo = indexPath.section - 1;
	eCellGuiLayout = cQuizGuiLayout.cellLayout(iSection);
	iQuestionNo = cQuizGuiLayout.questionNo(iSection);
	iQuestionMax = cQuiz.iQuestionMax();
	
	if (eCellGuiLayout == eQuGui_Question) {
		if (cQuiz.questionIsPicture(iQuestionNo)) {
			if (cQuiz.questionIsMultipleChoice(iQuestionNo) == YES) {
				fheight = self.iQuestionHeightMultipleChoicePic;
			} else {
				fheight = self.iQuestionHeightTextPic;
			};
		} else {
			if (cQuiz.questionIsMultipleChoice(iQuestionNo) == YES) {
				fheight = self.iQuestionHeightMultipleChoice;
			} else {
				fheight = self.iQuestionHeightText;
			};
		};
	} else if (eCellGuiLayout == eQuGui_Menu) {
		fheight = self.iQuestionHeightMenu;
	} else if (eCellGuiLayout == eQuGui_Advert) {
		fheight = kAdWhirlViewHeight;
	};
		
	return fheight; 
} 

#pragma mark -
#pragma mark Table view delegate

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
    // Navigation logic may go here. Create and push another view controller.
	/*
	 <#DetailViewController#> *detailViewController = [[<#DetailViewController#> alloc] initWithNibName:@"<#Nib name#>" bundle:nil];
     // ...
     // Pass the selected object to the new view controller.
	 [self.navigationController pushViewController:detailViewController animated:YES];
	 [detailViewController release];
	 */
}

#pragma mark -
#pragma mark Memory management

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Relinquish ownership any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
	iStartSecond = 0;
    // Relinquish ownership of anything that can be recreated in viewDidLoad or on demand.
    // For example: self.myOutlet = nil;
//	[tableview autorelease];
	[self.view release];
}

- (void)dealloc {
//    [self startThreadUpdateHeaderTimeSplit];
    [self updateHeaderTimeSplit];
	iStartSecond = 0;
	self.adView.delegate = nil;
	self.adView = nil;

    
//	cQuiz = Nil;
//	ClsPreferences cPreferences;
//	[cQuizGuiLayout dealloc];
	
//	int iStartSecond;
//	int iPreviousSecond;
//	int iQuestionHeightMenu;
//	int iQuestionHeightText;
//	int iQuestionHeightMultipleChoice;
//	int iQuestionHeightTextPic;
//	int iQuestionHeightMultipleChoicePic;
//	int iQuestionHeightAdvert;
//	[sStatus release];
	
//	[tTimer release];
	[dteTimeStamp release];
	
//	AdWhirlView *adView;
//	UIActivityIndicatorView* vwActiv;
	
	[super dealloc];
}

#pragma mark -
#pragma mark custom functions

-(void) setPref:(ClsPreferences*) cPref {
	int iDifficulty;
	int iQuestionCounter;
	NSString* sPathQuiz = [[[NSString alloc] initWithString:@""] autorelease];
	NSError* errError;
	
	[vwActiv startAnimating];
	cPreferences = *cPref;
	iDifficulty = (int)cPreferences.getDifficulty();
	
	const char* cPathTemp;
	cPathTemp = cPref->getPath()->c_str();
	NSString *sPathPreference = [[[NSString alloc] initWithString:[NSString stringWithCString:cPathTemp length:strlen(cPathTemp)]] autorelease];			

	//create a path address with time stamp for the quiz sPathQuiz 
	//use the changePath method in the ClsPreference class to create a new DB file at the new location
	//cPreferences.changePath(stdPathQuiz)
	//send the same new path address stdPathQuiz to the ClsQuiz class so that it can save all the
	//question information in the same file (obviously with different table names, except the header table)
	
	bool bCopyingProceed;
	
	if ([dteTimeStamp retainCount] == 0) {
		dteTimeStamp = [[[NSDate alloc] init] retain];
	} else if (dteTimeStamp == NULL or dteTimeStamp == nil) {
		dteTimeStamp = [[[NSDate alloc] init] retain];
	};
	
	sPathQuiz = [[[UIApplication sharedApplication] delegate] DirGenerateQuizPath:dteTimeStamp];
	bCopyingProceed = [[NSFileManager defaultManager] copyItemAtPath:sPathPreference toPath:sPathQuiz error:&errError];

	std::string* stdPathQuiz = new std::string([sPathQuiz UTF8String]);
//	std::string* stdPathPref = new std::string([sPathQuiz UTF8String]);
	
	cQuiz.setPath(*stdPathQuiz); //first
	cQuiz.createEmptyQuizTables();
	cQuiz.selectHeader();
	cPreferences.load_subjects();
	
	for (iQuestionCounter = 0; iQuestionCounter < cPreferences.getNoOfQuestions(); iQuestionCounter++) {
		cQuiz.addQuestion(&cPreferences);
	};
	
	std::stringstream ssNoOfQuestions;
	ssNoOfQuestions << iQuestionCounter;
	
	std::string sNoOfQuestions = ssNoOfQuestions.str();
	
	cQuiz.changeHeader(modMisc::sHeader_QuestionTotal, sNoOfQuestions);
	cQuiz.changeHeader(modMisc::sHeader_QuestionCount, (std::string)"0");
	cQuiz.changeHeader(modMisc::sHeader_QuizStatus, (std::string)"Incomplete");
	
	NSDate *dteNow = [[[NSDate alloc] init] autorelease];
	NSDateFormatter *dteFmt = [[[NSDateFormatter alloc] init] autorelease];
	[dteFmt setDateFormat:@"HH:mm:ss dd MMM yyyy"];
	
	//Optionally for time zone converstions
	[dteFmt setTimeZone:[NSTimeZone timeZoneWithName:@"..."]];
	
	NSString *sDate = [[[NSString alloc] initWithString:[dteFmt stringFromDate:dteNow]] autorelease];
	
	std::string stdDate = [sDate UTF8String];
	
	cQuiz.changeHeader(modMisc::sHeader_QuizCreatedDate, stdDate);
	cQuiz.changeHeader(modMisc::sHeader_AppVersion, modMisc::sVersion);
	
	cQuiz.selectHeader();
};

-(void) setQuiz:(NSString*)sPath {
	int iDifficulty;
	
	std::string stdPathQuiz = [sPath UTF8String];
	
	cPreferences.setPath(stdPathQuiz);
	iDifficulty = (int)cPreferences.getDifficulty();
	
	//create a path address with time stamp for the quiz sPathQuiz 
	//use the changePath method in the ClsPreference class to create a new DB file at the new location
	//cPreferences.changePath(stdPathQuiz)
	//send the same new path address stdPathQuiz to the ClsQuiz class so that it can save all the
	//question information in the same file (obviously with different table names, except the header table)
	
	cQuiz.setPath(stdPathQuiz);
	cQuiz.loadQuestions();
	cQuiz.selectHeader();
	cPreferences.load_subjects();
		
	NSDate *dteNow = [[[NSDate alloc] init] autorelease];
	NSDateFormatter *dteFmt = [[[NSDateFormatter alloc] init] autorelease];
	[dteFmt setDateFormat:@"HH:mm:ss dd MMM yyyy"];
	
	//Optionally for time zone converstions
	[dteFmt setTimeZone:[NSTimeZone timeZoneWithName:@"..."]];
	
	NSString *sDate = [[[NSString alloc] initWithString:[dteFmt stringFromDate:dteNow]] autorelease];
	
	std::string stdDate = [sDate UTF8String];
	
	cQuiz.changeHeader(modMisc::sHeader_QuizStartDateTime, stdDate);
	cQuiz.selectHeader();
};

-(void) markQuiz {
	[self checkHeaderTimeSplit];
	iStartSecond = 0;

	cQuiz.mark();
	cQuiz.changeHeader(modMisc::sHeader_QuizStatus, (std::string)"Finished");
	cQuiz.selectHeader();
	const char* cStatus;

	cStatus = cQuiz.headerValue(modMisc::sHeader_QuizStatus).c_str();
	sStatus = [[[NSString alloc] initWithString:[NSString stringWithCString:cStatus length:strlen(cStatus)]] retain];
	cStatus = NULL;
}

-(void) refreshQuiz {
	[[self.navigationController visibleViewController].view setNeedsDisplay];
	
	
	NSArray* arrVisibleCells = [[[NSArray alloc] initWithArray:[self.view visibleCells]] autorelease];
	
	NSEnumerator *e = [arrVisibleCells objectEnumerator];
	id object;
	
	while (object = [e nextObject]) {
		[object setNeedsDisplay];
	};
	
	
}

-(void) closeTableView {
	if ([tTimer isValid] == YES) {
		[tTimer invalidate];
	};
	
	[self checkHeaderTimeSplit];
	iStartSecond = 0;

	[self.navigationController popViewControllerAnimated:YES];
}

-(void) btnMultipleChoiceSelected:(int)iQuestionNo 
						  iChoice:(int)iChoice 
						bSelected:(bool)bSelected {
	[self checkHeaderTimeSplit];

	//set the value of the boolean to select this choice
	cQuiz.multipleChoiceAnswerSelect(iQuestionNo, iChoice, bSelected);
}

-(bool) btnMultipleChoiceIsSelected:(int)iQuestionNo 
						    iChoice:(int)iChoice {
	[self checkHeaderTimeSplit];

	//return if the choice is selected
	int iResult;
	bool bResult;
	
	iResult = cQuiz.multipleChoiceAnswerSelected(iQuestionNo, iChoice);
	
	if (iResult == 1) {
		bResult = true;
	} else {
		bResult = false;
	};
	
	return bResult;
}

-(bool) btnMultipleChoiceIsCorrect:(int)iQuestionNo 
						   iChoice:(int)iChoice {
	[self checkHeaderTimeSplit];

	//return if the choice is correct
	int iResult;
	bool bResult;
	
	iResult = cQuiz.multipleChoiceAnswerCorrect(iQuestionNo, iChoice);
	
	if (iResult == 1) {
		bResult = true;
	} else {
		bResult = false;
	};
	
	return bResult;
};

-(NSString*) sQuestionTextDecoded:(int)iQuestionNo {
	NSString *sQuestionTemp = [[NSString alloc] initWithString:[[[UIApplication sharedApplication] delegate] sQuestionTextDecoded:iQuestionNo cQuiz:&cQuiz]];
	
	return sQuestionTemp;
}

-(NSString*) sAnswerMulipleChoiceDecoded:(int)iQuestionNo
								 iChoice:(int)iChoice {
	NSString *sAnswerTemp = [[NSString alloc] initWithString:[[[UIApplication sharedApplication] delegate] sAnswerMulipleChoiceDecoded:iQuestionNo iChoice:iChoice cQuiz:&cQuiz]];
	
	return sAnswerTemp;
}

-(NSString*) sQuizStatus {
//	const char* cStatus;
//	cStatus = cQuiz.headerValue(modMisc::sHeader_QuizStatus).c_str();
//	NSString *sStatus =[[NSString alloc] initWithString:[NSString stringWithCString:cStatus length:strlen(cStatus)]];
	
//	cStatus = NULL;
	
	NSString* StatusTemp = [[[NSString alloc] initWithString:self.sStatus] retain]; 
	
	return StatusTemp;
};

-(NSString*) sQuestionGroup:(int)iQuestionNo {
//	std::string stdGroup = [cQuiz.sQuestionGroup(iQuestionNo).c
	
	NSString* sGroup = [[[NSString alloc] initWithCString:cQuiz.sQuestionGroup(iQuestionNo).c_str()] autorelease]; 
	
	return sGroup;
};

-(NSString*) sQuestionSubject:(int)iQuestionNo {
	NSString* sSubject = [[[NSString alloc] initWithCString:cQuiz.sQuestionSubject(iQuestionNo).c_str()] autorelease]; 
	
	return sSubject;
};


-(bool) isOnlyOneAnswer:(int)iQuestionNo {
	bool bOnlyOneAnswer = cQuiz.multipleChoiceInOnlyOneAnswer(iQuestionNo);

	return bOnlyOneAnswer;
};

-(void)startThreadCheckHeaderTimeSplit {
    [self performSelectorInBackground:@selector(checkHeaderTimeSplit) withObject:nil];
}

-(void)checkHeaderTimeSplit {
	NSDate *dteNow = [[NSDate alloc] init];
	NSTimeInterval tiNow = [dteNow timeIntervalSince1970];
	int iNow = (int)tiNow;
	
	if (iPreviousSecond + 30 <= iNow) {
		iStartSecond = 0;
	};

	NSDate *dtePrevious = [[NSDate alloc] init];
	NSTimeInterval tiPrevious = [dtePrevious timeIntervalSince1970];
	iPreviousSecond = (int)tiPrevious;
	
	[self updateHeaderTimeSplit];
    
    [dteNow release];
    [dtePrevious release];
};

//-(void)startThreadUpdateHeaderTimeSplit {
//    [self performSelectorInBackground:@selector(updateHeaderTimeSplit) withObject:nil];
//}

-(void)updateHeaderTimeSplit {
	if (iStartSecond == 0) {
		//if the has only just openned
		NSDate *dteNow = [[NSDate alloc] init];
		NSTimeInterval tiNow = [dteNow timeIntervalSince1970];
		int iNow = (int)tiNow;

		std::stringstream ssDate;
		
		ssDate << iNow;
		
		std::string sDate = ssDate.str();
		
		cQuiz.insertHeader(modMisc::sHeader_QuizDateTimeSplit, sDate, sDate);
		
		iStartSecond = iNow;
		iPreviousSecond = iNow;
        
        [dteNow release];
	} else {
		//if the view is already open
		NSDate *dteNow = [[NSDate alloc] init];
		NSTimeInterval tiNow =  [dteNow timeIntervalSince1970];
		int iNow = (int)tiNow;
		std::stringstream ssStartDate;
		
		ssStartDate << iStartSecond;
		
		std::string sStartDate = ssStartDate.str();
		std::stringstream ssEndDate;
		
		ssEndDate << iNow;
		
		std::string sEndDate;
		sEndDate = ssEndDate.str();
		
		cQuiz.updateHeaderValue2(modMisc::sHeader_QuizDateTimeSplit, sStartDate, sEndDate);

		iPreviousSecond = iNow;

        [dteNow release];
    };
};

-(void) clickedBtnDelete {
	iMsgBoxMode = iMsgBoxMode_Delete;
	
//	NSString* sStatus = [[[NSString alloc] initWithString:[self sQuizStatus]] autorelease];
	
//	if ([sStatus isEqualToString:@"Finished"] != YES) {
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
//	};
};

-(void) clickedBtnMark {
	iMsgBoxMode = iMsgBoxMode_Mark;
	
	NSString* sStatus = [[[NSString alloc] initWithString:[self sQuizStatus]] autorelease];
	
	if ([sStatus isEqualToString:@"Finished"] != YES) {
		NSString *sTitle = [[[NSString alloc] initWithString:@"Mark?"] autorelease];
		NSString *sMessage = [[[NSString alloc] initWithString:@"Do you want to mark this test?"] autorelease];
		NSString *sBtnCancel = [[[NSString alloc] initWithString:@"Cancel"] autorelease];
		NSString *sBtnMark = [[[NSString alloc] initWithString:@"Mark"] autorelease];
		
		UIAlertView *msgMark = [[UIAlertView alloc] initWithTitle:sTitle
														  message:sMessage
														 delegate:self 
												cancelButtonTitle:sBtnCancel 
												otherButtonTitles:sBtnMark, nil];
		[msgMark show];
		[msgMark release];
	};
};

-(void) clickedBtnSave {
	[self closeTableView];
};

- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex {
	//the delete pop up
	switch (iMsgBoxMode) {
		case iMsgBoxMode_Delete:
			if (buttonIndex == 1) {
				const char* cPathQuiz;
				cPathQuiz = cQuiz.getPath().c_str();
				NSString *sPathQuiz =[[[NSString alloc] initWithString:[NSString stringWithCString:cPathQuiz length:strlen(cPathQuiz)]] autorelease];			
				cPathQuiz = NULL;
				
				[[[UIApplication sharedApplication] delegate] DirDeleteFile:sPathQuiz];
				
				[self closeTableView];
			};
			break;

		case iMsgBoxMode_Mark:
			if (buttonIndex == 1) {
                [self markQuiz];
                [self refreshQuiz];
                [self configMenu];
            };
			break;
		default:
			break;
	}
	
}

-(void) configMenu {
	NSString* sStatusTemp = [[[NSString alloc] initWithString:[self sQuizStatus]] autorelease];

	UIBarButtonItem *btnDelete = [[UIBarButtonItem alloc] initWithTitle:@"Delete" 
																  style:UIBarButtonItemStyleBordered
																 target:self 
																 action:@selector(clickedBtnDelete)];
	self.navigationItem.rightBarButtonItem = btnDelete;
	[btnDelete release];
	
	CGRect frmMark = CGRectMake(0, 0, 100, 30);	
	UIView* vwTitleView = [[UIView alloc] initWithFrame:frmMark];
	[vwTitleView setBackgroundColor:[UIColor clearColor]];

	if ([sStatusTemp isEqualToString:@"Finished"] != YES) {
		UIButton* btnMark = [[UIButton alloc] initWithFrame:vwTitleView.frame];
		
		[btnMark setTitle:@"Mark" forState:UIControlStateNormal];
		[btnMark setBackgroundColor:[UIColor clearColor]];
		[btnMark setTitleColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"text"] forState:UIControlStateNormal];
		btnMark.titleLabel.font = [[[UIApplication sharedApplication] delegate] GuiFont:@"filedetails"];//Try the Gui Thing
		[btnMark setEnabled:YES];
		[btnMark setSelected:NO];
		[btnMark setHighlighted:NO];
		
		[btnMark setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Yellow] forState:UIControlStateNormal];//Try the Gui Thing
		[btnMark setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Black] forState:UIControlStateDisabled];//Try the Gui Thing
		[btnMark setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Red] forState:UIControlStateSelected];//Try the Gui Thing
		[btnMark setBackgroundImage:[[[UIApplication sharedApplication] delegate] GuiPicture:ePicBtn_Blue] forState:UIControlStateHighlighted];//Try the Gui Thing
		
		[btnMark addTarget:self action:@selector(clickedBtnMark) forControlEvents:UIControlEventTouchUpInside];
		
		[vwTitleView addSubview:btnMark];
		self.navigationItem.titleView = vwTitleView;
		[btnMark release];
	} else {
		UILabel* lblGrade = [[UILabel alloc] initWithFrame:vwTitleView.frame];
		
		cQuiz.selectHeader();
		const char* cQuestionsTotalValue;
		cQuestionsTotalValue = cQuiz.headerValue(modMisc::sHeader_QuestionTotal).c_str();
		NSString *sQuestionsTotalValue = [[NSString alloc] initWithString:[NSString stringWithCString:cQuestionsTotalValue length:strlen(cQuestionsTotalValue)]];

		const char* cQuestionsResultsValue;
		cQuestionsResultsValue = cQuiz.headerValue(modMisc::sHeader_QuestionCount).c_str();
		NSString *sQuestionsResultsValue = [[NSString alloc] initWithString:[NSString stringWithCString:cQuestionsResultsValue length:strlen(cQuestionsResultsValue)]];
		
		NSString *sResult = [[NSString alloc] initWithString:@""];
		
		sResult = [sQuestionsResultsValue stringByAppendingString:@" of "];
		sResult = [sResult stringByAppendingString:sQuestionsTotalValue];

		[lblGrade setText:sResult];
		[lblGrade setTextColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"highlight"]];
		[lblGrade setTextAlignment:UITextAlignmentCenter]; 
		[lblGrade setFont:[[[UIApplication sharedApplication] delegate] GuiFont:@"title"]];
		[lblGrade setBackgroundColor:[UIColor clearColor]];
		
		[vwTitleView addSubview:lblGrade];
		self.navigationItem.titleView = vwTitleView;
		[lblGrade release];
	};
	
	if ([sStatusTemp isEqualToString:@"Finished"] != YES) {
		UIBarButtonItem *btnSave = [[UIBarButtonItem alloc] initWithTitle:@"Save" 
																	style:UIBarButtonItemStyleBordered
																   target:self 
																   action:@selector(clickedBtnSave)];
		self.navigationItem.leftBarButtonItem = btnSave;
		[btnSave release];
	} else {
		UIBarButtonItem *btnSave = [[UIBarButtonItem alloc] initWithTitle:@"Exit" 
																	style:UIBarButtonItemStyleBordered
																   target:self 
																   action:@selector(clickedBtnSave)];
		self.navigationItem.leftBarButtonItem = btnSave;
		[btnSave release];
	};
	
    [sStatusTemp release];
};


#pragma mark -
#pragma mark AdWhirl


- (void)adjustAdSize {
	[UIView beginAnimations:@"AdResize" context:nil];
	[UIView setAnimationDuration:0.7];
	CGSize adSize = [adView actualAdSize];
	CGRect newFrame = adView.frame;
	newFrame.origin.x = (self.view.bounds.size.width - adSize.width)/2 - 10;
	newFrame.origin.y = 0;
	
	newFrame.size.height = adSize.height;
	newFrame.size.width = adSize.width;
	//	newFrame.origin.x = (self.view.bounds.size.width - adSize.width)/2;
	adView.frame = newFrame;
	[UIView commitAnimations];
}


- (UILabel *)label {
	return (UILabel *)[self.view viewWithTag:1337];
}

- (UITableView *)table {
	return (UITableView *)[self.view viewWithTag:3337];
}

#pragma mark AdWhirlDelegate methods

- (NSString *)adWhirlApplicationKey {
	return kSampleAppKey;
}

- (UIViewController *)viewControllerForPresentingModalView {
	return [((MatthTestAppDelegate*)[[UIApplication sharedApplication] delegate]) navigationController];
}

- (NSURL *)adWhirlConfigURL {
	return [NSURL URLWithString:kSampleConfigURL];
}

- (NSURL *)adWhirlImpMetricURL {
	return [NSURL URLWithString:kSampleImpMetricURL];
}

- (NSURL *)adWhirlClickMetricURL {
	return [NSURL URLWithString:kSampleClickMetricURL];
}

- (NSURL *)adWhirlCustomAdURL {
	return [NSURL URLWithString:kSampleCustomAdURL];
}

- (void)adWhirlDidReceiveAd:(AdWhirlView *)adWhirlView {
	self.label.text = [NSString stringWithFormat:
					   @"Got ad from %@, size %@",
					   [adWhirlView mostRecentNetworkName],
					   NSStringFromCGSize([adWhirlView actualAdSize])];
	[self adjustAdSize];
}

- (void)adWhirlDidFailToReceiveAd:(AdWhirlView *)adWhirlView usingBackup:(BOOL)yesOrNo {
	self.label.text = [NSString stringWithFormat:
					   @"Failed to receive ad from %@, %@. Error: %@",
					   [adWhirlView mostRecentNetworkName],
					   yesOrNo? @"will use backup" : @"will NOT use backup",
					   adWhirlView.lastError == nil? @"no error" : [adWhirlView.lastError localizedDescription]];
}

- (void)adWhirlReceivedRequestForDeveloperToFufill:(AdWhirlView *)adWhirlView {
	UILabel *replacement = [[UILabel alloc] initWithFrame:kAdWhirlViewDefaultFrame];
	replacement.backgroundColor = [UIColor redColor];
	replacement.textColor = [UIColor whiteColor];
	replacement.textAlignment = UITextAlignmentCenter;
	replacement.text = @"Generic Notification";
	[adWhirlView replaceBannerViewWith:replacement];
	[replacement release];
	[self adjustAdSize];
	self.label.text = @"Generic Notification";
}

- (void)adWhirlDidAnimateToNewAdIn:(AdWhirlView *)adWhirlView {
	[self.table reloadData];
}

- (void)adWhirlReceivedNotificationAdsAreOff:(AdWhirlView *)adWhirlView {
	self.label.text = @"Ads are off";
}

- (void)adWhirlWillPresentFullScreenModal {
	NSLog(@"TableView: will present full screen modal");
}

- (void)adWhirlDidDismissFullScreenModal {
	NSLog(@"TableView: did dismiss full screen modal");
}

- (void)adWhirlDidReceiveConfig:(AdWhirlView *)adWhirlView {
	self.label.text = @"Received config. Requesting ad...";
}

- (BOOL)adWhirlTestMode {
	return NO;
}

- (NSUInteger)jumptapTransitionType {
	return 3;
}

- (NSUInteger)quattroWirelessAdType {
	return 2;
}

- (NSString *)googleAdSenseCompanyName {
	return @"Your Company";
}

- (NSString *)googleAdSenseAppName {
	return @"AdWhirl Sample";
}

- (NSString *)googleAdSenseApplicationAppleID {
	return @"0";
}

- (NSString *)googleAdSenseKeywords {
	return @"apple,iphone,ipad,adwhirl";
}

#pragma mark event methods

- (void)performEvent {
	self.label.text = @"Event performed";
}

- (void)performEvent2:(AdWhirlView *)adWhirlView {
	UILabel *replacement = [[UILabel alloc] initWithFrame:kAdWhirlViewDefaultFrame];
	replacement.backgroundColor = [UIColor blackColor];
	replacement.textColor = [UIColor whiteColor];
	replacement.textAlignment = UITextAlignmentCenter;
	replacement.text = [NSString stringWithFormat:@"Event performed, view %x", adWhirlView];
	[adWhirlView replaceBannerViewWith:replacement];
	[replacement release];
	[self adjustAdSize];
	self.label.text = [NSString stringWithFormat:@"Event performed, view %x", adWhirlView];
}

@end

