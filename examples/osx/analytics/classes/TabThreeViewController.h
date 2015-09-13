//
//  TabThreeViewController.h
//  analytics
//

#import <Cocoa/Cocoa.h>

@interface TabThreeViewController : NSViewController

-(IBAction)sendEvent:(id)sender;
-(IBAction)sendEventWithMetric:(id)sender;
-(IBAction)sendEventWithDimension:(id)sender;

-(IBAction)crash:(id)sender;

- (void)loadView;

@end
