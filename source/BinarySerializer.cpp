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
    std::string text;
    int number;
    int number2;
};

class simple_struct_formatter : public formatter_base<simple_struct_formatter>
{
public:
    void serialize(ISerializer& serializer, SimpleStruct& simpleStruct)
    {
        serializer.serialize< string_formatter< little_endian<2> > >(simpleStruct.text);
        serializer.serialize< little_endian<4> >(simpleStruct.number);
        serializer.serialize< little_endian<1> >(simpleStruct.number2);
    }
};

class SimpleClass
{
    int number;
    int number2;
private:
    friend void serialize_serializable(ISerializer& serializer, SimpleClass simpleClass)
    {
        serializer.serialize< little_endian<4> >(simpleClass.number);
        serializer.serialize< little_endian<1> >(simpleClass.number2);
    }
};

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

    VectorSaveSerializer vectorWriter;
    vectorWriter.serialize(map, mapFormat);
    vectorWriter.serialize< const_formatter< fixed_size_array_formatter< little_endian<4> > > >("MAGIC_STRING");

    VectorLoadSerializer vectorReader(vectorWriter.getData());
    vectorReader.serialize(map2, mapFormat);
    vectorReader.serialize< const_formatter< fixed_size_array_formatter< little_endian<4> > > >("MAGIC_STRING");

    return 0;
}

