#include "gtest/gtest.h"
#include <SrcFiles.h>

using namespace std;

class SampleTest : public ::testing::Test {
public:
    SampleTest() {};

    void SetUp () override { }
};

TEST_F(SampleTest, LFileObjectInit) {
    std::string path = "./createTest.txt";
    std::string expectedName = "createTest.txt";
    std::string expectedPath = "./";

    LFile testFile(path);
    ASSERT_TRUE(expectedName == testFile.name);
    ASSERT_TRUE(expectedPath == testFile.path);
}
TEST_F(SampleTest, EmptyStyleCalled) {
    Styles testStyle{"tStyle"};
    EXPECT_EQ(testStyle.name, GetStyleByName(""));
}
TEST_F(SampleTest, CreateDefaultFileStyles) {
    CreateDefaultStyles();
    ASSERT_TRUE(fileStyles.size() == 5);
    CreateDefaultStyles();
    ASSERT_TRUE(fileStyles.size() == 5);
    ASSERT_TRUE(fileStyles[0].text_align == "");
}
TEST_F(SampleTest, InlineStatementsFormat){
    std::string path = "./createTestISFt.txt";
    std::string pubFileText;
    std::string line;
    LFile testFile(path);
    testFile.content.emplace_back("{link test, https://owensteele.co.uk}");
    testFile.content.emplace_back("{image , https://owensteele.co.uk/pictures/me2.PNG}");
    testFile.Publish();
    std::ifstream pubTestFile ("./createTestISFt.html", std::ios_base::app);
    while (getline(pubTestFile,line))
    {
        pubFileText += line;
    }
    ASSERT_TRUE(pubFileText.find("<a href=\"https://owensteele.co.uk\">test</a>") != std::string::npos);
    ASSERT_TRUE(pubFileText.find("<img src=\"https://owensteele.co.uk/pictures/me2.PNG\"") != std::string::npos);
}
TEST_F(SampleTest, MenuCall)
{
    EXPECT_DEATH(exitCalled = true; CodeMain(), "Exit called in while loop, exited base function.");
}

