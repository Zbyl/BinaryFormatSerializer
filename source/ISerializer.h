/////////////////////////////////////////////////////////////////////////////
/// BinaryFormatSerializer
///    Library for serializing data in arbitrary binary format.
///
/// ISerializer.h
///
/// This file contains ISerializer interface that represents a source or destination for serialization.
///
/// Distributed under Apache License, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0)
/// (c) 2014 Zbigniew Skowron, zbychs@gmail.com
///
/////////////////////////////////////////////////////////////////////////////

#ifndef BinaryFormatSerializer_ISerializer_H
#define BinaryFormatSerializer_ISerializer_H

#include <boost/cstdint.hpp>

namespace binary_format
{

class ISerializer
{
public:
    template<typename Formatter, typename T>
    void save(const T& object, Formatter& formatter = Formatter())
    {
        formatter.save(*this, object);
    }

    template<typename Formatter, typename T>
    void load(T& object, Formatter& formatter = Formatter())
    {
        formatter.load(*this, object);
    }

    template<typename Formatter, typename T>
    void serialize(T& object, Formatter& formatter = Formatter())
    {
        if (saving())
        {
            formatter.save(*this, object);
        }
        else
        {
            formatter.load(*this, object);
        }
    }

    bool loading() { return !saving(); }

    virtual bool saving() = 0;

public:
    void saveData(const boost::uint8_t* data, size_t size)
    {
        serializeData(const_cast<boost::uint8_t*>(data), size);
    }

    void loadData(boost::uint8_t* data, size_t size)
    {
        serializeData(data, size);
    }

    virtual void serializeData(boost::uint8_t* data, size_t size) = 0;
};

} // namespace binary_format

#endif // BinaryFormatSerializer_ISerializer_H
