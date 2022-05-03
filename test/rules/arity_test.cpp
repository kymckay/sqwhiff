#include "../analyzer/_test.h"
#include "src/rules/all_rules.h"

rule_ptr arity_rule = std::make_shared<ArityRule>();

TEST_F(AnalyzerTest, ReportsIncorrectUnaryUse) {
  analyze("1 comment \"\"", arity_rule);
  EXPECT_EQ(1, errorc_);
  EXPECT_EQ(
      "1:3 SemanticError - Incorrect arity, expected unary use: comment\n",
      output_);
}

TEST_F(AnalyzerTest, ReportsIncorrectBinaryUse) {
  analyze("select 2", arity_rule);
  EXPECT_EQ(1, errorc_);
  EXPECT_EQ(
      "1:1 SemanticError - Incorrect arity, expected binary use: select\n",
      output_);
}

TEST_F(AnalyzerTest, ReportsMultipleErrors) {
  analyze("1 comment \"\";\nselect 1", arity_rule);
  EXPECT_EQ(2, errorc_);
  EXPECT_EQ(
      "1:3 SemanticError - Incorrect arity, expected unary use: comment\n2:1 "
      "SemanticError - Incorrect arity, expected binary use: select\n",
      output_);
}
