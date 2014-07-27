/////////////////////////////////////////////////////////////////////////////
/// BinaryFormatSerializer
///    Library for serializing data in arbitrary binary format.
///
/// SizeCountingSerializer.h
///
/// This file contains SizeCountingSerializer that count number of bytes that would be produced by the serialization.
///
/// Distributed under Apache License, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0)
/// (c) 2014 Zbigniew Skowron, zbychs@gmail.com
///
/////////////////////////////////////////////////////////////////////////////

#ifndef BinaryFormatSerializer_SizeCountingSerializer_H
#define BinaryFormatSerializer_SizeCountingSerializer_H

#include "ISerializer.h"

#include <boost/cstdint.hpp>

namespace binary_format
{

class SizeCountingSerializer : public ISerializer
{
    boost::uintmax_t byteCount;
public:
    SizeCountingSerializer()
        : byteCount(0)
    {
    }

    /// @brief Returns number of bytes that has been stored to this serializer so far.
    boost::uintmax_t getByteCount()
    {
        return byteCount;
    }

    virtual bool saving()
    {
        return true;
    }

public:
    virtual void serializeData(boost::uint8_t* data, size_t size)
    {
        byteCount += size;
    }
};

} // namespace binary_format

#endif // BinaryFormatSerializer_SizeCountingSerializer_H
