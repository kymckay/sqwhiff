#include "./_test.h"
#include <string>

#define ASSERT_EXCEPTION(TO_TRY, EXCEPTION, MSG)   \
    try                                            \
    {                                              \
        TO_TRY;                                    \
        FAIL() << "Expected an exception of type " \
               << #EXCEPTION;                      \
    }                                              \
    catch (EXCEPTION const &err)                   \
    {                                              \
        EXPECT_EQ(std::string(MSG), err.what())     \
            << "Exception message is incorrect";   \
    }

TEST(SyntaxError, ThrownOnUnclosedArray)
{
    ASSERT_EXCEPTION(parse("[1,2,3,4,5"), SyntaxError, "1:10 SyntaxError - Unexpected token '', expected a closing square bracket");
}

TEST(SyntaxError, ThrownOnUnclosedCode)
{
    ASSERT_EXCEPTION(parse("[1] select {_x == 1"), SyntaxError, "1:19 SyntaxError - Unexpected token '', expected a closing curly bracket");
}

TEST(LexicalError, DISABLED_ThrownByParser)
{
    // See #16
    std::stringstream immediate("?");
    Preprocessor pp1(immediate);
    Lexer l1(pp1);
    Parser p1(l1);

    EXPECT_THROW(p1.parse(), LexicalError) << "Immediate lexical error was not thrown out of parser";

    std::stringstream otherwise("statement;statement;\n\n\n'this is unclosed");
    Preprocessor pp2(otherwise);
    Lexer l2(pp2);
    Parser p2(l2);

    EXPECT_THROW(p2.parse(), LexicalError) << "Lexical error was not thrown out of parser during parsing";
}