#include "./_test.h"

TEST_F(PreprocessorTest, CanBeInline) {
  EXPECT_EQ("selectRandom\n[1,2]",
            preprocess("selectRandom//comment \"test\"\n[1,2]"))
      << "Inline comments should not be in the preprocessor output";
}

TEST_F(PreprocessorTest, CanBeBlock) {
  EXPECT_EQ("selectRandom  [1,2]",
            preprocess("selectRandom /* //?#[20] */ [1,2]"))
      << "Block comments should not be in the preprocessor output";
}

TEST_F(PreprocessorTest, AreRemovedInPreprocessing) {
  EXPECT_EQ("selectRandom [1,2]",
            preprocess("select/* why would you do this */Random [1,2]"))
      << "Block comments should not delimit tokens";
}

TEST_F(PreprocessorTest, AreConsumedByDoubleQuotes) {
  EXPECT_EQ("\"/* this isn't a comment */\"",
            preprocess("\"/* this isn't a comment */\""))
      << "Comments within double quotes should appear in preprocessor output";
}

// The preprocessor ignores single quotes
TEST_F(PreprocessorTest, AreRemovedInSingleQuotes) {
  EXPECT_EQ("''", preprocess("\'/* this is a comment */\'"))
      << "Comments within single quotes should not appear in preprocessor "
         "output";
}

// This is consistent with being removed during preprocessing
TEST_F(PreprocessorTest, AreRemovedInBalancedQuotes) {
  EXPECT_EQ("\"\"\"\"", preprocess("\"\"/* this is a comment */\"\""))
      << "Comments within balanced sets of double quotes should not appear in "
         "preprocessor output";
}
