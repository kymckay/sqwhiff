#include "./_test.h"

TEST_F(PreprocessorTest, CanPeekMultiple)
{
    EXPECT_EQ(
        '6',
        peek("123456", 6).c
    )
    << "Peeking ahead should return the character the corresponding number of indexes ahead";
}
