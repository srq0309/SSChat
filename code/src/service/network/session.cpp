#include "session.h"
#include "network.h"
#include "package.hpp"

ssim::session::session(uint64_t session_id, tcp::socket socket, network *msg_net) :
    session_id_(session_id),
    socket_(std::move(socket)),
    msg_net_(msg_net),
    ssn_header_(8)
{
}

void ssim::session::start()
{
    do_read_ssn_header();
}

void ssim::session::do_read_ssn_header()
{
    auto self(shared_from_this());

    asio::async_read(socket_, asio::buffer(ssn_header_),
        [this, self](std::error_code ec, std::size_t length)
    {
        assert(length == ssn_header_len);
        if (!ec) {
            size_t nPtr = 0;
            SSN_header ssn_header;

            // magic number
            unpack(ssn_header_, nPtr, ssn_header.magic_);
            nPtr += sizeof(ssn_header.magic_);
            ssn_header.magic_ = ntohl(ssn_header.magic_);

            if (ssn_header.magic_ != 0x133ED55) {   // 【终止会话】1
                end_code_ = 1;
                end_msg_ = "magic number error";
                return;
            }

            // length
            unpack(ssn_header_, nPtr, ssn_header.length_);
            nPtr += sizeof(ssn_header.length_);
            ssn_header.length_ = ntohs(ssn_header.length_);
            auto left_length = ssn_header.length_ - ssn_header_len;
            if (left_length < 0) {   // 【终止会话】2
                end_code_ = 2;
                end_msg_ = "ssn total length exception";
                return;
            }

            // type
            unpack(ssn_header_, nPtr, ssn_header.type_);
            nPtr += sizeof(ssn_header.type_);

            if (ssn_header.type_ != 0x80) {
                end_code_ = 3;
                end_msg_ = "protocol not suppert";
                return;
            }

            // version
            unpack(ssn_header_, nPtr, ssn_header.version_);
            nPtr += sizeof(ssn_header.version_);
            switch (ssn_header.version_) {
            case 0x00:
                do_read_ssim(left_length);
                break;
            default:    // 【终止会话】4
                end_code_ = 4;
                end_msg_ = "version not suppert";
                return;
            }
        } else {    // 【终止会话】

        }
    });
}

void ssim::session::do_read_ssim(size_t length)
{
    assert(length >= 0);
    auto self(shared_from_this());

    auto pData = std::make_shared<std::vector<uint8_t>>(length);
    asio::async_read(socket_, asio::buffer(*pData),
        [this, self, length](std::error_code ec, std::size_t read_length)
    {
        assert(read_length == length);
        if (!ec) {

        } else {

        }
    });
}
