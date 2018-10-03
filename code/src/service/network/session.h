#ifndef SESSION_H_
#define SESSION_H_

/*!
 * @file	session.h
 *
 * @author	skiller
 * @date	2018-10-01
 *
 * @brief	基于asio的会话组件
 */

#include <cstdlib>
#include <iostream>
#include <vector>
#include <memory>
#include <utility>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

#include "ssim_server_interface.h"

using asio::ip::tcp;

namespace ssim
{
    class network;

    class session
        : public std::enable_shared_from_this<session>
    {
    public:
        session(uint64_t session_id, tcp::socket socket, network *msg_net);

        void start();

    private:
        // 读ssn头部
        void do_read_ssn_header();

        // 读ssim正文
        void do_read_ssim(size_t length);

        uint64_t session_id_;

        tcp::socket socket_;
        std::vector<uint8_t> ssn_header_;

        network * msg_net_;

        // 终止代码
        int end_code_;
        // 终止信息
        std::string end_msg_;
    };
}

#endif  // SESSION_H_