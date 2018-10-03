#ifndef NETWORK_H_
#define NETWORK_H_

/*!
 * @file	network.h
 *
 * @author	skiller
 * @date	2018-10-01
 *
 * @brief	定义实际的网络接口层
 */

#include <map>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <memory>
#include <utility>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

#include "ssim_server_interface.h"
#include "session.h"

using asio::ip::tcp;

namespace ssim
{
    class network :
        public network_interface
    {

    public:
        network();

        virtual void init(uint16_t port = 9301, int therad_num = 1) override;

        virtual void run() override;

        // 压入数据至数据接收队列
        void push_msg_recv_queue(session_data data);

        // 取数据发送队列的数据
        session_data pop_msg_send_queue();

        // 压入数据持久队列数据
        void push_msg_persistent_queue(std::shared_ptr<std::vector<uint8_t>> p_data);

    private:
        // 开始接收事件循环
        void do_accept();

        // 启动session_data发送循环
        void start_send_data();

        std::atomic<bool> start_send_;

        std::shared_ptr<asio::io_context> ioc_;
        std::shared_ptr<tcp::acceptor> acc_;
        std::shared_ptr<tcp::socket> socket_;

        uint64_t create_session_id();
        std::atomic<uint64_t> session_id_;

        std::map<int64_t, std::weak_ptr<session>> sess_;

        // 数据分发层接口
        std::shared_ptr<msg_route_interface> msg_route_;
    };
}

#endif	// NETWORK_H_
