#include <fstream>
#include <string>

#include "./_test.h"

#define ASSERT_EXCEPTION(TO_TRY, EXCEPTION, MSG)              \
  try {                                                       \
    TO_TRY;                                                   \
    FAIL() << "Expected an exception of type " << #EXCEPTION; \
  } catch (EXCEPTION const& err) {                            \
    EXPECT_EQ(std::string(MSG), err.what())                   \
        << "Exception message is incorrect";                  \
  }

// Any line starting with # (ignoring leading spaces) is taken to be a directive
TEST_F(PreprocessorTest, ErrorsOnInvalidDirective) {
  // Can't use SQF hash selection operator at line start due to preprocessing
  ASSERT_EXCEPTION(preprocess("[1,2,3]\n #2"), PreprocessingError,
                   "2:2 PreprocessingError - Invalid preprocessor directive, "
                   "no instruction immediately following the # character");
}

TEST_F(PreprocessorTest, ErrorsOnUnrecognisedDirective) {
  ASSERT_EXCEPTION(preprocess("#random"), PreprocessingError,
                   "1:1 PreprocessingError - Unrecognised preprocessor "
                   "directive '#random'");
}

TEST_F(PreprocessorTest, ErrorsOnInvalidMacroID) {
  ASSERT_EXCEPTION(preprocess("#define 3 body"), PreprocessingError,
                   "1:9 PreprocessingError - Macro ID must start with an "
                   "alpha character or _, found '3'");
}

TEST_F(PreprocessorTest, ErrorsOnInvalidMacroParamID) {
  ASSERT_EXCEPTION(preprocess("#define _M(A,B,3) body"), PreprocessingError,
                   "1:16 PreprocessingError - Macro parameter ID must start "
                   "with an alpha character or _, found '3'");
}

TEST_F(PreprocessorTest, ErrorsOnUnclosedMacroArguments) {
  ASSERT_EXCEPTION(preprocess("#define _M(A) A\n_M(anythinggoeshere\n"),
                   PreprocessingError,
                   "2:3 PreprocessingError - Unclosed macro arguments '_M('");
}

TEST_F(PreprocessorTest, ErrorsOnMismatchedMacroArguments) {
  ASSERT_EXCEPTION(preprocess("#define _M(A) A\n_M(1,2)"), PreprocessingError,
                   "2:1 PreprocessingError - Invalid number of macro "
                   "arguments for '_M' supplied, found 2, expected 1");
}

TEST_F(PreprocessorTest, DISABLED_ErrorsAtPositionInMacroBody) {
  ASSERT_EXCEPTION(preprocess("#define _M1(A) A\n#define _M2 _M1(2\n_M2"),
                   PreprocessingError,
                   "2:13 PreprocessingError - Unclosed macro arguments '_M1('");
}

TEST_F(PreprocessorTest, DISABLED_ErrorsAtPositionInMacroArguments) {
  ASSERT_EXCEPTION(preprocess("#define _M1(A) A\n_M1(1 + _M1(6)"),
                   PreprocessingError,
                   "2:9 PreprocessingError - Unclosed macro arguments '_M1('");
}

TEST_F(PreprocessorTest, DISABLED_ErrorsOnInvalidUndefineMacroID) {
  ASSERT_EXCEPTION(preprocess("#undef 3"), PreprocessingError,
                   "1:8 PreprocessingError - Macro ID must start with an "
                   "alpha character or _, found '3'");
}

// TODO: Check if mixed nesting should work (I suspect not)
TEST_F(PreprocessorTest, ErrorsOnNestedBranching) {
  ASSERT_EXCEPTION(preprocess("#ifdef ONE\n#ifdef TWO\n#endif\n#endif"),
                   PreprocessingError,
                   "2:1 PreprocessingError - Cannot nest #ifdef directives");
  ASSERT_EXCEPTION(preprocess("#ifndef ONE\n#ifndef TWO\n#endif\n#endif"),
                   PreprocessingError,
                   "2:1 PreprocessingError - Cannot nest #ifndef directives");
}

TEST_F(PreprocessorTest, ErrorsOnLoneElse) {
  ASSERT_EXCEPTION(preprocess("#else\n#endif"), PreprocessingError,
                   "1:1 PreprocessingError - Cannot use #else with no "
                   "preceeding #if, #ifdef or #ifndef directive");
}

TEST_F(PreprocessorTest, ErrorsOnMalformedInclude) {
  ASSERT_EXCEPTION(
      preprocess("#include unquoted.sqf"), PreprocessingError,
      "1:10 PreprocessingError - Malformed #include directive: unquoted.sqf");

  ASSERT_EXCEPTION(preprocess("#include [misquoted.sqf]"), PreprocessingError,
                   "1:10 PreprocessingError - Malformed #include directive: "
                   "[misquoted.sqf]");
}

TEST_F(PreprocessorTest, ErrorsOnMissingInclude) {
  ASSERT_EXCEPTION(
      preprocess("#include \"missing.sqf\""), PreprocessingError,
      "1:10 PreprocessingError - Included file not found: missing.sqf");
}

TEST_F(PreprocessorTest, ErrorsAtPositonInIncludedFile) {
  fs::path temp_file = tmp_dir_ / "simple.inc";
  std::ofstream(temp_file) << "[1,2,3]\n #2\n";

  ASSERT_EXCEPTION(preprocess("\n\n\n\n#include <./simple.inc>"),
                   PreprocessingError,
                   "2:2 PreprocessingError - Invalid preprocessor directive, "
                   "no instruction immediately following the # character");
}

TEST_F(PreprocessorTest, ErrorsOnBadInternalDirectory) {
  fs::remove_all(tmp_dir_ / "internal");

  ASSERT_EXCEPTION(preprocess("#include <\\internal.inc>"), PreprocessingError,
                   "1:10 PreprocessingError - Invalid internal directory given "
                   "to find included file: \\internal.inc");
}

// This will actually crash the RV engine
TEST_F(PreprocessorTest, ErrorsOnRecursiveInclusion) {
  fs::path temp_file = tmp_dir_ / "meta.inc";
  std::ofstream(temp_file) << "#include <./meta.inc>";

  ASSERT_EXCEPTION(
      preprocess("#include <./meta.inc>"), PreprocessingError,
      "1:10 PreprocessingError - Recursive inclusion of file: ./meta.inc");
}

TEST_F(PreprocessorTest, ErrorsOnNestedRecursiveInclusion) {
  fs::path temp_file = tmp_dir_ / "meta1.inc";
  std::ofstream(temp_file) << "#include <./meta2.inc>";

  temp_file = tmp_dir_ / "meta2.inc";
  std::ofstream(temp_file) << "#include <./meta1.inc>";

  ASSERT_EXCEPTION(
      preprocess("#include <./meta1.inc>"), PreprocessingError,
      "1:10 PreprocessingError - Recursive inclusion of file: ./meta1.inc");
}

// TODO: Test bad inclusion of a binary file
