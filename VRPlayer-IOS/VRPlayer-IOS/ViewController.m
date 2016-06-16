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
//    url = @"http://qjdlplay.bravovcloud.com.cn/live/test.m3u8";
//    url = @"http://vr.donglivr.net/qjdl/vrbox/vid/test.mp4";
    
    QGVideoPlayViewController *vc = [[QGVideoPlayViewController alloc] initWithNibName:@"QGVideoPlayViewController" bundle:nil url:[NSURL URLWithString:url] videoTitle:@"test"];
    
    [self presentViewController:vc animated:YES completion:nil];
    
//    TestViewController *vc = [[TestViewController alloc] initWithNibName:@"TestViewController" bundle:nil];
//    vc.view.frame = self.view.bounds;
//    NSLog(@"vc.view.frame===%@",NSStringFromCGRect(vc.view.frame));
//    vc.view.backgroundColor = [UIColor blueColor];
//    [self.view addSubview:vc.view];
//    [self addChildViewController:vc];
//    [vc didMoveToParentViewController:self];
    
    
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
