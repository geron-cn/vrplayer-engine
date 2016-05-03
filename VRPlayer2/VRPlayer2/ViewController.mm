//
//  ViewController.m
//  testliveframeworkios
//
//  Created by gero on 4/26/16.
//  Copyright © 2016 gero. All rights reserved.
//

#import "ViewController.h"
#import "vr/VRPlayer.h"

//#include "../VRLiveFramework.framework/Headers/VRPlayer.h"

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];

    VRPlayer* player = new VRPlayer();
    UIView* view1 = (__bridge UIView*)player->initialize();
    [self.view addSubview:view1];
    // Do any additional setup after loading the view, typically from a nib.
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
