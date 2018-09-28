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
    return [[super toString] stringByAppendingString:[NSString stringWithFormat:@",userName:%@,passWard:%@",self.userName,self.passWard]];
}
- (instancetype)init
{
    self = [super init];
    if (self) {
        self.subType = NSMESSAGE_TYPE_LOGIN;
    }
    return self;
}
-(instancetype)initWithMessage:(NSMessage *)message{
    if (message.subType == NSMESSAGE_TYPE_LOGIN) {
        self = [super initWithMessage:message];
    }else{
        self = [self init];
    }
    return self;
}

- (instancetype)initWithData:(NSData *)data{
    NSMessage * message = [[NSMessage alloc]initWithData:data];
    if (message.subType == NSMESSAGE_TYPE_LOGIN) {
        self = [super initWithData:data];
    }else{
        self = [self init];
    }
    return self;
}

- (void)setMessageData:(NSData *)data{
    [super setMessageData:data];
    NSData * tmpData = [NSData dataWithData:_overflowMsgData];
    if (tmpData.length < self_Variable_Length) {
        return;
    }
    NSData * userNameData = [tmpData subdataWithRange:NSMakeRange(0, 32)];
    NSData * passWardData = [tmpData subdataWithRange:NSMakeRange(32, 64)];
    _userName = [NSDataUtil getStringByData:userNameData];
    _passWard = [NSDataUtil getStringByData:passWardData];
    _overflowMsgData = [tmpData subdataWithRange:NSMakeRange(self_Variable_Length, tmpData.length - self_Variable_Length)];
}



-(void)reSetMsgData{
    [super reSetMsgData];
    NSMutableData * data = [NSMutableData dataWithData:_msgData];
    [data appendData:[NSDataUtil createDataByString:_userName andLength:32]];

    [data appendData:[NSDataUtil createDataByString:_passWard andLength:64]];
    _msgData = data;
}
@end
