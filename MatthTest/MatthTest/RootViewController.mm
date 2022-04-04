//
//  RootViewController.m
//  MatthTest
//
//  Created by Matthew Baynham on 1/22/11.
//  Copyright 2011 Baynham Coding. All rights reserved.
//

#import "MatthTestAppDelegate.h"
#import "RootViewController.h"
#import "CellMenuItem.h"
#import "AdWhirlView.h"
#import "SampleConstants.h"
#import "ClsPreferences.h"
#import "CellBaynhamCoding.h"

using namespace modMisc;

@implementation RootViewController

@synthesize adView, vwActiv;
@synthesize viewControllerFiles, viewControllerPreferencesSimple, viewControllerPreferencesByGroup, viewControllerPreferencesBySubject;

#pragma mark -
#pragma mark View lifecycle

- (id)init {
	if (self = [super initWithNibName:@"RootViewController" bundle:nil]) {
		self.title = @"Ad In Table";
	}
	
//	NSLog(@"init root");
	return self;
}


- (void)loadView
{
    //	[super loadView];
    //	NSLog(@"loadView root");
    
	NSString *sPathHome = [[[NSString alloc] initWithString:[[[UIApplication sharedApplication] delegate] DirDocumentsPath]] autorelease];
	std::string stdPathHome = [sPathHome UTF8String];
	
    //	cHelp.setPath(stdPathHome);
	
	self.navigationController.navigationBar.tintColor = [[[UIApplication sharedApplication] delegate] GuiColour:@"boarder"];//replace GUI
    
	//make sure the navigation bar is visible and black
	[[UIApplication sharedApplication] setStatusBarHidden:YES animated:YES];
	self.navigationController.navigationBar.hidden = NO;
	
    //	self.navigationController.navigationItem.backBarButtonItem = @"Stuff";
	[self.navigationController.navigationItem setHidesBackButton:NO animated:YES];
	
	
	self.wantsFullScreenLayout = YES;
	
	UITableView *tableView = [[UITableView alloc] initWithFrame:[[UIScreen mainScreen] applicationFrame]
														  style:UITableViewStyleGrouped];
	tableView.autoresizingMask = UIViewAutoresizingFlexibleHeight|UIViewAutoresizingFlexibleWidth;
	tableView.delegate = self;
	tableView.dataSource = self;
	
	[tableView reloadData];
	tableView.separatorColor = [[[UIApplication sharedApplication] delegate] GuiColour:@"boarder"];//replace GUI
    
	self.view = tableView;
	[tableView release];
	
	[self.adView retain];
    
    [self configMenu];
}

- (void)viewDidLoad {
    [super viewDidLoad];
	
    // Uncomment the following line to display an Edit button in the navigation bar for this view controller.
    
	self.view.backgroundColor = [[[UIApplication sharedApplication] delegate] GuiColour:@"boarder"];//replace GUI
	
	
	self.adView = [AdWhirlView requestAdWhirlViewWithDelegate:self];
	self.adView.autoresizingMask = UIViewAutoresizingFlexibleLeftMargin|UIViewAutoresizingFlexibleRightMargin;
	[self CheckPreference];
}


/*
 - (void)viewWillAppear:(BOOL)animated {
 [super viewWillAppear:animated];
 }
 */
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
	// Return YES for supported orientations.
	return YES;
}

- (void)willAnimateRotationToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation duration:(NSTimeInterval)duration
{
    //	[self.tableView reloadData];
	[self.adView rotateToOrientation:toInterfaceOrientation];
	[self adjustAdSize];
    //	[self reloadData];
}


#pragma mark -
#pragma mark Table view data source

// Customize the number of sections in the table view.
- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    return 5;
}


// Customize the number of rows in the table view.
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
	int iNoRows;
    
	switch (section) {
		case 0:
			iNoRows = 4;
			break;
		case 1:
			iNoRows = 1;
			break;
		case 2:
			iNoRows = 1;
			break;
		case 3:
			iNoRows = 1;
			break;
		case 4:
			iNoRows = 1;
			break;
		default:
			break;
	}
	
	return iNoRows;
}


// Customize the appearance of table view cells.
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    
    static NSString *CellIdentifier = [[[NSString alloc] initWithString:@""] autorelease];
    
	switch (indexPath.section) {
		case 0:
		{
			switch (indexPath.row) {
				case 0:
					CellIdentifier = @"Cell";
					break;
				case 1:
					CellIdentifier = @"CellMenuItem";
					break;
				case 2:
					CellIdentifier = @"CellMenuItem";
					break;
				case 3:
					CellIdentifier = @"CellMenuItem";
					break;
				default:
					CellIdentifier = @"Cell";
					break;
			}
			break;
		}
		case 1:
		{
			CellIdentifier = @"CellMenuItem";
			break;
		}
		case 2:
		{
			CellIdentifier = @"CellMenuItem";
			break;
		}
		case 3:
		{
			CellIdentifier = @"Cell";
			break;
		}
		case 4:
		{
			CellIdentifier = @"CellBaynhamCoding";
			break;
		}
		default:
			CellIdentifier = @"Cell";
			break;
	}
	
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    
    
	// Configure the cell.
	
	switch (indexPath.section) {
		case 0:
		{
			switch (indexPath.row) {
				case 0:
					if (cell == nil) {
						cell = [[[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault 
													   reuseIdentifier:CellIdentifier] 
								autorelease];
					}
                    
					[cell setText:@"Create New Quiz"];
					[cell.textLabel setFont:[[[UIApplication sharedApplication] delegate] GuiFont:@"title"]];//replace GUI
					cell.editingAccessoryType = UITableViewCellAccessoryNone;
					break;
				case 1:
					if (cell == nil) {
						cell = [[[CellMenuItem alloc] initWithStyle:UITableViewCellStyleDefault 
													reuseIdentifier:CellIdentifier] 
								autorelease];
					}
					
					[cell setText:@"Simply"];
					[cell setFont:[[[UIApplication sharedApplication] delegate] GuiFont:@"menu"]];//replace GUI
					cell.editingAccessoryType = UITableViewCellAccessoryDisclosureIndicator;
					break;
				case 2:
					if (cell == nil) {
						cell = [[[CellMenuItem alloc] initWithStyle:UITableViewCellStyleDefault 
													reuseIdentifier:CellIdentifier] 
								autorelease];
					}
					
					[cell setText:@"General Topic"];
					[cell setFont:[[[UIApplication sharedApplication] delegate] GuiFont:@"menu"]];//replace GUI
					cell.editingAccessoryType = UITableViewCellAccessoryDetailDisclosureButton;
					break;
				case 3:
					if (cell == nil) {
						cell = [[[CellMenuItem alloc] initWithStyle:UITableViewCellStyleDefault 
													reuseIdentifier:CellIdentifier] 
								autorelease];
					}
					
					[cell setText:@"Exact Selection"];
					[cell setFont:[[[UIApplication sharedApplication] delegate] GuiFont:@"menu"]];//replace GUI
					cell.editingAccessoryType = UITableViewCellAccessoryCheckmark;
					break;
				default:
					if (cell == nil) {
						cell = [[[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault 
													   reuseIdentifier:CellIdentifier] 
								autorelease];
					}
					
					[cell setText:@"Error"];
					[cell.textLabel setFont:[[[UIApplication sharedApplication] delegate] GuiFont:@"menu"]];//replace GUI
					break;
			};
            [cell.detailTextLabel setTextColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"cell"]];
            [cell.detailTextLabel setTextAlignment:UITextAlignmentLeft];
            [cell.detailTextLabel setTextHighlightedTextColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"text"]];
            [cell.textLabel setTextColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"text"]];
			break;
		}
		case 1:
		{
			if (cell == nil) {
				cell = [[[CellMenuItem alloc] initWithStyle:UITableViewCellStyleDefault 
											reuseIdentifier:CellIdentifier] 
						autorelease];
			}
			
			[cell setText:@"Previous Results"];
			[cell setFont:[[[UIApplication sharedApplication] delegate] GuiFont:@"title"]];
            [cell.detailTextLabel setTextAlignment:UITextAlignmentLeft];
            [cell.detailTextLabel setTextColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"cell"]];
            [cell.detailTextLabel setTextHighlightedTextColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"text"]];
            [cell.textLabel setTextColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"text"]];
			break;
		}
		case 2:
		{
			if (cell == nil) {
				cell = [[[CellMenuItem alloc] initWithStyle:UITableViewCellStyleDefault 
                                            reuseIdentifier:CellIdentifier] 
						autorelease];
			}
            
			[cell setText:@"Instructions (on line)"];
			[cell setFont:[[[UIApplication sharedApplication] delegate] GuiFont:@"title"]];
            [cell.detailTextLabel setTextAlignment:UITextAlignmentLeft];
            [cell.detailTextLabel setTextColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"cell"]];
            [cell.detailTextLabel setTextHighlightedTextColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"text"]];
            [cell.textLabel setTextColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"text"]];
			break;
		}
		case 3:
		{
			if (cell == nil) {
				cell = [[[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault 
											   reuseIdentifier:CellIdentifier]
						autorelease];
			};
			[cell setText:@"fun with maths..."];
            [cell.detailTextLabel setTextAlignment:UITextAlignmentCenter];
            [cell.detailTextLabel setTextColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"cell"]];
            [cell.detailTextLabel setTextHighlightedTextColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"highlight"]];
			[cell.textLabel setFont:[[[UIApplication sharedApplication] delegate] GuiFont:@"answers"]];
            [cell.textLabel setTextColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"highlight"]];
            [cell.textLabel setTextAlignment:UITextAlignmentCenter];
			[cell.contentView addSubview:adView];
			break;
		}
		case 4:
			if (cell == nil) {
				cell = [[[CellBaynhamCoding alloc] initWithStyle:UITableViewCellStyleDefault 
                                                 reuseIdentifier:CellIdentifier] 
						autorelease];
			}
			
            [cell.detailTextLabel setTextColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"cell"]];
            [cell.detailTextLabel setTextHighlightedTextColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"text"]];
            [cell.textLabel setTextColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"text"]];
			break;
		default:
			if (cell == nil) {
				cell = [[[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault 
											   reuseIdentifier:CellIdentifier] 
						autorelease];
			}
            
			[cell setText:@"Error"];
			[cell.textLabel setFont:[[[UIApplication sharedApplication] delegate] GuiFont:@"title"]];//replace GUI
            [cell.detailTextLabel setTextColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"cell"]];
            [cell.detailTextLabel setTextHighlightedTextColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"text"]];
            [cell.textLabel setTextColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"text"]];
			break;
	}
	
	cell.selectionStyle = UITableViewCellSelectionStyleNone;
    
	[cell setBackgroundColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"cell"]];
    
	//[cell.detailTextLabel setTextColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"cell"]];
	//[cell.detailTextLabel setTextHighlightedTextColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"text"]];
	//[cell.textLabel setTextColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"text"]];
	
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
 // Delete the row from the data source.
 [tableView deleteRowsAtIndexPaths:[NSArray arrayWithObject:indexPath] withRowAnimation:UITableViewRowAnimationFade];
 }   
 else if (editingStyle == UITableViewCellEditingStyleInsert) {
 // Create a new instance of the appropriate class, insert it into the array, and add a new row to the table view.
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


#pragma mark -
#pragma mark Table view delegate

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
    
	switch (indexPath.section) {
		case 0:
			switch (indexPath.row) {
				case 1:
				{
					self.viewControllerPreferencesSimple = [[ViewControllerPreferencesSimple alloc] init];
					[self.navigationController pushViewController:self.viewControllerPreferencesSimple animated:YES];
					[self.viewControllerPreferencesSimple release];
					break;
				}
				case 2:
				{
					self.viewControllerPreferencesByGroup = [[ViewControllerPreferencesByGroup alloc] init];
					[self.navigationController pushViewController:self.viewControllerPreferencesByGroup animated:YES];
					[self.viewControllerPreferencesByGroup release];
					break;
				}
				case 3:
				{
					self.viewControllerPreferencesBySubject = [[ViewControllerPreferencesBySubject alloc] init];
					[self.navigationController pushViewController:self.viewControllerPreferencesBySubject animated:YES];
					[self.viewControllerPreferencesBySubject release];
					break;
				}
				default:
					break;
			}
			break;
		case 1:
		{
			//
			[[[UIApplication sharedApplication] delegate] DirReInitialise];
			self.viewControllerFiles = [[ViewControllerFiles alloc] init];
			[self.navigationController pushViewController:self.viewControllerFiles animated:YES];
			[self.viewControllerFiles release];
			break;
		}
		case 2:
		{
            /*			
             vwActiv = [[[UIActivityIndicatorView alloc] initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleWhiteLarge] autorelease];    
             
             // we put our spinning "thing" right in the center of the current view
             
             CGPoint newCenter = (CGPoint) [self.view center];
             
             vwActiv.center = newCenter;
             
             
             [self.view addSubview:vwActiv];
             [vwActiv startAnimating];
             */
            //			ViewControllerTestFunction *viewControllerTestFunction = [[ViewControllerTestFunction alloc] init];
            //			[self.navigationController pushViewController:viewControllerTestFunction animated:YES];
            //			[viewControllerTestFunction release];
            
            NSString *sInstructionsPath = [[[NSString alloc] initWithString:@"http://baynhamcoding.mobi/Instructions/"] autorelease];
			NSString *sVersion = [[[NSString alloc] initWithCString:modMisc::sVersion.c_str()] autorelease];
            
            sInstructionsPath = [sInstructionsPath stringByAppendingString:sVersion];
            sInstructionsPath = [sInstructionsPath stringByAppendingString:@"/"];
            
            [[UIApplication sharedApplication] openURL:[NSURL URLWithString:sInstructionsPath]]; 
			break;
		}
		default:
		{
			break;
		}
	}
    
	// ...
	// Pass the selected object to the new view controller.
	
}


#pragma mark -
#pragma mark Memory management

//- (void)didReceiveMemoryWarning {
// Releases the view if it doesn't have a superview.
//   [super didReceiveMemoryWarning];

// Relinquish ownership any cached data, images, etc that aren't in use.
//}


- (void)viewDidUnload {
    // Relinquish ownership of anything that can be recreated in viewDidLoad or on demand.
    // For example: self.myOutlet = nil;
	[self.view release];
}


- (void)dealloc {
	self.adView.delegate = nil;
	self.adView = nil;
    [super dealloc];
};

#pragma mark -
#pragma mark Misc

-(void) CheckPreference {
	bool bIsPrefExist;
    //	ClsPreferences* cPref;
	
    //	if (cDir == NULL) {
    //		[self DirReInitialise];
    //	};
	
	if (cPref == NULL) {
		cPref = new ClsPreferences;
	};
	
	bIsPrefExist = [[[UIApplication sharedApplication] delegate] DirIsFoundPreferenceFile];
	
	if (bIsPrefExist != true) {
		NSString *sPathHome = [[[UIApplication sharedApplication] delegate] DirDocumentsPath];
		std::string stdPathHome = [sPathHome UTF8String];
        
		cPref->setPath(stdPathHome);
		cPref->ResetPreferences();
	};
	
    //	cPref = NULL;
};

-(void) configMenu {
    
    CGRect frmMark = CGRectMake(0, 0, 200, 30);	
	UIView* vwTitleView = [[UIView alloc] initWithFrame:frmMark];
	[vwTitleView setBackgroundColor:[UIColor clearColor]];
    
    UILabel* lblMatthTest = [[UILabel alloc] initWithFrame:vwTitleView.frame];
    
    NSString *sMatthTest = [[NSString alloc] initWithString:@"Matth Test"];
    
    [lblMatthTest setText:sMatthTest];
    [lblMatthTest setTextColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"highlight"]];
    [lblMatthTest setTextAlignment:UITextAlignmentCenter]; 
    [lblMatthTest setFont:[[[UIApplication sharedApplication] delegate] GuiFont:@"title"]];
    [lblMatthTest setBackgroundColor:[UIColor clearColor]];
    
    [vwTitleView addSubview:lblMatthTest];
    self.navigationItem.titleView = vwTitleView;
    [lblMatthTest release];
    
}

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

//extern NSString* const kGADAdSenseImageAdType;
//- (NSString *)googleAdSenseAdType {
//  return kGADAdSenseImageAdType;
//}

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

