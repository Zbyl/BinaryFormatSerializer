/////////////////////////////////////////////////////////////////////////////
/// BinaryFormatSerializer
///    Library for serializing data in arbitrary binary format.
///
/// fixed_size_array_formatter.h
///
/// This file contains fixed_size_array_formatter that formats an array as a sequence of elements. Array length is not serialized.
///
/// Distributed under Apache License, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0)
/// (c) 2014 Zbigniew Skowron, zbychs@gmail.com
///
/////////////////////////////////////////////////////////////////////////////

#ifndef BinaryFormatSerializer_fixed_size_array_formatter_H
#define BinaryFormatSerializer_fixed_size_array_formatter_H

namespace binary_format
{

template<typename ValueFormatter>
class fixed_size_array_formatter
{
    ValueFormatter value_formatter;

public:
    fixed_size_array_formatter(ValueFormatter value_formatter = ValueFormatter())
        : value_formatter(value_formatter)
    {
    }

    template<int Size, typename ValueType>
    void save(ISerializer& serializer, const ValueType(&array)[Size])
    {
        for (int i = 0; i < Size; ++i)
        {
            serializer.save(array[i], value_formatter);
        }
    }

    template<int Size, typename ValueType>
    void load(ISerializer& serializer, ValueType(&array)[Size])
    {
        for (int i = 0; i < Size; ++i)
        {
            serializer.load(array[i], value_formatter);
        }
    }
};

template<typename ValueFormatter>
fixed_size_array_formatter<ValueFormatter> create_fixed_size_array_formatter(ValueFormatter value_formatter = ValueFormatter())
{
    return fixed_size_array_formatter<ValueFormatter>(value_formatter);
}

} // namespace binary_format

#endif // BinaryFormatSerializer_fixed_size_array_formatter_H
