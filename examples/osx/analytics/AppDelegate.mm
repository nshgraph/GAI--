//
//  AppDelegate.m
//  analytics
//

#import "AppDelegate.h"

#include "GAI.h"

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
	// Insert code here to initialize your application
	
	GAI::Analytics::sharedInstance( "Analytics Example App", "./" );
}

@end
