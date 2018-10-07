#ifndef PACKAGE_HPP_
#define PACKAGE_HPP_

/*!
 * @file	package.hpp
 *
 * @author	skiller
 * @date	2018-10-02
 *
 * @brief	二进制打包解包工具
 */

#include <cstdint>
#include <vector>
#include <cassert>
#include <string>

 // 解包数据
template<typename _Ty>
void unpack(std::vector<uint8_t>& data, size_t dis, _Ty& val)
{
    assert(dis + sizeof(val) <= data.size());
    memcpy(&val, &data[dis], sizeof(val));
}

// 解包数据
template<typename _Ty>
void unpack(uint8_t *data, size_t len, size_t dis, _Ty& val)
{
    assert(dis + sizeof(val) <= len);
    memcpy(&val, data + dis, sizeof(val));
}
// 解包字符
std::string unpack_string(std::vector<uint8_t>& data, size_t dis, size_t len)
{
    assert(dis + len <= data.size());
    std::string str;
    for (int i = dis; i != dis + len; ++i) {
        if (static_cast<char>(data[i]) == '\0')
            break;
        str.push_back(data[i]);
    }
    return std::move(str);
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

#endif	// PACKAGE_HPP_