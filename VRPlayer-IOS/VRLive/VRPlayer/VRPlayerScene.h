
#import <AVFoundation/AVFoundation.h>
#import <UIKit/UIKit.h>

@interface VRPlayerScene : NSObject

+(void) index: (int)idx;

-(void) addLabelWithName: (NSString*)name Text: (NSString*)text NormalizedPosition:(CGPoint)position FontName:(NSString*)fontName FontColor:(UIColor*)color  FontSize:(int)fontSize;

- (void) addLabelWithName: (NSString*)name TexPath: (NSString*)path;

-(void) removeLabel: (NSString*)name;

-(void) addMenuItemWithName: (NSString*)name TexturePath:(NSString*)path Rect: (CGRect)rect;

-(void) removeMenuItem: (NSString*)name;

@end
