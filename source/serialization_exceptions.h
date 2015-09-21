/////////////////////////////////////////////////////////////////////////////
/// BinaryFormatSerializer
///    Library for serializing data in arbitrary binary format.
///
/// serialization_exceptions.h
///
/// This file contains exception classes for serialization.
///
/// Distributed under Apache License, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0)
/// (c) 2014 Zbigniew Skowron, zbychs@gmail.com
///
/////////////////////////////////////////////////////////////////////////////

#ifndef BinaryFormatSerializer_serialization_exceptions_H
#define BinaryFormatSerializer_serialization_exceptions_H

#include <exception>

#include <boost/exception/exception.hpp>
#include <boost/exception/all.hpp>
#include <boost/throw_exception.hpp>
#include <boost/cstdint.hpp>

namespace binary_format
{

/// @brief Base class for serialization exceptions.
class serialization_exception : public virtual std::exception, public virtual boost::exception
{
};

/// @brief end_of_input exception is thrown when more data is requested, but there is no more data available.
class end_of_input : public virtual serialization_exception
{
};

/// @brief end_of_space exception is thrown when more data needs to be written, but there is no more space available.
class end_of_space : public virtual serialization_exception
{
};

/// @brief lossy_conversion exception is thrown when requested serialization would cause data loss (serialization would not be reversible).
class lossy_conversion : public virtual serialization_exception
{
};

/// @brief invalid_data exception is thrown when deserialization encountered data that is not expected according to format.
class invalid_data : public virtual serialization_exception
{
};

namespace detail
{
    typedef boost::error_info<struct tag_description, const char*> errinfo_description;
    typedef boost::error_info<struct tag_requested_this_many_bytes_more, boost::uintmax_t> errinfo_requested_this_many_bytes_more;
} // namespace detail

} // namespace binary_format

#endif // BinaryFormatSerializer_serialization_exceptions_H
