#ifndef GP_NO_PLATFORM
#ifdef __APPLE__

#import <Foundation/Foundation.h>
#include "gameplay.h"

using namespace gameplay;

extern int __argc;
extern char** __argv;

/**
 * Main entry point.
 */
int main(int argc, char** argv)
{
    __argc = argc;
    __argv = argv;
    
    NSString *urlStr=[NSString stringWithFormat:@"http://apitest.tsminfo.com/api/v10/videomnt/homevideos/live/one"];
    NSURL *url=[NSURL URLWithString:urlStr];
    
    NSURLRequest *request=[NSURLRequest requestWithURL:url];
    NSError *error = nil;
    NSData *data = [NSURLConnection sendSynchronousRequest:request
                                               returningResponse:nil error:&error];
    if (data == nil) {
        NSLog(@"send request failed: %@", error);
        return 0;
    }

    NSDictionary *dict=[NSJSONSerialization JSONObjectWithData:data options:NSJSONReadingMutableLeaves error:nil];
    const char* liveurl = nullptr;
    if ([dict[@"status"] isEqual: @"200"])
    {
        NSDictionary* dict2 = dict[@"data"];
        NSString* url = dict2[@"videoUrl"];
        if (url)
        {
            liveurl = [url UTF8String];
        }
    }
    if (liveurl == nullptr)
    {
        NSLog(@"fail to find live url");
        return 0;
    }
    
    NSAutoreleasePool *p = [[NSAutoreleasePool alloc] init];
    Game* game = Game::getInstance();
    game->setLiveURL(liveurl);
    Platform* platform = Platform::create(game);
    GP_ASSERT(platform);
    int result = platform->enterMessagePump();
    delete platform;
    [p release];
    return result;
}

#endif
#endif
