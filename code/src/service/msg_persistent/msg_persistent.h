#ifndef MSG_PERSISTENT_H_
#define MSG_PERSISTENT_H_

/*!
 * @file	msg_persistent.h
 *
 * @author	梦丿无情
 * @date	2018-10-04
 *
 * @brief	数据持久层
 */

#include "ssim_server_interface.h"

namespace ssim
{
    class msg_persistent :
        public msg_persistent_interface
    {
        virtual bool check_user_passwd(const char * user, const char * passwd) override;
        virtual bool change_passwd_x4(const char * user, const char * newpasswd, const char * oldpasswd) override;
        virtual bool change_passwd_x5(const char * user, const char * newpasswd, const char * question, const char * answer) override;
        virtual int regist_user(const char * user, const char * passwd, const char * question, const char * answer) override;
        virtual void persist_msg(const char * user_id, std::shared_ptr<std::vector<uint8_t>> p_data) override;
        virtual std::shared_ptr<std::vector<uint8_t>> get_user_msg(const char * user) override;

        msg_persistent();
        ~msg_persistent();
    };
}


#endif	// MSG_PERSISTENT_H_