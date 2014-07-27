/////////////////////////////////////////////////////////////////////////////
/// BinaryFormatSerializer
///    Library for serializing data in arbitrary binary format.
///
/// generic_formatter.h
///
/// This file contains generic_formatter that formats a type using a serialize_serializable() function found using Argument Dependent Lookup.
///
/// Distributed under Apache License, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0)
/// (c) 2014 Zbigniew Skowron, zbychs@gmail.com
///
/////////////////////////////////////////////////////////////////////////////

#ifndef BinaryFormatSerializer_generic_formatter_H
#define BinaryFormatSerializer_generic_formatter_H

#include "formatter_base.h"

namespace binary_format
{

class generic_formatter : public formatter_base<generic_formatter>
{
public:
    template<typename T>
    void serialize(ISerializer& serializer, T& object)
    {
        serialize_serializable(serializer, object);    // function found using Argument Dependent Lookup
    }
};

} // namespace binary_format

#endif // BinaryFormatSerializer_generic_formatter_H
