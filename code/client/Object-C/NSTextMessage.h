//
//  NSMessage.h
//  Test
//
//  Created by 阿杰 on 2018/4/23.
//  Copyright © 2018年 阿杰. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "NSMessage.h"
@interface NSTextMessage : NSMessage
@property (nonatomic,copy)NSString * srcUserName;
@property (nonatomic,copy)NSString * disUserName;
@property (nonatomic,assign)int32_t msgType;
@property (nonatomic,assign)int32_t msgLen;
@property (nonatomic,assign)NSData* msgBuff;
@end
