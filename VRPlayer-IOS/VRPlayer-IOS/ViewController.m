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
//     url = @"http://vr.donglivr.net/qjdl/qiyiguo/12_05_15_comikazi_v01/mp4_fhd.mp4";
    url = @"http://vr.donglivr.net/qjdl/qiyiguo/StarWars360/mp4_fhd.mp4";
//    url = @"http://qjdlplay.bravovcloud.com.cn/live/test.m3u8";
//    url = @"http://vr.donglivr.net/qjdl/qjdl_new/vrbox/vid/3dmovie/movie_2016070801d/mp4_fhd.m3u8";
//    url = @"http://vr.donglivr.net/qjdl/qjdl_new/vrbox/vid/3dmovie/movie_2016071402/mp4_fhd.mp4";
    
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
