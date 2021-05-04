#include "test/tester.h"
#include <gtest/gtest.h>
#include <sstream>

// Issue #10
TEST(Program, CanEndWithoutFinalDelimiter)
{
    std::stringstream input("allunits; allunits");
    Lexer l(input);
    Parser p(l);
    Tester t(p);

    EXPECT_EQ(t.test(), "(allunits),(allunits)")
        << "Final statement can have no delimiter";
}

// Issue #10
TEST(Program, CanEndWithFinalDelimiter)
{
    std::stringstream input("allunits; allunits;");
    Lexer l(input);
    Parser p(l);
    Tester t(p);

    EXPECT_EQ(t.test(), "(allunits),(allunits),<NoOp>")
        << "Final (real) statement can have a delimiter";
}

// Issue #10
TEST(Program, CanBeEmpty)
{
    std::stringstream input("");
    Lexer l(input);
    Parser p(l);
    Tester t(p);

    EXPECT_EQ(t.test(), "<NoOp>")
        << "Program can contain no (real) statements at all";
}