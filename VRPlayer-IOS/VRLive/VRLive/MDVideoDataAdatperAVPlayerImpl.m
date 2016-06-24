//
//  MDVideoDataAdatperAVPlayerImpl.m
//  MD360Player4IOS
//
//  Created by ashqal on 16/4/9.
//  Copyright © 2016年 ashqal. All rights reserved.
//

#import "MDVideoDataAdatperAVPlayerImpl.h"

@interface MDVideoDataAdatperAVPlayerImpl ()
{
    CMTime lastBufferTime;
//    BOOL
}
@property (nonatomic, strong) AVPlayerItemVideoOutput* output;
@property (nonatomic, strong) AVPlayerItem* playerItem;

@end

@implementation MDVideoDataAdatperAVPlayerImpl
bool hadReady;

- (instancetype)initWithPlayerItem:(AVPlayerItem*) playerItem{
    self = [super init];
    if (self) {
        self.playerItem = playerItem;
        [self setup];
    }
    return self;
}

-(void) dealloc
{
    if (self.playerItem)
    {
        [self.playerItem removeObserver:self forKeyPath:@"status"];
        self.playerItem = nil;
    }
}

- (void) setup{
    [self.playerItem addObserver:self forKeyPath:@"status" options:nil context:nil];
    hadReady = false;
}

- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary *)change context:(void *)context
{
    if ([keyPath isEqualToString:@"status"])
    {
        if (AVPlayerItemStatusReadyToPlay == self.playerItem.status && !hadReady)
        {
            hadReady = true;
            NSDictionary *pixBuffAttributes = @{(id)kCVPixelBufferPixelFormatTypeKey: @(kCVPixelFormatType_32BGRA)};
            self.output = [[AVPlayerItemVideoOutput alloc] initWithPixelBufferAttributes:pixBuffAttributes];
            
            [self.playerItem addOutput:self.output];
        }
    }
}

- (CVPixelBufferRef)copyPixelBuffer{
    CMTime currentTime = [self.playerItem currentTime];
    
    // It is very strange, but worked well!
    [self.output hasNewPixelBufferForItemTime:currentTime];
    
    CVPixelBufferRef pixelBuffer = [self.output copyPixelBufferForItemTime:currentTime itemTimeForDisplay:nil];
    
    if (pixelBuffer != nil)
    {
        lastBufferTime = currentTime;
    }
    else{
        
    }
    return pixelBuffer;
}
@end
