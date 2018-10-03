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

 // 解包数据
template<typename _Ty>
void unpack(std::vector<uint8_t>& data, size_t dis, _Ty& val)
{
    assert(dis + sizeof(val) < data.size());
    memcpy(&val, &data[dis], sizeof(val));
}

// 解包数据
template<typename _Ty>
void unpack(uint8_t *data, size_t len, size_t dis, _Ty& val)
{
    assert(dis + sizeof(val) < len);
    memcpy(&val, data + dis, sizeof(val));
}

#endif	// PACKAGE_HPP_