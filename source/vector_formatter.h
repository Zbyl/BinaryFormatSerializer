/////////////////////////////////////////////////////////////////////////////
/// BinaryFormatSerializer
///    Library for serializing data in arbitrary binary format.
///
/// vector_formatter.h
///
/// This file contains vector_formatter that formats std::vector as length field followed by individual values.
///
/// Distributed under Apache License, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0)
/// (c) 2014 Zbigniew Skowron, zbychs@gmail.com
///
/////////////////////////////////////////////////////////////////////////////

#ifndef BinaryFormatSerializer_vector_formatter_H
#define BinaryFormatSerializer_vector_formatter_H

#include <vector>

namespace binary_format
{

template<typename SizeFormatter, typename ValueFormatter>
class vector_formatter
{
    SizeFormatter size_formatter;
    ValueFormatter value_formatter;

public:
    vector_formatter(SizeFormatter size_formatter = SizeFormatter(), ValueFormatter value_formatter = ValueFormatter())
        : size_formatter(size_formatter)
        , value_formatter(value_formatter)
    {
    }

    template<typename ValueType>
    void save(ISerializer& serializer, const std::vector<ValueType>& vector)
    {
        serializer.save(vector.size(), size_formatter);
        for (auto& kv : map)
        {
            serializer.save(kv.first, key_formatter);
            serializer.save(kv.second, value_formatter);
        }
    }

    template<typename ValueType>
    void load(ISerializer& serializer, std::vector<ValueType>& vector)
    {
        vector.clear();
        size_t vector_size;
        serializer.load(vector_size, size_formatter);
        for (size_t i = 0; i < vector_size; ++i)
        {
            vector.push_back(ValueType());
            serializer.load(vector.back(), value_formatter);
        }
    }
};

template<typename SizeFormatter, typename ValueFormatter>
vector_formatter<SizeFormatter, ValueFormatter> create_vector_formatter(SizeFormatter size_formatter = SizeFormatter(), ValueFormatter value_formatter = ValueFormatter())
{
    return vector_formatter<SizeFormatter, ValueFormatter>(size_formatter, value_formatter);
}

} // namespace binary_format

#endif // BinaryFormatSerializer_vector_formatter_H
