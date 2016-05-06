#import <UIKit/UIKit.h>

@interface VRPlayerIOS : NSObject

@property (nonatomic, readonly) UIViewController *viewController;

- (BOOL)play:(NSString*)url;
- (void)stop;
- (BOOL)isVREnabled;
- (void)setVREnabled:(BOOL)enabled;
-(double)duration;
-(BOOL)isPause;
-(BOOL)pause:(BOOL)paused;
-(double)currentPos;
-(BOOL)seek:(double)pos;

@end
