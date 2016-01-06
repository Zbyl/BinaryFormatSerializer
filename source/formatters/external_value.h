/////////////////////////////////////////////////////////////////////////////
/// BinaryFormatSerializer
///    Library for serializing data in arbitrary binary format.
///
/// external_value.h
///
/// This file contains external_value that holds a reference to a value. It checks if values match on save, and gets the value on load.
///
/// Distributed under Apache License, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0)
/// (c) 2014 Zbigniew Skowron, zbychs@gmail.com
///
/////////////////////////////////////////////////////////////////////////////

#ifndef BinaryFormatSerializer_external_value_H
#define BinaryFormatSerializer_external_value_H

#include "serialization_exceptions.h"

namespace binary_format
{

template<typename T>
class external_value
{
    T& ext_value;
public:
    explicit external_value(T& ext_value)
        : ext_value(ext_value)
    {
    }

    template<typename TSerializer, typename U>
    void save(TSerializer& serializer, const U& value) const
    {
        if (value != ext_value)
        {
            BOOST_THROW_EXCEPTION(serialization_exception());
        }
    }

    template<typename TSerializer, typename U>
    void load(TSerializer& serializer, U& value) const
    {
        value = ext_value;
    }
};

template<typename T>
external_value<T> create_external_value(T& ext_value)
{
    return external_value<T>(ext_value);
}

} // namespace binary_format

#endif // BinaryFormatSerializer_external_value_H
