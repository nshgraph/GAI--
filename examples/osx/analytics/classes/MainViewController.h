//
//  MainViewController.h
//  analytics
//

#import <Cocoa/Cocoa.h>

@interface MainViewController : NSViewController<NSTabViewDelegate>
{
    IBOutlet NSTabViewItem* tabOneView;
    IBOutlet NSTabViewItem* tabTwoView;
    IBOutlet NSTabViewItem* tabThreeView;
}

- (void)tabView:(NSTabView *)tabView didSelectTabViewItem:(NSTabViewItem *)tabViewItem;
- (IBAction)dispatchQueue:(id)sender;

@end
