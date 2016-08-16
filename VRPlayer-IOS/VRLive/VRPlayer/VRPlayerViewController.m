
#if 0
#import "VRPlayerViewController.h"
#import "VIMVideoPlayerView.h"
#import "VIMVideoPlayer.h"
#import "MDVRLibrary.h"
#import "logo.h"

@interface VRPlayerViewController()<VIMVideoPlayerViewDelegate>{
    CGRect videoframe;
}
@property (nonatomic, strong) VIMVideoPlayerView *videoPlayerView;
@property (nonatomic, strong) MDVRLibrary* vrLibrary;
@property (weak, nonatomic) IBOutlet UIButton *mInteractiveBtn;
@property (weak, nonatomic) IBOutlet UIButton *mDisplayBtn;
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
    self.videoPlayerView = nil;
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
    
    // Do any additional setup after loading the view, typically from a nib.
}

- (void)dealloc{
    self.vrLibrary = nil;
    self.videoPlayerView = nil;
    self.mURL = nil;
}

- (void) initWithURL:(NSURL*)url{
    self.mURL = url;
    [self initPlayer];
}

static  AVPlayerItem* s_playerItem;

- (void) initPlayer{
    //release old
    self.vrLibrary = nil;
    self.videoPlayerView = nil;
    
    // video player
//    self.videoPlayerView = [[VIMVideoPlayerView alloc] initWithFrame:CGRectMake(0, 0, [[UIScreen mainScreen] bounds].size.width, [[UIScreen mainScreen] bounds].size.height)];
    self.videoPlayerView = [[VIMVideoPlayerView alloc] initWithFrame:self.view.bounds];
    self.videoPlayerView.translatesAutoresizingMaskIntoConstraints = YES;
    self.videoPlayerView.delegate = self;
    [self.videoPlayerView setVideoFillMode:AVLayerVideoGravityResizeAspect];
    [self.videoPlayerView.player enableTimeUpdates];
    [self.videoPlayerView.player enableAirplay];
    
    AVPlayerItem* playerItem = [[AVPlayerItem alloc] initWithURL:self.mURL];
    [self.videoPlayerView.player setPlayerItem:playerItem];
    [self.videoPlayerView.player play];
    s_playerItem = playerItem;
    videoframe = self.view.frame;
}

- (void)play
{
    [self.videoPlayerView.player play];
}
- (void)pause
{
    [self.videoPlayerView.player pause];
}
- (void)seekToTime:(float)time
{
    [self.videoPlayerView.player seekToTime:time];
}
- (void)close
{
    [self.videoPlayerView.player reset];
    [self.presentingViewController dismissViewControllerAnimated:YES completion:nil];
}

- (void)startScrubbing
{
    [self.videoPlayerView.player startScrubbing];
}
- (void)scrub:(float)time
{
    [self.videoPlayerView.player scrub:time];
}
- (void)stopScrubbing
{
    [self.videoPlayerView.player stopScrubbing];
}

- (void)setVolume:(float)volume
{
    [self.videoPlayerView.player setVolume:volume];
}

//status
- (BOOL)isLooping
{
    return [self.videoPlayerView.player isLooping];
}
- (void)setLooping: (BOOL)loop
{
    self.videoPlayerView.player.looping = loop;
}
- (BOOL)isPlaying
{
    return [self.videoPlayerView.player isPlaying];
}
- (BOOL)isMuted
{
    return [self.videoPlayerView.player isMuted];
}
- (void)setMuted: (BOOL)muted
{
    self.videoPlayerView.player.muted = muted;
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

- (void)videoPlayerViewIsReadyToPlayVideo:(VIMVideoPlayerView *)videoPlayerView
{
    
    NSLog(@"videoPlayerIsReadyToPlayVideo: ");
    if ([self.delegate respondsToSelector:@selector(videoPlayerIsReadyToPlayVideo:)])
    {
        [self initView];
        [self.delegate videoPlayerIsReadyToPlayVideo:self];
    }
}

static const float RATIO_4_3 = 1.3333333333334; // 4/3
static const float RATIO_16_9 =  1.77777777777778;

-(void)initView
{
    BOOL is2d = false;
    
    if(s_viewMode == VIEW_DISPLAY_AUTODETECT)
    {
        AVAssetTrack* track = [[s_playerItem tracks].firstObject assetTrack];
        if(track != nil)
        {
            int v_h = track.naturalSize.height;
            float w_d_ratio = v_h <= 0? 0.0f : track.naturalSize.width / v_h;
            is2d = fabsf(w_d_ratio - RATIO_4_3) < 0.0001 || fabsf(w_d_ratio - RATIO_16_9) < 0.0001;
        }
        else
            is2d = false;
    }
    else
    {
        is2d = s_viewMode == VIEW_DISPLAY_FORCE_2DPLANE;
    }
    
    if(is2d)
    {
        self.vrLibrary = nil;
        
        // init subview
        CGRect frame = self.view.frame;
        [self.videoPlayerView setFrame:frame];
        [self.view addSubview:self.videoPlayerView];
        is2DViewDispaying = true;
    }
    else
    {
        /////////////////////////////////////////////////////// MDVRLibrary
        MDVRConfiguration* config = [MDVRLibrary createConfig];
        
        [config displayMode:MDModeDisplayGlass];
        [config interactiveMode:MDModeInteractiveMotion];
        [config asVideo:s_playerItem];
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

- (void) setViewMode:(ViewMode)viewMode
{
    s_viewMode = viewMode;
}

- (int) getVideoWidth
{
    AVAssetTrack* track = [[s_playerItem tracks].firstObject assetTrack];
    return track == nil ? 0 :track.naturalSize.width;
}

- (int) getVideoHeigth
{
    AVAssetTrack* track = [[s_playerItem tracks].firstObject assetTrack];
    return track == nil ? 0 :track.naturalSize.height;
}

- (void)videoPlayerViewDidReachEnd:(VIMVideoPlayerView *)videoPlayerView
{
    NSLog(@"videoPlayerDidReachEnd: ");
    if ([self.delegate respondsToSelector:@selector(videoPlayerDidReachEnd:)])
    {
        [self.delegate videoPlayerDidReachEnd:self];
    }
}
- (void)videoPlayerView:(VIMVideoPlayerView *)videoPlayerView timeDidChange:(CMTime)cmTime
{
    if ([self.delegate respondsToSelector:@selector(videoPlayer:timeDidChange:)])
    {
        [self.delegate videoPlayer:self timeDidChange:cmTime];
    }
}
- (void)videoPlayerView:(VIMVideoPlayerView *)videoPlayerView loadedTimeRangeDidChange:(float)duration
{
    if ([self.delegate respondsToSelector:@selector(videoPlayer:loadedTimeRangeDidChange:)])
    {
        [self.delegate videoPlayer:self loadedTimeRangeDidChange:duration];
    }
}
- (void)videoPlayerViewPlaybackBufferEmpty:(VIMVideoPlayerView *)videoPlayerView
{
    NSLog(@"videoPlayerViewPlaybackBufferEmpty: ");
    if ([self.delegate respondsToSelector:@selector(videoPlayerPlaybackBufferEmpty:)])
    {
        [self.delegate videoPlayerPlaybackBufferEmpty:self];
    }
}
- (void)videoPlayerViewPlaybackLikelyToKeepUp:(VIMVideoPlayerView *)videoPlayerView
{
    NSLog(@"videoPlayerViewPlaybackLikelyToKeepUp: ");
    if ([self.delegate respondsToSelector:@selector(videoPlayerPlaybackLikelyToKeepUp:)])
    {
        [self.delegate videoPlayerPlaybackLikelyToKeepUp:self];
    }
}
- (void)videoPlayerView:(VIMVideoPlayerView *)videoPlayerView didFailWithError:(NSError *)error
{
    NSLog(@"error: %@", [error localizedDescription]);
    if ([self.delegate respondsToSelector:@selector(videoPlayer:didFailWithError:)])
    {
        [self.delegate videoPlayer:self didFailWithError:error];
    }
}

- (CMTime)duration
{
    return self.videoPlayerView.player.player.currentItem.duration;
}
- (CMTime)currentTime
{
    return self.videoPlayerView.player.player.currentTime;
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

- (NSString*) version
{
    return @"1.8";
}

@end
#endif
