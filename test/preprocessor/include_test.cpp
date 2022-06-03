#include <fstream>

#include "./_test.h"

namespace fs = std::filesystem;

TEST_F(PreprocessorTest, IncludesSimpleFileAbsolute) {
  fs::path temp_file = tmp_dir_ / "simple.inc";

  std::ofstream(temp_file) << "1\n";

  EXPECT_EQ("1\n", preprocess("#include \"" + temp_file.string() + "\""))
      << "The contents of the included file should appear in preprocessor "
         "output";
}

TEST_F(PreprocessorTest, IncludesSimpleFileRelative) {
  fs::path temp_file = tmp_dir_ / "simpleRel.inc";

  std::ofstream(temp_file) << "230494\n";

  EXPECT_EQ("230494\n", preprocess("#include \"././simpleRel.inc\""))
      << "Relative file paths should be resolved in relation to the file being "
         "preprocessed";

  EXPECT_EQ("230494\n", preprocess("#include \"../sqwhiff/simpleRel.inc\""))
      << "Parent directory traversal is supported in relative file paths";
}

TEST_F(PreprocessorTest, IncludesSimpleFileInternal) {
  fs::path temp_file = tmp_dir_ / "internal/definitions.inc";

  std::ofstream(temp_file) << "4\n";

  EXPECT_EQ("4\n", preprocess("#include \"\\definitions.inc\""))
      << "Internal file paths (an RV concept) should be included from the set "
         "directory";
}
