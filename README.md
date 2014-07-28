BinaryFormatSerializer
======================

BinaryFormatSerializer is a library for serializing data in arbitrary binary format.
This is currently a prototype under development.

Distributed under Apache License, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0)
(c) 2014 Zbigniew Skowron, zbychs@gmail.com

Rationale
=========

This library is meant to solve two problems:

1. Binary format should be defined entirely by the user.
  - The library does not limit what binary formats are possible.
  - Every call to a serialization function specifies the binary format explicitly. There are no default values for formats. This way the code is easy to verify against the format specification.
2. Serialization and deserialization should be realized by the same, declarative code.
  - Declarative way of specifying the binary format is another feature that makes it easy to verify the code against the specification.
  - Unifying serialization and deserialization reduces the amout of code and eliminates the need to keep two functions in sync.
 
What this library will not provide:

1. Versioning of the binary format.
  - Since the library does not enforce any specific format, it also won't add any version tags. This is left for the user to implement is any way he chooses. Helper classes implementing versioning might be added in the future.
2. Default formats for common types.
  - The library provides formatters for common types, but it will not make them a default. The format will always have to be specified explicitly to make the code verifiable agains the format specification.

Competition
===========

There are many serialization libraries out there, but most of them enforce a specific binary format, like Google Protocol Buffers or boost::serialization.
If you need to support a specific binary format they cannot help you.

Google Protocol Buffers also utilizes a code generator, which is always more trouble than pure C++.

Example
=======

```cpp
struct SimpleStruct
{
    int number;
    std::string text;
    std::map<int, std::string> map;
};

class simple_struct_formatter : public formatter_base<simple_struct_formatter>
{
public:
    void serialize(ISerializer& serializer, SimpleStruct& simpleStruct)
    {
        /// syntax 1: binary format specified as template parameter
        serializer.serialize< little_endian<4> >(simpleStruct.number);
        serializer.serialize< string_formatter< little_endian<2> > >(simpleStruct.text);

        /// syntax 2: binary format specified as a function parameter (allows for stateful formatters)
        ///             element count     key formatter            value formatter
        map_formatter< little_endian<4>, little_endian<1>, string_formatter< little_endian<4> > > mapFormat;
        serializer.serialize(simpleStruct.map, mapFormat);
    }
};

void example()
{
    // serialization
    VectorSaveSerializer vectorWriter;
    SimpleStruct simple;
    vectorWriter.serialize<simple_struct_formatter>(simple);

    // deserialization
    VectorLoadSerializer vectorReader(vectorWriter.getData());
    SimpleStruct simple2;
    vectorReader.serialize<simple_struct_formatter>(simple2);
}
```

Notes
=====

1. This library uses boost where standard C++ 11 would suffice. This is done because I need to support C++ 03.
   This will change as soon as I'm able to use C++ 11 in my code.
2. Google Protocol Buffers formatters would be a nice addition to this library.
3. Helper classes implementing versioning would be a nice addition too.
