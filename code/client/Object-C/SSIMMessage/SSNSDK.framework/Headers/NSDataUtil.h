//
//  NSDataUtil.h
//  Test
//
//  Created by 阿杰 on 2018/4/4.
//  Copyright © 2018年 阿杰. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
@interface NSDataUtil : NSObject
+(NSData *)createDataByString:(NSString*)string;
+(NSData *)createDataByPNG:(UIImage*)image;
+(NSData *)createDataByJPG:(UIImage*)image;
+(NSData *)createDataByFile:(NSString*)filePath;
+(NSData *)createDataByInt32:(int32_t)number;
+(int32_t)getInt32ByData:(NSData *)data;
@end
