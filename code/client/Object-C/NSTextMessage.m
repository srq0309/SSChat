//
//  NSMessage.m
//  Test
//
//  Created by 阿杰 on 2018/4/23.
//  Copyright © 2018年 阿杰. All rights reserved.
//

#import "NSTextMessage.h"
static int self_Variable_Length = 72;

@implementation NSTextMessage
-(NSData *)getMessageData{
    NSData * data = [super getMessageData];
    return data;
}
-(NSString *)toString{
    NSString * string  = [super toString];
    return string;
}
- (instancetype)init
{
    self = [super init];
    if (self) {
        self.subType = 0x00000002;
    }
    return self;
}
-(instancetype)initWithMessage:(NSMessage *)message{
    if (message.subType == 0x00000002) {
        self = [super initWithMessage:message];
        if (self) {
            [self initSelfDataWithData:msgData];
        }
    }else{
        self = [super init];
        if (self) {
            self.subType = 0x00000002;
        }
    }
    return self;
}

-(void)setMsgLen:(int32_t)msgLen{
    _msgLen =  msgLen;
    [self reSetMsgData];
}

-(void)setMsgType:(int32_t)msgType{
    _msgType =  msgType;
    [self reSetMsgData];
}

-(void)setMsgBuff:(NSData *)msgBuff{
    _msgBuff = msgBuff;
    [self reSetMsgData];
}

- (void)setSrcUserName:(NSString *)srcUserName{
    if (!srcUserName) {
        _srcUserName = nil;
        return;
    }
    NSData * data = [srcUserName dataUsingEncoding:NSUTF8StringEncoding];
    _srcUserName = [[NSString alloc]initWithData:[NSData dataWithBytes:data.bytes length:32] encoding:NSUTF8StringEncoding];
    [self reSetMsgData];
}



-(void)setDisUserName:(NSString *)disUserName{
    if (!disUserName) {
        _disUserName = nil;
        return;
    }
    NSData * data = [disUserName dataUsingEncoding:NSUTF8StringEncoding];
    _disUserName = [[NSString alloc]initWithData:[NSData dataWithBytes:data.bytes length:64] encoding:NSUTF8StringEncoding];
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
    NSData * srcUserNameData = [tmpData subdataWithRange:NSMakeRange(0, 32)];
    NSData * disUserNameData = [tmpData subdataWithRange:NSMakeRange(32, 32)];
    NSData * msgTypeData = [tmpData subdataWithRange:NSMakeRange(64, 4)];
    NSData * msgLenData = [tmpData subdataWithRange:NSMakeRange(68, 4)];
    _srcUserName = [[NSString alloc]initWithData:srcUserNameData encoding:NSUTF8StringEncoding];
    _disUserName = [[NSString alloc]initWithData:disUserNameData encoding:NSUTF8StringEncoding];
    _msgType = [NSDataUtil getInt32ByData:msgTypeData];
    _msgLen = [NSDataUtil getInt32ByData:msgLenData];
    msgData = [tmpData subdataWithRange:NSMakeRange(self_Variable_Length, tmpData.length - self_Variable_Length)];
}
-(void)reSetMsgData{
    [super reSetMsgData];
    NSMutableData * data = [NSMutableData dataWithData:msgData];
    
    if (_srcUserName) {
        [data appendData:[_srcUserName dataUsingEncoding:NSUTF8StringEncoding]];
    }else{
        [data appendBytes:[@"NULL" dataUsingEncoding:NSUTF8StringEncoding].bytes length:32];
    }
    if (_disUserName) {
        [data appendData:[_disUserName dataUsingEncoding:NSUTF8StringEncoding]];
    }else{
        [data appendBytes:[@"NULL" dataUsingEncoding:NSUTF8StringEncoding].bytes length:64];
    }
    
    [data appendBytes:&_msgType length:4];
    [data appendBytes:&_msgLen length:4];
    [data appendData:_msgBuff];
    msgData = data;
}
@end
