/////////////////////////////////////////////////////////////////////////////
/// XmlFormatSerializer
///    Library for serializing data in arbitrary xml format.
///
/// document_formatter.h
///
/// This file contains document_formatter that formats entire xml document.
///
/// Distributed under Apache License, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0)
/// (c) 2016 Zbigniew Skowron, zbychs@gmail.com
///
/////////////////////////////////////////////////////////////////////////////

#ifndef XmlFormatSerializer_document_formatter_H
#define XmlFormatSerializer_document_formatter_H

#include "declaration_formatter.h"
#include "element_formatter.h"
#include "xml_exceptions.h"

#include "formatters/const_formatter.h"

#include <string>
#include <vector>
#include <type_traits>

#include <boost/optional.hpp>
#include <boost/algorithm/string.hpp>

namespace xml_format
{

template<typename ValueFormatter>
class document_formatter
{
private:
    ValueFormatter value_formatter;

public:
    explicit document_formatter(ValueFormatter value_formatter = ValueFormatter())
        : value_formatter(value_formatter)
    {
    }

    template<typename T, typename XmlDocument>
    void save(XmlDocument& xmlDocument, const T& object) const
    {
        declaration_formatter<>().save(xmlDocument, xmlDocument.getEncoding());

        auto element = xmlDocument.getDocumentElement();
        value_formatter.save(element, object);
        if (element.hasName())
        {
            BOOST_THROW_EXCEPTION(xml_exception() << detail::errinfo_description("Document must not be named."));
        }
        if (element.attributeCount() > 0)
        {
            BOOST_THROW_EXCEPTION(xml_exception() << detail::errinfo_description("Document must not have attributes."));
        }
    }

    template<typename T, typename XmlDocument>
    void load(XmlDocument& xmlDocument, T& object) const
    {
        binary_format::const_formatter< declaration_formatter<> >().load(xmlDocument, xmlDocument.getEncoding());

        auto element = xmlDocument.getDocumentElement();
        value_formatter.load(element, object);
        if (!element.isEmpty())
        {
            BOOST_THROW_EXCEPTION(xml_exception() << detail::errinfo_description("Document not consumed completely."));
        }
    }
};

template<typename ValueFormatter>
document_formatter<ValueFormatter> create_document_formatter(ValueFormatter value_formatter = ValueFormatter())
{
    return document_formatter<ValueFormatter>(value_formatter);
}

template<typename ValueFormatter>
auto create_document_formatter(const std::string& name, ValueFormatter value_formatter = ValueFormatter())
{
    return create_document_formatter(create_element_formatter(name, value_formatter));
}

} // namespace xml_format

#endif // XmlFormatSerializer_document_formatter_H
