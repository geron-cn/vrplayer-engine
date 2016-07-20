
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
#import "MD360Renderer.h"
#import "logo.h"

@interface VRPlayerViewController(){
    CGRect videoframe;
    IJKFFMoviePlayerController *player;
    BOOL shouldPlayAfterScrubing;
    BOOL isScrubbing;
    NSTimer *timer;
}
@property (nonatomic, strong) MDVRLibrary* vrLibrary;
@property (nonatomic, strong) NSURL* mURL;
@property (nonatomic, strong) UIImageView *logoImageView;

@end
@implementation VRPlayerViewController

static ViewMode s_viewMode = VIEW_DISPLAY_AUTODETECT;
static bool is2DViewDispaying = false;

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
    [player shutdown];
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

static const float RATIO_4_3 = 1.3333333333334; // 4/3
static const float RATIO_16_9 =  1.77777777777778;

-(void)initView
{
    BOOL is2d = false;
    
//    if(s_viewMode == VIEW_DISPLAY_AUTODETECT)
//    {
//        AVAssetTrack* track = [[s_playerItem tracks].firstObject assetTrack];
//        if(track != nil)
//        {
//            int v_h = track.naturalSize.height;
//            float w_d_ratio = v_h <= 0? 0.0f : track.naturalSize.width / v_h;
//            is2d = fabsf(w_d_ratio - RATIO_4_3) < 0.0001 || fabsf(w_d_ratio - RATIO_16_9) < 0.0001;
//        }
//        else
//            is2d = false;
//    }
//    else
//    {
//        is2d = s_viewMode == VIEW_DISPLAY_FORCE_2DPLANE;
//    }
    
    if(is2d)
    {
        self.vrLibrary = nil;
        
        // init subview
        CGRect frame = self.view.frame;
//        [self.videoPlayerView setFrame:frame];
//        [self.view addSubview:self.videoPlayerView];
        is2DViewDispaying = true;
    }
    else
    {
        /////////////////////////////////////////////////////// MDVRLibrary
        MDVRConfiguration* config = [MDVRLibrary createConfig];
        
        [config displayMode:MDModeDisplayGlass];
        [config interactiveMode:MDModeInteractiveMotion];
//        [config asVideo:s_playerItem];
        [config setContainer:self view:self.view];
        [config pinchEnabled:true];
        
        self.vrLibrary = [config build];
        //init subview
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
        is2DViewDispaying = false;
    }
    [super viewDidLayoutSubviews];
}

- (int) getVideoWidth
{
    if (player)
    {
        return player.naturalSize.width;
    }
    return 0;
}

- (int) getVideoHeigth
{
    if (player)
    {
        return player.naturalSize.height;
    }
    return 0;
}

+ (void) setViewMode:(ViewMode)viewMode
{
    int mode = 0;
    if (viewMode == VIEW_DISPLAY_FORCE_2DPLANE)
        mode = 1;
    else if (viewMode == VIEW_DISPLAY_FORCE_VRMODE)
        mode = 2;
    [MD360Renderer setVideoRenderMode: mode];
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
    
    [self installMovieNotificationObservers];
    
    [player prepareToPlay];
}

- (void)viewDidDisappear:(BOOL)animated {
    [player shutdown];
    [super viewDidDisappear:animated];
    
    [self removeMovieNotificationObservers];
}

- (void)loadStateDidChange:(NSNotification*)notification
{
    //    MPMovieLoadStateUnknown        = 0,
    //    MPMovieLoadStatePlayable       = 1 << 0,
    //    MPMovieLoadStatePlaythroughOK  = 1 << 1, // Playback will be automatically started in this state when shouldAutoplay is YES
    //    MPMovieLoadStateStalled        = 1 << 2, // Playback will be automatically paused in this state, if started
    
    IJKMPMovieLoadState loadState = player.loadState;
    
    if ((loadState & IJKMPMovieLoadStatePlaythroughOK) != 0) {
        NSLog(@"loadStateDidChange: IJKMPMovieLoadStatePlaythroughOK: %d\n", (int)loadState);
    } else if ((loadState & IJKMPMovieLoadStateStalled) != 0) {
        NSLog(@"loadStateDidChange: IJKMPMovieLoadStateStalled: %d\n", (int)loadState);
        if ([self.delegate respondsToSelector:@selector(videoPlayerPlaybackBufferEmpty:)])
        {
            [self.delegate videoPlayerPlaybackBufferEmpty:self];
        }
    } else {
        NSLog(@"loadStateDidChange: ???: %d\n", (int)loadState);
    }
}

- (void)moviePlayBackDidFinish:(NSNotification*)notification
{
    //    MPMovieFinishReasonPlaybackEnded,
    //    MPMovieFinishReasonPlaybackError,
    //    MPMovieFinishReasonUserExited
    int reason = [[[notification userInfo] valueForKey:IJKMPMoviePlayerPlaybackDidFinishReasonUserInfoKey] intValue];
    
    switch (reason)
    {
        case IJKMPMovieFinishReasonPlaybackEnded:
            NSLog(@"playbackStateDidChange: IJKMPMovieFinishReasonPlaybackEnded: %d\n", reason);
                if ([self.delegate respondsToSelector:@selector(videoPlayerDidReachEnd:)])
                {
                    [self.delegate videoPlayerDidReachEnd:self];
                }
            break;
            
        case IJKMPMovieFinishReasonUserExited:
            NSLog(@"playbackStateDidChange: IJKMPMovieFinishReasonUserExited: %d\n", reason);
            break;
            
        case IJKMPMovieFinishReasonPlaybackError:
        {
            NSDictionary *userInfo = [NSDictionary dictionaryWithObject:@"文件播放遇到错误，网络连接失败或者服务器上文件损坏！"                                                                      forKey:NSLocalizedDescriptionKey];
            NSError *error = [NSError errorWithDomain:@"VRLive Player" code:reason userInfo:userInfo];
            NSLog(@"error: %@", [error localizedDescription]);
            if ([self.delegate respondsToSelector:@selector(videoPlayer:didFailWithError:)])
                {
                    [self.delegate videoPlayer:self didFailWithError:error];
                }
        }
//            NSLog(@"playbackStateDidChange: IJKMPMovieFinishReasonPlaybackError: %d\n", reason);

            break;
            
        default:
            NSLog(@"playbackPlayBackDidFinish: ???: %d\n", reason);
            break;
    }
}

- (void)mediaIsPreparedToPlayDidChange:(NSNotification*)notification
{
    NSLog(@"mediaIsPreparedToPlayDidChange\n");
    if ([self.delegate respondsToSelector:@selector(videoPlayerIsReadyToPlayVideo:)])
    {
        [self.delegate videoPlayerIsReadyToPlayVideo:self];
    }
    if ([self.delegate respondsToSelector:@selector(videoPlayerPlaybackLikelyToKeepUp:)])
    {
            [self.delegate videoPlayerPlaybackLikelyToKeepUp:self];
    }
}

- (void)moviePlayBackStateDidChange:(NSNotification*)notification
{
    //    MPMoviePlaybackStateStopped,
    //    MPMoviePlaybackStatePlaying,
    //    MPMoviePlaybackStatePaused,
    //    MPMoviePlaybackStateInterrupted,
    //    MPMoviePlaybackStateSeekingForward,
    //    MPMoviePlaybackStateSeekingBackward
    
    switch (player.playbackState)
    {
        case IJKMPMoviePlaybackStateStopped: {
            NSLog(@"IJKMPMoviePlayBackStateDidChange %d: stoped", (int)player.playbackState);
            break;
        }
        case IJKMPMoviePlaybackStatePlaying: {
            NSLog(@"IJKMPMoviePlayBackStateDidChange %d: playing", (int)player.playbackState);
            break;
        }
        case IJKMPMoviePlaybackStatePaused: {
            {
            NSLog(@"IJKMPMoviePlayBackStateDidChange %d: paused", (int)player.playbackState);
            }
            break;
        }
        case IJKMPMoviePlaybackStateInterrupted: {
            NSLog(@"IJKMPMoviePlayBackStateDidChange %d: interrupted", (int)player.playbackState);
            break;
        }
        case IJKMPMoviePlaybackStateSeekingForward:
        case IJKMPMoviePlaybackStateSeekingBackward: {
            NSLog(@"IJKMPMoviePlayBackStateDidChange %d: seeking", (int)player.playbackState);
            break;
        }
        default: {
            NSLog(@"IJKMPMoviePlayBackStateDidChange %d: unknown", (int)player.playbackState);
            break;
        }
    }
}

- (void)loadedTimeRangeDidChange:(NSNotification*)notification
{
    if ([self.delegate respondsToSelector:@selector(videoPlayer:loadedTimeRangeDidChange:)])
    {
        NSInteger dur = player.playableDuration;
        [self.delegate videoPlayer:self loadedTimeRangeDidChange:dur];
    }
}

#pragma mark Install Movie Notifications

/* Register observers for the various movie object notifications. */
-(void)installMovieNotificationObservers
{
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(loadStateDidChange:)
                                                 name:IJKMPMoviePlayerLoadStateDidChangeNotification
                                               object:player];
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(moviePlayBackDidFinish:)
                                                 name:IJKMPMoviePlayerPlaybackDidFinishNotification
                                               object:player];
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(mediaIsPreparedToPlayDidChange:)
                                                 name:IJKMPMediaPlaybackIsPreparedToPlayDidChangeNotification
                                               object:player];
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(moviePlayBackStateDidChange:)
                                                 name:IJKMPMoviePlayerPlaybackStateDidChangeNotification
                                               object:player];
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(loadedTimeRangeDidChange:) name:IJKMPMoviePlayerBufferingTimeChangedNotification object:player];
    
    timer = [NSTimer scheduledTimerWithTimeInterval:0.5f target:self selector:@selector(timeChanged:) userInfo:nil repeats:YES];
}

#pragma mark Remove Movie Notification Handlers

/* Remove the movie notification observers from the movie object. */
-(void)removeMovieNotificationObservers
{
    [[NSNotificationCenter defaultCenter]removeObserver:self name:IJKMPMoviePlayerLoadStateDidChangeNotification object:player];
    [[NSNotificationCenter defaultCenter]removeObserver:self name:IJKMPMoviePlayerPlaybackDidFinishNotification object:player];
    [[NSNotificationCenter defaultCenter]removeObserver:self name:IJKMPMediaPlaybackIsPreparedToPlayDidChangeNotification object:player];
    [[NSNotificationCenter defaultCenter]removeObserver:self name:IJKMPMoviePlayerPlaybackStateDidChangeNotification object:player];
    [[NSNotificationCenter defaultCenter]removeObserver:self name:IJKMPMoviePlayerBufferingTimeChangedNotification object:player];
    [timer invalidate];
}

- (void)timeChanged:(id)userinfo
{
    if ([self isPlaying] && [self.delegate respondsToSelector:@selector(videoPlayer:timeDidChange:)])
    {
        [self.delegate videoPlayer:self timeDidChange:[self currentTime]];
    }
}

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
