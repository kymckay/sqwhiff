#include "test/tester.h"
#include <gtest/gtest.h>
#include <sstream>

// Issue #10
TEST(Statements, CanEndWithoutFinalDelimiter)
{
    std::stringstream input("allunits; allunits");
    Lexer l(input);
    Parser p(l);
    Tester t(p);

    EXPECT_EQ(t.test(), "(allunits),(allunits)")
        << "final statement can have no delimiter";
}

// Issue #10
TEST(Statements, CanEndWithFinalDelimiter)
{
    std::stringstream input("allunits; allunits;");
    Lexer l(input);
    Parser p(l);
    Tester t(p);

    EXPECT_EQ(t.test(), "(allunits),(allunits)")
        << "final statement can have a delimiter";
}