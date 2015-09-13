//
//  TabThreeViewController.m
//  analytics
//

#import "TabThreeViewController.h"
#include "gai++/GAI.h"

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
    gai->getDefaultTracker()->sendEvent("Button","Pressed","",0);
}

-(IBAction)sendEventWithMetric:(id)sender
{
    GAI::Analytics* gai = GAI::Analytics::getInstance();
    GAI::CustomDimensionMap dimensions;
    GAI::CustomMetricMap metrics;
    metrics[1] = "5";
    gai->getDefaultTracker()->sendEvent("Button","Pressed","",0, dimensions, metrics);
}
-(IBAction)sendEventWithDimension:(id)sender
{
    GAI::Analytics* gai = GAI::Analytics::getInstance();
    GAI::CustomDimensionMap dimensions;
    GAI::CustomMetricMap metrics;
    dimensions[1] = "testing dimension";
    gai->getDefaultTracker()->sendEvent("Button","Pressed","",0, dimensions, metrics);
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
