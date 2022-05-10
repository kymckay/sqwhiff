#include "./_test.h"

TEST_F(PreprocessorTest, FollowsSimpleBranch) {
  EXPECT_EQ("\n\n1\n", preprocess("#define TEST\n#ifdef TEST\n1\n#endif"))
      << "The contents of the branch should appear in preprocessor output";
}

TEST_F(PreprocessorTest, SkipsSimpleBranch) {
  EXPECT_EQ("", preprocess("#ifdef TEST\n1\n#endif"))
      << "The contents of the branch should not appear in preprocessor "
         "output";
}

// TODO: The first char of else branch seems to be skipped
TEST_F(PreprocessorTest, FollowsElseBranch) {
  EXPECT_EQ("\n2\n", preprocess("#ifdef TEST\n1\n#else\n2\n#endif"))
      << "Only the contents of the else branch should appear in preprocessor "
         "output";
}

TEST_F(PreprocessorTest, SkipsElseBranch) {
  EXPECT_EQ("\n\n1\n",
            preprocess("#define TEST\n#ifdef TEST\n1\n#else\n2\n#endif"))
      << "The contents of the else branch should not appear in preprocessor "
         "output";
}

// TODO: Proper handling of #if directive requires interpretation, then remove
// this test
TEST_F(PreprocessorTest, AlwaysFollowsIfBranch) {
  EXPECT_EQ("\n1\n", preprocess("#if 0\n1\n#else\n2\n#endif"))
      << "The first if branch should always be followed until proper handling "
         "is added";
}
