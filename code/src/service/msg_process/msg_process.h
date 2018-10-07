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

#include <map>
#include <string>
#include <shared_mutex>

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

        // 持久化存储线程
        void do_persistent();
        std::thread t_persistent_;

        // 工作线程数
        int thread_num_;
        // 工作线程
        void do_work();
        std::thread t_work_;

        // 解析登陆
        std::shared_ptr<std::vector<uint8_t>> analysis_login(SSIM_header& header, std::vector<uint8_t>& data, uint64_t sessid);
        // 解析即时消息
        std::shared_ptr<std::vector<uint8_t>> analysis_msg(SSIM_header& header, std::vector<uint8_t>& data, uint64_t sessid);
        // 解析注册
        std::shared_ptr<std::vector<uint8_t>> analysis_regist(SSIM_header& header, std::vector<uint8_t>& data, uint64_t sessid);
        // 解析修改密码0x04
        std::shared_ptr<std::vector<uint8_t>> analysis_passwd_x4(SSIM_header& header, std::vector<uint8_t>& data, uint64_t sessid);
        // 解析修改密码0x05
        std::shared_ptr<std::vector<uint8_t>> analysis_passwd_x5(SSIM_header& header, std::vector<uint8_t>& data, uint64_t sessid);
        // 解析注销登陆
        std::shared_ptr<std::vector<uint8_t>> analysis_logout(SSIM_header& header, std::vector<uint8_t>& data, uint64_t sessid);

        // 解析不支持协议
        std::shared_ptr<std::vector<uint8_t>> analysis_no_support(SSIM_header& header, std::vector<uint8_t>& data, uint64_t sessid);

        // 创建反馈消息
        std::shared_ptr<std::vector<uint8_t>> create_feedback(uint32_t subtype, uint32_t ss_err, uint64_t refid);

        // 创建一个refid
        uint64_t create_refid();

        // 解析目标用户名
        std::string unpack_dis_user(std::vector<uint8_t>& data);

        // 在线用户表
        struct active_user
        {
            uint64_t sessid_;
            uint64_t refid_;
        };
        std::map<std::string, active_user> users_;
        std::shared_mutex users_mu_;

        // 添加在线用户
        void insert_active_user(const std::string& user, active_user user_map);
        // 移除在线用户
        void remove_active_user(const std::string& user);
        // 查询在线用户
        active_user get_user_map(const std::string& user);
    };

}
#endif	// MSG_PROCESS_H_