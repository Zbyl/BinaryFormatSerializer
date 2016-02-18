/////////////////////////////////////////////////////////////////////////////
/// XmlFormatSerializer
///    Library for serializing data in arbitrary xml format.
///
/// declaration_formatter.h
///
/// This file contains declaration_formatter that formats xml declaration.
///
/// Distributed under Apache License, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0)
/// (c) 2016 Zbigniew Skowron, zbychs@gmail.com
///
/////////////////////////////////////////////////////////////////////////////

#ifndef XmlFormatSerializer_declaration_formatter_H
#define XmlFormatSerializer_declaration_formatter_H

#include "lexical_stringizer.h"

#include "xml_exceptions.h"

#include <string>
#include <vector>
#include <type_traits>

#include <boost/optional.hpp>
#include <boost/algorithm/string.hpp>

namespace xml_format
{

template<typename ValueStringizer = lexical_stringizer>
class declaration_formatter
{
private:
    ValueStringizer value_stringizer;

public:
    explicit declaration_formatter(ValueStringizer value_stringizer = ValueStringizer())
        : value_stringizer(value_stringizer)
    {
    }

    template<typename T, typename XmlDocument>
    void save(XmlDocument& xmlDocument, const T& object) const
    {
        auto docElement = xmlDocument.getDocumentElement();
        if (!docElement.isEmpty())
        {
            BOOST_THROW_EXCEPTION(xml_exception() << detail::errinfo_description("xml declaration must be the first node of the document."));
        }

        std::string value;
        value_stringizer.save(value, object);

        auto xmlDeclaration = docElement.addElement(boost::none, true);
        auto version = xmlDeclaration.addAttribute("version");
        auto encoding = xmlDeclaration.addAttribute("encoding");
        version.setTextContent("1.0");
        encoding.setTextContent(value);
    }

    template<typename T, typename XmlDocument>
    void load(XmlDocument& xmlDocument, T& object) const
    {
        auto docElement = xmlDocument.getDocumentElement();

        auto xmlDeclaration = docElement.eatElement(boost::none, true);
        if (!xmlDeclaration.isDeclaration())
        {
            BOOST_THROW_EXCEPTION(xml_exception() << detail::errinfo_description("Document should start with an xml declaration."));
        }

        auto encodingAttr = xmlDeclaration.eatAttribute("encoding", false);
        auto encoding = encodingAttr.eatTextContent();
        value_stringizer.load(encoding, object);
    }
};

template<typename ValueStringizer = lexical_stringizer>
declaration_formatter<ValueStringizer> create_declaration_formatter(ValueStringizer value_stringizer = ValueStringizer())
{
    return declaration_formatter<ValueStringizer>(value_stringizer);
}

} // namespace xml_format

#endif // XmlFormatSerializer_declaration_formatter_H
