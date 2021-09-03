#include "./_test.h"

// Issue #11
TEST_F(ParserTest, BindsNullaryKeywordsCorrectly)
{
    EXPECT_EQ(
        "((allunits) select (random <Dec:10>))",
        parse("allunits select random 10")
    )
    << "(nullary binary unary ATOM) is not mistaken for (unary (unary (unary ATOM)))";
}

// Relevant to test since keywords share the same token type
TEST_F(ParserTest, BindsUnaryMoreThanBinary)
{
    EXPECT_EQ(
        "((selectrandom [<Var:obj>]) getdir <Var:obj2>)",
        parse("selectrandom [obj] getdir obj2")
    )
    << "(unary ATOM binary ATOM) becomes ((unary ATOM) binary ATOM)";
}

TEST_F(ParserTest, BindsUnaryMoreThanHash)
{
    EXPECT_EQ(
        "((selectrandom [[<Dec:1>,<Dec:1>]]) # (+ <Dec:1>))",
        parse("selectrandom [[1,1]] # +1")
    )
    << "(unary ATOM # unary ATOM) becomes ((unary ATOM) # (unary ATOM))";
}

TEST_F(ParserTest, BindsHashMoreThanExponent)
{
    EXPECT_EQ(
        "(([<Dec:2>] # <Dec:0>) ^ ([<Dec:2>] # <Dec:0>))",
        parse("[2] # 0 ^ [2] # 0")
    )
    << "(ATOM # ATOM ^ ATOM # ATOM) becomes ((ATOM # ATOM) ^ (ATOM # ATOM))";
}

TEST_F(ParserTest, BindsExponentMoreThanFactor)
{
    EXPECT_EQ(
        "((<Dec:2> ^ <Dec:2>) % (<Dec:2> ^ <Dec:3>))",
        parse("2^2 % 2^3")
    )
    << R"(ATOM ^ ATOM % ATOM ^ ATOM) becomes ((ATOM ^ ATOM) % (ATOM ^ ATOM))";
}

TEST_F(ParserTest, BindsFactorMoreThanTerms)
{
    EXPECT_EQ(
        "((<Dec:1> / <Dec:4>) - (<Dec:3> * <Dec:5>))",
        parse("1 / 4 - 3 * 5")
    )
    << "(ATOM / ATOM - ATOM * ATOM) becomes ((ATOM / ATOM) - (ATOM * ATOM))";
}

// Invalid SQF, but only the precedence is significant
TEST_F(ParserTest, BindsTermsMoreThanElse)
{
    EXPECT_EQ(
        "((<Dec:1> + <Dec:2>) else (<Dec:3> - <Dec:6>))",
        parse("1 + 2 else 3 - 6")
    )
    << "(ATOM + ATOM else ATOM - ATOM) becomes ((ATOM + ATOM) else (ATOM - ATOM))";
}

// Invalid SQF, but only the precedence is significant
TEST_F(ParserTest, BindsElseMoreThanBinary)
{
    EXPECT_EQ(
        "((<Dec:1> else <Dec:2>) then (<Dec:3> else <Dec:4>))",
        parse("1 else 2 then 3 else 4")
    )
    << "(ATOM else ATOM binary ATOM else ATOM) becomes ((ATOM else ATOM) binary (ATOM else ATOM))";
}

TEST_F(ParserTest, BindsBinaryMoreThanComparison)
{
    EXPECT_EQ(
        "((<Var:obj> getdir <Var:obj2>) > (<Var:obj> getdir <Var:obj3>))",
        parse("obj getdir obj2 > obj getdir obj3")
    )
    << "(ATOM binary ATOM > ATOM binary ATOM) becomes ((ATOM binary ATOM) > (ATOM binary ATOM))";
}

TEST_F(ParserTest, BindsComparisonMoreThanConjunction)
{
    EXPECT_EQ(
        "((<Dec:1> > <Dec:2>) && (<Dec:5> < <Dec:8>))",
        parse("1 > 2 && 5 < 8")
    )
    << "(ATOM > ATOM && ATOM < ATOM) becomes ((ATOM > ATOM) && (ATOM < ATOM))";
}

TEST_F(ParserTest, BindsConjunctionMoreThanDisjunction)
{
    EXPECT_EQ(
        "(((true) && (false)) || ((false) and (true)))",
        parse("true && false || false and true")
    )
    << "(ATOM && ATOM || ATOM and ATOM) becomes ((ATOM && ATOM) || (ATOM and ATOM))";
}

TEST_F(ParserTest, BindsFactorsLeft)
{
    EXPECT_EQ(
        "(((((<Dec:1> % <Dec:2>) mod <Dec:3>) atan2 <Dec:4>) * <Dec:5>) / <Dec:6>)",
        parse("1 % 2 mod 3 atan2 4 * 5 / 6")
    )
    << "All the factor operators are left associative (same precedence)";
}

TEST_F(ParserTest, BindsTermsLeft)
{
    EXPECT_EQ(
        "((((<Dec:1> + <Dec:2>) - <Dec:3>) min <Dec:4>) max <Dec:5>)",
        parse("1 + 2 - 3 min 4 max 5")
    )
    << "All the term operators are left associative (same precedence)";
}

// Invalid SQF, but only the precedence is significant
TEST_F(ParserTest, BindsComparisonsLeft)
{
    EXPECT_EQ(
        "(((((((<Dec:1> == <Dec:2>) != <Dec:3>) > <Dec:4>) < <Dec:5>) <= <Dec:6>) >= <Dec:7>) >> <Dec:8>)",
        parse("1 == 2 != 3 > 4 < 5 <= 6 >= 7 >> 8")
    )
    << "All the comparison operators are left associative (same precedence)";
}

TEST_F(ParserTest, BindsConjunctionsLeft)
{
    EXPECT_EQ(
        "(((true) and (true)) && (true))",
        parse("true and true && true")
    )
    << "Both conjunction operators are left associative (same precedence)";
}

TEST_F(ParserTest, BindsDisjunctionsLeft)
{
    EXPECT_EQ(
        "(((true) or (true)) || (true))",
        parse("true or true || true")
    )
    << "Both disjunction operators are left associative (same precedence)";
}