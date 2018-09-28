//
//  NSMessage.m
//  Test
//
//  Created by 阿杰 on 2018/4/23.
//  Copyright © 2018年 阿杰. All rights reserved.
//

#import "NSInstantMessage.h"
static int self_Variable_Length = 72;

@implementation NSInstantMessage
-(NSString *)toString{
    return [[super toString] stringByAppendingString:[NSString stringWithFormat:@"srcUserName:%@,disUserName:%@,msgLen:%d,msgType:%d,msgBuff:%@",_srcUserName,_disUserName,_msgLen,_msgType,[self resolveMsgBuff]]];
}

- (instancetype)init
{
    self = [super init];
    if (self) {
        self.subType = NSMESSAGE_TYPE_INSTANT;
    }
    return self;
}
-(instancetype)initWithMessage:(NSMessage *)message{
    if (message.subType == NSMESSAGE_TYPE_INSTANT) {
        self = [super initWithMessage:message];
    }else{
        self = [self init];
    }
    return self;
}
-(instancetype)initWithData:(NSData *)data{
    NSMessage * message = [[NSMessage alloc]initWithData:data];
    if (message.subType == NSMESSAGE_TYPE_INSTANT) {
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
    NSData * srcUserNameData = [tmpData subdataWithRange:NSMakeRange(0, 32)];
    NSData * disUserNameData = [tmpData subdataWithRange:NSMakeRange(32, 32)];
    NSData * msgTypeData = [tmpData subdataWithRange:NSMakeRange(64, 4)];
    NSData * msgLenData = [tmpData subdataWithRange:NSMakeRange(68, 4)];
    _srcUserName = [NSDataUtil getStringByData:srcUserNameData];
    _disUserName = [NSDataUtil getStringByData:disUserNameData];
    _msgType = [NSDataUtil getInt32ByData:msgTypeData];
    _msgLen = [NSDataUtil getInt32ByData:msgLenData];
    _overflowMsgData = [tmpData subdataWithRange:NSMakeRange(self_Variable_Length, tmpData.length - self_Variable_Length)];
    if (_overflowMsgData.length >= _msgLen) {
        _msgBuff = [_overflowMsgData subdataWithRange:NSMakeRange(0, _msgLen)];
        _overflowMsgData = [_overflowMsgData subdataWithRange:NSMakeRange(_msgLen, _overflowMsgData.length - _msgLen)];
    }else{
        NSMutableData * tmpMsgBuff = [NSMutableData dataWithData:_overflowMsgData];
        Byte * bytes = alloca(sizeof(Byte)*(_msgLen - _overflowMsgData.length));
        for (int i = 0; i < _msgLen - _overflowMsgData.length; i++) {
            bytes[i] = 0;
        }
        [tmpMsgBuff appendBytes:bytes length:_msgLen - _overflowMsgData.length];
        _msgBuff = [NSData dataWithData:tmpMsgBuff];
        _overflowMsgData = [NSData data];
    }
}

-(void)setMsgBuff:(NSData *)msgBuff{
    if (msgBuff) {
        _msgBuff = msgBuff;
    }else{
        _msgBuff = [NSData data];
    }
    _msgLen = (int32_t)_msgBuff.length;
}

-(void)reSetMsgData{
    [super reSetMsgData];
    NSMutableData * data = [NSMutableData dataWithData:_msgData];
    [data appendData:[NSDataUtil createDataByString:_srcUserName andLength:32]];
    [data appendData:[NSDataUtil createDataByString:_disUserName andLength:32]];
    [data appendBytes:&_msgType length:4];
    [data appendBytes:&_msgLen length:4];
    [data appendData:_msgBuff];
    _msgData = data;
}

-(id)resolveMsgBuff{
    id sender = _msgBuff;
    switch (_msgType) {
        case NSINSTANTMESSAGE_TYPE_TEXT:
            sender = [NSDataUtil getStringByData:_msgBuff];
            break;
        case NSINSTANTMESSAGE_TYPE_IMAGE:
            sender = [UIImage imageWithData:_msgBuff];
            break;
        default:
            break;
    }
    return sender;
}
@end
