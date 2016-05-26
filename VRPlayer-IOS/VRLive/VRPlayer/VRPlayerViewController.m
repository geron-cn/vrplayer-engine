
#import "VRPlayerViewController.h"
#import "VIMVideoPlayerView.h"
#import "VIMVideoPlayer.h"
#import "MDVRLibrary.h"

@interface VRPlayerViewController()<VIMVideoPlayerViewDelegate>{
    CGRect videoframe;
}
@property (nonatomic, strong) VIMVideoPlayerView *videoPlayerView;
@property (nonatomic, strong) MDVRLibrary* vrLibrary;
@property (weak, nonatomic) IBOutlet UIButton *mInteractiveBtn;
@property (weak, nonatomic) IBOutlet UIButton *mDisplayBtn;
@property (nonatomic, strong) NSURL* mURL;
@end
@implementation VRPlayerViewController
- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.vrLibrary = nil;
    self.videoPlayerView = nil;
    self.mURL = nil;
    // Do any additional setup after loading the view, typically from a nib.
}

- (void)dealloc{
    self.vrLibrary = nil;
    self.videoPlayerView = nil;
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
    

    /////////////////////////////////////////////////////// MDVRLibrary
    MDVRConfiguration* config = [MDVRLibrary createConfig];
    
    [config displayMode:MDModeDisplayGlass];
    [config interactiveMode:MDModeInteractiveMotion];
    [config asVideo:playerItem];
    [config setContainer:self view:self.view];
    [config pinchEnabled:true];
    
    self.vrLibrary = [config build];
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
        [self.delegate videoPlayerIsReadyToPlayVideo:self];
    }
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

@end
