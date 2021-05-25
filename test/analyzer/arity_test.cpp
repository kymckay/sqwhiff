#include "./_test.h"

TEST_F(AnalyzerTest, ReportsIncorrectUnaryUse)
{
    analyze("1 comment \"\"");
    EXPECT_EQ("1:3 SemanticError - Incorrect arity, expected unary use: comment\n", output_);
}

TEST_F(AnalyzerTest, ReportsIncorrectBinaryUse)
{
    analyze("select 2");
    EXPECT_EQ("1:1 SemanticError - Incorrect arity, expected binary use: select\n", output_);
}

TEST_F(AnalyzerTest, ReportsMultipleErrors)
{
    analyze("1 comment \"\";\nselect 1");
    EXPECT_EQ("1:3 SemanticError - Incorrect arity, expected unary use: comment\n2:1 SemanticError - Incorrect arity, expected binary use: select\n", output_);
}