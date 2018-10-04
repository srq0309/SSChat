#ifndef MSG_PROCESS_H_
#define MSG_PROCESS_H_

/*!
 * @file	msg_process.h
 *
 * @author	skiller
 * @date	2018-10-03
 *
 * @brief	数据处理层
 */

#include "ssimdef.h"
#include "ssim_server_interface.h"

namespace ssim
{

    class msg_process :
        public msg_process_interface
    {
    public:
        virtual void init(std::shared_ptr<msg_route_interface> msg_route,
            std::shared_ptr<msg_persistent_interface> msg_persistent,
            int thread_num) override;

        virtual void run() override;

    private:
        // 数据分发层
        std::shared_ptr<msg_route_interface> msg_route_;
        // 数据持久层
        std::shared_ptr<msg_persistent_interface> msg_persistent_;
        // 工作线程数
        int thread_num_;
    };

}
#endif	// MSG_PROCESS_H_