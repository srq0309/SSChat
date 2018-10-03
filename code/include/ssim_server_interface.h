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

    // 网络交互层接口
    class network_interface
    {
    public:
        // 初始化端口和工作线程数，建议单线程，此模块只涉及网络io操作
        virtual void init(uint16_t port = 9301, int therad_num = 1) = 0;

        // 开始服务
        virtual void run() = 0;

    };
    SSIM_API network_interface *create_network_interface();
    SSIM_API void destory_network_interface(network_interface *network);

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
        virtual void pop_msg_recv_queue() = 0;

        virtual void push_msg_send_queue() = 0;
        virtual session_data pop_msg_send_queue() = 0;

        virtual void push_msg_persistent_queue(std::shared_ptr<std::vector<uint8_t>> p_data) = 0;
        virtual void pop_msg_persistent_queue() = 0;

        virtual void insert_session() = 0;
        virtual void remove_session() = 0;
        virtual void is_active_session() = 0;
    };

    // 数据处理层接口
    class msg_process_interface
    {

    };

    // 数据持久层接口
    class msg_persistent_interface
    {

    };

}

#endif	// SSIM_SERVER_INTERFACE_H_
