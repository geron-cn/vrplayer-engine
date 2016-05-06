//
//  PlayViewController.m
//  VRPlayer2
//
//  Created by yangxiao on 16/5/5.
//  Copyright © 2016年 yangxiao. All rights reserved.
//

#import "PlayViewController.h"
#import "vr/VRPlayerIOS.h"


@interface PlayViewController ()

@end

@implementation PlayViewController
{
    VRPlayerIOS *player;
    IBOutlet UIButton *btn;
}

-(void)dealloc {
    
}

- (void)viewDidLoad {
    [super viewDidLoad];
    player = [[VRPlayerIOS alloc] init];
    UIViewController* vc = player.viewController;
    [player play:@"http://gotye-live-10022.ufile.ucloud.com.cn/08fc0f4a-5120-43dd-98f2-424660dd263c.mp4?k=52d00265544a4faf&t=1461064771"];
    [self addChildViewController:vc];
    [self.view addSubview:vc.view];
    vc.view.backgroundColor = [UIColor redColor];
//    vc.view.frame = CGRectMake(100, 100, 200, 200);
    [vc didMoveToParentViewController:self];
    
    [self.view bringSubviewToFront:btn];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (IBAction)btnClicked:(id)sender {
    [self dismissViewControllerAnimated:YES completion:nil];
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

@end
