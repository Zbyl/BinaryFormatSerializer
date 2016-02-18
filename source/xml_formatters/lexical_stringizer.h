/////////////////////////////////////////////////////////////////////////////
/// XmlFormatSerializer
///    Library for serializing data in arbitrary xml format.
///
/// lexical_stringizer.h
///
/// This file contains lexical_stringizer that formats values using boost::lexical_cast.
///
/// Distributed under Apache License, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0)
/// (c) 2016 Zbigniew Skowron, zbychs@gmail.com
///
/////////////////////////////////////////////////////////////////////////////

#ifndef XmlFormatSerializer_lexical_stringizer_H
#define XmlFormatSerializer_lexical_stringizer_H

#include <boost/lexical_cast.hpp>

namespace xml_format
{

class lexical_stringizer
{
public:
    template<typename T, typename String>
    void save(String& string, const T& object) const
    {
        string = boost::lexical_cast<String>(object);
    }

    template<typename T, typename String>
    void load(const String& string, T& object) const
    {
        object = boost::lexical_cast<T>(string);
    }
};

inline lexical_stringizer create_lexical_stringizer()
{
    return lexical_stringizer();
}

} // namespace xml_format

#endif // XmlFormatSerializer_lexical_stringizer_H
