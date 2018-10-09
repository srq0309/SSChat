#include <iostream>
#include <thread>

#include "ssim_server_interface.h"

int main(int argc, char* argv[])
{
    auto msg_net = ssim::create_network_interface();
    auto msg_route = ssim::create_msg_route_interface();
    auto msg_process = ssim::create_msg_process_interface();
    auto msg_persistent = ssim::create_msg_persistent_interface();

    msg_net->init(msg_route);
    msg_process->init(msg_route, msg_persistent, 4);
   
    msg_process->run();
    msg_net->run();

    return 0;
}
