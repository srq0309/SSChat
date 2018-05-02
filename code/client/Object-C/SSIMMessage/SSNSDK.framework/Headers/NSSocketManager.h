//
//  NSSocketManager.h
//  Test
//
//  Created by 阿杰 on 2018/4/2.
//  Copyright © 2018年 阿杰. All rights reserved.
//
@protocol NSSocketDelegate;

#import <Foundation/Foundation.h>
@interface NSSocketManager : NSObject<NSStreamDelegate>
@property (nonatomic, strong) NSInputStream *inputStream;//对应输入流
@property (nonatomic, strong) NSOutputStream *outputStream;//对应输出流
@property (nonatomic, weak) id<NSSocketDelegate> delegate;//对应输出流
@property (nonatomic, strong) NSMutableArray *chatMsgs;//聊天消息数组
+(NSSocketManager *)manager;
-(void)connectToHost:(NSString *)host andPort:(int)port;
-(void)sendMessage:(NSData *)msg;
-(void)openLog:(BOOL)isopen;
@end
@protocol NSSocketDelegate<NSObject>
-(void)onSocketConnectWithResult:(BOOL) isSuccess;
-(void)onSocketGetMessage:(NSData *)messages;
-(void)onSocketCanSendMessage;
-(void)onSocketCloseConnect;
@end
