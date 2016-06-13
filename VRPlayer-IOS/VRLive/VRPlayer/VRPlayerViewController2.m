
#if 1
#import "VRPlayerViewController.h"
#import "IJKMediaPlayer.h"
#import "IJKMediaPlayback.h"
#import <IJKFFOptions.h>
#import <IJKFFMoviePlayerController.h>
#import <IJKAVMoviePlayerController.h>
#import <IJKMediaModule.h>
#import <IJKMediaPlayer.h>
#import <IJKNotificationManager.h>
#import <IJKKVOController.h>
#import "MDVRLibrary.h"
#import "logo.h"

@interface VRPlayerViewController(){
    CGRect videoframe;
    IJKFFMoviePlayerController *player;
    BOOL shouldPlayAfterScrubing;
    BOOL isScrubbing;
}
@property (nonatomic, strong) MDVRLibrary* vrLibrary;
@property (nonatomic, strong) NSURL* mURL;
@property (nonatomic, strong) UIImageView *logoImageView;

@end
@implementation VRPlayerViewController

static BOOL   hasAdvertisement = YES;

- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.vrLibrary = nil;
    self.mURL = nil;
    if (hasAdvertisement)
    {
        NSData* data = [NSData dataWithBytes:[logoData logoDataBuffer] length:[logoData logoDataSize]];
        self.logoImageView = [[UIImageView alloc] initWithImage:[UIImage imageWithData:data]];
        self.logoImageView.translatesAutoresizingMaskIntoConstraints = NO;
        [self.view addSubview:self.logoImageView];
        
        [self.view addConstraint:[NSLayoutConstraint constraintWithItem:self.logoImageView
                                                              attribute:NSLayoutAttributeTop
                                                              relatedBy:NSLayoutRelationEqual
                                                                 toItem:self.view
                                                              attribute:NSLayoutAttributeTop
                                                             multiplier:1
                                                               constant:10]];
        
        [self.view addConstraint:[NSLayoutConstraint constraintWithItem:self.logoImageView
                                                              attribute:NSLayoutAttributeLeft
                                                              relatedBy:NSLayoutRelationEqual
                                                                 toItem:self.view
                                                              attribute:NSLayoutAttributeLeft
                                                             multiplier:1
                                                               constant:10]];
        
    }
    
    isScrubbing = NO;
    // Do any additional setup after loading the view, typically from a nib.
}

- (void)dealloc{
    self.vrLibrary = nil;
    self.mURL = nil;
}

-(void)viewDidLayoutSubviews {
    [super viewDidLayoutSubviews];
    
    if (self.vrLibrary != nil)
    {
        CGRect frame = self.view.frame;//self.vrLibrary.bounds;
        //self.videoPlayerView.frame;
        if (videoframe.origin.x != frame.origin.x || videoframe.origin.y != frame.origin.y || videoframe.size.width != frame.size.width || videoframe.size.height != frame.size.height)
        {
            videoframe = frame;
            NSLog(@"viewDidLayoutSubviews @", NSStringFromCGRect(frame));
            if (self.vrLibrary != nil)
            {
                [self.vrLibrary resize];
            }
        }
        
    }
}

- (void) initWithURL:(NSURL*)url{
    self.mURL = url;
    [self initPlayer];
}

- (void) initPlayer{
    //release old
    self.vrLibrary = nil;
    
    // video player
#ifdef DEBUG
    [IJKFFMoviePlayerController setLogReport:YES];
    [IJKFFMoviePlayerController setLogLevel:k_IJK_LOG_DEBUG];
#else
    [IJKFFMoviePlayerController setLogReport:NO];
    [IJKFFMoviePlayerController setLogLevel:k_IJK_LOG_INFO];
#endif
    
    [IJKFFMoviePlayerController checkIfFFmpegVersionMatch:YES];
    // [IJKFFMoviePlayerController checkIfPlayerVersionMatch:YES major:1 minor:0 micro:0];
    
    IJKFFOptions *options = [IJKFFOptions optionsByDefault];
    
    player = [[IJKFFMoviePlayerController alloc] initWithContentURL:self.mURL withOptions:options];
    player.scalingMode = IJKMPMovieScalingModeAspectFit;
    player.shouldAutoplay = YES;
    

    /////////////////////////////////////////////////////// MDVRLibrary
    MDVRConfiguration* config = [MDVRLibrary createConfig];
    
    [config displayMode:MDModeDisplayGlass];
    [config interactiveMode:MDModeInteractiveMotion];
    [config setContainer:self view:self.view];
    [config pinchEnabled:true];
    
    self.vrLibrary = [config build];
    videoframe = self.view.frame;
}

- (void)play
{
    [player play];
}
- (void)pause
{
    [player pause];
}
- (void)seekToTime:(float)time
{
    player.currentPlaybackTime = time;
}
- (void)close
{
    [player stop];
    [self.presentingViewController dismissViewControllerAnimated:YES completion:nil];
}

- (void)startScrubbing
{
    if (player.isPlaying)
    {
        shouldPlayAfterScrubing = YES;
        isScrubbing = YES;
        [player pause];
    }
}
- (void)scrub:(float)time
{
    if (isScrubbing == NO)
    {
        [self startScrubbing];
    }
    player.currentPlaybackTime = time;
}
- (void)stopScrubbing
{
    isScrubbing = NO;
    if (shouldPlayAfterScrubing == YES)
    {
        [player play];
    }
}

- (void)setVolume:(float)volume
{
    [player setVolume:volume];
}

//status
- (BOOL)isLooping
{
    return [player isLoop];
}
- (void)setLooping: (BOOL)loop
{
    [player setLoop:loop];
}
- (BOOL)isPlaying
{
    return [player isPlaying];
}
- (BOOL)isMuted
{
    return [player isMuted];
}
- (void)setMuted: (BOOL)muted
{
    [player setMuted:muted];
}

//vr control
- (BOOL)isVRMode
{
    return self.vrLibrary.getDisplayMdoe == MDModeDisplayGlass;
}
- (void)setVRMode: (BOOL)vrMode
{
    BOOL curMode = ([self.vrLibrary getDisplayMdoe] == MDModeDisplayGlass);
    if (curMode != vrMode)
    {
        [self.vrLibrary switchDisplayMode];
    }
}
- (BOOL)isMotionControl
{
    return self.vrLibrary.getInteractiveMdoe == MDModeInteractiveMotion;
}
- (void)setMotionControl: (BOOL)useMotionSensor
{
    BOOL curControl = ([self.vrLibrary getInteractiveMdoe] == MDModeInteractiveMotion);
    if (curControl != useMotionSensor)
    {
        [self.vrLibrary switchInteractiveMode];
    }
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    
//    [self installMovieNotificationObservers];
    
    [player prepareToPlay];
}

- (void)viewDidDisappear:(BOOL)animated {
    [super viewDidDisappear:animated];
    
    [player shutdown];
//    [self removeMovieNotificationObservers];
}

//- (void)videoPlayerViewIsReadyToPlayVideo:(VIMVideoPlayerView *)videoPlayerView
//{
//    
//    NSLog(@"videoPlayerIsReadyToPlayVideo: ");
//    if ([self.delegate respondsToSelector:@selector(videoPlayerIsReadyToPlayVideo:)])
//    {
//        [self.delegate videoPlayerIsReadyToPlayVideo:self];
//    }
//}
//- (void)videoPlayerViewDidReachEnd:(VIMVideoPlayerView *)videoPlayerView
//{
//    NSLog(@"videoPlayerDidReachEnd: ");
//    if ([self.delegate respondsToSelector:@selector(videoPlayerDidReachEnd:)])
//    {
//        [self.delegate videoPlayerDidReachEnd:self];
//    }
//}
//- (void)videoPlayerView:(VIMVideoPlayerView *)videoPlayerView timeDidChange:(CMTime)cmTime
//{
//    if ([self.delegate respondsToSelector:@selector(videoPlayer:timeDidChange:)])
//    {
//        [self.delegate videoPlayer:self timeDidChange:cmTime];
//    }
//}
//- (void)videoPlayerView:(VIMVideoPlayerView *)videoPlayerView loadedTimeRangeDidChange:(float)duration
//{
//    if ([self.delegate respondsToSelector:@selector(videoPlayer:loadedTimeRangeDidChange:)])
//    {
//        [self.delegate videoPlayer:self loadedTimeRangeDidChange:duration];
//    }
//}
//- (void)videoPlayerViewPlaybackBufferEmpty:(VIMVideoPlayerView *)videoPlayerView
//{
//    NSLog(@"videoPlayerViewPlaybackBufferEmpty: ");
//    if ([self.delegate respondsToSelector:@selector(videoPlayerPlaybackBufferEmpty:)])
//    {
//        [self.delegate videoPlayerPlaybackBufferEmpty:self];
//    }
//}
//- (void)videoPlayerViewPlaybackLikelyToKeepUp:(VIMVideoPlayerView *)videoPlayerView
//{
//    NSLog(@"videoPlayerViewPlaybackLikelyToKeepUp: ");
//    if ([self.delegate respondsToSelector:@selector(videoPlayerPlaybackLikelyToKeepUp:)])
//    {
//        [self.delegate videoPlayerPlaybackLikelyToKeepUp:self];
//    }
//}
//- (void)videoPlayerView:(VIMVideoPlayerView *)videoPlayerView didFailWithError:(NSError *)error
//{
//    NSLog(@"error: %@", [error localizedDescription]);
//    if ([self.delegate respondsToSelector:@selector(videoPlayer:didFailWithError:)])
//    {
//        [self.delegate videoPlayer:self didFailWithError:error];
//    }
//}

- (CMTime)duration
{
    return CMTimeMakeWithSeconds(player.duration, [player playbackRate]);
}
- (CMTime)currentTime
{
    return CMTimeMakeWithSeconds(player.currentPlaybackTime,[player playbackRate]);
}

- (void)setEyeDistance: (float)distance
{
    [MDVRLibrary setSphereRadius:distance];
}
- (float)getEyeDistance
{
    return [MDVRLibrary getSphereRadius];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)AdvertisementOff
{
    hasAdvertisement = NO;
}

@end
#endif
