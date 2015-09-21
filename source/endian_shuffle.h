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

#include "uint_of_size.h"

#include <boost/static_assert.hpp>
#include <boost/type_traits/is_pod.hpp>

namespace binary_format
{

namespace detail
{
    inline uint_of_size<1>::type endian_shuffle(const uint_of_size<1>::type& value)
    {
        return value;
    }

    template<int Size>
    typename uint_of_size<Size>::type endian_shuffle(const typename uint_of_size<Size>::type& value)
    {
        BOOST_STATIC_ASSERT(Size % 2 == 0);

        typename uint_of_size<Size>::type result;

        typename uint_of_size<Size / 2>::type* value_half_ptr = reinterpret_cast<typename uint_of_size<Size / 2>::type*>(&value);
        typename uint_of_size<Size / 2>::type* result_half_ptr = reinterpret_cast<typename uint_of_size<Size / 2>::type*>(&result);

        result[0] = endian_shuffle<Size / 2>(value[1]);
        result[1] = endian_shuffle<Size / 2>(value[0]);

        return result;
    }

    template<int N>
    struct is_power_of_two {
        enum { value = (N > 1) & !(N & (N - 1)) };
    };

} // namespace detail

template<typename T>
T endian_shuffle(const T& value)
{
    BOOST_STATIC_ASSERT(boost::is_pod<T>::value);
    BOOST_STATIC_ASSERT(detail::is_power_of_two< sizeof(T) >::value);
    typename uint_of_size<sizeof(T)>::type result = detail::endian_shuffle(reinterpret_cast<const typename uint_of_size<sizeof(T)>::type&>(value));
    return reinterpret_cast<const T&>(result);
}

template<typename T>
T native_to_little_endian(const T& value)
{
    BOOST_STATIC_ASSERT(boost::is_pod<T>::value);
    //BOOST_STATIC_ASSERT(detail::is_power_of_two< sizeof(T) >::value);
    return value;
}

template<typename T>
T native_to_big_endian(const T& value)
{
    return endian_shuffle(reinterpret_cast<const typename uint_of_size<1>::type&>(value));
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
