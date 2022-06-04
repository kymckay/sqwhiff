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
  std::ofstream(temp_file) << "2\n";

  EXPECT_EQ("2\n", preprocess("#include \"././simpleRel.inc\""))
      << "Relative file paths should be resolved in relation to the file being "
         "preprocessed";

  EXPECT_EQ("2\n", preprocess("#include \"../sqwhiff/simpleRel.inc\""))
      << "Parent directory traversal is supported in relative file paths";
}

TEST_F(PreprocessorTest, IncludesSimpleFileInternal) {
  fs::create_directory(tmp_dir_ / "internal");

  fs::path temp_file = tmp_dir_ / "internal/definitions.inc";
  std::ofstream(temp_file) << "3\n";

  EXPECT_EQ("3\n", preprocess("#include \"\\definitions.inc\""))
      << "Internal file paths (an RV concept) should be included from the set "
         "directory";
}

TEST_F(PreprocessorTest, IncludesNestedIncludes) {
  fs::path temp_file = tmp_dir_ / "level1.inc";
  std::ofstream(temp_file) << "first\n#include <./level2.inc>";

  temp_file = tmp_dir_ / "level2.inc";
  std::ofstream(temp_file) << "second\n#include <./level3.inc>";

  temp_file = tmp_dir_ / "level3.inc";
  std::ofstream(temp_file) << "third\n";

  EXPECT_EQ("first\nsecond\nthird\n", preprocess("#include <./level1.inc>"))
      << "The contents of the nested included files should appear in "
         "preprocessor output";
}

TEST_F(PreprocessorTest, ExpandsMacrosFromIncluder) {
  fs::path temp_file = tmp_dir_ / "child.inc";
  std::ofstream(temp_file) << "PARENT";

  EXPECT_EQ("\n1", preprocess("#define PARENT 1\n#include <./child.inc>"))
      << "The macro defined in the parent should be expanded when used in the "
         "child";
}

TEST_F(PreprocessorTest, ExpandsMacrosFromIncluded) {
  fs::path temp_file = tmp_dir_ / "child.inc";
  std::ofstream(temp_file) << "#define CHILD 2";

  EXPECT_EQ("CHILD\n\n2", preprocess("CHILD\n#include <./child.inc>\nCHILD"))
      << "The macro defined in the child should be expanded in the parent "
         "after inclusion";
}

TEST_F(PreprocessorTest, UndefsMacrosFromIncluder) {
  fs::path temp_file = tmp_dir_ / "child.inc";
  std::ofstream(temp_file) << "#undef PARENT";

  EXPECT_EQ(
      "\n1\n\nPARENT",
      preprocess("#define PARENT 1\nPARENT\n#include <./child.inc>\nPARENT"))
      << "The macro defined in the parent should be undefined after inclusion";
}

TEST_F(PreprocessorTest, RedefsMacrosFromIncluder) {
  fs::path temp_file = tmp_dir_ / "child.inc";
  std::ofstream(temp_file) << "#define PARENT 2";

  EXPECT_EQ(
      "\n1\n\n2",
      preprocess("#define PARENT 1\nPARENT\n#include <./child.inc>\nPARENT"))
      << "The macro defined in the parent should be overridden after inclusion";
}
