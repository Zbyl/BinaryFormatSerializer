/////////////////////////////////////////////////////////////////////////////
/// BinaryFormatSerializer
///    Library for serializing data in arbitrary binary format.
///
/// uint_of_size.h
///
/// This file contains uint_of_size template providing unsigned ints of desired size.
///
/// Distributed under Apache License, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0)
/// (c) 2014 Zbigniew Skowron, zbychs@gmail.com
///
/////////////////////////////////////////////////////////////////////////////

#ifndef BinaryFormatSerializer_uint_of_size_H
#define BinaryFormatSerializer_uint_of_size_H

#include <boost/cstdint.hpp>

namespace binary_format
{

template<int Size>
struct uint_of_size;

template<>
struct uint_of_size<1>
{
    typedef boost::uint8_t type;
};

template<>
struct uint_of_size<2>
{
    typedef boost::uint16_t type;
};

template<>
struct uint_of_size<4>
{
    typedef boost::uint32_t type;
};

template<>
struct uint_of_size<8>
{
    typedef boost::uint64_t type;
};

} // namespace binary_format

#endif // BinaryFormatSerializer_uint_of_size_H
