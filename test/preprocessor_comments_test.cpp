#include "test/tester.h"
#include <gtest/gtest.h>
#include <sstream>

TEST(Comments, CanBeInline)
{
    std::stringstream input("selectrandom//comment \"test\"\n[1,2]");
    Preprocessor pp(input);
    Lexer l(pp);
    Parser p(l);
    Tester t(p);

    EXPECT_EQ(t.test(), "(selectrandom <Array:[<Dec:1>,<Dec:2>]>)")
        << "Inline comment is ignored";
}

TEST(Comments, CanBeBlock)
{
    std::stringstream input("selectrandom /* //?#[20] */ [1,2]");
    Preprocessor pp(input);
    Lexer l(pp);
    Parser p(l);
    Tester t(p);

    EXPECT_EQ(t.test(), "(selectrandom <Array:[<Dec:1>,<Dec:2>]>)")
        << "Block comment is ignored";
}

TEST(Comments, AreRemovedInPreprocessing)
{
    std::stringstream input("select/* why would you do this */random [1,2]");
    Preprocessor pp(input);
    Lexer l(pp);
    Parser p(l);
    Tester t(p);

    EXPECT_EQ(t.test(), "(selectrandom <Array:[<Dec:1>,<Dec:2>]>)")
        << "Block comment does not delimit tokens";
}

TEST(Comments, AreConsumedByStringLiterals)
{
    std::stringstream input("\"/* this isn't a comment */\"");
    Preprocessor pp(input);
    Lexer l(pp);
    Parser p(l);
    Tester t(p);

    EXPECT_EQ(t.test(), "<Str:/* this isn't a comment */>")
        << "There's no such thing as a comment within a string literal";
}