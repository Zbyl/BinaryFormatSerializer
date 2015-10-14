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

#include "ISeekable.h"
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
    /// @note size_formatter must always store the same number of bytes
    size_prefix_formatter(SizeFormatter size_formatter = SizeFormatter(), ValueFormatter value_formatter = ValueFormatter())
        : size_formatter(size_formatter)
        , value_formatter(value_formatter)
    {
    }

    template<typename ValueType, typename TSerializer>
    void save(TSerializer& serializer, const ValueType& value)
    {
        offset_t initialPosition = serializer.position();

        offset_t byteCount = 0;
        size_formatter.save(serializer, byteCount);  // reserve space for storing byteCount

        offset_t dataPosition = serializer.position();  // position where data starts
        value_formatter.save(serializer, value);
        offset_t endPosition = serializer.position();  // position where data ends

        byteCount = endPosition - dataPosition;
        serializer.seek(initialPosition);
        size_formatter.save(serializer, byteCount); // write actual byteCount

        offset_t afterSizePosition = serializer.position();

        if (afterSizePosition != dataPosition)
        {
            BOOST_THROW_EXCEPTION(serialization_exception() << detail::errinfo_description("size_formatter must always store the same number of bytes."));
        }
    }

    /// @brief This method will verify that deserialization read exactly the number of bytes stored in the size field.
    ///        It will throw end_of_input when more data was attepted to be read, and invalid_data if not all data was read.
    template<typename ValueType, typename TSerializer>
    void load(TSerializer& serializer, ValueType& value)
    {
        offset_t byteCount;
        size_formatter.load(serializer, byteCount);

        ScopedSerializer scopedSerializer(serializer, byteCount);
        scopedSerializer.load(value, byteCount, value_formatter);
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
