//
//  MainViewController.m
//  analytics
//

#import "MainViewController.h"

#include "gai++/GAI.h"

@interface MainViewController ()

@end

@implementation MainViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Initialization code here.
    }
    
    return self;
}

-(void)tabView:(NSTabView *)tabView didSelectTabViewItem:(NSTabViewItem *)tabViewItem
{
    
    GAI::Analytics* gai = GAI::Analytics::getInstance();
    if( tabViewItem == tabOneView )
    {
        gai->getDefaultTracker()->sendView("Tab One");
    }
    else if( tabViewItem == tabTwoView )
    {
        GAI::CustomDimensionMap dimensions;
        GAI::CustomMetricMap metrics;
        dimensions[1] = "tabber";
        gai->getDefaultTracker()->sendView("Tab Two",dimensions,metrics);
    }
    else if( tabViewItem == tabThreeView )
    {
        gai->getDefaultTracker()->sendView("Tab Three");
    }
}

- (IBAction)dispatchQueue:(id)sender
{
    GAI::Analytics* gai = GAI::Analytics::getInstance();
    gai->dispatch();
}

@end
