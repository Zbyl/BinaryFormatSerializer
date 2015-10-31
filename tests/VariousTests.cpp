// VariousTests.cpp - tests for BinaryFormatSerializer
//

#include "VectorSerializer.h"
#include "ArraySerializer.h"

#include "endian_formatter.h"
#include "const_formatter.h"
#include "size_prefix_formatter.h"

#include "gtest/gtest.h"

namespace {

using namespace binary_format;
    
TEST(ConstFormatterWorks, SavingAndLoading)
{
    {
        VectorSaveSerializer vectorWriter;
        save< const_formatter< little_endian<4> > >(vectorWriter, 0x12345678);
        const auto value = std::vector<uint8_t> { 0x78, 0x56, 0x34, 0x12 };
        EXPECT_EQ(vectorWriter.getData(), value);
    }

    {
        std::vector<uint8_t> data { 0x12, 0x34, 0x56, 0x78 };
        VectorLoadSerializer vectorReader(data);
        EXPECT_NO_THROW( load< const_formatter< little_endian<4> > >(vectorReader, 0x78563412) );
    }

    {
        std::vector<uint8_t> data { 0x12, 0x34, 0x56, 0x78 };
        VectorLoadSerializer vectorReader(data);
        ASSERT_THROW(load< const_formatter< little_endian<4> > >(vectorReader, 0), invalid_data);
    }
}

TEST(SizePrefixFormatterWorks, SavingAndLoading)
{
    {
        std::array<uint8_t, 8> data;
        ArraySaveSerializer vectorWriter(data);
        save< size_prefix_formatter< little_endian<4>, little_endian<4> > >(vectorWriter, 0x12345678);
        const auto value = std::vector<uint8_t> { 0x04, 0x00, 0x00, 0x00, 0x78, 0x56, 0x34, 0x12 };
        ASSERT_TRUE(( std::equal(value.begin(), value.end(), vectorWriter.getData()) ));
    }

    {
        uint8_t data[] = { 0x04, 0x00, 0x00, 0x00, 0x78, 0x56, 0x34, 0x12 };
        ArrayLoadSerializer arrayReader(data, sizeof(data));
        int value;
        load< size_prefix_formatter< little_endian<4>, little_endian<4> > >(arrayReader, value);
        EXPECT_EQ(value, 0x12345678);
    }

    {
        uint8_t data[] = { 0x12, 0x34, 0x56, 0x78 };
        ArrayLoadSerializer arrayReader(data, sizeof(data));
        int value;
        ASSERT_THROW((load< size_prefix_formatter< little_endian<2>, little_endian<2> > >(arrayReader, value)), invalid_data);
    }
}

}  // namespace
