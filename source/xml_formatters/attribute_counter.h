/////////////////////////////////////////////////////////////////////////////
/// XmlFormatSerializer
///    Library for serializing data in arbitrary xml format.
///
/// attribute_counter.h
///
/// This file contains attribute_counter that formats object as the count of attributes of xml element.
/// This counter can count all attributes or attributes with specific name.
///
/// Distributed under Apache License, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0)
/// (c) 2016 Zbigniew Skowron, zbychs@gmail.com
///
/////////////////////////////////////////////////////////////////////////////

#ifndef XmlFormatSerializer_attribute_counter_H
#define XmlFormatSerializer_attribute_counter_H

#include <string>

#include <boost/optional.hpp>

namespace xml_format
{

class attribute_counter
{
private:
    boost::optional<std::string> name;

public:
    explicit attribute_counter(const boost::optional<std::string>& name = boost::none)
        : name(name)
    {
    }

    explicit attribute_counter(const char* name)
        : attribute_counter(std::string(name))
    {
    }

    template<typename T, typename XmlTree>
    void save(XmlTree& xmlTree, const T& object) const
    {
        // @note Nothing to save.
    }

    template<typename T, typename XmlTree>
    void load(XmlTree& xmlTree, T& object) const
    {
        object = static_cast<T>(xmlTree.attributeCount(name));
    }
};

} // namespace xml_format

#endif // XmlFormatSerializer_attribute_counter_H
