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

- (void)onSocketConnectWithResult:(BOOL)isSuccess {
    if (isSuccess) {
        NSLog(@"connectSuccess");
    }else{
        NSLog(@"connectFaile");
    }
}
- (IBAction)sendMessage:(id)sender {
    NSMessage * message = [[NSMessage alloc]init];
    message.subType = -3;
    message.msgError = -6;
    message.srcUserRefid = -9;
    message.disUserRefid = -2345;
    NSLog(@"%@",message.toString);
    NSData * data =  [message getMessageData];
    //test
    NSLog(@"%@",[[NSMessage alloc]initWithMessage:message].toString);
    NSLog(@"%@",[[NSMessage alloc]initWithData:data].toString);
    NSLog(@"----–––-------------------------------------");
    message.subType = 0x000000002;
    NSInstantMessage * text = [[NSInstantMessage alloc]initWithMessage:message];
    text.srcUserName = @"abc";
    text.disUserName = @"abc";
    text.msgType = 1;
    text.msgBuff = [NSDataUtil createDataByString:@"我们好jlkasjdl实打实大师大法。 salad。     dasd 啊"];
    NSData * textData = [text getMessageData];
    NSLog(@"%@",text.toString);
    NSLog(@"%@",[[[NSInstantMessage alloc]initWithMessage:text] toString]);
    NSInstantMessage * message1 = [[NSInstantMessage alloc]initWithData:textData];
    NSLog(@"%@",[message1 toString]);
    NSLog(@"----–––-------------------------------------");
    NSLoginMessage * login = [[NSLoginMessage alloc]init];
    login.userName = @"wenjie";
    login.passWard = @"23456";
    NSData * loginData = [login getMessageData];
    NSLog(@"%@",login.toString);
    NSLog(@"%@",[[NSLoginMessage alloc]initWithMessage:login].toString);
    NSLog(@"%@",[[NSLoginMessage alloc]initWithData:loginData].toString);
    
    NSUser * user = [[NSUser alloc]init];
    user.userName = @"345";
    user.password = @"123";
    NSLoginMessage * loginmessage = [[NSLoginMessage alloc]init];
    loginmessage.userName = user.userName;
    loginmessage.passWard = user.password;
    [manager sendMessage:loginmessage.getMessageData];
}

- (void)onSocketGetMessage:(NSData *)messages {
    NSLog(@"%@",messages);
    NSMessageFactory * factory = [[NSMessageFactory alloc]init];
    [factory registMessageWithClass:NSLoginMessage.class andSubType:0x00000001];
    [factory registMessageWithClass:NSInstantMessage.class andSubType:0x00000002];
    NSMessage * message = [factory createMessageByData:messages];
    NSLog(@"%@",message.toString);
}

- (IBAction)connect:(id)sender {
    [manager connectToHost:@"121.69.81.74" andPort:80];
}

@end
