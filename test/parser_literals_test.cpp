#include "test/tester.h"
#include <gtest/gtest.h>
#include <sstream>

// Issue #18
TEST(ArrayDisplay, CanBeEmpty)
{
    std::stringstream input("[]");
    Lexer l(input);
    Parser p(l);
    Tester t(p);

    EXPECT_EQ(t.test(), "<Array:[]>") << "Empty array produced";
}

// Issue #18
TEST(CodeDisplay, CanBeEmpty)
{
    std::stringstream input("{}");
    Lexer l(input);
    Parser p(l);
    Tester t(p);

    EXPECT_EQ(t.test(), "<Code:{<NoOp>}>") << "Empty code produced";
}