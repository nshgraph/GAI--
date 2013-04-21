//
//  TabThreeViewController.h
//  analytics
//

#import <Cocoa/Cocoa.h>

@interface TabThreeViewController : NSViewController
-(IBAction)sendEvent:(id)sender;

-(IBAction)dispatchQueue:(id)sender;

-(IBAction)crash:(id)sender;

- (void)loadView;

@end
