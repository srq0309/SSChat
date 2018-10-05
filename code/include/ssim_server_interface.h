#ifndef SSIM_SERVER_INTERFACE_H_
#define SSIM_SERVER_INTERFACE_H_

/*!
 * @file	ssim_server_interface.h
 *
 * @author	梦丿无情
 * @date	2018-10-01
 *
 * @brief	定义模块接口
 */

#include <cstdint>
#include <memory>
#include <vector>

#include <ssimdef.h>

#ifdef SSIM_EXPORTS
# define SSIM_API __declspec(dllexport)
#else
# define SSIM_API __declspec(dllimport)
#endif

namespace ssim
{

    // 会话关联数据
    struct session_data
    {
        uint64_t session_id_;
        std::shared_ptr<std::vector<uint8_t>> p_data_;
    };
    // 数据分发层接口
    class msg_route_interface
    {
    public:
        virtual void push_msg_recv_queue(session_data data) = 0;
        virtual session_data pop_msg_recv_queue() = 0;

        virtual void push_msg_send_queue(session_data data) = 0;
        virtual session_data pop_msg_send_queue() = 0;

        virtual void push_msg_persistent_queue(std::shared_ptr<std::vector<uint8_t>> p_data) = 0;
        virtual std::shared_ptr<std::vector<uint8_t>> pop_msg_persistent_queue() = 0;
    };
    SSIM_API std::shared_ptr<msg_route_interface> create_msg_route_interface();

    // 数据持久层接口
    class msg_persistent_interface
    {
    public:
        // 检查密码
        virtual int check_user_passwd(const char *user, const char *passwd) = 0;

        // 更改密码
        virtual int change_passwd_x4(const char *user, const char *newpasswd,
            const char *oldpasswd) = 0;
        // 更改密码
        virtual int change_passwd_x5(const char *user, const char *newpasswd,
            const char *question, const char *answer) = 0;


        // 注册一个用户
        virtual int regist_user(const char *user, const char *passwd,
            const char *question, const char *answer) = 0;

        // 持久化离线消息，用户名为目标用户名
        virtual void persist_msg(const char *user,
            std::shared_ptr<std::vector<uint8_t>> p_data) = 0;

        // 获取某个用户的一条离线数据
        virtual std::shared_ptr<std::vector<uint8_t>> get_user_msg(const char *user) = 0;
    };
    SSIM_API std::shared_ptr<msg_persistent_interface> create_msg_persistent_interface();

    // 网络交互层接口
    class network_interface
    {
        // 本模块涉及thread_num+1个线程，具体是asio工作线程（处理网络）和一个取待发送数据的线程（处理发送队列）
    public:
        // 初始化端口和工作线程数，建议单线程，此模块只涉及网络io操作
        virtual void init(std::shared_ptr<msg_route_interface> msg_route, uint16_t port = 9301, int therad_num = 1) = 0;

        // 开始服务
        virtual void run() = 0;

    };
    SSIM_API std::shared_ptr<network_interface> create_network_interface();

    // 数据处理层接口
    class msg_process_interface
    {
        // 本模块涉及thread_num+1个线程，具体是处理数据的工作线程（处理接收队列）和一个处理持久化队列的线程（处理持久化队列）
    public:
        // 初始化
        virtual void init(std::shared_ptr<msg_route_interface> msg_route,
            std::shared_ptr<msg_persistent_interface> msg_persistent,
            int thread_num) = 0;

        // 启动工作线程
        virtual void run() = 0;

    };
    SSIM_API std::shared_ptr<msg_process_interface> create_msg_process_interface();

}

#endif	// SSIM_SERVER_INTERFACE_H_
