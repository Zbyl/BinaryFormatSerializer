/////////////////////////////////////////////////////////////////////////////
/// BinaryFormatSerializer
///    Library for serializing data in arbitrary binary format.
///
/// map_formatter.h
///
/// This file contains map_formatter that formats std::map as a length field followed by pairs of keys and values.
///
/// Distributed under Apache License, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0)
/// (c) 2014 Zbigniew Skowron, zbychs@gmail.com
///
/////////////////////////////////////////////////////////////////////////////

#ifndef BinaryFormatSerializer_map_formatter_H
#define BinaryFormatSerializer_map_formatter_H

#include <map>

namespace binary_format
{

template<typename SizeFormatter, typename KeyFormatter, typename ValueFormatter>
class map_formatter
{
    SizeFormatter size_formatter;
    KeyFormatter key_formatter;
    ValueFormatter value_formatter;

public:
    map_formatter(SizeFormatter size_formatter = SizeFormatter(), KeyFormatter key_formatter = KeyFormatter(), ValueFormatter value_formatter = ValueFormatter())
        : size_formatter(size_formatter)
        , key_formatter(key_formatter)
        , value_formatter(value_formatter)
    {
    }

    template<typename KeyType, typename ValueType, typename TSerializer>
    void save(TSerializer& serializer, const std::map<KeyType, ValueType>& map) const
    {
        serializer.save(map.size(), size_formatter);
        for (auto& kv : map)
        {
            serializer.save(kv.first, key_formatter);
            serializer.save(kv.second, value_formatter);
        }
    }

    template<typename KeyType, typename ValueType, typename TSerializer>
    void load(TSerializer& serializer, std::map<KeyType, ValueType>& map) const
    {
        map.clear();
        size_t map_size;
        serializer.load(map_size, size_formatter);
        for (size_t i = 0; i < map_size; ++i)
        {
            KeyType key;
            serializer.load(key, key_formatter);
            serializer.load(map[key], value_formatter);
        }
    }
};

template<typename SizeFormatter, typename KeyFormatter, typename ValueFormatter>
map_formatter<SizeFormatter, KeyFormatter, ValueFormatter> create_map_formatter(SizeFormatter size_formatter = SizeFormatter(), KeyFormatter key_formatter = KeyFormatter(), ValueFormatter value_formatter = ValueFormatter())
{
    return map_formatter<SizeFormatter, KeyFormatter, ValueFormatter>(size_formatter, key_formatter, value_formatter);
}

} // namespace binary_format

#endif // BinaryFormatSerializer_map_formatter_H
