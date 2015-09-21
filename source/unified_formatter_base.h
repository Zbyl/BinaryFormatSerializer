/////////////////////////////////////////////////////////////////////////////
/// BinaryFormatSerializer
///    Library for serializing data in arbitrary binary format.
///
/// unified_formatter_base.h
///
/// This file contains unified_formatter_base mixin class that provides save() and load() by the means of the serialize() method.
///
/// Distributed under Apache License, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0)
/// (c) 2014 Zbigniew Skowron, zbychs@gmail.com
///
/////////////////////////////////////////////////////////////////////////////

#ifndef BinaryFormatSerializer_unified_formatter_base_H
#define BinaryFormatSerializer_unified_formatter_base_H

#include "ISerializer.h"

#include <boost/type_traits/remove_const.hpp>

namespace binary_format
{

template<typename Derived, typename TSerializer = ISerializer>
class unified_formatter_base
{
public:
    template<typename T>
    void save(TSerializer& serializer, const T& object) const
    {
        static_cast<const Derived*>(this)->serialize(serializer, const_cast<typename boost::remove_const<T>::type&>(object));
    }

    template<typename T>
    void load(TSerializer& serializer, T& object) const
    {
        static_cast<const Derived*>(this)->serialize(serializer, object);
    }
};

} // namespace binary_format

#endif // BinaryFormatSerializer_unified_formatter_base_H
