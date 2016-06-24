//
//  ViewController.m
//  VRPlayer-IOS
//
//  Created by yangxiao on 16/5/24.
//  Copyright © 2016年 yangxiao. All rights reserved.
//

#import "ViewController.h"
#import "VRPlayerViewController.h"

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
    UIStoryboard *storyboard = [UIStoryboard storyboardWithName:@"vrplayer" bundle:nil];
    MyPlayerViewController *vc = (MyPlayerViewController *)[storyboard instantiateViewControllerWithIdentifier:@"MyPlayerViewController"];
    
    [self presentViewController:vc animated:YES completion:^{
        [vc initWithURL:[NSURL URLWithString:url] ];
    }];
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
