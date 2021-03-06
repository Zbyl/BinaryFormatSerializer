/////////////////////////////////////////////////////////////////////////////
/// XmlFormatSerializer
///    Library for serializing data in arbitrary xml format.
///
/// XmlTree.h
///
/// This file contains XmlTree that formats std::vector as length field followed by individual values.
///
/// Distributed under Apache License, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0)
/// (c) 2016 Zbigniew Skowron, zbychs@gmail.com
///
/////////////////////////////////////////////////////////////////////////////

#ifndef XmlFormatSerializer_XmlTree_H
#define XmlFormatSerializer_XmlTree_H

#include "xml_exceptions.h"

#include <vector>
#include <ostream>

#include <rapidxml.hpp>
#include <rapidxml_print.hpp>

#include <boost/optional.hpp>

namespace xml_format
{

class RapidXmlAttribute
{
public:
    explicit RapidXmlAttribute(rapidxml::xml_attribute<>& attribute)
        : attribute(attribute)
    {
    }

    /// @brief This function sets name of this attribute.
    ///        This function can't be called on a attribute that has a name already.
    ///        This function must not be called more than once on any given attribute.
    void setName(const std::string& name)
    {
        if (hasName())
        {
            BOOST_THROW_EXCEPTION(xml_exception() << detail::errinfo_description("Attribute already has a name."));
        }

        auto doc = attribute.document();
        char *attribute_name = doc->allocate_string(name.c_str());
        attribute.name(attribute_name);
    }

    std::string eatName()
    {
        if (!hasName())
        {
            BOOST_THROW_EXCEPTION(xml_exception() << detail::errinfo_description("Attribute must have a name."));
        }

        auto name = attribute.name();
        attribute.name("");
        return name;
    }

    bool hasName() const
    {
        return attribute.name_size() > 0;
    }

    /// @brief This function sets text content of this attribute.
    ///        This function must not be called more than once on any given attribute.
    void setTextContent(const std::string& value)
    {
        if (hasTextContent())
        {
            BOOST_THROW_EXCEPTION(xml_exception() << detail::errinfo_description("Attribute already has a value."));
        }

        auto doc = attribute.document();
        char *attribute_value = doc->allocate_string(value.c_str());
        attribute.value(attribute_value);
    }

    std::string eatTextContent()
    {
        auto value = attribute.value();
        attribute.value("");
        return value;
    }

    bool hasTextContent() const
    {
        return attribute.value_size() > 0;
    }

private:
    rapidxml::xml_attribute<>& attribute;
};

class RapidXmlTree
{
public:
    explicit RapidXmlTree(rapidxml::xml_node<>& node)
        : node(node)
    {
    }

    bool isDeclaration()
    {
        return node.type() == rapidxml::node_declaration;
    }

    RapidXmlAttribute addAttribute(const boost::optional<std::string>& name)
    {
        auto doc = node.document();
        char *attribute_name = name ? doc->allocate_string(name->c_str()) : nullptr;
        auto attr = doc->allocate_attribute(attribute_name);
        node.append_attribute(attr);
        return RapidXmlAttribute(*attr);
    }

    RapidXmlAttribute addAttribute(const char* name)
    {
        return addAttribute(std::string(name));
    }

    RapidXmlAttribute eatAttribute(const boost::optional<std::string>& name, bool sequential)
    {
        const char *attr_name = name ? name->c_str() : nullptr;
        if (sequential)
        {
            attr_name = nullptr;
        }
        auto attr = node.first_attribute(attr_name);
        if (!attr)
        {
            BOOST_THROW_EXCEPTION(invalid_data() << detail::errinfo_description("Attribute not found."));
        }
        if (sequential && name && (attr->name() != *name))
        {
            BOOST_THROW_EXCEPTION(invalid_data() << detail::errinfo_description("Attribute has unexpected name."));
        }

        node.remove_attribute(attr);

        return RapidXmlAttribute(*attr);
    }

    RapidXmlAttribute eatAttribute(const char* name, bool sequential)
    {
        return eatAttribute(std::string(name), sequential);
    }

    RapidXmlTree addElement(const boost::optional<std::string>& name, bool declaration = false)
    {
        if (declaration && name)
        {
            BOOST_THROW_EXCEPTION(xml_exception() << detail::errinfo_description("Declaration must not have a name."));
        }

        auto doc = node.document();

        char *node_name = name ? doc->allocate_string(name->c_str()) : nullptr;
        auto element = doc->allocate_node(declaration ? rapidxml::node_declaration : rapidxml::node_element, node_name);
        node.append_node(element);

        return RapidXmlTree(*element);
    }

    RapidXmlTree addElement(const char* name, bool declaration = false)
    {
        return addElement(std::string(name), declaration);
    }

    RapidXmlTree eatElement(const boost::optional<std::string>& name, bool sequential)
    {
        const char *node_name = name ? name->c_str() : nullptr;
        if (sequential)
        {
            node_name = nullptr;
        }
        auto element = node.first_node(node_name);
        if (!element)
        {
            BOOST_THROW_EXCEPTION(invalid_data() << detail::errinfo_description("Element not found."));
        }
        if (sequential && name && (element->name() != *name))
        {
            BOOST_THROW_EXCEPTION(invalid_data() << detail::errinfo_description("Element has unexpected name."));
        }

        node.remove_node(element);

        return RapidXmlTree(*element);
    }

    RapidXmlTree eatElement(const char* name, bool sequential)
    {
        return eatElement(std::string(name), sequential);
    }

    /// @brief This function sets name of this node.
    ///        This function can't be called on a node that has a name already.
    ///        This function must not be called more than once on any given node.
    void setName(const std::string& name)
    {
        if (hasName())
        {
            BOOST_THROW_EXCEPTION(xml_exception() << detail::errinfo_description("Element already has a name."));
        }

        auto doc = node.document();
        char *node_name = doc->allocate_string(name.c_str());
        node.name(node_name);
    }

    std::string eatName()
    {
        if (!hasName())
        {
            BOOST_THROW_EXCEPTION(xml_exception() << detail::errinfo_description("Element must have a name."));
        }

        auto name = node.name();
        node.name("");
        return name;
    }

    bool hasName() const
    {
        return node.name_size() > 0;
    }

    /// @brief This function sets text content of this node.
    ///        This function must not be called more than once on any given node.
    void setTextContent(const std::string& value)
    {
        if (hasTextContent())
        {
            BOOST_THROW_EXCEPTION(xml_exception() << detail::errinfo_description("Element already has text content."));
        }

        auto doc = node.document();
        char *node_value = doc->allocate_string(value.c_str());
        node.value(node_value);
    }

    std::string eatTextContent()
    {
        auto value = node.value();
        node.value("");
        return value;
    }

    bool hasTextContent() const
    {
        return node.value_size() > 0;
    }

    /// @brief Returns number of child elements with given name, or number of all child elements if name is boost::none.
    int elementCount(const boost::optional<std::string>& name = boost::none) const
    {
        const char* element_name = name ? name->c_str() : nullptr;

        int count = 0;

        auto element = node.first_node(element_name);
        while (element)
        {
            count++;
            element = element->next_sibling(element_name);
        }

        return count;
    }

    int elementCount(const char* name) const
    {
        return elementCount(std::string(name));
    }

    /// @brief Returns number of attributes with given name, or number of all attributes if name is boost::none.
    int attributeCount(const boost::optional<std::string>& name = boost::none) const
    {
        const char* attrib_name = name ? name->c_str() : nullptr;

        int count = 0;

        auto attrib = node.first_attribute(attrib_name);
        while (attrib)
        {
            count++;
            attrib = attrib->next_attribute(attrib_name);
        }

        return count;
    }

    int attributeCount(const char* name) const
    {
        return attributeCount(std::string(name));
    }

    /// @brief Returns true if element has no text content, no child elements and no attributes.
    bool isEmpty()
    {
        bool hasElements = elementCount() > 0;
        bool hasAttributes = attributeCount() > 0;
        return !(hasElements || hasAttributes || hasTextContent());
    }

private:
    rapidxml::xml_node<>& node;
};

class RapidXmlDocument
{
public:
    RapidXmlDocument(const std::string& encoding)
        : m_encoding(encoding)
    {
    }

    explicit RapidXmlDocument(const std::string& xml, const std::string& encoding)
        : m_xml(xml.c_str(), xml.c_str() + xml.length() + 1)
        , m_encoding(encoding)
    {
        m_document.parse<rapidxml::parse_no_data_nodes | rapidxml::parse_declaration_node>(m_xml.data());
    }

    const std::string& getEncoding()
    {
        return m_encoding;
    }

    RapidXmlTree getDocumentElement()
    {
        return RapidXmlTree(m_document);
    }

    operator RapidXmlTree()
    {
        return getDocumentElement();
    }

    friend std::ostream& operator<<(std::ostream& os, const RapidXmlDocument& doc)
    {
        return os << doc.m_document;
    }

private:
    std::vector<char> m_xml;
    std::string m_encoding;
    rapidxml::xml_document<> m_document;
};

} // namespace xml_format

#endif // XmlFormatSerializer_XmlTree_H
