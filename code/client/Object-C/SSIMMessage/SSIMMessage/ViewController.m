//
//  ViewController.m
//  Test
//
//  Created by 阿杰 on 2018/3/23.
//  Copyright © 2018年 阿杰. All rights reserved.
//
#import "NSMessage.h"
#import "NSTextMessage.h"
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
    NSLog(@"%@",message.toString);
    NSData * data =  [message getMessageData];
    [manager sendMessage:data];
    
    //test
    NSLog(@"%@",[[NSMessage alloc]initWithMessage:message].toString);
    NSLog(@"%@",[[NSMessage alloc]initWithData:data].toString);
    NSLog(@"----–––-------------------------------------");
    NSTextMessage * text = [[NSTextMessage alloc]init];
    text.srcUserName = @"abc";
    text.disUserName = @"abc";
    text.msgType = 1;
    text.msgLen = 5000;
    text.msgBuff = [@"我们好" dataUsingEncoding:NSUTF8StringEncoding];
    NSData * textData = [text getMessageData];
    NSLog(@"%@",text.toString);
    NSLog(@"%@",[[NSMessage alloc]initWithMessage:text].toString);
    NSLog(@"%@",[[NSMessage alloc]initWithData:textData].toString);
    NSLog(@"----–––-------------------------------------");
    NSLoginMessage * login = [[NSLoginMessage alloc]init];
    login.userName = @"wenjie";
    login.passWard = @"23456";
    NSData * loginData = [login getMessageData];
    NSLog(@"%@",login.toString);
    NSLog(@"%@",[[NSMessage alloc]initWithMessage:login].toString);
    NSLog(@"%@",[[NSMessage alloc]initWithData:loginData].toString);
    
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
    [factory registMessageWithClass:NSTextMessage.class andSubType:0x00000002];
    NSMessage * message = [factory createMessageByData:messages];
    NSLog(@"%@",message.toString);
}

- (IBAction)connect:(id)sender {
    [manager connectToHost:@"121.69.81.74" andPort:80];
}

@end
