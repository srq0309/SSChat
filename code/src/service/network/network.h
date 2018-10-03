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

class session;
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

    private:
        // 开始接收事件循环
        void do_accept();

        std::shared_ptr<asio::io_context> ioc_;
        std::shared_ptr<tcp::acceptor> acc_;
        std::shared_ptr<tcp::socket> socket_;

        uint64_t create_session_id();
        std::atomic<uint64_t> session_id_;

        std::map<int64_t, std::shared_ptr<session>> sess_;
    };
}

#endif	// NETWORK_H_
