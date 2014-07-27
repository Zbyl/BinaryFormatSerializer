// BinarySerializer.cpp : Defines the entry point for the console application.
//

#include "ISerializer.h"

#include "formatter_base.h"
#include "string_formatter.h"
#include "map_formatter.h"
#include "vector_formatter.h"
#include "endian_formatter.h"
#include "const_formatter.h"
#include "fixed_size_array_formatter.h"
#include "inefficient_size_prefix_formatter.h"

#include "VectorSerializer.h"
#include "CoutSerializer.h"
#include "SizeCountingSerializer.h"

#include <cstdint>
#include <string>
#include <map>
#include <vector>

using namespace binary_format;

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

int main(int argc, char* argv[])
{
    map_formatter< little_endian<4>, little_endian<1>, string_formatter< little_endian<4> > > mapFormat;

    std::map<int, std::string> map;

    map[0] = "Ala";
    map[1] = "Ma";
    map[2] = "Kota";

    CoutSerializer coutSerializer(true);
    coutSerializer.serialize(map, mapFormat);

    coutSerializer.serialize< map_formatter<
                                    little_endian<4>,
                                    little_endian<1>,
                                    string_formatter< little_endian<4> >
                                > >(map);

    std::map<int, std::string> map2;

    boost::uint8_t lola = 5;
    boost::uint8_t lola2;

    VectorSaveSerializer vectorWriter;
    vectorWriter.serialize(map, mapFormat);
    vectorWriter.serialize< const_formatter< fixed_size_array_formatter< little_endian<4> > > >("MAGIC_STRING");
    vectorWriter.serialize< inefficient_size_prefix_formatter< little_endian<1>, little_endian<4> > >(lola);

    VectorLoadSerializer vectorReader(vectorWriter.getData());
    vectorReader.serialize(map2, mapFormat);
    vectorReader.serialize< const_formatter< fixed_size_array_formatter< little_endian<4> > > >("MAGIC_STRING");
    vectorReader.serialize< inefficient_size_prefix_formatter< little_endian<1>, little_endian<4> > >(lola2);

    return 0;
}

