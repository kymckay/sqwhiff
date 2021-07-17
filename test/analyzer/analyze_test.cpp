#include "./_test.h"

TEST_F(AnalyzerTest, ReturnsNumberOfErrors)
{
    // Case with multiple errors
    analyze("select 1; select 2; select 3;");
    EXPECT_EQ(3, errorc_);

    // Case with no errors
    analyze("[1] select 1;");
    EXPECT_EQ(0, errorc_);

    // Case with preprocessor error
    analyze("#random");
    EXPECT_EQ(1, errorc_);

    // Case with lexical error
    analyze("?");
    EXPECT_EQ(1, errorc_);

    // Case with parsing error
    analyze("[1,");
    EXPECT_EQ(1, errorc_);
}