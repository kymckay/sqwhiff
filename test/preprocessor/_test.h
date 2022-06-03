#pragma once
#include <gtest/gtest.h>

#include <filesystem>
#include <sstream>
#include <string>

#include "src/preprocessor/preprocessor.h"

namespace fs = std::filesystem;

class PreprocessorTest : public ::testing::Test {
 protected:
  fs::path tmp_dir_ = fs::temp_directory_path() / "sqwhiff";

  std::string preprocess(std::string s) {
    std::stringstream ss(s);
    // Pretend processing file in temp dir for relative path testing
    Preprocessor pp(ss, tmp_dir_ / "fake.sqf");

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

 public:
  //  Ensure unique temporary directory exists for creation of test files
  PreprocessorTest() { fs::create_directory(tmp_dir_); }
};
