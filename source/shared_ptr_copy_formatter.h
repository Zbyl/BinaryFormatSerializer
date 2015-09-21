/////////////////////////////////////////////////////////////////////////////
/// BinaryFormatSerializer
///    Library for serializing data in arbitrary binary format.
///
/// shared_ptr_copy_formatter.h
///
/// This file contains shared_ptr_copy_formatter that formats std::shared_ptr as a flag field followed by the value.
/// NOTE: Every instance of a shared_ptr will be serialized as an independent copy (so the shared ownership will NOT be preserved).
/// NOTE: Upon loading operator new will be used to create new instance of the pointed object.
///
/// Distributed under Apache License, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0)
/// (c) 2014 Zbigniew Skowron, zbychs@gmail.com
///
/////////////////////////////////////////////////////////////////////////////

#ifndef BinaryFormatSerializer_shared_ptr_copy_formatter_H
#define BinaryFormatSerializer_shared_ptr_copy_formatter_H

#include <memory>

namespace binary_format
{

template<typename FlagFormatter, typename ValueFormatter>
class shared_ptr_copy_formatter
{
    FlagFormatter flag_formatter;
    ValueFormatter value_formatter;

public:
    shared_ptr_copy_formatter(FlagFormatter flag_formatter = FlagFormatter(), ValueFormatter value_formatter = ValueFormatter())
        : flag_formatter(flag_formatter)
        , value_formatter(value_formatter)
    {
    }

    template<typename ValueType, typename TSerializer>
    void save(TSerializer& serializer, const std::shared_ptr<ValueType>& value) const
    {
        serializer.save(value.get() != NULL, flag_formatter);
        if (value)
        {
            serializer.save(*value, value_formatter);
        }
    }

    template<typename ValueType, typename TSerializer>
    void load(TSerializer& serializer, std::shared_ptr<ValueType>& value) const
    {
        value.reset();
        bool value_flag;
        serializer.load(value_flag, flag_formatter);
        if (value_flag)
        {
            value.reset(new ValueType());
            serializer.load(*value, value_formatter);
        }
    }
};

template<typename FlagFormatter, typename ValueFormatter>
shared_ptr_copy_formatter<FlagFormatter, ValueFormatter> create_shared_ptr_copy_formatter(FlagFormatter flag_formatter = FlagFormatter(), ValueFormatter value_formatter = ValueFormatter())
{
    return shared_ptr_copy_formatter<FlagFormatter, ValueFormatter>(flag_formatter, value_formatter);
}

} // namespace binary_format

#endif // BinaryFormatSerializer_shared_ptr_copy_formatter_H
