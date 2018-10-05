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
#include <sqlite3.h>
#include <shared_mutex>
#include <map>

namespace ssim
{
    class msg_persistent :
        public msg_persistent_interface
    {
    public:
        virtual int check_user_passwd(const char *user, const char *passwd) override;
        virtual int change_passwd_x4(const char *user, const char *newpasswd, const char *oldpasswd) override;
        virtual int change_passwd_x5(const char *user, const char *newpasswd, const char *question, const char *answer) override;
        virtual int regist_user(const char * user, const char * passwd, const char * question, const char * answer) override;
        virtual void persist_msg(const char * user, std::shared_ptr<std::vector<uint8_t>> p_data) override;
        // 返回一条结果，对于同一个用户的数据不可重入
        virtual std::shared_ptr<std::vector<uint8_t>> get_user_msg(const char * user) override;

        msg_persistent();
        ~msg_persistent();

    private:
        sqlite3 *db_;

        // 用于保存未关闭的sqlite3 stmt对象
        std::map<std::string, sqlite3_stmt *> stmts_;
        std::shared_mutex stmts_mu_;
        sqlite3_stmt *get_user_stmt(const char *user);
        void destory_user_stmt_from_stmts(const char *user);
    };
}


#endif	// MSG_PERSISTENT_H_