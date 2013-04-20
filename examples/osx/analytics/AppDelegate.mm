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
	
	GAI::Analytics* gai = GAI::Analytics::getInstance( "Analytics Example App", "1.0", "./" );
    gai->createTracker("My Tracker");
    
}

@end
