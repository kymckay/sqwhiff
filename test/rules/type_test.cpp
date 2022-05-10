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

TEST_F(AnalyzerTest, DISABLED_ReportsIncorrecTypeFromVariable) {
  analyze("_test = 1; allturrets _test", type_rule);
  EXPECT_EQ(1, errorc_);
  EXPECT_EQ("1:11 SemanticError - Incorrect type used with: allturrets\n",
            output_);
}
