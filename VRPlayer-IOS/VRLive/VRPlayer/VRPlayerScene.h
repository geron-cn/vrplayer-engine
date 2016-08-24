
#import <AVFoundation/AVFoundation.h>
#import <UIKit/UIKit.h>

@interface VRPlayerScene : NSObject

-(void) addLabelWithName: (NSString*)name Text: (NSString*)text FontName:(NSString*)fontName FontColor:(UIColor*)color  FontSize:(int)fontSize NormalizedY:(float)y Duration:(float)duration;

- (void) addTextureLabelWithName: (NSString*)name TexPath: (NSString*)path NormalizedStart: (CGPoint)start NormalizedEnd: (CGPoint)end Duration:(float)duration FadeInAndOut:(BOOL) useFade;

- (void) addLabelWithName2: (NSString*)name TexDir: (NSString*)dir BaseIndex: (int)baseIndex FrameCount: (int)framecount NormalizedStart: (CGPoint)start NormalizedEnd: (CGPoint)end Duration:(float)duration;

-(void) removeLabel: (NSString*)name;

-(void) addMenuItemWithName: (NSString*)name TexturePath:(NSString*)path Rect: (CGRect)rect;

-(void) removeMenuItem: (NSString*)name;

- (void) showPlayerHeadControlMenu: (BOOL)show;

- (void) showPlayerCustomMenu: (BOOL)show;

- (void) showCusor: (BOOL)show;

+ (void) labelRotation:(int)degree;

@end
