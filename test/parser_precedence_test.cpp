#include "test/tester.h"
#include <gtest/gtest.h>
#include <sstream>

// Issue #11
TEST(Precedence, BindsNullaryKeywordsCorrectly)
{
    std::stringstream input("allunits select random 10");
    Lexer l(input);
    Parser p(l);
    Tester t(p);

    EXPECT_EQ(t.test(), "((allunits) select (random <Dec:10>))")
        << "(nullary binary unary ATOM) is not mistaken for (unary unary unary ATOM)";
}

TEST(Precedence, BindsUnaryHigherThanBinary)
{
    std::stringstream input("selectrandom [obj] getdir obj2");
    Lexer l(input);
    Parser p(l);
    Tester t(p);

    EXPECT_EQ(t.test(), "((selectrandom <Array:[<Var:obj>]>) getdir <Var:obj2>)")
        << "(unary ATOM binary ATOM) does not become (unary (ATOM binary ATOM))";
}
