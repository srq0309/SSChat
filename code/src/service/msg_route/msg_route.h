#ifndef MSG_ROUTE_H_
#define MSG_ROUTE_H_

/*!
 * @file	msg_route.h
 *
 * @author	梦丿无情
 * @date	2018-10-03
 *
 * @brief	数据分发层
 */

#include <cstdint>
#include <vector>
#include <memory>

#include "ssimdef.h"
#include "ssim_server_interface.h"

#include "ts_queue.hpp"


namespace ssim
{
    class msg_route :
        public msg_route_interface
    {
    public:
        msg_route() = default;
        virtual void push_msg_recv_queue(session_data data) override;
        virtual session_data pop_msg_recv_queue() override;
        virtual void push_msg_send_queue(session_data data) override;
        virtual session_data pop_msg_send_queue() override;
        virtual void push_msg_persistent_queue(std::shared_ptr<std::vector<uint8_t>> p_data) override;
        virtual std::shared_ptr<std::vector<uint8_t>> pop_msg_persistent_queue() override;
    private:
        // 数据接收队列
        ts_queue<session_data> msg_recv_queue;
        // 数据发送队列
        ts_queue<session_data> msg_send_queue;
        // 数据持久化队列
        ts_queue<std::vector<uint8_t>> msg_persistent_queue;
    };
}

#endif	// MSG_ROUTE_H_