//
//  CellMenuItem.h
//  MatthTest
//
//  Created by Matthew Baynham on 2/7/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface CellMenuItem : UITableViewCell {
	UIImageView *imgVwPic;
	UILabel *lblText;
}

@property (nonatomic,retain) UIImageView *imgVwPic;
@property (nonatomic,retain) UILabel *lblText;

-(void)setFont:(UIFont *)fntFont;
-(void)setText:(NSString *)txtText;

@end
