// VariousTests.cpp - tests for BinaryFormatSerializer
//

#include "VectorSerializer.h"

#include "endian_formatter.h"
#include "const_formatter.h"

#include "gtest/gtest.h"

namespace {

using namespace binary_format;
    
TEST(ConstFormatterWorks, SavingAndLoading)
{
    VectorSaveSerializer vectorWriter;
    save< const_formatter< little_endian<4> > >(vectorWriter, 0x12345678);
    const auto value = std::vector<uint8_t> { 0x78, 0x56, 0x34, 0x12 };
    EXPECT_EQ(vectorWriter.getData(), value);

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

}  // namespace
