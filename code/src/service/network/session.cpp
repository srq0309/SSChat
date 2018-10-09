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

ssim::session::~session()
{
    msg_net_->remove_sess(session_id_);
}

void ssim::session::start()
{
    msg_net_->insert_sess(session_id_, std::weak_ptr<session>(shared_from_this()));
    do_read_ssn_header();
}

void ssim::session::do_write_msg(std::shared_ptr<std::vector<uint8_t>> p_data)
{
    do_write_ssn_header(p_data);
}

void ssim::session::do_read_ssn_header()
{
    auto self(shared_from_this());

    asio::async_read(socket_, asio::buffer(ssn_header_),
        [this, self](std::error_code ec, std::size_t length)
    {
        if (!ec) {
            assert(length == ssn_header_len);
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
        } else {    // 【终止会话】5
            end_code_ = 5;
            end_msg_ = "normal error for do_read_ssn_header()";
            return;
        }
    });
}

void ssim::session::do_read_ssim(size_t length)
{
    assert(length >= 0);
    auto self(shared_from_this());

    if (0 == length) {
        // 继续接收数据
        do_read_ssn_header();
        return;
    }

    auto pData = std::make_shared<std::vector<uint8_t>>(length);
    asio::async_read(socket_, asio::buffer(*pData),
        [this, self, pData, length](std::error_code ec, std::size_t read_length)
    {
        if (!ec) {
            assert(read_length == length);
            // 压入数据分发层接收数据队列
            msg_net_->push_msg_recv_queue({ session_id_, pData });
            // 继续接收数据
            do_read_ssn_header();
        } else {    // 【终止会话】6
            end_code_ = 6;
            end_msg_ = "normal error for do_read_ssim()";
            return;
        }
    });
}

void ssim::session::do_write_ssn_header(std::shared_ptr<std::vector<uint8_t>> p_data)
{
    SSN_header ssn_header = {
        htonl(0x133ED55),
        htons(static_cast<uint16_t>(p_data->size() + ssn_header_len)),
        0x80,
        0x00
    };
    std::shared_ptr<std::vector<uint8_t>> p_data2 = std::make_shared<std::vector<uint8_t>>();
    std::vector<uint8_t>& data = *p_data2;
    pack(data, data.size(), ssn_header.magic_);
    pack(data, data.size(), ssn_header.length_);
    pack(data, data.size(), ssn_header.type_);
    pack(data, data.size(), ssn_header.version_);

    auto self(shared_from_this());
    asio::async_write(socket_, asio::buffer(data),
        [this, self, p_data, p_data2](std::error_code ec, std::size_t length)
    {
        if (!ec) {
            assert(length == ssn_header_len);
            do_write_ssim(p_data);
        } else {    // // 【终止会话】7
            end_code_ = 7;
            end_msg_ = "normal error for do_write_ssn_header()";
            msg_net_->push_msg_persistent_queue(p_data);
            socket_.close();
            return;
        } 
    });
}

void ssim::session::do_write_ssim(std::shared_ptr<std::vector<uint8_t>> p_data)
{
    auto self(shared_from_this());
    asio::async_write(socket_, asio::buffer(*p_data),
        [this, self, p_data](std::error_code ec, std::size_t length)
    {
        if (!ec) {
            assert(length == p_data->size());

        } else {    // 【终止会话】8
            end_code_ = 8;
            end_msg_ = "normal error for do_write_ssim()";
            msg_net_->push_msg_persistent_queue(p_data);
            socket_.close();
            return;
        }
    });
}
