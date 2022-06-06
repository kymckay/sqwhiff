#include <gmock/gmock.h>

#include <fstream>

#include "./_test.h"

using sqwhiff::PreprocessingError;

// TODO: Establish method of testing column and line with one call

TEST_F(PreprocessorTest, ErrorsAtStartOfFile) {
  EXPECT_THAT([this] { preprocess("#random"); },
              ::testing::Throws<PreprocessingError>(::testing::Field(
                  &PreprocessingError::line, ::testing::Eq(1))))
      << "Error line should be 1-indexed and error should point to directive #";

  EXPECT_THAT([this] { preprocess("#random"); },
              ::testing::Throws<PreprocessingError>(
                  ::testing::Field(&PreprocessingError::col, ::testing::Eq(1))))
      << "Error column should be 1-indexed and error should point to directive "
         "#";
}

TEST_F(PreprocessorTest, DISABLED_ErrorsAtPositionInMacroBody) {
  EXPECT_THAT(
      [this] { preprocess("#define _M1(A) A\n#define _M2 _M1(2\n_M2"); },
      ::testing::Throws<PreprocessingError>(
          ::testing::Field(&PreprocessingError::line, ::testing::Eq(2))));

  EXPECT_THAT(
      [this] { preprocess("#define _M1(A) A\n#define _M2 _M1(2\n_M2"); },
      ::testing::Throws<PreprocessingError>(
          ::testing::Field(&PreprocessingError::col, ::testing::Eq(13))));
}

TEST_F(PreprocessorTest, DISABLED_ErrorsAtPositionInMacroArguments) {
  EXPECT_THAT([this] { preprocess("#define _M1(A) A\n_M1(1 + _M1(6)"); },
              ::testing::Throws<PreprocessingError>(::testing::Field(
                  &PreprocessingError::line, ::testing::Eq(2))));

  EXPECT_THAT([this] { preprocess("#define _M1(A) A\n_M1(1 + _M1(6)"); },
              ::testing::Throws<PreprocessingError>(::testing::Field(
                  &PreprocessingError::col, ::testing::Eq(9))));
}

TEST_F(PreprocessorTest, ErrorsAtPositonInIncludedFile) {
  fs::path temp_file = tmp_dir_ / "simple.inc";
  std::ofstream(temp_file) << "[1,2,3]\n #2\n";

  EXPECT_THAT([this] { preprocess("\n\n\n\n#include <./simple.inc>"); },
              ::testing::Throws<PreprocessingError>(::testing::Field(
                  &PreprocessingError::line, ::testing::Eq(2))))
      << "The error is in the included file so the line should be from the "
         "start of that file (not the file including it)";

  EXPECT_THAT([this] { preprocess("\n\n\n\n#include <./simple.inc>"); },
              ::testing::Throws<PreprocessingError>(
                  ::testing::Field(&PreprocessingError::col, ::testing::Eq(2))))
      << "The error is in the included file so the column should be from the "
         "start of that file (not the file including it)";
}
