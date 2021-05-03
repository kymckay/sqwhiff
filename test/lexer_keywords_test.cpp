#include "test/tester.h"
#include <gtest/gtest.h>
#include <sstream>

TEST(Keywords, AreCaseInsensitive)
{
    std::stringstream nullary("aLLuNiTs");
    std::stringstream unary("cOmMeNt");
    std::stringstream binary("SelECT");
    std::stringstream priv("PRIvate");

    Lexer l1(nullary);
    EXPECT_EQ(l1.nextToken().type, TokenType::nullary) << "Ignore case of nullary keywords";

    Lexer l2(unary);
    EXPECT_EQ(l2.nextToken().type, TokenType::keyword) << "Ignore case of unary keywords";

    Lexer l3(binary);
    EXPECT_EQ(l3.nextToken().type, TokenType::keyword) << "Ignore case of binary keywords";

    Lexer l4(priv);
    EXPECT_EQ(l4.nextToken().type, TokenType::private_op) << "Ignore case of private keyword";
}
