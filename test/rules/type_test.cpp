#include "../analyzer/_test.h"
#include "src/rules/all_rules.h"

rule_ptr type_rule = std::make_shared<TypeRule>();

TEST_F(AnalyzerTest, ReportsIncorrectUnaryType) {
  analyze("round \"\"", type_rule);
  EXPECT_EQ(1, errorc_);
  EXPECT_EQ("1:1 SemanticError - Incorrect type used with: round\n", output_);
}

TEST_F(AnalyzerTest, ReportsIncorrectBinaryLeftType) {
  analyze("\"\" atan2 5", type_rule);
  EXPECT_EQ(1, errorc_);
  EXPECT_EQ("1:4 SemanticError - Incorrect type used with: atan2\n", output_);
}

TEST_F(AnalyzerTest, ReportsIncorrectBinaryRightType) {
  analyze("2 atan2 \"\"", type_rule);
  EXPECT_EQ(1, errorc_);
  EXPECT_EQ("1:3 SemanticError - Incorrect type used with: atan2\n", output_);
}

TEST_F(AnalyzerTest, ReportsIncorrectTypeFromNullary) {
  analyze("asin objnull", type_rule);
  EXPECT_EQ(1, errorc_);
  EXPECT_EQ("1:1 SemanticError - Incorrect type used with: asin\n", output_);
}

TEST_F(AnalyzerTest, ReportsIncorrectTypeInCode) {
  analyze("{comment \"\";round \"\"}", type_rule);
  EXPECT_EQ(1, errorc_);
  EXPECT_EQ("1:13 SemanticError - Incorrect type used with: round\n", output_);
}

TEST_F(AnalyzerTest, ReportsIncorrectTypeInArray) {
  analyze("[0,round \"\"]", type_rule);
  EXPECT_EQ(1, errorc_);
  EXPECT_EQ("1:4 SemanticError - Incorrect type used with: round\n", output_);
}

TEST_F(AnalyzerTest, HandlesArrays) {
  analyze("[21,5,3] select [0,1];", type_rule);
  EXPECT_EQ(0, errorc_);
  EXPECT_EQ("", output_) << "The expressions inside the arrays should not "
                            "confuse analysis of `select`.";
}

TEST_F(AnalyzerTest, HandlesDifferingParameters) {
  analyze("[] select []; \"\" select []", type_rule);
  EXPECT_EQ(0, errorc_);
  EXPECT_EQ("", output_) << "There are multiple ways to use `select`, any "
                            "valid permutation should work.";
}

TEST_F(AnalyzerTest, ReportsIncorrectDifferingReturn) {
  analyze("comment (\"\" select [0,2]); comment ([] select [0,2])", type_rule);
  EXPECT_EQ(1, errorc_);
  EXPECT_EQ("1:28 SemanticError - Incorrect type used with: comment\n", output_)
      << "The return type of `select` should differ by usage";
}

TEST_F(AnalyzerTest, DISABLED_ReportsIncorrectTypeFromVariable) {
  analyze("_test = 1; allturrets _test", type_rule);
  EXPECT_EQ(1, errorc_);
  EXPECT_EQ("1:11 SemanticError - Incorrect type used with: allturrets\n",
            output_);
}
