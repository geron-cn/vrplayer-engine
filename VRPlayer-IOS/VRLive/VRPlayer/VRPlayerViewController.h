
#import <AVFoundation/AVFoundation.h>
#import <UIKit/UIKit.h>
#import "VRPlayerScene.h"

@class VRPlayerViewController;


/**
 * @brief @~chinese 播放器代理，播放器的状态信息可以通过该类获取
 * @~english media player delegate, state of player can be access through it.
 */
@protocol VRPlayerDelegate <NSObject>

@optional
/**
 * @~chinese 播放器准备好时调用，已经拿到待播放的数据
 * @~english media player is ready to play, data is comming
 * @param videoPlayer @~chinese 播放器对象 @~english media player object
 */
- (void)videoPlayerIsReadyToPlayVideo:(VRPlayerViewController *)videoPlayer;
/**
 * @~chinese 视频播放结束时调用 @~english media player reached the end
 * @param videoPlayer @~chinese 播放器对象 @~english media player object
 */
- (void)videoPlayerDidReachEnd:(VRPlayerViewController *)videoPlayer;
/**
 * @~chinese 播放时间变化时调用，可以在此时修改播放进度条 @~english Called when media player time changed, you can modify the time process through this function.
 * @param videoPlayer @~chinese 播放器对象 @~english media player object
 * @param cmTime @~chinese 播放器当前时间 @~english curren time
 */
- (void)videoPlayer:(VRPlayerViewController *)videoPlayer timeDidChange:(CMTime)cmTime;
/**
 * @~chinese 加载的视频时间变化时调用 @~english Called when loaded time of media player changed
 * @param videoPlayer @~chinese 播放器对象 @~english media player object
 * @param duration @~chinese 加载时间 @~english duration of loaded time
 */
- (void)videoPlayer:(VRPlayerViewController *)videoPlayer loadedTimeRangeDidChange:(float)duration;
/**
 * @~chinese 播放缓存为空时调用 @~english Called when playback buffer empty
 * @param videoPlayer @~chinese 播放器对象 @~english media player object
 */
- (void)videoPlayerPlaybackBufferEmpty:(VRPlayerViewController *)videoPlayer;
/**
 * @~chinese 播放器即将展现出来时调用 @~english Called when player is going to play video
 * @param videoPlayer @~chinese 播放器对象 @~english media player object
 */
- (void)videoPlayerPlaybackLikelyToKeepUp:(VRPlayerViewController *)videoPlayer;
/**
 * @~chinese 视频播放出现错误时调用 @~english Called when error occured.
 * @param videoPlayer @~chinese 播放器对象 @~english media player object
 * @param error @~chinese 错误信息 @~english error info
 */
- (void)videoPlayer:(VRPlayerViewController *)videoPlayer didFailWithError:(NSError *)error;

@end

/**
 * @brief chinese 视图构建模式 @~english view display mode
 * <b>VIEW_DISPLAY_AUTODETECT</b>@~chinese 根据视频源自动检测 @~english auto detect view display mode<br>
 * <b>VIEW_DISPLAY_FORCE_VRMODE</b>@~chinese 强制平面模式 @~english force use plane 2d/3d view<br>
 * <b>VIEW_DISPLAY_FORCE_2DPLANE</b>@~chinese 强制全景模式 @~english force use vr mode display view<br> */
typedef enum{
    VIEW_DISPLAY_AUTODETECT = 1 << 0,
    VIEW_DISPLAY_FORCE_VRMODE = 1 << 1,
    VIEW_DISPLAY_FORCE_2DPLANE = 1 << 2
} ViewMode;

/**
 * @brief @~chinese 播放器类 @~english media player view controller
 */
@interface VRPlayerViewController : UIViewController

/**
 * @~chinese 播放器代理 @~english delegate
 */
@property (nonatomic, weak) id<VRPlayerDelegate> delegate;

/**
 * @~chinese 用待播放的url初始化播放器 @~english initialize the player with url
 * @param url @~chinese 待播放的url @~english url to be played
 */
- (void) initWithURL:(NSURL*)url;

/**
 * @~chinese 开始视频播放 @~english play the video
 */
- (void)play;
/**
 * @~chinese 暂停视频播放 @~english pause the video
 */
- (void)pause;
/**
 * @~chinese 视频跳转到指定时间 @~english seek media player to time
 * @param time @~chinese 跳转到的时间 @~english the time to seek to
 */
- (void)seekToTime:(float)time;
/**
 * @~chinese 关闭视频播放 @~english close the video player
 */
- (void)close;

/**
 * @~chinese 配合时间进度条使用，开始拖动时候调用 @~english together with time process bar, can be called when start to scrub the time bar
 */
- (void)startScrubbing;
/**
 * @~chinese 配合时间进度条使用，拖动过程中调用 @~english together with time bar, can be called during scrubbing
 * @param time @~chinese 拖动到的时间 @~english time to scrub
 */
- (void)scrub:(float)time;
/**
 * @~chinese 配合时间进度条使用，结束拖动时候调用 @~english together with time bar, can be called when stop scrubbing
 */
- (void)stopScrubbing;

/**
 * @~chinese 设置视频声音大小(0 - 1) @~english set volume
 * @param volume @~chinese 声音大小 @~english volume
 */
- (void)setVolume:(float)volume;

/**
 * @~chinese 是否循环播放 @~english is looping
 * @return @~chinese 是否循环播放 @~english is looping
 */
- (BOOL)isLooping;
/**
 * @~chinese 设置是否循环播放 @~english set looping
 * @param loop @~chinese 是否循环播放，YES是，NO否 @~english is looping or not
 */
- (void)setLooping: (BOOL)loop;
/**
 * @~chinese 是否正在播放 @~english is it playing
 * @return @~chinese 是否正在播放 @~english is it playing
 */
- (BOOL)isPlaying;
/**
 * @~chinese 是否静音 @~english is muted?
 * @return @~chinese 是否静音 @~english is it muted
 */
- (BOOL)isMuted;
/**
 * @~chinese 设置静音 @~english set mute or not
 * @param muted @~chinese 是否静音，YES是，NO否 @~english muted or not
 */
- (void)setMuted: (BOOL)muted;
/**
 * @~chinese 视频时间长度 @~english get video duration
 * @return @~chinese 视频时间长度 @~english video duration
 */
- (CMTime)duration;
/**
 * @~chinese 视频播放的当前时间 @~english get video current time
 * @return @~chinese 视频播放的当前时间 @~english video current time
 */
- (CMTime)currentTime;

/**
 * @~chinese 是否分屏模式 @~english is vr mode? vr mode is for using glass
 * @return @~chinese 是否分屏模式 @~english is vr mode or not
 */
- (BOOL)isVRMode;
/**
 * @~chinese 设置分屏模式 @~english set vr mode
 * @param vrMode @~chinese 是否分屏模式 @~english YES for vr mode, else for not
 */
- (void)setVRMode: (BOOL)vrMode;

/**
 * @brief @~chinese 设置视图初始化模式
 @~english set view init mode
 * @param viewMode @see #ViewMode
 *
 */
+ (void)setViewMode: (ViewMode) viewMode;

/**
 * @brief @~chinese 视频宽 @~english video width
 */
- (int)getVideoWidth;

/**
 * @brief @~chinese 视频高 @~english video height
 */
- (int)getVideoHeigth;

/**
 * @~chinese 是否陀螺仪控制旋转 @~english is using gyroscope sensor to control rotation
 * reutrn @~chinese 是否陀螺仪控制旋转 @~english is using gyroscope?
 */
- (BOOL)isMotionControl;
/**
 * @~chinese 设置是否陀螺仪控制旋转 @~english set use gyroscope or not
 * @param useMotionSensor @~chinese 是否启用陀螺仪控制 @~english use gyroscope or not
 */
- (void)setMotionControl: (BOOL)useMotionSensor;

/**
 * @~chinese 设置眼睛到场景的距离 @~english set distance from eye to scene
 * @param distance @~chinese 眼睛到场景的距离 @~english distance from eye to scene
 */
- (void)setEyeDistance: (float)distance;
/**
 * @~chinese 获取眼睛到场景的距离 @~english get distance from eye to scene
 * @return @~chinese 眼睛到场景的距离 @~english distance from eye to scene
 */
- (float)getEyeDistance;

- (VRPlayerScene*) getVRPlayerScene;

/**
 * 内部接口关闭水印
 */
- (void)AdvertisementOff;

@end
