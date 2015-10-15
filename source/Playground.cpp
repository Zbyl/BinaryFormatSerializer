// BinarySerializer.cpp : Defines the entry point for the console application.
//

// TODO: Base 128 Varint formatter and all of Google protocol buffers

#include "ISerializer.h"

#include "string_formatter.h"
#include "map_formatter.h"
#include "endian_formatter.h"
#include "type_formatter.h"
#include "any_formatter.h"
#include "vector_formatter.h"
#include "const_formatter.h"

#include "VectorSerializer.h"

#include <cstdint>
#include <string>
#include <map>
#include <vector>

#include "has_field.h"

struct WithX { int X; };
struct WithoutX { int Y; };
GENERATE_HAS_FIELD(X);
void ala()
{
    static_assert(has_field_X<WithX>::value == 1, "WithX should have X!");
    static_assert(has_field_X<WithoutX>::value == 0, "WithoutX should not have X!");
}

using namespace binary_format;

class TempVectorSaveSerializer
{
    std::vector<boost::uint8_t> buffer;
public:
    const std::vector<boost::uint8_t>& getData()
    {
        return buffer;
    }

    bool saving()
    {
        return true;
    }

    /// @brief Returs true if serializer is a loading serializer.
    bool loading() { return !saving(); }

    /// @brief Saves given object using specified formatter.
    ///        Will throw serialization_exception if serializer is a loading serializer.
    template<typename Formatter, typename T>
    void save(const T& object, Formatter& formatter = Formatter())
    {
        if (!saving())
        {
            BOOST_THROW_EXCEPTION(serialization_exception() << detail::errinfo_description("Can't save to a loading serializer."));
        }

        formatter.save(*this, object);
    }

    /// @brief Loads given object using specified formatter.
    ///        Will throw serialization_exception if serializer is a saving serializer.
    template<typename Formatter, typename T>
    void load(T& object, Formatter& formatter = Formatter())
    {
        if (!loading())
        {
            BOOST_THROW_EXCEPTION(serialization_exception() << detail::errinfo_description("Can't load from a saving serializer."));
        }

        formatter.load(*this, object);
    }

    /// @brief Serializes given object using specified formatter.
    ///        Calls save() or load() depending on whether serializer if a saving or loading serializer.
    template<typename Formatter, typename T>
    void serialize(T& object, const Formatter& formatter = Formatter())
    {
        if (saving())
        {
            formatter.save(*this, object);
        }
        else
        {
            formatter.load(*this, object);
        }
    }

public:
    void saveData(const boost::uint8_t* data, size_t size)
    {
        serializeData(const_cast<boost::uint8_t*>(data), size);
    }

    void loadData(boost::uint8_t* data, size_t size)
    {
        serializeData(data, size);
    }

    void serializeData(boost::uint8_t* data, size_t size)
    {
        buffer.insert(buffer.end(), data, data + size);
    }
};

int main(int argc, char* argv[])
{
    map_formatter< little_endian<4>, little_endian<1>, string_formatter< little_endian<4> > > mapFormat;

    std::map<int, std::string> map;

    map[0] = "Ala";
    map[1] = "Ma";
    map[2] = "Kota";

    std::map<int, std::string> map2;
    std::map<int, std::string> map3;
    std::map<int, std::string> map4;

    type_formatter< TempVectorSaveSerializer, std::map<int, std::string> > mapFormat2(mapFormat);

    any_formatter<VectorLoadSerializer> mapFormat3(make_any_formatter<VectorLoadSerializer, std::map<int, std::string> >(mapFormat));

    TempVectorSaveSerializer vectorWriter;
    vectorWriter.serialize(map, mapFormat);
    vectorWriter.serialize(map, mapFormat2);

    VectorLoadSerializer vectorReader(vectorWriter.getData());
    vectorReader.serialize(map2, mapFormat3);
    vectorReader.serialize(map3, mapFormat);

    std::vector<int> vector {1, 2, 3, 4, 5};

    VectorSaveSerializer vectorWriter2;
    serialize< vector_formatter< little_endian<1>, little_endian<4> > >(vectorWriter2, vector);
    serialize< vector_formatter< little_endian<1>, little_endian<1> > >(vectorWriter2, vector);
    serialize< vector_formatter< little_endian<1>, little_endian<1> > >(vectorWriter2, vector);
    save< big_endian<4> >(vectorWriter2, 0x12345678);
    save< big_endian<4> >(vectorWriter2, 0x12345678);

    std::vector<int> vector2;
    std::vector<int> vector3;
    std::vector<char> vector4 {1, 2, 3, 4, 5};
    VectorLoadSerializer vectorReader2(vectorWriter2.getData());
    serialize< vector_formatter< little_endian<1>, little_endian<4> > >(vectorReader2, vector2);
    serialize< vector_formatter< little_endian<1>, little_endian<1> > >(vectorReader2, vector3);
    serialize< const_formatter< vector_formatter< little_endian<1>, little_endian<1> > > >(vectorReader2, vector4);
    serialize< const_formatter< big_endian<4> > >(vectorReader2, 0x12345678);
    load< const_formatter< little_endian<4> > >(vectorReader2, 0x78563412);

    return 0;
}

