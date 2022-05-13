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
