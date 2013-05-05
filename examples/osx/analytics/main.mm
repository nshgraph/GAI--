//
//  main.m
//  analytics
//

#import <Cocoa/Cocoa.h>

#include "gai++/GAI.h"

int main(int argc, char *argv[])
{
	// Insert code here to initialize your application
	
	GAI::Analytics* gai = GAI::Analytics::getInstance( "Analytics Example App", "1.0b2", "./" );
    gai->createTracker("UA-39515904-1");
    
	return NSApplicationMain(argc, (const char **)argv);
}
