#include "./_test.h"

// Issue #11
TEST(Precedence, BindsNullaryKeywordsCorrectly)
{
    EXPECT_EQ(
        "((allunits) select (random <Dec:10>))",
        parse("allunits select random 10")
    )
    << "(nullary binary unary ATOM) is not mistaken for (unary (unary (unary ATOM)))";
}

// Relevant to test since keywords share the same token type
TEST(Precedence, BindsUnaryMoreThanBinary)
{
    EXPECT_EQ(
        "((selectrandom [<Var:obj>]) getdir <Var:obj2>)",
        parse("selectrandom [obj] getdir obj2")
    )
    << "(unary ATOM binary ATOM) becomes ((unary ATOM) binary ATOM)";
}

TEST(Precedence, BindsUnaryMoreThanHash)
{
    EXPECT_EQ(
        "((selectrandom [[<Dec:1>,<Dec:1>]]) # (+ <Dec:1>))",
        parse("selectrandom [[1,1]] # +1")
    )
    << "(unary ATOM # unary ATOM) becomes ((unary ATOM) # (unary ATOM))";
}

TEST(Precedence, BindsHashMoreThanExponent)
{
    EXPECT_EQ(
        "(([<Dec:2>] # <Dec:0>) ^ ([<Dec:2>] # <Dec:0>))",
        parse("[2] # 0 ^ [2] # 0")
    )
    << "(ATOM # ATOM ^ ATOM # ATOM) becomes ((ATOM # ATOM) ^ (ATOM # ATOM))";
}

TEST(Precedence, BindsExponentMoreThanFactor)
{
    EXPECT_EQ(
        "((<Dec:2> ^ <Dec:2>) % (<Dec:2> ^ <Dec:3>))",
        parse("2^2 % 2^3")
    )
    << R"(ATOM ^ ATOM % ATOM ^ ATOM) becomes ((ATOM ^ ATOM) % (ATOM ^ ATOM))";
}

TEST(Precedence, BindsAllFactorsEqually)
{
    EXPECT_EQ(
        "(((((<Dec:1> % <Dec:2>) mod <Dec:3>) atan2 <Dec:4>) * <Dec:5>) / <Dec:6>)",
        parse("1 % 2 mod 3 atan2 4 * 5 / 6")
    )
    << "All the factor operators are left associative (same precedence)";
}

TEST(Precedence, BindsFactorMoreThanTerms)
{
    EXPECT_EQ(
        "((<Dec:1> / <Dec:4>) - (<Dec:3> * <Dec:5>))",
        parse("1 / 4 - 3 * 5")
    )
    << "(ATOM / ATOM - ATOM * ATOM) becomes ((ATOM / ATOM) - (ATOM * ATOM))";
}