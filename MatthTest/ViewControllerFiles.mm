//  ViewControllerFiles.mm
//  Hello World Table View
//
//  Created by Matthew Baynham on 10/11/10.
//  Copyright 2010 Baynham Coding. All rights reserved.

#import "ViewControllerFiles.h"
#import "CellFileDetails.h"

@implementation ViewControllerFiles

//@synthesize viewControllerQuiz;

#pragma mark -
#pragma mark Initialization
- (void)loadView
{
	[[[UIApplication sharedApplication] delegate] DirReInitialise];
	
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
//    [super viewDidLoad];
    // Uncomment the following line to preserve selection between presentations.

	UIBarButtonItem *btnBack = [[UIBarButtonItem alloc] initWithTitle:@"Back..." 
																style:UIBarButtonItemStyleBordered
															   target:self 
															   action:@selector(clickedBackBtn:)];
	
	self.navigationItem.leftBarButtonItem = btnBack;
	
	[btnBack release];
	
	self.tableView.separatorColor = [[[UIApplication sharedApplication] delegate] GuiColour:@"boarder"];//try GUI stuff	
	self.tableView.backgroundColor = [[[UIApplication sharedApplication] delegate] GuiColour:@"boarder"];//try GUI stuff

	// Uncomment the following line to display an Edit button in the navigation bar for this view controller.
    // self.navigationItem.rightBarButtonItem = self.editButtonItem;
}

- (void)viewWillAppear:(BOOL)animated {
	[super viewWillAppear:animated];
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

- (void)drawRect:(CGRect)rect
{
 	[[UIApplication sharedApplication] setStatusBarHidden:YES animated:YES];
 	self.navigationController.navigationBar.hidden = NO;
}

// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    return YES;
}

#pragma mark -
#pragma mark Table view data source

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    // Return the number of sections.
	int iFileMax;
	
	iFileMax = (int)[[[UIApplication sharedApplication] delegate] DirGetFileMax];

	return iFileMax;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    // Return the number of rows in the section.
	return 1;
}

// Customize the appearance of table view cells.
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    static NSString *CellIdentifier = @"CellFileDetails";
    NSString *sFullPath = [[NSString alloc] initWithString:@""];
    int iFileMax;
	int iFileNo;
    
	iFileMax = (int)[[[UIApplication sharedApplication] delegate] DirGetFileMax];

    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil) {
		int iSection;
		iSection = indexPath.section;
		iFileNo = iFileMax - iSection - 1;

		sFullPath = [[[UIApplication sharedApplication] delegate]  DirGetFileName:iFileNo];

		cell = [[[CellFileDetails alloc] initWithStyle:UITableViewCellStyleSubtitle 
									   reuseIdentifier:nil
												 sPath:sFullPath
										   iFileNumber:iFileNo] 
				autorelease];

		[cell setParent:self];
    }
    
	[cell setBackgroundColor:[[[UIApplication sharedApplication] delegate] GuiColour:@"cell"]];
	cell.selectionStyle = UITableViewCellSelectionStyleNone;

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

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath: (NSIndexPath *)indexPath { 
	CGFloat fheight; 
	
	fheight = 180;
	
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
    // Relinquish ownership of anything that can be recreated in viewDidLoad or on demand.
    // For example: self.myOutlet = nil;
//	[self.tableView autorelease];
}

- (void)dealloc {
    [super dealloc];
}

- (void)clickedBackBtn:(UIBarButtonItem *)myButton {
	[self.navigationController popViewControllerAnimated:YES];
}

-(void)openQuiz:(NSString*)sFullPath {
	ViewControllerQuiz *viewControllerQuiz = [[ViewControllerQuiz alloc] init];
	[viewControllerQuiz setQuiz:sFullPath];
	[self.navigationController pushViewController:viewControllerQuiz animated:YES];
	[viewControllerQuiz release];
};

@end

