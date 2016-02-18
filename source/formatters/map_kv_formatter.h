/////////////////////////////////////////////////////////////////////////////
/// BinaryFormatSerializer
///    Library for serializing data in arbitrary binary format.
///
/// map_kv_formatter.h
///
/// This file contains map_kv_formatter that formats std::map as a length field followed by pairs of keys and values.
///
/// Distributed under Apache License, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0)
/// (c) 2014 Zbigniew Skowron, zbychs@gmail.com
///
/////////////////////////////////////////////////////////////////////////////

#ifndef BinaryFormatSerializer_map_kv_formatter_H
#define BinaryFormatSerializer_map_kv_formatter_H

#include <map>
#include <utility>

namespace binary_format
{

template<typename SizeFormatter, typename KeyValueFormatter>
class map_kv_formatter
{
    SizeFormatter size_formatter;
    KeyValueFormatter key_value_formatter;

public:
    map_kv_formatter(SizeFormatter size_formatter = SizeFormatter(), KeyValueFormatter key_value_formatter = KeyValueFormatter())
        : size_formatter(size_formatter)
        , key_value_formatter(key_value_formatter)
    {
    }

    template<typename KeyType, typename ValueType, typename TSerializer>
    void save(TSerializer& serializer, const std::map<KeyType, ValueType>& map) const
    {
        size_formatter.save(serializer, map.size());
        for (auto& kv : map)
        {
            key_value_formatter.save(serializer, kv);
        }
    }

    template<typename KeyType, typename ValueType, typename TSerializer>
    void load(TSerializer& serializer, std::map<KeyType, ValueType>& map) const
    {
        map.clear();
        size_t map_size;
        size_formatter.load(serializer, map_size);
        for (size_t i = 0; i < map_size; ++i)
        {
            std::pair<KeyType, ValueType> kv;
            key_value_formatter.load(serializer, kv);
            map[kv.first] = kv.second;
        }
    }
};

template<typename SizeFormatter, typename KeyValueFormatter>
map_kv_formatter<SizeFormatter, KeyValueFormatter> create_map_kv_formatter(SizeFormatter size_formatter = SizeFormatter(), KeyValueFormatter key_value_formatter = KeyValueFormatter())
{
    return map_kv_formatter<SizeFormatter, KeyValueFormatter>(size_formatter, key_value_formatter);
}

} // namespace binary_format

#endif // BinaryFormatSerializer_map_kv_formatter_H
