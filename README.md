BinaryFormatSerializer
======================

BinaryFormatSerializer is a library for serializing data in arbitrary binary format.
This is currently a prototype under development.

Distributed under Apache License, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0)
(c) 2014 Zbigniew Skowron, zbychs@gmail.com

Example
=======

```c_cpp
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
    vectorWriter.serialize<simple_struct_formatter>(simple2);
}
```
