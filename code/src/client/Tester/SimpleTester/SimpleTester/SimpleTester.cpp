// SimpleTester.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>

#define ASIO_STANDALONE
#include <asio.hpp>
#include "ssimdef.h"

using namespace ssim;

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

int main()
{
    asio::io_context ioc;
    asio::ip::tcp::socket sock(ioc);
    sock.connect(asio::ip::tcp::endpoint(asio::ip::address::from_string("127.0.0.1"), 9301));

    auto p_data = std::make_shared<std::vector<uint8_t>>();
    send_ssim(sock, p_data);

    return 0;
}