/////////////////////////////////////////////////////////////////////////////
/// BinaryFormatSerializer
///    Library for serializing data in arbitrary binary format.
///
/// optional_formatter.h
///
/// This file contains optional_formatter that formats boost::optional as a flag field followed by the value.
///
/// Distributed under Apache License, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0)
/// (c) 2014 Zbigniew Skowron, zbychs@gmail.com
///
/////////////////////////////////////////////////////////////////////////////

#ifndef BinaryFormatSerializer_optional_formatter_H
#define BinaryFormatSerializer_optional_formatter_H

#include <boost/optional.hpp>

namespace binary_format
{

template<typename FlagFormatter, typename ValueFormatter>
class optional_formatter
{
    FlagFormatter flag_formatter;
    ValueFormatter value_formatter;

public:
    optional_formatter(FlagFormatter flag_formatter = FlagFormatter(), ValueFormatter value_formatter = ValueFormatter())
        : flag_formatter(flag_formatter)
        , value_formatter(value_formatter)
    {
    }

    template<typename ValueType, typename TSerializer>
    void save(TSerializer& serializer, const boost::optional<ValueType>& value) const
    {
        serializer.save(value.is_initialized(), flag_formatter);
        if (value)
        {
            serializer.save(*value, value_formatter);
        }
    }

    template<typename ValueType, typename TSerializer>
    void load(TSerializer& serializer, boost::optional<ValueType>& value) const
    {
        value.reset();
        bool value_flag;
        serializer.load(value_flag, flag_formatter);
        if (value_flag)
        {
            value = ValueType();
            serializer.load(*value, value_formatter);
        }
    }
};

template<typename FlagFormatter, typename ValueFormatter>
optional_formatter<FlagFormatter, ValueFormatter> create_optional_formatter(FlagFormatter flag_formatter = FlagFormatter(), ValueFormatter value_formatter = ValueFormatter())
{
    return optional_formatter<FlagFormatter, ValueFormatter>(flag_formatter, value_formatter);
}

} // namespace binary_format

#endif // BinaryFormatSerializer_optional_formatter_H
