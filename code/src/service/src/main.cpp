#include <iostream>
#include <thread>

#include "ssim_server_interface.h"

int main(int argc, char* argv[])
{
    auto network = ssim::create_network_interface();
    network->init();
    network->run();

    ssim::destory_network_interface(network);

    return 0;
}
