//
//  HTY360PlayerVC.m
//  HTY360Player
//
//  Created by  on 11/8/15.
//  Copyright © 2015 Hanton. All rights reserved.
//

#import "QGVideoPlayViewController.h"
#import "VRPlayerViewController.h"
#import "FeThreeDotGlow.h"
#import "VRPlayerScene.h"

#define HIDE_CONTROL_DELAY 5.0f
#define DEFAULT_VIEW_ALPHA 1.f

#define SECOND_FRAMENT(s,a) [self convertTime:s append:a];

@interface QGVideoPlayViewController () <VRPlayerDelegate> {
    
    VRPlayerViewController *vrPlayerViewController;
    BOOL seekToZeroBeforePlay;
    NSTimer* timer;
}

@property (assign, nonatomic) CGFloat   totalTime;
@property (assign, nonatomic) CGFloat   currentTime;
@property (assign, nonatomic) CGFloat remainTime;

@property (copy, nonatomic) NSString *videoTitle;
@property (strong, nonatomic) NSURL *videoURL;
@property (strong, nonatomic) IBOutlet UIView *playerControlView;
@property (strong, nonatomic) IBOutlet UIView *playerNavView;
@property (strong, nonatomic) IBOutlet UILabel *titleLabel;
@property (strong, nonatomic) IBOutlet UIButton *playButton;
@property (strong, nonatomic) IBOutlet UIButton *vrButton;
@property (strong, nonatomic) IBOutlet UISlider *progressSlider;
@property (strong, nonatomic) IBOutlet UIButton *backButton;
@property (strong, nonatomic) IBOutlet UILabel *currentTimeLabel;
@property (strong, nonatomic) IBOutlet UILabel *remainTimeLabel;

@property (strong, nonatomic) FeThreeDotGlow *threeDot;

@end

@implementation QGVideoPlayViewController

-(void)dealloc {
    [[NSNotificationCenter defaultCenter] removeObserver:self];
    
    [UIApplication sharedApplication].idleTimerDisabled = NO;
    
    [self setPlayerControlView:nil];
    [self setPlayerNavView:nil];
    [self setPlayButton:nil];
    [self setProgressSlider:nil];
    [self setBackButton:nil];
}

- (id)initWithNibName:(NSString *)nibNameOrNil
               bundle:(NSBundle *)nibBundleOrNil
                  url:(NSURL *)url
           videoTitle:(NSString *)title {
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        [self setVideoURL:url];
        [self setVideoTitle:title];
        
        
        UITapGestureRecognizer *singleTapRecognizer = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(handleSingleTapGesture:)];
        singleTapRecognizer.numberOfTapsRequired = 1;
        [self.view addGestureRecognizer:singleTapRecognizer];
    }
    
    return self;
}

-(void)viewDidLoad {
    [super viewDidLoad];
    
    [UIApplication sharedApplication].idleTimerDisabled = YES;
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(applicationWillResignActive:)
                                                 name:UIApplicationWillResignActiveNotification
                                               object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(applicationDidBecomeActive:)
                                                 name:UIApplicationDidBecomeActiveNotification
                                               object:nil];
    
    [ [NSNotificationCenter defaultCenter] addObserver:self selector:@selector(HeadCursorPauseEvent) name: @"pause" object:nil];
    [ [NSNotificationCenter defaultCenter] addObserver:self selector:@selector(HeadCursorBackEvent) name: @"back" object:nil];
    
    [ [NSNotificationCenter defaultCenter] addObserver:self selector:@selector(HeadCursorForwardEvent) name: @"forward" object:nil];
    [ [NSNotificationCenter defaultCenter] addObserver:self selector:@selector(HeadCursorBackwardEvent) name: @"backward" object:nil];
   
    
    [self configureGLKView];
    [self setupVideoPlaybackForURL:_videoURL];
    
    
    [self configurePlayButton];
    [self configureProgressSlider];
    [self configureBackButton];
    
    self.titleLabel.text = self.videoTitle;
    
    VRPlayerScene* scene = [vrPlayerViewController getVRPlayerScene];
    [scene addMenuItemWithName:@"bingkuai" TexturePath:@"menu/bingkuai.png" Rect:CGRectMake(-4.5, -24, 3, 3)];
    [scene addMenuItemWithName:@"pijiu" TexturePath:@"menu/pijiu.png" Rect:CGRectMake(-1.5, -24, 3, 3)];
    [scene addMenuItemWithName:@"qiche" TexturePath:@"menu/qiche.png" Rect:CGRectMake(1.5, -24, 3, 3)];
    [scene addMenuItemWithName:@"dianzan" TexturePath:@"menu/dianzan.png" Rect:CGRectMake(4.5, -24, 3, 3)];
    
    [scene addMenuItemWithName:@"feiji" TexturePath:@"menu/feiji.png" Rect:CGRectMake(-4.5, -27, 3, 3)];
    [scene addMenuItemWithName:@"youlun" TexturePath:@"menu/youlun.png" Rect:CGRectMake(-1.5, -27, 3, 3)];
    [scene addMenuItemWithName:@"xianhua" TexturePath:@"menu/xianhua.png" Rect:CGRectMake(1.5, -27, 3, 3)];
    [scene addMenuItemWithName:@"huojian" TexturePath:@"menu/huojian.png" Rect:CGRectMake(4.5, -27, 3, 3)];
    
    [ [NSNotificationCenter defaultCenter] addObserver:self selector:@selector(GiftProcess:) name: @"bingkuai" object:nil];
    [ [NSNotificationCenter defaultCenter] addObserver:self selector:@selector(GiftProcess:) name: @"pijiu" object:nil];
    [ [NSNotificationCenter defaultCenter] addObserver:self selector:@selector(GiftProcess:) name: @"qiche" object:nil];
    [ [NSNotificationCenter defaultCenter] addObserver:self selector:@selector(GiftProcess:) name: @"dianzan" object:nil];
    
    [ [NSNotificationCenter defaultCenter] addObserver:self selector:@selector(GiftProcess:) name: @"feiji" object:nil];
    [ [NSNotificationCenter defaultCenter] addObserver:self selector:@selector(GiftProcess:) name: @"youlun" object:nil];
    [ [NSNotificationCenter defaultCenter] addObserver:self selector:@selector(GiftProcess:) name: @"xianhua" object:nil];
    [ [NSNotificationCenter defaultCenter] addObserver:self selector:@selector(GiftProcess:) name: @"huojian" object:nil];
    
    timer = [NSTimer scheduledTimerWithTimeInterval:3.0
                                     target:self
                                   selector:@selector(updateTimer:)  userInfo:nil
                                    repeats:YES];
}

-(void)GiftProcess:(NSNotification *)gift
{
    NSLog(@"%@",gift.name);
    VRPlayerScene* scene = [vrPlayerViewController getVRPlayerScene];
    if ([gift.name  isEqual: @"qiche"])
    {
        [VRPlayerScene index:0];
        [scene addLabelWithName:@"gift" TexPath:@"gift/daqiche.png"];
    }
}

- (void)updateTimer:(NSTimer *)paramTimer{
    
//    return;
    static int i = 0;
    
    VRPlayerScene* scene = [vrPlayerViewController getVRPlayerScene];
    [VRPlayerScene index:i];
    switch (i) {
        case 0:
        {
            float g = (arc4random() % 100) / 100.f;
            float b = (arc4random() % 100) / 100.f;
            UIColor* color = [UIColor colorWithRed:1.f green:g blue:b alpha:1.f];
            int fontsize = arc4random() % 30 + 20;
            [scene addLabelWithName:@"TestText" Text:@"VR 真实幻境 VIDEO" NormalizedPosition:CGPointMake(0.5f, 0.5f) FontName:@"Helvetica" FontColor:color FontSize:fontsize];
            
            [scene addLabelWithName:@"gift" TexPath:@"gift/daqiche.png"];
        }
            break;
        case 1:
        {
            float g = (arc4random() % 100) / 100.f;
            float b = (arc4random() % 100) / 100.f;
            UIColor* color = [UIColor colorWithRed:1.f green:g blue:b alpha:1.f];
            int fontsize = arc4random() % 30 + 20;
            [scene addLabelWithName:@"TestText" Text:@"VR 真实幻境 VIDEO" NormalizedPosition:CGPointMake(0.5f, 0.5f) FontName:@"Helvetica" FontColor:color FontSize:fontsize];
            
            [scene addLabelWithName:@"gift1" TexPath:@"gift/dabingkuai.png"];
        }
            break;
        case 2:
        {
            float g = (arc4random() % 100) / 100.f;
            float b = (arc4random() % 100) / 100.f;
            UIColor* color = [UIColor colorWithRed:1.f green:g blue:b alpha:1.f];
            int fontsize = arc4random() % 30 + 20;
            [scene addLabelWithName:@"TestText" Text:@"VR 真实幻境 VIDEO" NormalizedPosition:CGPointMake(0.5f, 0.5f) FontName:@"Helvetica" FontColor:color FontSize:fontsize];
            
            [scene addLabelWithName:@"gift2" TexPath:@"gift/dafeiji.png"];
        }
            break;
        case 3:
        {
            float g = (arc4random() % 100) / 100.f;
            float b = (arc4random() % 100) / 100.f;
            UIColor* color = [UIColor colorWithRed:1.f green:g blue:b alpha:1.f];
            int fontsize = arc4random() % 30 + 20;
            [scene addLabelWithName:@"TestText" Text:@"VR 真实幻境 VIDEO" NormalizedPosition:CGPointMake(0.5f, 0.5f) FontName:@"Helvetica" FontColor:color FontSize:fontsize];
            
            [scene addLabelWithName:@"gift3" TexPath:@"gift/dadianzan.png"];
        }
            break;
        case 4:
        {
            float g = (arc4random() % 100) / 100.f;
            float b = (arc4random() % 100) / 100.f;
            UIColor* color = [UIColor colorWithRed:1.f green:g blue:b alpha:1.f];
            int fontsize = arc4random() % 30 + 20;
            [scene addLabelWithName:@"TestText" Text:@"VR 真实幻境 VIDEO" NormalizedPosition:CGPointMake(0.5f, 0.5f) FontName:@"Helvetica" FontColor:color FontSize:fontsize];
            
            [scene addLabelWithName:@"gift4" TexPath:@"gift/dahuojian.png"];
        }
            break;
        case 5:
        {
            float g = (arc4random() % 100) / 100.f;
            float b = (arc4random() % 100) / 100.f;
            UIColor* color = [UIColor colorWithRed:1.f green:g blue:b alpha:1.f];
            int fontsize = arc4random() % 30 + 20;
            [scene addLabelWithName:@"TestText" Text:@"VR 真实幻境 VIDEO" NormalizedPosition:CGPointMake(0.5f, 0.5f) FontName:@"Helvetica" FontColor:color FontSize:fontsize];
            
            [scene addLabelWithName:@"gift5" TexPath:@"gift/dapijiu.png"];
        }
            break;
        case 6:
        {
            float g = (arc4random() % 100) / 100.f;
            float b = (arc4random() % 100) / 100.f;
            UIColor* color = [UIColor colorWithRed:1.f green:g blue:b alpha:1.f];
            int fontsize = arc4random() % 30 + 20;
            [scene addLabelWithName:@"TestText" Text:@"VR 真实幻境 VIDEO" NormalizedPosition:CGPointMake(0.5f, 0.5f) FontName:@"Helvetica" FontColor:color FontSize:fontsize];
            
            [scene addLabelWithName:@"gift6" TexPath:@"gift/daxianhua.png"];
        }
            break;
        case 7:
        {
            float g = (arc4random() % 100) / 100.f;
            float b = (arc4random() % 100) / 100.f;
            UIColor* color = [UIColor colorWithRed:1.f green:g blue:b alpha:1.f];
            int fontsize = arc4random() % 30 + 20;
            [scene addLabelWithName:@"TestText" Text:@"VR 真实幻境 VIDEO" NormalizedPosition:CGPointMake(0.5f, 0.5f) FontName:@"Helvetica" FontColor:color FontSize:fontsize];
            
            [scene addLabelWithName:@"gift7" TexPath:@"gift/dayoulun.png"];
        }
            break;
            
        default:
            break;
    }
    
    i++;
    i = i % 8;
}

- (void)HeadCursorBackEvent
{
    [self backButtonTouched: nil];
}

- (void)HeadCursorPauseEvent
{
    [self playButtonTouched: nil];
}

- (void)HeadCursorForwardEvent
{
    CMTime cur = [vrPlayerViewController currentTime];
    float t = CMTimeGetSeconds(cur);
    [vrPlayerViewController scrub:10 + t];
    [vrPlayerViewController stopScrubbing];
}

- (void)HeadCursorBackwardEvent
{
    CMTime cur = [vrPlayerViewController currentTime];
    float t = CMTimeGetSeconds(cur);
    [vrPlayerViewController scrub:-10 + t];
    [vrPlayerViewController stopScrubbing];
}

- (void)viewDidLayoutSubviews {
    [super viewDidLayoutSubviews];
    vrPlayerViewController.view.frame = self.view.bounds;
}

- (void)handleSingleTapGesture:(UITapGestureRecognizer *)recognizer {
    [self toggleControls];
}

- (void)setVRMode:(BOOL)isVRMode
{
    [vrPlayerViewController setVRMode:isVRMode];
}

- (BOOL)isVRMode
{
    return vrPlayerViewController.isVRMode;
}

- (void)applicationWillResignActive:(NSNotification *)notification {
    
}

- (void)applicationDidBecomeActive:(NSNotification *)notification {
    [self updatePlayButton];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
}

- (UIInterfaceOrientationMask)supportedInterfaceOrientations {
    return UIInterfaceOrientationMaskLandscape;
}

- (UIInterfaceOrientation)preferredInterfaceOrientationForPresentation {
    return UIInterfaceOrientationLandscapeRight;
}

- (BOOL)shouldAutorotate {
    return NO;
}

- (void)viewWillDisappear:(BOOL)animated {
    [super viewWillDisappear:animated];
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    [self updatePlayButton];
     [self startLoading];
}


#pragma mark video setting

- (void)setupVideoPlaybackForURL:(NSURL*)url {
    [vrPlayerViewController initWithURL:url];
}

#pragma mark rendering glk view management

- (void)configureGLKView {
    vrPlayerViewController = [[VRPlayerViewController alloc] init];
    vrPlayerViewController.delegate = self;
    [self.view insertSubview:vrPlayerViewController.view belowSubview:_playerControlView];
    [self addChildViewController:vrPlayerViewController];
    [vrPlayerViewController didMoveToParentViewController:self];
}

#pragma mark play button management

- (void)configurePlayButton{
    _playButton.backgroundColor = [UIColor clearColor];
    _playButton.showsTouchWhenHighlighted = YES;
    
    [self disablePlayerButtons];
    
    [self updatePlayButton];
}

- (IBAction)playButtonTouched:(id)sender {
    [NSObject cancelPreviousPerformRequestsWithTarget:self];
    if ([self isPlaying]) {
        [self pause];
    } else {
        [self play];
    }
}

- (void)updatePlayButton {
    [_playButton setImage:[UIImage imageNamed:[self isPlaying] ? @"icon_pasue" : @"icon_play"]
                 forState:UIControlStateNormal];
}

- (IBAction)vrButtonTouched:(id)sender {
    [NSObject cancelPreviousPerformRequestsWithTarget:self];
    if ([self isVRMode]) {
        [self setVRMode:NO];
    } else {
        [self setVRMode:YES];
    }
    [self updateVrButton];
}

- (void)updateVrButton {
    [_vrButton setImage:[UIImage imageNamed:[self isVRMode] ? @"icon_1pin" : @"icon_2pin"]
               forState:UIControlStateNormal];
}

- (void)play {
    if ([self isPlaying])
        return;
    
    if (YES == seekToZeroBeforePlay) {
        seekToZeroBeforePlay = NO;
        [vrPlayerViewController seekToTime:0];
    }
    
    
    [vrPlayerViewController play];
    [self updatePlayButton];
    
    [self scheduleHideControls];
}

- (void)pause {
    if (![self isPlaying])
        return;
    
    [vrPlayerViewController pause];
    [self updatePlayButton];
    
    [self scheduleHideControls];
}

#pragma mark progress slider management

-(void)configureProgressSlider {
    _progressSlider.continuous = NO;
    _progressSlider.value = 0;
    
    [_progressSlider setThumbImage:[UIImage imageNamed:@"icon_slid_drop"] forState:UIControlStateNormal];
    [_progressSlider setThumbImage:[UIImage imageNamed:@"icon_slid_drop"] forState:UIControlStateHighlighted];
    
    [_progressSlider setMaximumTrackImage:[UIImage imageNamed:@"bg_play_proess"]
                                 forState:UIControlStateNormal];
    [_progressSlider setMinimumTrackImage:[UIImage imageNamed:@"bg_play_proess_2"]
                                 forState:UIControlStateNormal];
    
    _progressSlider.enabled = NO;
}

#pragma mark back and gyro button management

-(void)configureBackButton {
    _backButton.backgroundColor = [UIColor clearColor];
    _backButton.showsTouchWhenHighlighted = YES;
}

#pragma mark controls management

- (void)enablePlayerButtons {
    _playButton.enabled = YES;
}

- (void)disablePlayerButtons {
    _playButton.enabled = NO;
}

- (void)toggleControls {
    if(_playerControlView.hidden){
        [self showControlsFast];
    }else{
        [self hideControlsFast];
    }
    
    [self scheduleHideControls];
}

-(void)scheduleHideControls {
    if(!_playerControlView.hidden) {
        [NSObject cancelPreviousPerformRequestsWithTarget:self];
        [self performSelector:@selector(hideControlsSlowly) withObject:nil afterDelay:HIDE_CONTROL_DELAY];
    }
}

-(void)hideControlsWithDuration:(NSTimeInterval)duration {
    _playerControlView.alpha = DEFAULT_VIEW_ALPHA;
    _playerNavView.alpha = DEFAULT_VIEW_ALPHA;
    [UIView animateWithDuration:duration
                          delay:0.0
                        options:UIViewAnimationOptionCurveEaseIn
                     animations:^(void) {
                         
                         _playerControlView.alpha = 0.0f;
                         _playerNavView.alpha = 0.0f;
                     }
                     completion:^(BOOL finished){
                         if(finished)
                             _playerControlView.hidden = YES;
                         _playerNavView.hidden = YES;
                     }];
    
}

-(void)hideControlsFast {
    [self hideControlsWithDuration:0.2];
}

-(void)hideControlsSlowly {
    [self hideControlsWithDuration:1.0];
}

-(void)showControlsFast {
    _playerControlView.alpha = 0.0;
    _playerControlView.hidden = NO;
    
    _playerNavView.alpha = 0.0;
    _playerNavView.hidden = NO;
    
    [UIView animateWithDuration:0.2
                          delay:0.0
                        options:UIViewAnimationOptionCurveEaseIn
                     animations:^(void) {
                         
                         _playerControlView.alpha = DEFAULT_VIEW_ALPHA;
                         _playerNavView.alpha = DEFAULT_VIEW_ALPHA;
                     }
                     completion:nil];
}

- (void)removeTimeObserverFro_player {
    
}

- (CMTime)playerItemDuration {
    return [vrPlayerViewController duration];
}

- (void)syncScrubber {
    CMTime playerDuration = [self playerItemDuration];
    if (CMTIME_IS_INVALID(playerDuration)) {
        _progressSlider.minimumValue = 0.0;
        return;
    }
    
    double duration = CMTimeGetSeconds(playerDuration);
    if (isfinite(duration)) {
        float minValue = [_progressSlider minimumValue];
        float maxValue = [_progressSlider maximumValue];
        double time = CMTimeGetSeconds([vrPlayerViewController currentTime]);
        
        [_progressSlider setValue:(maxValue - minValue) * time / duration + minValue];
        
        
        self.currentTime = time;
        self.remainTime = duration - self.currentTime;
        [self updateTimeLabel];
        
    }
}

- (IBAction)beginScrubbing:(id)sender {
    [vrPlayerViewController startScrubbing];
}

- (IBAction)scrub:(id)sender
{
    if ([sender isKindOfClass:[UISlider class]]) {
        UISlider* slider = sender;
        
        CMTime playerDuration = [self playerItemDuration];
        if (CMTIME_IS_INVALID(playerDuration)) {
            return;
        }
        
        double duration = CMTimeGetSeconds(playerDuration);
        if (isfinite(duration)) {
            float minValue = [slider minimumValue];
            float maxValue = [slider maximumValue];
            float value = [slider value];
            
            double time = duration * (value - minValue) / (maxValue - minValue);
            
            [vrPlayerViewController scrub:time];
        }
    }
}

- (IBAction)endScrubbing:(id)sender {
    [vrPlayerViewController stopScrubbing];
}

-(void)enableScrubber {
    _progressSlider.enabled = YES;
}

-(void)disableScrubber {
    _progressSlider.enabled = NO;
}

-(void)assetFailedToPrepareForPlayback:(NSError *)error {
    
    [self syncScrubber];
    [self disableScrubber];
    [self disablePlayerButtons];
    
    /* Display the error. */
    UIAlertController *alertController = [UIAlertController
                                          alertControllerWithTitle:@"提示"
                                          message:[error localizedDescription]
                                          preferredStyle:UIAlertControllerStyleAlert];
    UIAlertAction *okAction = [UIAlertAction
                               actionWithTitle:@"确定"
                               style:UIAlertActionStyleDefault
                               handler:^(UIAlertAction *action)
                               {
                                   NSLog(@"OK action");
                               }];
    [alertController addAction:okAction];
    [self presentViewController:alertController animated:YES completion:nil];
}

- (BOOL)isPlaying {
    return vrPlayerViewController.isPlaying;
}

- (void)playerItemDidReachEnd:(NSNotification *)notification {
    seekToZeroBeforePlay = YES;
}

#pragma mark - VRPlayerDelegate Method
- (void)videoPlayerIsReadyToPlayVideo:(VRPlayerViewController *)videoPlayer {
    seekToZeroBeforePlay = NO;
    [self syncScrubber];
    
    [self enableScrubber];
    [self enablePlayerButtons];
    [self updatePlayButton];
}

- (void)videoPlayerDidReachEnd:(VRPlayerViewController *)videoPlayer {
    seekToZeroBeforePlay = YES;
//    [vrPlayerViewController openURL:[NSURL URLWithString:@"http://vr.donglivr.net/qjdl/qiyiguo/12_05_15_comikazi_v01/mp4_fhd.mp4"]];
    [self updatePlayButton];
    
    
//    [self setupVideoPlaybackForURL:[NSURL URLWithString:@"http://vr.donglivr.net/qjdl/qiyiguo/12_05_15_comikazi_v01/mp4_fhd.mp4"]];
}

- (void)videoPlayer:(VRPlayerViewController *)videoPlayer timeDidChange:(CMTime)cmTime {
    [self syncScrubber];
}

- (void)videoPlayer:(VRPlayerViewController *)videoPlayer loadedTimeRangeDidChange:(float)duration {
    [self stopLoading];
}

- (void)videoPlayerPlaybackBufferEmpty:(VRPlayerViewController *)videoPlayer {
    [self startLoading];
}

- (void)videoPlayerPlaybackLikelyToKeepUp:(VRPlayerViewController *)videoPlayer {
    [self stopLoading];
}

- (void)videoPlayer:(VRPlayerViewController *)videoPlayer didFailWithError:(NSError *)error {
    [self stopLoading];
    [self assetFailedToPrepareForPlayback:error];
}


#pragma mark back button

- (IBAction)backButtonTouched:(id)sender {
    [NSObject cancelPreviousPerformRequestsWithTarget:self];
    
    
    [vrPlayerViewController close];
    
    [vrPlayerViewController removeFromParentViewController];
    vrPlayerViewController = nil;
    
    [self dismissViewControllerAnimated:YES completion:nil];
}



- (void)updateTimeLabel {
    self.currentTimeLabel.text = SECOND_FRAMENT(self.currentTime,@"");
    self.remainTimeLabel.text = SECOND_FRAMENT(self.remainTime,@"");
}

- (NSString *)convertTime:(CGFloat)second append:(NSString *)append
{
    NSMutableString *str = [NSMutableString string];
    int m = second/60;
    if (m <= 9)
    {
        [str appendFormat:@"0%d",m];
    }
    else
        [str appendFormat:@"%d",m];
    
    int s = (int)second%60;
    if (s <= 9)
    {
        [str appendFormat:@":0%d",s];
    }
    else
        [str appendFormat:@":%d",s];
    
    if (append)
    {
        [str appendString:append];
    }
    
    return str;
}

- (void)startLoading {
    if (!self.threeDot) {
        self.threeDot = [[FeThreeDotGlow alloc] initWithView:self.view blur:NO];
        [self.view insertSubview:self.threeDot belowSubview:self.playerControlView];
    }
    
    self.threeDot.hidden = NO;
    [self.threeDot show];
}

- (void)stopLoading {
    if (self.threeDot.isShowing) {
        [self.threeDot dismiss];
        self.threeDot.hidden = YES;
    }
}

@end
