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

#include "serialization_exceptions.h"

#include <boost/cstdint.hpp>

namespace binary_format
{

class ISerializer
{
public:
    /// @brief Saves given object using specified formatter.
    ///        Will throw serialization_exception if serializer is a loading serializer.
    template<typename Formatter, typename T>
    void save(const T& object, Formatter& formatter = Formatter())
    {
        if (!saving())
        {
            BOOST_THROW_EXCEPTION(serialization_exception() << detail::errinfo_description("Can't save to a loading serializer."));
        }

        formatter.save(*this, object);
    }

    /// @brief Loads given object using specified formatter.
    ///        Will throw serialization_exception if serializer is a saving serializer.
    template<typename Formatter, typename T>
    void load(T& object, Formatter& formatter = Formatter())
    {
        if (!loading())
        {
            BOOST_THROW_EXCEPTION(serialization_exception() << detail::errinfo_description("Can't load from a saving serializer."));
        }

        formatter.load(*this, object);
    }

    /// @brief Serializes given object using specified formatter.
    ///        Calls save() or load() depending on whether serializer if a saving or loading serializer.
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

    /// @brief Returs true if serializer is a loading serializer.
    bool loading() { return !saving(); }

    /// @brief Returs true if serializer is a saving serializer.
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

    /// @brief Serializes a buffer of bytes.
    virtual void serializeData(boost::uint8_t* data, size_t size) = 0;
};

} // namespace binary_format

#endif // BinaryFormatSerializer_ISerializer_H
