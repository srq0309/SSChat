//
//  NSMessageFactory.h
//  Test
//
//  Created by 阿杰 on 2018/4/28.
//  Copyright © 2018年 阿杰. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "NSMessage.h"
@interface NSMessageFactory : NSObject
-(void)registMessageWithClass:(Class)classType andSubType:(int32_t)subType;
-(NSMessage *)createMessageByData:(NSData *)data;
@end
