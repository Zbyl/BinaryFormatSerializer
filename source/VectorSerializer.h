/////////////////////////////////////////////////////////////////////////////
/// BinaryFormatSerializer
///    Library for serializing data in arbitrary binary format.
///
/// VectorSerializer.h
///
/// This file contains VectorSaveSerializer and VectorLoadSerializer that write to / read from a vector.
///
/// Distributed under Apache License, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0)
/// (c) 2014 Zbigniew Skowron, zbychs@gmail.com
///
/////////////////////////////////////////////////////////////////////////////

#ifndef BinaryFormatSerializer_VectorSerializer_H
#define BinaryFormatSerializer_VectorSerializer_H

#include "ISerializer.h"

#include "serialization_exceptions.h"

#include <vector>

namespace binary_format
{

class VectorSaveSerializer : public ISerializer
{
    std::vector<boost::uint8_t> buffer;
public:
    const std::vector<boost::uint8_t>& getData()
    {
        return buffer;
    }

    virtual bool saving()
    {
        return true;
    }

public:
    virtual void serializeData(boost::uint8_t* data, size_t size)
    {
        buffer.insert(buffer.end(), data, data + size);
    }
};

class VectorLoadSerializer : public ISerializer
{
    const std::vector<boost::uint8_t>& buffer;
    size_t position;
public:
    VectorLoadSerializer(const std::vector<boost::uint8_t>& buffer)
        : buffer(buffer)
        , position(0)
    {
    }

    virtual bool saving()
    {
        return false;
    }

public:
    virtual void serializeData(boost::uint8_t* data, size_t size)
    {
        if (position + size > buffer.size())
        {
            BOOST_THROW_EXCEPTION(end_of_input() << detail::errinfo_requested_this_many_bytes_more(position + size - buffer.size()));
        }

        std::copy(buffer.begin() + position, buffer.begin() + position + size, data);
        position += size;
    }
};

} // namespace binary_format

#endif // BinaryFormatSerializer_VectorSerializer_H
