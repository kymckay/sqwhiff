#include "./_test.h"

// See issue #8
TEST(Private, CanModifyAssignment)
{
    EXPECT_EQ(
        "<p=><Var:_x> = (<Dec:1>)",
        parse("private _x = 1")
    )
    << "Parse private as an assignment modifier";
}

// See issue #8
TEST(Private, CanBeUnaryWithVariable)
{
    EXPECT_EQ(
        "(private <Var:_x>)",
        parse("private _x")
    )
    << "Parse private as a unary used with a variable";
}

TEST(Private, CanAppearInArray)
{
    EXPECT_EQ(
        "[(private <Str:_x>)]",
        parse("[private \"_x\"]")
    )
    << "Parse private as a unary in an array expression";
}