//
//  MDVRLibrary.h
//  MD360Player4IOS
//
//  Created by ashqal on 16/4/7.
//  Copyright © 2016年 ashqal. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import <UIKit/UIKit.h>

#define MDVR_RAW_NAME @ "vrlibraw.bundle"
#define MDVR_RAW_PATH [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent: MDVR_RAW_NAME]
#define MDVR_RAW [NSBundle bundleWithPath: MDVR_RAW_PATH]

typedef NS_ENUM(NSInteger, MDModeInteractive) {
    MDModeInteractiveTouch,
    MDModeInteractiveMotion,
};

typedef NS_ENUM(NSInteger, MDModeDisplay) {
    MDModeDisplayNormal,
    MDModeDisplayGlass,
};

@class MDVRLibrary;
@class IJKSDLGLView;

#pragma mark MDVRConfiguration
@interface MDVRConfiguration : NSObject
- (void) asVideo:(AVPlayerItem*)playerItem;
- (void) asImage:(id)data;
- (void) interactiveMode:(MDModeInteractive)interactiveMode;
- (void) displayMode:(MDModeDisplay)displayMode;
- (void) pinchEnabled:(bool)pinch;
- (void) setContainer:(UIViewController*)vc;
- (void) setContainer:(UIViewController*)vc view:(UIView*)view;
- (void) setIJKView:(IJKSDLGLView*)view;
- (MDVRLibrary*) build;
@end

#pragma mark MDVRLibrary
@interface MDVRLibrary : NSObject
+ (MDVRConfiguration*) createConfig;
+ (void)setSphereRadius:(float)radius;
+ (float)getSphereRadius;

- (void) switchInteractiveMode;
// - (void) switchInteractiveMode:(MDModeInteractive)interactiveMode;
- (MDModeInteractive) getInteractiveMdoe;

- (void) switchDisplayMode:(MDModeDisplay)displayMode;
- (void) switchDisplayMode;
- (MDModeDisplay) getDisplayMdoe;
- (void)resize;
- (CGRect)bounds;
@end

@protocol IMDDestroyable <NSObject>
-(void) destroy;
@end
