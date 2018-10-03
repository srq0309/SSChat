#ifndef SSIMDEF_H_
#define SSIMDEF_H_

/*!
 * @file	ssimdef.h
 *
 * @author	skiller
 * @date	2018-09-28
 *
 * @brief	定义协议相关结构
 */

namespace ssim
{
    
    /*!
     * @brief	SSN头部
     */
    struct SSN_header
    {
        uint32_t magic_;         // 校验头
        uint16_t length_;        // 报文总长
        uint8_t type_;           // 消息类别号
        uint8_t version_;        // 消息版本号
    };
    constexpr size_t ssn_header_len = sizeof(SSN_header);

    /*!
     * @brief	SSIM头部
     */
    struct SSIM_header
    {
        uint32_t subtype_;       // 子类别
        uint32_t errno_;         // 错误码
        uint64_t refid_;         // 用户回话标识
    };

    /*!
     * @brief	SSIM登陆信息
     */
    struct SSIM_bodyLogin
    {
        char username_[16];     // 用户名
        char password_[32];     // 密码密文
    };

    /*!
     * @brief	SSIM消息
     */
    struct SSIM_bodyMessage
    {
        char src_user_[16];     // 原用户标识
        char dis_user_[16];     // 目标用户标识
        int32_t msg_type_;      // 消息类别 1-文本 2-图片
        int32_t msg_len_;       // 消息长度
        char binary_[1];        // 消息正文
    };

    /*!
     * @brief	注册消息
     */
    struct SSIM_bodyRegist
    {
        char username_[16];     // 用户名
        char password_[32];     // 密码密文
        char question_[512];    // 验证问题
        char answer_[512];      // 验证答案
    };

    /*!
     * @brief   修改密码（已知原密码）
     */
    struct SSIM_bodyRePassX4
    {
        char username_[16];     // 用户名
        char oldpass_[32];      // 原密码密文
        char newpass_[32];      // 新密码密文
    };

    /*!
     * @brief	修改密码（未知原密码）
     */
    struct SSIM_bodyRePassX5
    {
        char username_[16];     // 用户名
        char newpass_[32];      // 新密码密文
        char question_[512];    // 验证问题
        char answer_[512];      // 验证答案
    };

}

#endif	// SSIMDEF_H_
