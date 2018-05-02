//
//  NSMessageFactory.m
//  Test
//
//  Created by 阿杰 on 2018/4/28.
//  Copyright © 2018年 阿杰. All rights reserved.
//

#import "NSMessageFactory.h"
@interface NSMessageFactory()
@property (nonatomic,strong) NSMutableDictionary * classDic;
@end
@implementation NSMessageFactory

-(void)registMessageWithClass:(Class)classType andSubType:(int32_t)subType{
    if (!_classDic) {
        _classDic = [NSMutableDictionary dictionary];
    }
    if ([classType isKindOfClass:NSMessage.class]) {
        [_classDic setObject:classType forKey:[NSNumber numberWithInteger:subType]];
    }
}

- (NSMessage *)createMessageByData:(NSData *)data{
    NSMessage * message = [[NSMessage alloc]initWithData:data];
    for (NSNumber * num in _classDic.allKeys) {
        if ([num intValue] == message.subType) {
            Class classType = [_classDic objectForKey:num];
            message = [[classType alloc]initWithMessage:message];
            break;
        }
    }
    return message;
}
@end
