//
//  main.m
//  MatthTest
//
//  Created by Matthew Baynham on 3/20/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#define TEST_ENV = false
#import <UIKit/UIKit.h>

int main(int argc, char *argv[])
{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    int retVal = UIApplicationMain(argc, argv, nil, nil);
    [pool release];
    return retVal;
}
