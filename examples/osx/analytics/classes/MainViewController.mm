//
//  MainViewController.m
//  analytics
//

#import "MainViewController.h"

#include "GAI.h"

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

-(void)tabView:(NSTabView *)tabView didSelectTabViewItem:(NSTabViewItem *)tabView
{
    
    GAI::Analytics* gai = GAI::Analytics::getInstance();
    if( tabView == tabOneView )
    {
        gai->getDefaultTracker()->sendView("Tab One");
    }
    else if( tabView == tabTwoView )
    {
        gai->getDefaultTracker()->sendView("Tab Two");
    }
    else if( tabView == tabThreeView )
    {
        gai->getDefaultTracker()->sendView("Tab Three");
    }
}

@end
