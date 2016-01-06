/////////////////////////////////////////////////////////////////////////////
/// BinaryFormatSerializer
///    Library for serializing data in arbitrary binary format.
///
/// VectorSaveSerializer.h
///
/// This file contains VectorSaveSerializer that write to a vector.
///
/// Distributed under Apache License, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0)
/// (c) 2014 Zbigniew Skowron, zbychs@gmail.com
///
/////////////////////////////////////////////////////////////////////////////

#ifndef BinaryFormatSerializer_VectorSaveSerializer_H
#define BinaryFormatSerializer_VectorSaveSerializer_H

#include "ISerializer.h"

#include "serialization_exceptions.h"

#include <vector>

namespace binary_format
{

class VectorSaveSerializer : public SerializerMixin<VectorSaveSerializer>
{
    std::vector<boost::uint8_t> buffer;
    size_t pos;
public:
    VectorSaveSerializer()
        : pos(0)
    {
    }

    const std::vector<boost::uint8_t>& getData()
    {
        return buffer;
    }

    bool saving()
    {
        return true;
    }

    size_t position()
    {
        return pos;
    }

    void seek(size_t position)
    {
        if (position > buffer.size())
        {
            buffer.resize(position);
        }

        pos = position;
    }

public:
    void serializeData(boost::uint8_t* data, size_t size)
    {
        if (pos == buffer.size())
        {
            buffer.insert(buffer.end(), data, data + size);
            pos += size;
            return;
        }

        if (pos + size > buffer.size())
        {
            buffer.resize(pos + size);
        }

        std::copy(data, data + size, buffer.begin() + pos);
        pos += size;
    }
};

} // namespace binary_format

#endif // BinaryFormatSerializer_VectorSaveSerializer_H
