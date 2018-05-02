//
//  NSMessage.h
//  Test
//
//  Created by 阿杰 on 2018/4/23.
//  Copyright © 2018年 阿杰. All rights reserved.
//
#import <Foundation/Foundation.h>
#import "NSDataUtil.h"
@interface NSMessage : NSObject{
    NSData * msgData;
}
@property (nonatomic,assign) int32_t subType;
@property (nonatomic,assign) int32_t msgError;
@property (nonatomic,assign) int32_t srcUserRefid;
@property (nonatomic,assign) int32_t disUserRefid;
-(instancetype)initWithMessage:(NSMessage *)message;
-(instancetype)initWithData:(NSData *)data;
-(NSData *)getMessageData;
-(void)setMessageData:(NSData *)data;
-(void)reSetMsgData;
-(NSString *)toString;
@end
