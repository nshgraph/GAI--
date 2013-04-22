//
//  TabThreeViewController.m
//  analytics
//

#import "TabThreeViewController.h"
#include "GAI.h"

@interface TabThreeViewController ()

@end

@implementation TabThreeViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Initialization code here.
    }
    
    return self;
}

-(IBAction)sendEvent:(id)sender
{
    GAI::Analytics* gai = GAI::Analytics::getInstance();
    gai->getDefaultTracker()->sendEvent("Button","Pressed","","");
}

-(IBAction)crash:(id)sender
{
    GAI::Analytics* gai = GAI::Analytics::getInstance();
    gai->getDefaultTracker()->sendException(true, "Pressed the Crash Button");
}

- (void)loadView
{
    [super loadView];
}

@end
