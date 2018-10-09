#include "msg_persistent.h"

#ifdef WIN32
# define DB_LOCAL "d:\\_Tmp\\SSChat.db"
#else
# define DB_LOCAL "/var/SSChat/SSChat.db"
#endif

using namespace ssim;

int ssim::msg_persistent::check_user_passwd(const char *user, const char *passwd)
{
    static const char *sql_struct = R"(select password from ssim_user where username = '%s')";
    char sql[512];
    snprintf(sql, sizeof(sql), sql_struct, user);

    sqlite3_stmt *stmt;
    const char *tail;
    auto ret = sqlite3_prepare_v2(db_, sql, -1, &stmt, &tail);

    if (ret != SQLITE_OK) {
        sqlite3_finalize(stmt);
        return 3;   // 未知错误
    }

    if (SQLITE_ROW != sqlite3_step(stmt)) {
        sqlite3_finalize(stmt);
        return 1;   // 用户名不存在
    }

    auto res = sqlite3_column_text(stmt, 0);
    if (0 != strcmp(passwd, (const char *)res)) {
        sqlite3_finalize(stmt);
        return 2;   // 密码错误
    }

    sqlite3_finalize(stmt);
    return 0;
}

int ssim::msg_persistent::change_passwd_x4(const char *user, const char *newpasswd, const char *oldpasswd)
{
    static const char *sql_struct_1 = R"(select password from ssim_user where username = '%s')";
    static const char *sql_struct_2 = R"(update ssim_user set password = '%s' where username = '%s')";
    char sql_1[512], sql_2[512];
    snprintf(sql_1, sizeof(sql_1), sql_struct_1, user);
    snprintf(sql_2, sizeof(sql_2), sql_struct_2, newpasswd, user);

    sqlite3_stmt *stmt;
    const char *tail;
    auto ret = sqlite3_prepare_v2(db_, sql_1, -1, &stmt, &tail);

    if (ret != SQLITE_OK) {
        sqlite3_finalize(stmt);
        // TODO: log
        return 3;   // 未知错误
    }

    if (SQLITE_ROW != sqlite3_step(stmt)) {
        sqlite3_finalize(stmt);
        return 1;   // 用户名不存在
    }

    auto res = sqlite3_column_text(stmt, 0);
    if (0 != strcmp(oldpasswd, (const char *)res)) {
        sqlite3_finalize(stmt);
        return 2;   // 密码错误
    }

    sqlite3_finalize(stmt);

    char *error;
    ret = sqlite3_exec(db_, sql_2, nullptr, nullptr, &error);
    if (ret != SQLITE_OK) {
        // TODO: log
        return 3;
    }

    return ret;
}

int ssim::msg_persistent::change_passwd_x5(const char *user, const char *newpasswd, const char *question, const char *answer)
{
    static const char *sql_struct_1 = R"(select question, answer from ssim_user where username = '%s')";
    static const char *sql_struct_2 = R"(update ssim_user set password = '%s' where username = '%s')";
    char sql_1[512], sql_2[512];
    snprintf(sql_1, sizeof(sql_1), sql_struct_1, user);
    snprintf(sql_2, sizeof(sql_2), sql_struct_2, newpasswd, user);

    sqlite3_stmt *stmt;
    const char *tail;
    auto ret = sqlite3_prepare_v2(db_, sql_1, -1, &stmt, &tail);

    if (ret != SQLITE_OK) {
        sqlite3_finalize(stmt);
        // TODO: log
        return 3;   // 未知错误
    }

    if (SQLITE_ROW != sqlite3_step(stmt)) {
        sqlite3_finalize(stmt);
        return 1;   // 用户名不存在
    }

    auto res0 = sqlite3_column_text(stmt, 0);
    auto res1 = sqlite3_column_text(stmt, 1);
    if (0 != strcmp(question, (const char *)res0) ||
        0 != strcmp(answer, (const char *)res1)) {
        sqlite3_finalize(stmt);
        return 2;   // 验证问题失败
    }

    sqlite3_finalize(stmt);

    char *error;
    ret = sqlite3_exec(db_, sql_2, nullptr, nullptr, &error);
    if (ret != SQLITE_OK) {
        // TODO: log
        return 3;
    }

    return ret;
}

int ssim::msg_persistent::regist_user(const char * user, const char * passwd, const char * question, const char * answer)
{
    static const char *sql_struct = R"(insert into ssim_user values('%s','%s','%s','%s'))";
    char sql[4096];
    snprintf(sql, sizeof(sql), sql_struct, user, passwd, question, answer);

    char *error;
    auto ret = sqlite3_exec(db_, sql, nullptr, nullptr, &error);
    if (ret == SQLITE_OK) {
        return 0;
    }
    if (ret == SQLITE_CONSTRAINT) {
        return 1;   // 用户名存在（这里用户名重复是唯一可能导致约束条件冲突的原因）
    }

    // TODO: log
    return 2;   // 未知错误
}

void ssim::msg_persistent::persist_msg(const char * user, std::shared_ptr<std::vector<uint8_t>> p_data)
{
    static const char *sql_struct = R"(insert into ssim_msg values('%s', ?))";
    char sql[512];
    snprintf(sql, sizeof(sql), sql_struct, user);

    sqlite3_stmt *stmt;
    if (SQLITE_OK != sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr)) {
        sqlite3_finalize(stmt);
        // TODO: log
        return;
    }

    sqlite3_bind_blob(stmt, 1, &(*p_data)[0], p_data->size(), nullptr);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

std::shared_ptr<std::vector<uint8_t>> ssim::msg_persistent::get_user_msg(const char * user)
{
    auto stmt = get_user_stmt(user);
    if (!stmt) {
        return nullptr;
    }

    if (SQLITE_DONE == sqlite3_step(stmt)) {
        erase_user_msg(user);
        destory_user_stmt_from_stmts(user);
        sqlite3_finalize(stmt);
        return nullptr;
    }
    auto pData = reinterpret_cast<const uint8_t *>(sqlite3_column_blob(stmt, 0));
    auto nSize = static_cast<size_t>(sqlite3_column_bytes(stmt, 0));
    auto ret = std::make_shared<std::vector<uint8_t>>(pData, pData + nSize);

    return ret;
}

void ssim::msg_persistent::erase_user_msg(const char * user)
{
    static const char *sql_struct = R"(delete from ssim_msg where username = '%s')";
    char sql[512];
    snprintf(sql, sizeof(sql), sql_struct, user);

    sqlite3_stmt *stmt;
    if (SQLITE_OK != sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr)) {
        sqlite3_finalize(stmt);
        // TODO: log
        return;
    }
    if (SQLITE_DONE != sqlite3_step(stmt)) {
        // TODO: log
    }
}


ssim::msg_persistent::msg_persistent()
{
    auto ret = sqlite3_open(DB_LOCAL, &db_);
    if (ret) {
        throw std::exception(std::string("opensqlite3_open: " + std::string(sqlite3_errmsg(db_))).c_str());
    }
}

ssim::msg_persistent::~msg_persistent()
{
    sqlite3_close(db_);
}

sqlite3_stmt * ssim::msg_persistent::get_user_stmt(const char * user)
{
    {
        std::shared_lock<std::shared_mutex> lk(stmts_mu_);
        auto iter = stmts_.find(user);
        if (iter != stmts_.end()) {
            return iter->second;
        }
    }

    static const char *sql_struct = R"(select message from ssim_msg where username = '%s')";
    char sql[512];
    snprintf(sql, sizeof(sql), sql_struct, user);

    sqlite3_stmt *stmt;
    const char *tail;
    auto ret = sqlite3_prepare_v2(db_, sql, -1, &stmt, &tail);

    if (ret != SQLITE_OK) {
        sqlite3_finalize(stmt);
        // TODO: log
        return nullptr;
    }
    {
        std::lock_guard<std::shared_mutex> lk(stmts_mu_);
        stmts_.insert({ user, stmt });
    }
    return stmt;
}

void ssim::msg_persistent::destory_user_stmt_from_stmts(const char * user)
{
    std::lock_guard<std::shared_mutex> lk(stmts_mu_);
    stmts_.erase(user);
}

SSIM_API std::shared_ptr<msg_persistent_interface> ssim::create_msg_persistent_interface()
{
    return std::make_shared<msg_persistent>();
}
