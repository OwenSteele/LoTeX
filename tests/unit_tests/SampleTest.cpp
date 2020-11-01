#include "gtest/gtest.h"

using namespace std;

class SampleTest : public ::testing::Test {
public:
    SampleTest() {};

    void SetUp () override {

    }

};

TEST_F(SampleTest, DeviceIdIsCorrectlySet) {
    auto const expected = 2;
    auto const result = 1 + 1;
    EXPECT_EQ(expected, result);
}
