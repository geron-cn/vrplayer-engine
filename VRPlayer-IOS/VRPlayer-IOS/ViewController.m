//
//  ViewController.m
//  VRPlayer-IOS
//
//  Created by yangxiao on 16/5/24.
//  Copyright © 2016年 yangxiao. All rights reserved.
//

#import "ViewController.h"
#import "VRPlayerViewController.h"
#import "QGVideoPlayViewController.h"
#import "TestViewController.h"

@interface MyPlayerViewController : VRPlayerViewController

@end

@implementation MyPlayerViewController
- (IBAction)close:(id)sender {
    [self close];
}
@end

@interface ViewController ()

@end

@implementation ViewController
{
    IBOutlet UITextView *textView;
}

- (IBAction)play:(id)sender {
    NSString* url = self.url.text;
    //url = @"http://qjdlplay.bravovcloud.com.cn/live/test.m3u8";
//     url = @"http://vr.donglivr.net/qjdl/qiyiguo/BEACH_MASTER_rough_CUT/mp4_hd.mp4";
    
    QGVideoPlayViewController *vc = [[QGVideoPlayViewController alloc] initWithNibName:@"QGVideoPlayViewController" bundle:nil url:[NSURL URLWithString:url] videoTitle:@"test"];
    
    [self presentViewController:vc animated:YES completion:nil];
    
}


- (IBAction)btn:(id)sender {
    [textView resignFirstResponder];
}

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
