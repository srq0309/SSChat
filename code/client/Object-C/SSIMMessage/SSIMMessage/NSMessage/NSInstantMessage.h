//
//  NSMessage.h
//  Test
//
//  Created by 阿杰 on 2018/4/23.
//  Copyright © 2018年 阿杰. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "NSMessage.h"
#define NSINSTANTMESSAGE_TYPE_TEXT 1
#define NSINSTANTMESSAGE_TYPE_IMAGE 2
@interface NSInstantMessage : NSMessage
@property (nonatomic,copy)NSString * srcUserName;
@property (nonatomic,copy)NSString * disUserName;
@property (nonatomic,assign)int32_t msgType;
@property (nonatomic,assign,readonly)int32_t msgLen;
@property (nonatomic,strong) NSData* msgBuff;

-(id)resolveMsgBuff;
@end
