/////////////////////////////////////////////////////////////////////////////
/// BinaryFormatSerializer
///    Library for serializing data in arbitrary binary format.
///
/// type_formatter.h
///
/// This file contains type_formatter - a type erasure formatter that formats a type using any formatter.
///
/// Distributed under Apache License, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0)
/// (c) 2014 Zbigniew Skowron, zbychs@gmail.com
///
/////////////////////////////////////////////////////////////////////////////

#ifndef BinaryFormatSerializer_type_formatter_H
#define BinaryFormatSerializer_type_formatter_H

#include "ISerializer.h"

#include <memory>

#include <boost/any.hpp>

namespace binary_format
{

template<typename TSerializer, typename T>
class type_formatter
{
public:
    template<typename Formatter>
    type_formatter(const Formatter& formatter = Formatter())
        : typeFormatter(new TypeFormatter<Formatter>(formatter))
    {
    }

    void save(TSerializer& serializer, const T& object) const
    {
        typeFormatter->save(serializer, object);
    }

    void load(TSerializer& serializer, T& object) const
    {
        typeFormatter->load(serializer, object);
    }

private:
    class ITypeFormatter
    {
    public:
        virtual ~ITypeFormatter() {}
        virtual void save(TSerializer& serializer, const T& object) const = 0;
        virtual void load(TSerializer& serializer, T& object) const = 0;
    };

    template<typename Formatter>
    class TypeFormatter : public ITypeFormatter
    {
    public:
        TypeFormatter(const Formatter& formatter = Formatter())
            : formatter(formatter)
        {}

    public:
        virtual void save(TSerializer& serializer, const T& object) const
        {
            formatter.save(serializer, object);
        }

        virtual void load(TSerializer& serializer, T& object) const
        {
            formatter.load(serializer, object);
        }

    private:
        Formatter formatter;
    };

    std::unique_ptr<ITypeFormatter> typeFormatter;
};

template<typename TSerializer, typename T, typename Formatter>
type_formatter<TSerializer, T> make_type_formatter(const Formatter& formatter = Formatter())
{
    return type_formatter<TSerializer, T>(formatter);
}

} // namespace binary_format

#endif // BinaryFormatSerializer_type_formatter_H
