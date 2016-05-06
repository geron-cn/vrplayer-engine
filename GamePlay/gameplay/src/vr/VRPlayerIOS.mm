#import "VRPlayerIOS.h"
#import "VRPlayer.h"

@implementation VRPlayerIOS
- (BOOL)play:(NSString*)url
{
    VRPlayer::getInstance()->play([url UTF8String]);
}
- (void)stop
{
    VRPlayer::getInstance()->play("");
}
- (BOOL)isVREnabled
{
    return VRPlayer::getInstance()->isVREnabled();
}
- (void)setVREnabled:(BOOL)enabled
{
    VRPlayer::getInstance()->setVREnabled(enabled);
}

-(instancetype)init {
    if ( self = [super init] )
    {
        _viewController = (__bridge UIViewController*)VRPlayer::getInstance()->initialize();
    }
    return self;
}

-(double)duration
{
    return VRPlayer::getInstance()->getDuration();
}

-(BOOL)isPause
{
    return VRPlayer::getInstance()->isPause();
}
-(BOOL)pause:(BOOL)paused
{
    return VRPlayer::getInstance()->pause(paused);
}
-(double)currentPos
{
    return VRPlayer::getInstance()->getCurrentPos();
}
-(BOOL)seek:(double)pos
{
    return VRPlayer::getInstance()->seek(pos);
}

- (void)dealloc
{
    VRPlayer::getInstance()->finalize();
    _viewController = nullptr;
}
@end

