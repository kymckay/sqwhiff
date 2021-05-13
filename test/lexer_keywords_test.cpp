#include "test/tester.h"
#include <gtest/gtest.h>
#include <sstream>

TEST(Keywords, AreCaseInsensitive)
{
    std::stringstream nullary("aLLuNiTs");
    std::stringstream unary("cOmMeNt");
    std::stringstream binary("SelECT");
    std::stringstream priv("PRIvate");

    Preprocessor pp1(nullary);
    Lexer l1(pp1);
    EXPECT_EQ(l1.nextToken().type, TokenType::nullary) << "Ignore case of nullary keywords";

    Preprocessor pp2(unary);
    Lexer l2(pp2);
    EXPECT_EQ(l2.nextToken().type, TokenType::keyword) << "Ignore case of unary keywords";

    Preprocessor pp3(binary);
    Lexer l3(pp3);
    EXPECT_EQ(l3.nextToken().type, TokenType::keyword) << "Ignore case of binary keywords";

    Preprocessor pp4(priv);
    Lexer l4(pp4);
    EXPECT_EQ(l4.nextToken().type, TokenType::private_op) << "Ignore case of private keyword";
}
