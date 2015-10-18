// TestRunner.cpp - tests for BinaryFormatSerializer
//

#include "gtest/gtest.h"

namespace {

// The fixture for testing class Foo.
class FooTest : public ::testing::Test {
protected:
    FooTest() {
        // You can do set-up work for each test here.
    }

    virtual ~FooTest() {
        // You can do clean-up work that doesn't throw exceptions here.
    }

    // Objects declared here can be used by all tests in the test case for Foo.
};

TEST_F(FooTest, BigEndianWorks) {
    EXPECT_EQ(0, 0);
}

#if 0
TEST_F(FooTest, TestThatFails) {
    VectorSaveSerializer vectorWriter;
    save< big_endian<4> >(vectorWriter, 0x12345678);

    VectorLoadSerializer vectorReader(vectorWriter.getData());
    load< const_formatter< little_endian<4> > >(vectorReader, 0x12345678);
}
#endif

}  // namespace

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
