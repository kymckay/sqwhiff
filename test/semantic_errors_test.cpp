#include "src/analyzer/analyzer.h"
#include <gtest/gtest.h>
#include <sstream>

TEST(Analyzer, WillReportIncorrectUnaryUse)
{
    std::stringstream input("1 comment \"\"");
    std::stringstream output("");
    Preprocessor pp(input);
    Lexer l(pp);
    Parser p(l);
    Analyzer a(p, output);
    a.analyze();

    EXPECT_EQ(output.str(), "1:4 SemanticError - Incorrect arity, expected unary use: comment\n");
}

TEST(Analyzer, WillReportIncorrectBinaryUse)
{
    std::stringstream input("select 2");
    std::stringstream output("");
    Preprocessor pp(input);
    Lexer l(pp);
    Parser p(l);
    Analyzer a(p, output);
    a.analyze();

    EXPECT_EQ(output.str(), "1:2 SemanticError - Incorrect arity, expected binary use: select\n");
}

TEST(Analyzer, WillReportMultipleErrors)
{
    std::stringstream input("1 comment \"\";\nselect 1");
    std::stringstream output("");
    Preprocessor pp(input);
    Lexer l(pp);
    Parser p(l);
    Analyzer a(p, output);
    a.analyze();

    EXPECT_EQ(output.str(), "1:4 SemanticError - Incorrect arity, expected unary use: comment\n2:1 SemanticError - Incorrect arity, expected binary use: select\n");
}