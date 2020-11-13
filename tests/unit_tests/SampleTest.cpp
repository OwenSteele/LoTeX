#include "gtest/gtest.h"
#include <SrcFiles.h>

using namespace std;

class SampleTest : public ::testing::Test {
public:
    SampleTest() {};

    void SetUp () override { }
};

TEST_F(SampleTest, LFileObjectInit) {
    std::string path = "c:/Users/owenf/documents/createTest.txt";
    std::string expectedName = "createTest.txt";
    std::string expectedPath = "c:/Users/owenf/documents";

    LFile testFile(path, true);
    ASSERT_TRUE(expectedName == testFile.name);
    ASSERT_TRUE(expectedPath == testFile.path);
}


