//
//  NSLoginMessage.m
//  Test
//
//  Created by 阿杰 on 2018/4/26.
//  Copyright © 2018年 阿杰. All rights reserved.
//

#import "NSLoginMessage.h"
static int self_Variable_Length = 96;

@implementation NSLoginMessage
-(NSString *)toString{
    return [[super toString] stringByAppendingString:@"userName:%@,passWard:%@"];
}
- (instancetype)init
{
    self = [super init];
    if (self) {
        self.subType = 0x00000001;
    }
    return self;
}
-(instancetype)initWithMessage:(NSMessage *)message{
    if (message.subType == 0x00000001) {
        self = [super initWithMessage:message];
        if (self) {
            [self initSelfDataWithData:msgData];
        }
    }else{
        self = [super init];
        if (self) {
            self.subType = 0x00000001;
        }
    }
    return self;
}

- (void)setUserName:(NSString *)userName{
    if (!userName) {
        _userName = nil;
        return;
    }
    NSData * data = [userName dataUsingEncoding:NSUTF8StringEncoding];
    _userName = [[NSString alloc]initWithData:[NSData dataWithBytes:data.bytes length:32] encoding:NSUTF8StringEncoding];
    [self reSetMsgData];
}

- (void)setMessageData:(NSData *)data{
    [super setMessageData:data];
    [self initSelfDataWithData:msgData];
}

-(void)initSelfDataWithData:(NSData *)data{
    NSData * tmpData = [NSData dataWithData:data];
    if (tmpData.length < self_Variable_Length) {
        return;
    }
    NSData * userNameData = [tmpData subdataWithRange:NSMakeRange(0, 32)];
    NSData * passWardData = [tmpData subdataWithRange:NSMakeRange(32, 64)];
    _userName = [[NSString alloc]initWithData:userNameData encoding:NSUTF8StringEncoding];
    _passWard = [[NSString alloc]initWithData:passWardData encoding:NSUTF8StringEncoding];
    msgData = [tmpData subdataWithRange:NSMakeRange(self_Variable_Length, tmpData.length - self_Variable_Length)];
}

-(void)setPassWard:(NSString *)passWard{
    if (!passWard) {
        _passWard = nil;
        return;
    }
    NSData * data = [passWard dataUsingEncoding:NSUTF8StringEncoding];
    _passWard = [[NSString alloc]initWithData:[NSData dataWithBytes:data.bytes length:64] encoding:NSUTF8StringEncoding];
    [self reSetMsgData];
}


-(void)reSetMsgData{
    [super reSetMsgData];
    NSMutableData * data = [NSMutableData dataWithData:msgData];
    if (_userName) {
        [data appendData:[_userName dataUsingEncoding:NSUTF8StringEncoding]];
    }else{
        [data appendBytes:[@"NULL" dataUsingEncoding:NSUTF8StringEncoding].bytes length:32];
    }
    if (_passWard) {
        [data appendData:[_passWard dataUsingEncoding:NSUTF8StringEncoding]];
    }else{
        [data appendBytes:[@"NULL" dataUsingEncoding:NSUTF8StringEncoding].bytes length:64];
    }
    msgData = data;
}
@end
