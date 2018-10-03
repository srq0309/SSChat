#include "msg_route.h"

using namespace ssim;

void ssim::msg_route::push_msg_recv_queue(session_data data)
{
    msg_recv_queue.push(std::make_shared<ssim::session_data>(data));
}

session_data ssim::msg_route::pop_msg_recv_queue()
{
    session_data ret;
    for (;;) {
        auto data = msg_recv_queue.pop();
        if (data && data->p_data_) {
            ret = *data;
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    return ret;
}

void ssim::msg_route::push_msg_send_queue(session_data data)
{
    msg_send_queue.push(std::make_shared<ssim::session_data>(data));
}

session_data ssim::msg_route::pop_msg_send_queue()
{
    session_data ret;
    for (;;) {
        auto data = msg_send_queue.pop();
        if (data && data->p_data_) {
            ret = *data;
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    return ret;
}

void ssim::msg_route::push_msg_persistent_queue(std::shared_ptr<std::vector<uint8_t>> p_data)
{
    msg_persistent_queue.push(p_data);
}

std::shared_ptr<std::vector<uint8_t>> ssim::msg_route::pop_msg_persistent_queue()
{
    std::shared_ptr<std::vector<uint8_t>> ret;
    for (;;) {
        auto data = msg_persistent_queue.pop();
        if (data) {
            ret = data;
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    return ret;
}
