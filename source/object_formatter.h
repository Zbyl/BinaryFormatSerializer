/////////////////////////////////////////////////////////////////////////////
/// BinaryFormatSerializer
///    Library for serializing data in arbitrary binary format.
///
/// object_formatter.h
///
/// This file contains object_formatter that formats an object using it's serialize() method.
///
/// Distributed under Apache License, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0)
/// (c) 2014 Zbigniew Skowron, zbychs@gmail.com
///
/////////////////////////////////////////////////////////////////////////////

#ifndef BinaryFormatSerializer_object_formatter_H
#define BinaryFormatSerializer_object_formatter_H

#include "unified_formatter_base.h"

namespace binary_format
{

class object_formatter : public unified_formatter_base<object_formatter>
{
public:
    template<typename T, typename TSerializer>
    void serialize(TSerializer& serializer, T& object)
    {
        object->serialize(serializer);
    }
};

} // namespace binary_format

#endif // BinaryFormatSerializer_object_formatter_H
