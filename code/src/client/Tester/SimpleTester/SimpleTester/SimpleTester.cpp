// SimpleTester.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

#define ASIO_STANDALONE
#include <asio.hpp>
#include "ssimdef.h"


using namespace ssim;
using namespace std;


// 解包数据
template<typename _Ty>
void unpack(std::vector<uint8_t>& data, size_t dis, _Ty& val)
{
    assert(dis + sizeof(val) <= data.size());
    memcpy(&val, &data[dis], sizeof(val));
}


// 打包数据
template<typename _Ty>
void pack(std::vector<uint8_t>& data, size_t dis, _Ty& val)
{
    if (data.size() < dis + sizeof(val)) {
        data.resize(dis + sizeof(val));
    }
    memcpy(&data[dis], &val, sizeof(val));
}

void send_ssim(asio::ip::tcp::socket& sock, std::shared_ptr<std::vector<uint8_t>> p_data)
{
    SSN_header ssn_header = {
        htonl(0x133ED55),
        htons(static_cast<uint16_t>(p_data->size() + ssn_header_len)),
        0x80,
        0x00
    };
    std::vector<uint8_t> data;
    pack(data, data.size(), ssn_header.magic_);
    pack(data, data.size(), ssn_header.length_);
    pack(data, data.size(), ssn_header.type_);
    pack(data, data.size(), ssn_header.version_);

    sock.send(asio::buffer(data, data.size()));
    sock.send(asio::buffer(*p_data));
}

SSN_header read_ssn_header(asio::ip::tcp::socket& sock)
{
    vector<uint8_t> ssn_header_(8);
    asio::read(sock, asio::buffer(ssn_header_));

    // 解包

    SSN_header ssn_header;
    size_t nPtr = 0;
    unpack(ssn_header_, nPtr, ssn_header.magic_);
    nPtr += sizeof(ssn_header.magic_);
    ssn_header.magic_ = ntohl(ssn_header.magic_);

    unpack(ssn_header_, nPtr, ssn_header.length_);
    nPtr += sizeof(ssn_header.length_);
    ssn_header.length_ = ntohs(ssn_header.length_);

    unpack(ssn_header_, nPtr, ssn_header.type_);
    nPtr += sizeof(ssn_header.type_);
    unpack(ssn_header_, nPtr, ssn_header.version_);
    nPtr += sizeof(ssn_header.version_);

    return ssn_header;
}

SSIM_header read_ssim_header(SSN_header &ssn_header, asio::ip::tcp::socket& sock)
{
    vector<uint8_t> left_buff(ssn_header.length_ - 8);
    asio::read(sock, asio::buffer(left_buff));

    SSIM_header ssim_header;

    size_t nPtr = 0;

    unpack(left_buff, nPtr, ssim_header.subtype_);
    nPtr += sizeof(ssim_header.subtype_);
    ssim_header.subtype_ = ntohl(ssim_header.subtype_);

    unpack(left_buff, nPtr, ssim_header.errno_);
    nPtr += sizeof(ssim_header.errno_);
    ssim_header.errno_ = ntohl(ssim_header.errno_);

    unpack(left_buff, nPtr, ssim_header.refid_);
    nPtr += sizeof(ssim_header.refid_);
    ssim_header.refid_ = ntohll(ssim_header.refid_);
    return ssim_header;
}

uint64_t check_login(asio::ip::tcp::socket& sock)
{
    auto p_data = std::make_shared<std::vector<uint8_t>>();
    SSIM_header ssim_header = { htonl(1),0,0 };
    SSIM_bodyLogin ssim_login = { "u1","p1" };
    pack(*p_data, p_data->size(), ssim_header);
    pack(*p_data, p_data->size(), ssim_login);

    send_ssim(sock, p_data);

    auto ssn_header = read_ssn_header(sock);
    auto ssim_header2 = read_ssim_header(ssn_header, sock);

    return ssim_header2.refid_;
}

uint64_t check_login2(asio::ip::tcp::socket& sock)
{
    auto p_data = std::make_shared<std::vector<uint8_t>>();
    SSIM_header ssim_header = { htonl(1),0,0 };
    SSIM_bodyLogin ssim_login = { "u0","p0" };
    pack(*p_data, p_data->size(), ssim_header);
    pack(*p_data, p_data->size(), ssim_login);

    send_ssim(sock, p_data);

    auto ssn_header = read_ssn_header(sock);
    auto ssim_header2 = read_ssim_header(ssn_header, sock);

    auto ssn_header2 = read_ssn_header(sock);
    auto ssim_header22 = read_ssim_header(ssn_header2, sock);

    return ssim_header2.refid_;
}

uint32_t check_msg(asio::ip::tcp::socket& sock, uint64_t refid)
{
    auto p_data = std::make_shared<std::vector<uint8_t>>();
    SSIM_header ssim_header = { htonl(2),0, htonll(refid) };
    SSIM_bodyMessage ssim_msg = { "u1","u0", htonl(1), htonl(1), 'c' };
    pack(*p_data, p_data->size(), ssim_header);
    pack(*p_data, p_data->size(), ssim_msg);

    send_ssim(sock, p_data);

    auto ssn_header = read_ssn_header(sock);
    auto ssim_header2 = read_ssim_header(ssn_header, sock);

    return ssim_header2.errno_;
}

int main()
{
    asio::io_context ioc;
    asio::ip::tcp::socket sock(ioc);
    sock.connect(asio::ip::tcp::endpoint(asio::ip::address::from_string("127.0.0.1"), 9301));

    auto refid = check_login(sock);
    auto ret = check_msg(sock, refid);

    asio::io_context ioc2;
    asio::ip::tcp::socket sock2(ioc);
    sock2.connect(asio::ip::tcp::endpoint(asio::ip::address::from_string("127.0.0.1"), 9301));
    auto refid2 = check_login2(sock2);

    return 0;
}