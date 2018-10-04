#include "msg_persistent.h"

using namespace ssim;

bool ssim::msg_persistent::check_user_passwd(const char * user, const char * passwd)
{
    return false;
}

bool ssim::msg_persistent::change_passwd_x4(const char * user, const char * newpasswd, const char * oldpasswd)
{
    return false;
}

bool ssim::msg_persistent::change_passwd_x5(const char * user, const char * newpasswd, const char * question, const char * answer)
{
    return false;
}

int ssim::msg_persistent::regist_user(const char * user, const char * passwd, const char * question, const char * answer)
{
    return 0;
}

void ssim::msg_persistent::persist_msg(const char * user_id, std::shared_ptr<std::vector<uint8_t>> p_data)
{
}

std::shared_ptr<std::vector<uint8_t>> ssim::msg_persistent::get_user_msg(const char * user)
{
    return std::shared_ptr<std::vector<uint8_t>>();
}


ssim::msg_persistent::msg_persistent()
{

}

ssim::msg_persistent::~msg_persistent()
{
    
}

SSIM_API std::shared_ptr<msg_persistent_interface> ssim::create_msg_persistent_interface()
{
    return std::shared_ptr<msg_persistent>();
}
