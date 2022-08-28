#include "tests/parser/_test.hpp"

TEST_F(ParserTest, HandlesIfStructure) {
  EXPECT_EQ("((if <Var:condition>) then {(true)})",
            parse("if (condition) then {true}"));
}

TEST_F(ParserTest, HandlesIfElseStructure) {
  EXPECT_EQ("((if <Var:condition>) then ({(true)} else {(false)}))",
            parse("if (condition) then {true} else {false}"));
}

TEST_F(ParserTest, HandlesIfArrayStructure) {
  EXPECT_EQ("((if <Var:condition>) then [{(true)},{(false)}])",
            parse("if (condition) then [{true},{false}]"));
}

TEST_F(ParserTest, HandlesWhileLoop) {
  EXPECT_EQ("((while {<Var:condition>}) do {<NoOp>})",
            parse("while {condition} do {}"));
}

TEST_F(ParserTest, HandlesForLoop) {
  EXPECT_EQ(
      "((for [{<Var:_i> = (<Dec:0>)},{(<Var:_i> < <Dec:10>)},{<Var:_i> = "
      "((<Var:_i> + <Dec:1>))}]) do {<NoOp>})",
      parse("for [{_i = 0}, {_i < 10}, {_i = _i+1}] do {}"));
}

TEST_F(ParserTest, HandlesForFromToLoop) {
  EXPECT_EQ("((((for <Str:_i>) from <Dec:0>) to <Dec:10>) do {<NoOp>})",
            parse("for \"_i\" from 0 to 10 do {}"));
}

TEST_F(ParserTest, HandlesForFromToStepLoop) {
  EXPECT_EQ(
      "(((((for <Str:_i>) from <Dec:0>) to <Dec:10>) step <Dec:2>) do "
      "{<NoOp>})",
      parse("for \"_i\" from 0 to 10 step 2 do {}"));
}

TEST_F(ParserTest, HandlesForEachLoop) {
  EXPECT_EQ("({<NoOp>} foreach [<Dec:1>,<Dec:2>,<Dec:3>,<Dec:4>])",
            parse("{} foreach [1,2,3,4]"));
}
