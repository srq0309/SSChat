/*!
 * @file	network.h
 *
 * @author	skiller
 * @date	2018-10-01
 *
 * @brief	定义实际的网络接口层
 */

#include "ssim_server_interface.h"

namespace ssim
{
    class Network :
        public NetworkInterface
    {

    public:
        virtual void Init(uint16_t port = 9301, int therad_num = 1) override;


        virtual void Run() override;

    };
}
