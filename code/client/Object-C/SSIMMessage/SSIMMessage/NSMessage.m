//
//  NSMessage.m
//  Test
//
//  Created by 阿杰 on 2018/4/23.
//  Copyright © 2018年 阿杰. All rights reserved.
//

#import "NSMessage.h"
#import "NSError.h"
static int self_Variable_Length = 16;

@implementation NSMessage

-(void)setMessageData:(NSData *)data{
    if (data.length <self_Variable_Length) {
        return;
    }
    NSData * subTypeData = [data subdataWithRange:NSMakeRange(0, 4)];
    NSData * msgErrData = [data subdataWithRange:NSMakeRange(4, 4)];
    NSData * srcUserRefidData = [data subdataWithRange:NSMakeRange(8, 4)];
    NSData * disUserRefidData = [data subdataWithRange:NSMakeRange(12, 4)];
    _subType = [NSDataUtil getInt32ByData:subTypeData];
    _msgError = [NSDataUtil getInt32ByData:msgErrData];
    _srcUserRefid = [NSDataUtil getInt32ByData:srcUserRefidData];
    _disUserRefid = [NSDataUtil getInt32ByData:disUserRefidData];
    msgData = [data subdataWithRange:NSMakeRange(self_Variable_Length, data.length - self_Variable_Length)];
}

-(NSData *)getMessageData{
    NSMutableData * data = [NSMutableData data];
    [data appendData:[NSDataUtil createDataByInt32:_subType]];
    [data appendData:[NSDataUtil createDataByInt32:_msgError]];
    [data appendData:[NSDataUtil createDataByInt32:_srcUserRefid]];
    [data appendData:[NSDataUtil createDataByInt32:_disUserRefid]];
    [data appendData:msgData];
    return data;
}
-(NSString *)toString{
    return [NSString stringWithFormat:@"Message ={subtype:%d,err:%d,src:%d,dst:%d,msgData:%@",self.subType,self.msgError,self.srcUserRefid,self.disUserRefid,msgData];
}

- (instancetype)init
{
    self = [super init];
    if (self) {
        self.subType = 0;
        self.msgError = ERROR_MESSAGE_OTHER;
        self.srcUserRefid = 0;
        self.disUserRefid = 0;
        msgData = [NSData data];
    }
    return self;
}
- (instancetype)initWithMessage:(NSMessage *)message{
    if (message) {
        self = [[NSMessage alloc]initWithData:[message getMessageData]];
    }else{
        self = [[NSMessage alloc]init];
    }
    return self;
}

-(instancetype)initWithData:(NSData *)data{
    self = [super init];
    if (self) {
        [self setMessageData:data];
    }
    return self;
}
- (void)reSetMsgData{
    msgData = [NSMutableData data];
}


@end
