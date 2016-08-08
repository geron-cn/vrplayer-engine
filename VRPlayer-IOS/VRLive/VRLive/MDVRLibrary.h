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
#import "MD360Director.h"
#import "MDVideoDataAdapter.h"
#import "MD360Renderer.h"

typedef NS_ENUM(NSInteger, MDModeInteractive) {
    MDModeInteractiveTouch,
    MDModeInteractiveMotion,
    MDModeInteractiveMotionWithTouch,
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
- (void) asVideoWithDataAdatper:(id<MDVideoDataAdapter>)adapter;

- (void) displayAsDome;
- (void) displayAsSphere;

- (void) asImage:(id<IMDImageProvider>)data;
- (void) interactiveMode:(MDModeInteractive)interactiveMode;
- (void) displayMode:(MDModeDisplay)displayMode;
- (void) pinchEnabled:(bool)pinch;
- (void) setContainer:(UIViewController*)vc;
- (void) setContainer:(UIViewController*)vc view:(UIView*)view;
- (void) setIJKView:(IJKSDLGLView*)view;
- (void) setDirectorFactory:(id<MD360DirectorFactory>) directorFactory;
- (MDVRLibrary*) build;
@end

#pragma mark MDVRLibrary
@interface MDVRLibrary : NSObject
+ (MDVRConfiguration*) createConfig;
+ (void)setSphereRadius:(float)radius;
+ (float)getSphereRadius;

@property (nonatomic,strong) MD360Renderer* renderer;

- (void) switchInteractiveMode;
// - (void) switchInteractiveMode:(MDModeInteractive)interactiveMode;
- (MDModeInteractive) getInteractiveMdoe;

- (void) switchDisplayMode:(MDModeDisplay)displayMode;
- (void) switchDisplayMode;
- (MDModeDisplay) getDisplayMdoe;
- (void)resize;
- (CGRect)bounds;
@end
