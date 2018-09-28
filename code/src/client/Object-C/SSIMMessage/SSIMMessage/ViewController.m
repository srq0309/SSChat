//
//  ViewController.m
//  Test
//
//  Created by 阿杰 on 2018/3/23.
//  Copyright © 2018年 阿杰. All rights reserved.
//
#import "NSMessage.h"
#import "NSInstantMessage.h"
#import "NSLoginMessage.h"
#import "NSMessageFactory.h"
#import <SSNSDK/SSNSDK.h>
#import "NSUser.h"
#import "ViewController.h"
#import <AVKit/AVKit.h>
@interface ViewController ()<NSSocketDelegate>{
    NSSocketManager * manager;
    BOOL canSendMessage;
}
@property (weak, nonatomic) IBOutlet UIView *bgview;
@property (weak, nonatomic) IBOutlet UITextField *usernameTx;
@property (weak, nonatomic) IBOutlet UITextField *passwordTx;
@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    manager = [NSSocketManager manager];
    manager.delegate = self;
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


- (void)onSocketCanSendMessage {
    canSendMessage = YES;
}

- (void)onSocketCloseConnect {
    NSLog(@"connectClose");
}

-(void)Login{
    NSLoginMessage * message = [[NSLoginMessage alloc]init];
    message.userName = _usernameTx.text;
    message.passWard = _passwordTx.text;
    [manager sendMessage:message.getMessageData];
}

- (void)onSocketConnectWithResult:(BOOL)isSuccess {
    if (isSuccess) {
        NSLog(@"connectSuccess");
        [self Login];
    }else{
        NSLog(@"connectFaile");
        UIAlertController * alc = [UIAlertController alertControllerWithTitle:@"提示" message:@"连接超时" preferredStyle:UIAlertControllerStyleAlert];
        [self presentViewController:alc animated:YES completion:nil];
    }
}
- (IBAction)sendMessage:(id)sender {
   
    NSInstantMessage * text = [[NSInstantMessage alloc]init];
    text.srcUserName = @"abc";
    text.disUserName = @"abc";
    text.msgType = NSINSTANTMESSAGE_TYPE_IMAGE;
    UIImage *iv = [UIImage imageNamed:@"Image"];
    text.msgBuff = [NSDataUtil createDataByPNG:iv];
    [manager openLog:YES];
    [manager sendMessage:text.getMessageData];
}

- (void)onSocketGetMessage:(NSData *)messages {
    NSLog(@"%@",messages);
    NSMessageFactory * factory = [[NSMessageFactory alloc]init];
    [factory registMessageWithClass:NSLoginMessage.class andSubType:NSMESSAGE_TYPE_LOGIN];
    [factory registMessageWithClass:NSInstantMessage.class andSubType:NSMESSAGE_TYPE_INSTANT];
    NSMessage * message = [factory createMessageByData:messages];
    NSLog(@"%@",message.toString);
    if (message.subType == NSMESSAGE_TYPE_LOGIN) {
        UIAlertController * alc = [UIAlertController alertControllerWithTitle:@"提示" message:@"登录成功" preferredStyle:UIAlertControllerStyleAlert];
        [self presentViewController:alc animated:YES completion:nil];
    }
}

- (IBAction)connect:(id)sender {
    [manager connectToHost:@"121.69.81.74" andPort:6677];
}

@end
