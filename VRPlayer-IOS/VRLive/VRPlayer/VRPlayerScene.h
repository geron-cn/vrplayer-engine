
#import <AVFoundation/AVFoundation.h>
#import <UIKit/UIKit.h>

@interface VRPlayerScene : NSObject

-(void) addLabel: (NSString*)name Text: (NSString*)text NormalizedPosition:(CGPoint)position FontName:(NSString*)fontName FontColor:(UIColor*)color  FontSize:(int)fontSize;

-(void) removeLabel: (NSString*)name;

-(void) addMenuItem: (NSString*)name TexturePath:(NSString*)path Rect: (CGRect)rect;

-(void) removeMenuItem: (NSString*)name;

@end
