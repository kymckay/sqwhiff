#include "./_test.h"

TEST_F(AnalyzerTest, OutputsLowerErrors)
{
    // Case with preprocessor error
    analyze("#random");
    EXPECT_EQ(1, errorc_);
    EXPECT_EQ("1:1 PreprocessingError - Unrecognised preprocessor directive '#random'", output_);

    // Case with lexical error
    analyze("?");
    EXPECT_EQ(1, errorc_);
    EXPECT_EQ("1:1 LexicalError - Unexpected character '?'", output_);

    // Case with parsing error
    analyze("[1,");
    EXPECT_EQ(1, errorc_);
    EXPECT_EQ("1:3 SyntaxError - Unexpected token '', expected a variable, literal or expression", output_);
}
