/*!
 * @file	ssim_server_interface.h
 *
 * @author	skiller
 * @date	2018-10-01
 *
 * @brief	定义模块接口
 */

#include <cstdint>

#include <ssimdef.h>

namespace ssim
{

    // 网络交互层接口
    class NetworkInterface
    {
    public:
        // 初始化端口和工作线程数
        virtual void Init(uint16_t port = 9301, int therad_num = 1) = 0;

        // 开始服务
        virtual void Run() = 0;

    };

    // 数据分发层接口
    class MsgRouteInterface
    {
    public:
        virtual void PushMsgSendQueue() = 0;
        virtual void PopMsgSendQueue() = 0;

        virtual void PushMsgRecvQueue() = 0;
        virtual void PopMsgSendQueue() = 0;

        virtual void PushMsgPersistentQueue() = 0;
        virtual void PopMsgPersistentQueue() = 0;

        virtual void InsertSession() = 0;
        virtual void RemoveSession() = 0;
        virtual void IsActiveSession() = 0;
    };

    // 数据处理层接口
    class MsgProcessInterface
    {

    };

    // 数据持久层接口
    class MsgPersistentInterface
    {

    };

}