#include "test/tester.h"
#include <gtest/gtest.h>
#include <sstream>

// See issue #8
TEST(Private, CanModifyAssignment)
{
    std::stringstream input("private _x = 1");
    Preprocessor pp(input);
    Lexer l(pp);
    Parser p(l);
    Tester t(p);

    EXPECT_EQ(t.test(), "<p=><Var:_x> = (<Dec:1>)") << "Parse private as an assignment modifier";
}

// See issue #8
TEST(Private, CanBeUnaryWithVariable)
{
    std::stringstream input("private _x");
    Preprocessor pp(input);
    Lexer l(pp);
    Parser p(l);
    Tester t(p);

    EXPECT_EQ(t.test(), "(private <Var:_x>)") << "Parse private as a unary used with a variable";
}

TEST(Private, CanAppearInArray)
{
    std::stringstream input("[private \"_x\"]");
    Preprocessor pp(input);
    Lexer l(pp);
    Parser p(l);
    Tester t(p);

    EXPECT_EQ(t.test(), "<Array:[(private <Str:_x>)]>") << "Parse private as a unary in an array expression";
}