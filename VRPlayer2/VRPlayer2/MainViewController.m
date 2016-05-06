//
//  MainViewController.m
//  VRPlayer2
//
//  Created by yangxiao on 16/5/5.
//  Copyright © 2016年 yangxiao. All rights reserved.
//

#import "MainViewController.h"
#import "PlayViewController.h"


@interface MainViewController ()

@end

@implementation MainViewController 


- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


- (IBAction)btnClicked:(id)sender {
    UIStoryboard *storyBoard = [UIStoryboard storyboardWithName:@"Main" bundle:nil];
    PlayViewController *vc = [storyBoard instantiateViewControllerWithIdentifier:@"PlayViewController"];
    [self presentViewController:vc animated:YES completion:^{
        
    }];
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
