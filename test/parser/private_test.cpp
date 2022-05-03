#include "./_test.h"

// See issue #8
TEST_F(ParserTest, HandlesPrivateAssignment) {
  EXPECT_EQ("<p=><Var:_x> = (<Dec:1>)", parse("private _x = 1"))
      << "Parse private as an assignment modifier";
}

// See issue #8
TEST_F(ParserTest, HandlesPrivateAsUnaryOp) {
  EXPECT_EQ("(private <Var:_x>)", parse("private _x"))
      << "Parse private as a unary used with a variable";
}
