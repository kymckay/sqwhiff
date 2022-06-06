#include <gmock/gmock.h>

#include <fstream>

#include "./_test.h"

using sqwhiff::PreprocessingError;

// Any line starting with # (ignoring leading spaces) is taken to be a directive
TEST_F(PreprocessorTest, ErrorsOnInvalidDirective) {
  // Can't use SQF hash selection operator at line start due to preprocessing
  EXPECT_THAT([this] { preprocess("[1,2,3]\n #2"); },
              ::testing::ThrowsMessage<PreprocessingError>(
                  "Invalid preprocessor directive, "
                  "no instruction immediately following the # character"));
}

TEST_F(PreprocessorTest, ErrorsOnUnrecognisedDirective) {
  EXPECT_THAT(
      [this] { preprocess("#random"); },
      ::testing::ThrowsMessage<PreprocessingError>("Unrecognised preprocessor "
                                                   "directive '#random'"));
}

TEST_F(PreprocessorTest, ErrorsOnInvalidMacroID) {
  EXPECT_THAT(
      [this] { preprocess("#define 3 body"); },
      ::testing::ThrowsMessage<PreprocessingError>(
          "Macro ID must start with an alpha character or _, found '3'"));
}

TEST_F(PreprocessorTest, ErrorsOnInvalidMacroParamID) {
  EXPECT_THAT([this] { preprocess("#define _M(A,B,3) body"); },
              ::testing::ThrowsMessage<PreprocessingError>(
                  "Macro parameter ID must start "
                  "with an alpha character or _, found '3'"));
}

TEST_F(PreprocessorTest, ErrorsOnUnclosedMacroArguments) {
  EXPECT_THAT([this] { preprocess("#define _M(A) A\n_M(anythinggoeshere\n"); },
              ::testing::ThrowsMessage<PreprocessingError>(
                  "Unclosed macro arguments '_M('"));
}

TEST_F(PreprocessorTest, ErrorsOnMismatchedMacroArguments) {
  EXPECT_THAT([this] { preprocess("#define _M(A) A\n_M(1,2)"); },
              ::testing::ThrowsMessage<PreprocessingError>(
                  "Invalid number of macro arguments for '_M' supplied, found "
                  "2, expected 1"));
}

TEST_F(PreprocessorTest, DISABLED_ErrorsOnInvalidUndefineMacroID) {
  EXPECT_THAT(
      [this] { preprocess("#undef 3"); },
      ::testing::ThrowsMessage<PreprocessingError>(
          "Macro ID must start with an alpha character or _, found '3'"));
}

// TODO: Check if mixed nesting should work (I suspect not)
TEST_F(PreprocessorTest, ErrorsOnNestedBranching) {
  EXPECT_THAT([this] { preprocess("#ifdef ONE\n#ifdef TWO\n#endif\n#endif"); },
              ::testing::ThrowsMessage<PreprocessingError>(
                  "Cannot nest #ifdef directives"));
  EXPECT_THAT(
      [this] { preprocess("#ifndef ONE\n#ifndef TWO\n#endif\n#endif"); },
      ::testing::ThrowsMessage<PreprocessingError>(
          "Cannot nest #ifndef directives"));
}

TEST_F(PreprocessorTest, ErrorsOnLoneElse) {
  EXPECT_THAT([this] { preprocess("#else\n#endif"); },
              ::testing::ThrowsMessage<PreprocessingError>(
                  "Cannot use #else with no preceeding #if, #ifdef or #ifndef "
                  "directive"));
}

TEST_F(PreprocessorTest, ErrorsOnMalformedInclude) {
  EXPECT_THAT([this] { preprocess("#include unquoted.sqf"); },
              ::testing::ThrowsMessage<PreprocessingError>(
                  "Malformed #include directive: unquoted.sqf"));

  EXPECT_THAT([this] { preprocess("#include [misquoted.sqf]"); },
              ::testing::ThrowsMessage<PreprocessingError>(
                  "Malformed #include directive: [misquoted.sqf]"));
}

TEST_F(PreprocessorTest, ErrorsOnMissingInclude) {
  EXPECT_THAT([this] { preprocess("#include \"missing.sqf\""); },
              ::testing::ThrowsMessage<PreprocessingError>(
                  "Included file not found: missing.sqf"));
}

TEST_F(PreprocessorTest, ErrorsOnBadInternalDirectory) {
  fs::remove_all(tmp_dir_ / "internal");

  EXPECT_THAT([this] { preprocess("#include <\\internal.inc>"); },
              ::testing::ThrowsMessage<PreprocessingError>(
                  "Invalid internal directory given to find included file: "
                  "\\internal.inc"));
}

// This will actually crash the RV engine
TEST_F(PreprocessorTest, ErrorsOnRecursiveInclusion) {
  fs::path temp_file = tmp_dir_ / "meta.inc";
  std::ofstream(temp_file) << "#include <./meta.inc>";

  EXPECT_THAT([this] { preprocess("#include <./meta.inc>"); },
              ::testing::ThrowsMessage<PreprocessingError>(
                  "Recursive inclusion of file: ./meta.inc"));
}

TEST_F(PreprocessorTest, ErrorsOnNestedRecursiveInclusion) {
  fs::path temp_file = tmp_dir_ / "meta1.inc";
  std::ofstream(temp_file) << "#include <./meta2.inc>";

  temp_file = tmp_dir_ / "meta2.inc";
  std::ofstream(temp_file) << "#include <./meta1.inc>";

  EXPECT_THAT([this] { preprocess("#include <./meta1.inc>"); },
              ::testing::ThrowsMessage<PreprocessingError>(
                  "Recursive inclusion of file: ./meta1.inc"));
}

// TODO: Test bad inclusion of a binary file
