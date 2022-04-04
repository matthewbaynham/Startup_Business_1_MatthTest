//
//  ViewControllerPreferencesByGroup.mm
//  Hello World Table View
//
//  Created by Matthew Baynham on 12/9/10.
//  Copyright 2010 Baynham Coding. All rights reserved.
//

#import <string>
#import "ViewControllerPreferencesByGroup.h"
#import "ViewControllerQuiz.h"
#import "ClsPreferences.h"
#import "CellPreferenceSlider.h"
#import "CellPreferenceSwitch.h"
#import "CellPreferenceSwitchSlider.h"
#import "CellSectionTitle.h"
#import "CellMenuItem.h"

@implementation ViewControllerPreferencesByGroup

@synthesize viewControllerQuiz, vwActiv, vwSemiClear, tmrStartQuiz;

#pragma mark -
#pragma mark Initialization

- (void)loadView
{
	bool bIsFoundPreferenceFile;

	UIColor *colTemp = [[[UIApplication sharedApplication] delegate] GuiColour:@"boarder"];
	self.navigationController.navigationBar.tintColor = colTemp;
	
	//make sure the navigation bar is visible and black
	[[UIApplication sharedApplication] setStatusBarHidden:YES animated:YES];
	self.navigationController.navigationBar.hidden = NO;
	
	bIsFoundPreferenceFile = [[[UIApplication sharedApplication] delegate] DirIsFoundPreferenceFile];
	
	NSString *sPathHome = [[[NSString alloc] initWithString:[[[UIApplication sharedApplication] delegate] DirDocumentsPath]] autorelease];
	std::string stdPathHome = [sPathHome UTF8String];
	
    cPreferences = new ClsPreferences();

	cPreferences->setPath(stdPathHome);
	cPreferences->setTypeByGroup();
	
	self.wantsFullScreenLayout = YES;
	
	UITableView *tableView = [[UITableView alloc] initWithFrame:[[UIScreen mainScreen] applicationFrame]
														  style:UITableViewStyleGrouped];
	tableView.autoresizingMask = UIViewAutoresizingFlexibleHeight|UIViewAutoresizingFlexibleWidth;
	tableView.delegate = self;
	tableView.dataSource = self;
	
	[tableView reloadData];
	self.view = tableView;
	[tableView release];
}

/*
- (id)initWithStyle:(UITableViewStyle)style {
    // Override initWithStyle: if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
    if ((self = [super initWithStyle:style])) {
    }
    return self;
}
*/


#pragma mark -
#pragma mark View lifecycle

- (void)viewDidLoad {
	[super viewDidLoad];
	[[[UIApplication sharedApplication] delegate] DirReInitialise];

	// Uncomment the following line to preserve selection between presentations.
	[[UIApplication sharedApplication] setStatusBarHidden:YES animated:YES];
	self.navigationController.navigationBar.hidden = NO;
	
	self.tableView.separatorColor = [[[UIApplication sharedApplication] delegate] GuiColour:@"boarder"];	
	self.tableView.backgroundColor = [[[UIApplication sharedApplication] delegate] GuiColour:@"boarder"];
	
	self.navigationController.navigationBar.tintColor = [[[UIApplication sharedApplication] delegate] GuiColour:@"boarder"];
	
	UIBarButtonItem *btnBack = [[UIBarButtonItem alloc] initWithTitle:@"Back..." 
																style:UIBarButtonItemStyleBordered
															   target:self 
															   action:@selector(clickedBackBtn:)];
	self.navigationItem.leftBarButtonItem = btnBack;
	[btnBack release];
	
	UIBarButtonItem *btnStart = [[UIBarButtonItem alloc] initWithTitle:@"Start Quiz" 
																 style:UIBarButtonItemStyleBordered
																target:self 
																action:@selector(createQuiz)];
	self.navigationItem.rightBarButtonItem = btnStart;
	[btnStart release];
	
	self.vwSemiClear = [[UIView alloc] initWithFrame:self.tableView.frame];
	[self.vwSemiClear setHidden:YES];
	[self.vwSemiClear setBackgroundColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"boarder"]];
	[self.vwSemiClear setAlpha:0.75];
	[self.view addSubview:self.vwSemiClear];
	
	self.vwActiv = [[[UIActivityIndicatorView alloc] initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleWhiteLarge] retain];    
	self.vwActiv.center = (CGPoint)[[[[UIApplication sharedApplication] delegate] window] center];
	[self.vwActiv setHidden:YES];
	[self.vwActiv stopAnimating];
	[self.view addSubview:vwActiv];
	
	// Uncomment the following line to display an Edit button in the navigation bar for this view controller.
}

- (void)viewWillAppear:(BOOL)animated {
	[super viewWillAppear:animated];
 	[[UIApplication sharedApplication] setStatusBarHidden:YES animated:YES];
 	self.navigationController.navigationBar.hidden = NO;
}


- (void)drawRect:(CGRect)rect
{
 	[[UIApplication sharedApplication] setStatusBarHidden:YES animated:YES];
 	self.navigationController.navigationBar.hidden = NO;
}


/*
- (void)viewDidAppear:(BOOL)animated {
    [super viewDidAppear:animated];
}
*/
/*
- (void)viewWillDisappear:(BOOL)animated {
    [super viewWillDisappear:animated];
}
*/
/*
- (void)viewDidDisappear:(BOOL)animated {
    [super viewDidDisappear:animated];
}
*/

// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
	 // Return YES for supported orientations
	 return YES;
}
 
- (void)willAnimateRotationToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation duration:(NSTimeInterval)duration
{
	[self.tableView reloadData];
}

#pragma mark -
#pragma mark Table view data source

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    // Return the number of sections.
	//1 = click to start quiz
	//2 = all the different groups
	//3 = reset preferences

	return 3;
}


- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    // Return the number of rows in the section.
	int iNoRows;
	
	if (section == 0) {
		//No of Questions
		iNoRows = 1;
	} else if (section == 1) {
		//number of groups
		iNoRows = cPreferences->countGroups();
	} else if (section == 2) {
		//reset preferences
		iNoRows = 1;
	} else {
		iNoRows = 1;//error:???????
    };
	
	if (iNoRows <= 0) {
		iNoRows = 1;
	};
	
	return iNoRows;
}


// Customize the appearance of table view cells.
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    NSString *sTitleMain = [[[NSString alloc] initWithString:@""] autorelease];
    NSString *sPrefGroup = [[[NSString alloc] initWithString:@""] autorelease];
	int iCellNo;
	int iSection;
	float fSliderMax;
	NSNumber* nSliderMax = [[[NSNumber alloc] initWithInt:0] autorelease];
	NSNumber* nSliderMin = [[[NSNumber alloc] initWithInt:0] autorelease];
	enumQuestionGroup eGroup;
	
	iCellNo = indexPath.row;
	iSection = indexPath.section;
	
	switch (iSection) {
		case 0:
			//No of Questions
			sPrefGroup = @"";
			sTitleMain = @"No. of Questions";
			break;
		case 1:
			//Groups
			const char* cTempGroup;
			cTempGroup = cPreferences->getSubjectGroup(iCellNo).c_str();
//			NSString *sTempGroup = [stringWithCString:cTempGroup length:strlen(cTempGroup)];			

			sPrefGroup = [NSString stringWithCString:cTempGroup length:strlen(cTempGroup)];			
			break;
		case 2:
			//Reset Preferences
			sPrefGroup = @"";
			break;
		default:
			sPrefGroup = @"Error";
			break;
	};
	
	
	static NSString *CellIdentifier;
	
	UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
	
	if (iSection == 0) {
		//No of Questions
		CellIdentifier = @"CellPreferenceSlider";
		nSliderMax = [[NSNumber alloc] initWithFloat:(float)20.0];
		nSliderMin = [[NSNumber alloc] initWithFloat:(float)3.0];
		
		if (cell == nil) {
			cell = [[[CellPreferenceSlider alloc] initWithStyle:UITableViewCellStyleSubtitle 
												reuseIdentifier:nil
												   sTitlePrefix:sTitleMain
														  cPref:cPreferences
													  nLimitMin:nSliderMin
													  nLimitMax:nSliderMax] 
					autorelease];
			[cell setParent:self];
		};
		cell.selectionStyle = UITableViewCellSelectionStyleNone;
	} else if (iSection == 1) {
		CellIdentifier = @"CellPreferenceSwitchSlider";
		std::string *stdPrefGroup = new std::string([sPrefGroup UTF8String]);

		eGroup = cPreferences->getGroupEnum(stdPrefGroup);

		fSliderMax = (float)cPreferences->getSubjectGroupDiffMax(eGroup);
		
		nSliderMax = [[NSNumber alloc] initWithFloat:(float)(fSliderMax + 0.5)];
		nSliderMin = [[NSNumber alloc] initWithFloat:(float)1.5];
		
		if (cell == nil) {
			cell = [[[CellPreferenceSwitchSlider alloc] initWithStyle:UITableViewCellStyleSubtitle 
													  reuseIdentifier:nil
														   sGroupName:sPrefGroup
																cPref:cPreferences
															nLimitMin:nSliderMin
															nLimitMax:nSliderMax] 
						autorelease];
			[cell setParent:self];
		};
		cell.selectionStyle = UITableViewCellSelectionStyleNone;
	} else {
		if (iCellNo==0) {
			//Title
			CellIdentifier = @"CellSectionTitle";
			
			if (cell == nil) {
				cell = [[[CellSectionTitle alloc] initWithStyle:UITableViewCellStyleSubtitle 
												reuseIdentifier:nil
													 sGroupName:@"Reset Preferences"
														  cPref:cPreferences] 
						autorelease];
				[cell setParent:self];
			};
		}
		else {
		};
    };
	
	[cell setBackgroundColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"cell"]];
	[cell setSelected:NO];

    // Configure the cell...
    return cell;
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
	
	switch (indexPath.section) {
		case 0:
			//No of Questions
			fheight = 90.0; 
			break;
		case 1:
			//groups
			fheight = 120.0; 
			break;
		case 2:
			//reset preferences
			fheight = 60.0; 
			break;
		default:
			fheight = 60.0; 
			break;
	};
	
	return fheight; 
} 


#pragma mark -
#pragma mark Table view delegate

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
	if (indexPath.section == 2) {
		cPreferences->ResetPreferences();
		[[super navigationController] popViewControllerAnimated:YES];
	};
}

-(void)createQuiz {
	[self.vwSemiClear setHidden:NO];
	
	CGPoint pntOffset = [[self tableView] contentOffset];
	[self.vwSemiClear setFrame:CGRectMake(self.view.frame.origin.x + pntOffset.x, self.view.frame.origin.y + pntOffset.y, self.view.frame.size.width, self.view.frame.size.height)];

//	self.vwActiv = [[[UIActivityIndicatorView alloc] initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleWhiteLarge] retain];    

	[self.vwActiv setAutoresizesSubviews:YES];
	[self.vwActiv setAutoresizingMask:UIViewAutoresizingFlexibleHeight];
	
	[self.vwActiv startAnimating];
	
	self.vwActiv.center = self.vwSemiClear.center;
	
	self.tmrStartQuiz = [NSTimer scheduledTimerWithTimeInterval:0.01 
													target:self 
												  selector:@selector(openQuiz) 
												  userInfo:nil 
												   repeats:NO];
}

-(void)openQuiz {
    NSString* sPathHome = [[NSString alloc] initWithString:[[[UIApplication sharedApplication] delegate] DirDocumentsPath]];
	std::string stdPathHome = [sPathHome UTF8String];
	
	cPreferences->setPath(stdPathHome);
	cPreferences->setTypeByGroup();

	if (cPreferences->isQuestionPossible() == true) {
		self.viewControllerQuiz = nil;
		
		if(self.viewControllerQuiz == nil){
			ViewControllerQuiz *tempView = [[ViewControllerQuiz alloc] initWithNibName:@"viewControllerQuiz" 
																				bundle:[NSBundle mainBundle]];
			self.viewControllerQuiz = tempView;
			[tempView release];
		}
		[self.viewControllerQuiz setPref:cPreferences];
		[self.navigationController pushViewController:self.viewControllerQuiz animated:YES];

		[self.vwSemiClear setHidden:YES];
		[self.vwActiv stopAnimating];
	} else {
		[self.vwSemiClear setHidden:YES];
		[self.vwActiv stopAnimating];

		NSString *sTitle = [[[NSString alloc] initWithString:@"Can't"] autorelease];
		NSString *sMessage = [[[NSString alloc] initWithString:@"Can't create Quiz please select more options"] autorelease];
		NSString *sBtnCancel = [[[NSString alloc] initWithString:@"OK"] autorelease];
		
		UIAlertView *msgWarning = [[UIAlertView alloc] initWithTitle:sTitle
															message:sMessage
														   delegate:self 
												  cancelButtonTitle:sBtnCancel 
												  otherButtonTitles:nil];
		[msgWarning show];
		[msgWarning release];

		NSArray* arrVisibleCells = [[[NSArray alloc] initWithArray:[self.view visibleCells]] autorelease];
		
		NSEnumerator *e = [arrVisibleCells objectEnumerator];
		id object;
		
		while (object = [e nextObject]) {
			[object setSelected:NO];
		};
	};
}

#pragma mark -
#pragma mark Memory management

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Relinquish ownership any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
    // Relinquish ownership of anything that can be recreated in viewDidLoad or on demand.
    // For example: self.myOutlet = nil;
	[self.vwActiv dealloc];
	[self.vwSemiClear dealloc];

	[self.tableView release];
};


- (void)dealloc {
//	delete cPreferences;
    self.viewControllerQuiz = NULL;

    [super dealloc];
}

#pragma mark -
#pragma mark Custom Functions

- (void)clickedBackBtn:(UIBarButtonItem *)myButton
{
	[self.navigationController popViewControllerAnimated:YES];
}

-(void)groupEnabled:(NSString*)sGroup bEnabled:(NSNumber*)bEnabled {
	
}; 

@end

