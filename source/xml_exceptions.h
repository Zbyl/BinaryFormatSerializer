/////////////////////////////////////////////////////////////////////////////
/// XmlFormatSerializer
///    Library for serializing data in arbitrary xml format.
///
/// xml_exceptions.h
///
/// This file contains exception classes for XmlFormatSerializer.
///
/// Distributed under Apache License, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0)
/// (c) 2016 Zbigniew Skowron, zbychs@gmail.com
///
/////////////////////////////////////////////////////////////////////////////

#ifndef XmlFormatSerializer_xml_exceptions_H
#define XmlFormatSerializer_xml_exceptions_H

#include <exception>

#include <boost/exception/exception.hpp>
#include <boost/exception/all.hpp>
#include <boost/throw_exception.hpp>

namespace xml_format
{

/// @brief Base class for xml serialization exceptions.
class xml_exception : public virtual std::exception, public virtual boost::exception
{
};

/// @brief invalid_data exception is thrown when deserialization encountered data that is not expected according to format.
class invalid_data : public virtual xml_exception
{
};

namespace detail
{
    typedef boost::error_info<struct tag_description, const char*> errinfo_description;
} // namespace detail

} // namespace xml_format

#endif // XmlFormatSerializer_xml_exceptions_H
