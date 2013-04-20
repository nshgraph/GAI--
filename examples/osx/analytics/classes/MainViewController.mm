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

-(void)tabView:(NSTabView *)tabView didSelectTabViewItem:(NSTabViewItem *)tabViewItem
{
    
    GAI::Analytics* gai = GAI::Analytics::getInstance();
    if( tabViewItem == tabOneView )
    {
        gai->getDefaultTracker()->sendView("Tab One");
    }
    else if( tabViewItem == tabTwoView )
    {
        gai->getDefaultTracker()->sendView("Tab Two");
    }
    else if( tabViewItem == tabThreeView )
    {
        gai->getDefaultTracker()->sendView("Tab Three");
    }
}

@end
