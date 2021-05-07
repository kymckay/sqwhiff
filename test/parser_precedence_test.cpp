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
        << "(nullary binary unary ATOM) is not mistaken for (unary (unary (unary ATOM)))";
}

// Relevant to test since keywords share the same token type
TEST(Precedence, BindsUnaryMoreThanBinary)
{
    std::stringstream input("selectrandom [obj] getdir obj2");
    Lexer l(input);
    Parser p(l);
    Tester t(p);

    EXPECT_EQ(t.test(), "((selectrandom <Array:[<Var:obj>]>) getdir <Var:obj2>)")
        << "(unary ATOM binary ATOM) becomes ((unary ATOM) binary ATOM)";
}

TEST(Precedence, BindsUnaryMoreThanHash)
{
    std::stringstream input("selectrandom [[1,1]] # +1");
    Lexer l(input);
    Parser p(l);
    Tester t(p);

    EXPECT_EQ(t.test(), "((selectrandom <Array:[<Array:[<Dec:1>,<Dec:1>]>]>) # (+ <Dec:1>))")
        << "(unary ATOM # unary ATOM) becomes ((unary ATOM) # (unary ATOM))";
}

TEST(Precedence, BindsHashMoreThanExponent)
{
    std::stringstream input("[2] # 0 ^ [2] # 0");
    Lexer l(input);
    Parser p(l);
    Tester t(p);

    EXPECT_EQ(t.test(), "((<Array:[<Dec:2>]> # <Dec:0>) ^ (<Array:[<Dec:2>]> # <Dec:0>))")
        << "(ATOM # ATOM ^ ATOM # ATOM) becomes ((ATOM # ATOM) ^ (ATOM # ATOM))";
}

TEST(Precedence, BindsExponentMoreThanFactor)
{
    std::stringstream input("2^2 % 2^3");
    Lexer l(input);
    Parser p(l);
    Tester t(p);

    EXPECT_EQ(t.test(), "((<Dec:2> ^ <Dec:2>) % (<Dec:2> ^ <Dec:3>))")
        << R"(ATOM ^ ATOM % ATOM ^ ATOM) becomes ((ATOM ^ ATOM) % (ATOM ^ ATOM))";
}

TEST(Precedence, BindsAllFactorsEqually)
{
    std::stringstream input("1 % 2 mod 3 atan2 4 * 5 / 6");
    Lexer l(input);
    Parser p(l);
    Tester t(p);

    EXPECT_EQ(t.test(), "(((((<Dec:1> % <Dec:2>) mod <Dec:3>) atan2 <Dec:4>) * <Dec:5>) / <Dec:6>)")
        << "All the factor operators are left associative (same precedence)";
}

TEST(Precedence, BindsFactorMoreThanTerms)
{
    std::stringstream input("1 / 4 - 3 * 5");
    Lexer l(input);
    Parser p(l);
    Tester t(p);

    EXPECT_EQ(t.test(), "((<Dec:1> / <Dec:4>) - (<Dec:3> * <Dec:5>))")
        << "(ATOM / ATOM - ATOM * ATOM) becomes ((ATOM / ATOM) - (ATOM * ATOM))";
}