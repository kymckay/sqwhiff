#include "./_test.h"
#include <string>

#define ASSERT_EXCEPTION(TO_TRY, EXCEPTION, MSG)   \
    try                                            \
    {                                              \
        TO_TRY;                                    \
        FAIL() << "Expected an exception of type " \
        << #EXCEPTION;                             \
    }                                              \
    catch (EXCEPTION const &err)                   \
    {                                              \
        EXPECT_EQ(std::string(MSG), err.what())    \
        << "Exception message is incorrect";       \
    }

TEST_F(LexerTest, NoErrorOnInstantiation)
{
    EXPECT_NO_THROW(
        std::stringstream ss("#random");
        Preprocessor pp(ss);
        Lexer l(pp);
    )
    << "Lexer instantiation logic should not expose preprocessing exceptions";
}

TEST_F(LexerTest, ErrorsOnUnexpectedChar)
{
    ASSERT_EXCEPTION(tokenize("?"), LexicalError, "1:1 LexicalError - Unexpected character '?'");
}

TEST_F(LexerTest, ErrorsOnUnclosedStr)
{
    ASSERT_EXCEPTION(tokenize("\""), LexicalError, "1:1 LexicalError - Unclosed string");
}

TEST_F(LexerTest, ErrorsOnIncompleteSciNotation)
{
    ASSERT_EXCEPTION(tokenize("1e+"), LexicalError, "1:1 LexicalError - Unfinished numeric literal '1e+'");
}
