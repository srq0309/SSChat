#include "msg_process.h"

using namespace ssim;

void ssim::msg_process::init(std::shared_ptr<msg_route_interface> msg_route,
    std::shared_ptr<msg_persistent_interface> msg_persistent,
    int thread_num)
{
}

void msg_process::run()
{
}

SSIM_API std::shared_ptr<msg_process_interface> ssim::create_msg_process_interface()
{
    return std::shared_ptr<msg_process>();
}