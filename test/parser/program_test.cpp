#include "test/parser/_test.h"
#include <gtest/gtest.h>

// Issue #10
TEST(Program, CanEndWithoutFinalDelimiter)
{
    EXPECT_EQ(
        "(allunits),(allunits)",
        parse("allunits; allunits")
    )
    << "Final statement can have no delimiter";
}

// Issue #10
TEST(Program, CanEndWithFinalDelimiter)
{
    EXPECT_EQ(
        "(allunits),(allunits),<NoOp>",
        parse("allunits; allunits;")
    )
    << "Final (real) statement can have a delimiter";
}

// Issue #10
TEST(Program, CanBeEmpty)
{
    EXPECT_EQ(
        "<NoOp>",
        parse("")
    )
    << "Program can contain no (real) statements at all";
}