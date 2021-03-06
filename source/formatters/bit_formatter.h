/////////////////////////////////////////////////////////////////////////////
/// BinaryFormatSerializer
///    Library for serializing data in arbitrary binary format.
///
/// bit_formatter.h
///
/// This file contains bit_formatter that stores individual values or tuples of values packed in bitfields.
///
/// Distributed under Apache License, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0)
/// (c) 2014 Zbigniew Skowron, zbychs@gmail.com
///
/////////////////////////////////////////////////////////////////////////////

#ifndef BinaryFormatSerializer_bit_formatter_H
#define BinaryFormatSerializer_bit_formatter_H

#include "endian_formatter.h"
#include "utility/bit_packer.h"

namespace binary_format
{

template<boost::endian::order TargetOrder, int... Bits>
class bit_formatter
{
    using packer = bit_packer<Bits...>;
    using value_formatter = endian_formatter<TargetOrder, sizeof(packer::packed_type)>;

public:
    template<typename TSerializer, typename... Ts>
    void save(TSerializer& serializer, Ts... vals) const
    {
        packer::packed_type val = packer::pack(vals...);
        value_formatter().save(serializer, val);
    }

    template<typename TSerializer, typename... Ts>
    void load(TSerializer& serializer, Ts&... vals) const
    {
        packer::packed_type val;
        value_formatter().load(serializer, val);
        packer::unpack(val, vals...);
    }
    template<typename TSerializer, typename... Ts>
    void save(TSerializer& serializer, const std::tuple<Ts...>& vals) const
    {
        packer::packed_type val = packer::pack(vals);
        value_formatter().save(serializer, val);
    }

    /// Unpacks into a tuple of references.
    template<typename TSerializer, typename... Ts>
    void load(TSerializer& serializer, const std::tuple<Ts&...>& vals) const
    {
        packer::packed_type val;
        value_formatter().load(serializer, val);
        packer::unpack(val, vals);
    }

    /// Unpacks into a reference to a tuple of values.
    template<typename TSerializer, typename... Ts>
    void load(TSerializer& serializer, std::tuple<Ts...>& vals) const
    {
        packer::packed_type val;
        value_formatter().load(serializer, val);
        packer::unpack(val, vals);
    }
};

} // namespace binary_format

#endif // BinaryFormatSerializer_bit_formatter_H
