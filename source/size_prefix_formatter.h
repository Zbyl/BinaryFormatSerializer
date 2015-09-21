/////////////////////////////////////////////////////////////////////////////
/// BinaryFormatSerializer
///    Library for serializing data in arbitrary binary format.
///
/// size_prefix_formatter.h
///
/// This file contains size_prefix_formatter that stores size of the serialized value, followed by the serialized value itself.
/// NOTE: This formatter requires it's serializer to be ISeekable.
///
/// Distributed under Apache License, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0)
/// (c) 2014 Zbigniew Skowron, zbychs@gmail.com
///
/////////////////////////////////////////////////////////////////////////////

#ifndef BinaryFormatSerializer_size_prefix_formatter_H
#define BinaryFormatSerializer_size_prefix_formatter_H

#include "SizeCountingSerializer.h"
#include "ScopedSerializer.h"

namespace binary_format
{

template<typename SizeFormatter, typename ValueFormatter>
class size_prefix_formatter
{
    SizeFormatter size_formatter;
    ValueFormatter value_formatter;

public:
    size_prefix_formatter(SizeFormatter size_formatter = SizeFormatter(), ValueFormatter value_formatter = ValueFormatter())
        : size_formatter(size_formatter)
        , value_formatter(value_formatter)
    {
    }

    template<typename ValueType, typename TSerializer>
    void save(TSerializer& serializer, const ValueType& value) const
    {
        SizeCountingSerializer sizeCountingSerializer;
        sizeCountingSerializer.save(value, value_formatter);

        boost::uintmax_t byteCount = sizeCountingSerializer.getByteCount();
        serializer.save(byteCount, size_formatter);

        serializer.save(value, value_formatter);
    }

    /// @brief This method will verify that deserialization read exactly the number of bytes stored in the size field.
    ///        It will throw end_of_input when more data was attepted to be read, and invalid_data if not all data was read.
    template<typename ValueType, typename TSerializer>
    void load(TSerializer& serializer, ValueType& value) const
    {
        boost::uintmax_t byteCount;
        serializer.load(byteCount, size_formatter);

        ScopedSerializer scopedSerializer(serializer, byteCount);
        scopedSerializer.load(value, value_formatter);
        scopedSerializer.verifyAllBytesProcessed();
    }
};

template<typename SizeFormatter, typename ValueFormatter>
size_prefix_formatter<SizeFormatter, ValueFormatter> create_size_prefix_formatter(SizeFormatter size_formatter = SizeFormatter(), ValueFormatter value_formatter = ValueFormatter())
{
    return size_prefix_formatter<SizeFormatter, ValueFormatter>(size_formatter, value_formatter);
}

} // namespace binary_format

#endif // BinaryFormatSerializer_size_prefix_formatter_H
