#pragma once
#include <gtest/gtest.h>

#include <filesystem>
#include <sstream>
#include <string>

#include "sqwhiff/preprocessor/preprocessor.hpp"

namespace fs = std::filesystem;

class PreprocessorTest : public ::testing::Test {
 protected:
  static fs::path tmp_dir_;

  std::string preprocess(std::string s) {
    std::stringstream ss(s);

    // Pretend processing file in temp dir for relative path testing
    // Set internal directory for internal path testing
    Preprocessor pp(ss, tmp_dir_ / "fake.sqf", tmp_dir_ / "internal");

    char c = pp.get();
    std::string result = "";
    while (c != '\0') {
      result.push_back(c);
      c = pp.get();
    }

    return result;
  }

  PosChar peek(std::string s, int i) {
    std::stringstream ss(s);
    return Preprocessor(ss).peek(i);
  }

  //  Ensure unique temporary directory exists for creation of test files
  static void SetUpTestSuite() { fs::create_directory(tmp_dir_); }

  // Don't leave files around to ensure consistent testing environment
  static void TearDownTestSuite() { fs::remove_all(tmp_dir_); }
};

fs::path PreprocessorTest::tmp_dir_ = fs::temp_directory_path() / "sqwhiff";
