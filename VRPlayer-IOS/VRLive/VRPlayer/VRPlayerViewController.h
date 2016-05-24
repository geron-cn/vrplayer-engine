
#import <AVFoundation/AVFoundation.h>
#import <UIKit/UIKit.h>

@class VRPlayerViewController;

@protocol VRPlayerDelegate <NSObject>

@optional
- (void)videoPlayerIsReadyToPlayVideo:(VRPlayerViewController *)videoPlayer;
- (void)videoPlayerDidReachEnd:(VRPlayerViewController *)videoPlayer;
- (void)videoPlayer:(VRPlayerViewController *)videoPlayer timeDidChange:(CMTime)cmTime;
- (void)videoPlayer:(VRPlayerViewController *)videoPlayer loadedTimeRangeDidChange:(float)duration;
- (void)videoPlayerPlaybackBufferEmpty:(VRPlayerViewController *)videoPlayer;
- (void)videoPlayerPlaybackLikelyToKeepUp:(VRPlayerViewController *)videoPlayer;
- (void)videoPlayer:(VRPlayerViewController *)videoPlayer didFailWithError:(NSError *)error;

@end

@interface VRPlayerViewController : UIViewController

@property (nonatomic, weak) id<VRPlayerDelegate> delegate;

// set url
- (void) initWithURL:(NSURL*)url;

// Playback
- (void)play;
- (void)pause;
- (void)seekToTime:(float)time;
- (void)close;

// Scrubbing
- (void)startScrubbing;
- (void)scrub:(float)time;
- (void)stopScrubbing;

// Volume
- (void)setVolume:(float)volume;

//status
- (BOOL)isLooping;
- (void)setLooping: (BOOL)loop;
- (BOOL)isPlaying;
- (BOOL)isMuted;
- (void)setMuted: (BOOL)muted;

//vr control
- (BOOL)isVRMode;
- (void)setVRMode: (BOOL)vrMode;
- (BOOL)isMotionControl;
- (void)setMotionControl: (BOOL)useMotionSensor;

@end
