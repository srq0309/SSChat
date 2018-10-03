#include <iostream>
#include <thread>

#include "ssim_server_interface.h"

int main(int argc, char* argv[])
{
    //auto network = ssim::create_network_interface();
    //network->init();
    //network->run();

    //ssim::destory_network_interface(network);

    ssim::SSN_header h;

    printf("%x\t%x\t%x\t%x\n", &h.magic_, &h.length_, &h.type_, &h.version_);

    system("pause");
    return 0;
}
