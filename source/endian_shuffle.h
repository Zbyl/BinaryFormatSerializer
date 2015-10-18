/////////////////////////////////////////////////////////////////////////////
/// BinaryFormatSerializer
///    Library for serializing data in arbitrary binary format.
///
/// endian_shuffle.h
///
/// This file contains functions for converting data between little and big endian formats.
/// NOTE: this will become obsolete once boost::endian is available
/// NOTE: this code assumes a little endian machine
/// NOTE: performance of this code has not been analyzed
///
/// Distributed under Apache License, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0)
/// (c) 2014 Zbigniew Skowron, zbychs@gmail.com
///
/////////////////////////////////////////////////////////////////////////////

#ifndef BinaryFormatSerializer_endian_shuffle_H
#define BinaryFormatSerializer_endian_shuffle_H

#include <algorithm>
#include <type_traits>

namespace binary_format
{

namespace detail
{

/// @brief This function reverses the order of bytes in a value.
///        Value must be a pod.
///        This function is compatible with strict aliasing rules.
template<typename T>
void endian_shuffle(T& value)
{
    static_assert(std::is_pod<T>::value, "Type must be a pod.");
    char* valueBytes = reinterpret_cast<char*>(&value);
    std::reverse(valueBytes, valueBytes + sizeof(T));
}

template<int N>
struct is_power_of_two {
    enum { value = (N > 0) && !(N & (N - 1)) };
};

} // namespace detail

template<typename T>
T endian_shuffle(const T& value)
{
    T shuffled = value;
    detail::endian_shuffle(shuffled);
    return shuffled;
}

template<typename T>
T native_to_little_endian(const T& value)
{
    return value;
}

template<typename T>
T native_to_big_endian(const T& value)
{
    return endian_shuffle(value);
}

template<typename T>
T little_endian_to_native(const T& value)
{
    return native_to_little_endian(value);
}

template<typename T>
T big_endian_to_native(const T& value)
{
    return native_to_big_endian(value);
}

struct little_endian_tag;
struct big_endian_tag;

namespace detail
{
    template<typename endian_tag>
    struct native_to_helper
    {
        template<typename T>
        T endian_shuffle(const T& value)
        {
            return native_to_little_endian(value);
        }
    };

    template<>
    struct native_to_helper<big_endian_tag>
    {
        template<typename T>
        T endian_shuffle(const T& value)
        {
            return native_to_big_endian(value);
        }
    };
} // namespace detail

template<typename endian_tag, typename T>
T native_to(const T& value)
{
    return detail::native_to_helper<endian_tag>().endian_shuffle(value);
}

} // namespace binary_format

#endif // BinaryFormatSerializer_endian_shuffle_H
