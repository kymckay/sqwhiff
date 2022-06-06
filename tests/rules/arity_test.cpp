#include "sqwhiff/rules/all_rules.hpp"
#include "tests/analyzer/_test.hpp"

rule_ptr arity_rule = std::make_shared<ArityRule>();

TEST_F(AnalyzerTest, ReportsIncorrectUnaryUse) {
  analyze("1 comment \"\"", arity_rule);
  EXPECT_EQ(1, errorCount());
  EXPECT_EQ("Incorrect arity, expected unary use: comment", errorMessage(0));
}

TEST_F(AnalyzerTest, ReportsIncorrectBinaryUse) {
  analyze("select 2", arity_rule);
  EXPECT_EQ(1, errorCount());
  EXPECT_EQ("Incorrect arity, expected binary use: select", errorMessage(0));
}

TEST_F(AnalyzerTest, ReportsMultipleErrors) {
  analyze("1 comment \"\";\nselect 1", arity_rule);
  EXPECT_EQ(2, errorCount());
  EXPECT_EQ("Incorrect arity, expected unary use: comment", errorMessage(0));
  EXPECT_EQ(1, errorLine(0));
  EXPECT_EQ("Incorrect arity, expected binary use: select", errorMessage(1));
  EXPECT_EQ(2, errorLine(1));
}

TEST_F(AnalyzerTest, ReportsErrorsInCodeDisplay) {
  analyze("{select 1}", arity_rule);
  EXPECT_EQ(1, errorCount());
  EXPECT_EQ("Incorrect arity, expected binary use: select", errorMessage(0));
}

TEST_F(AnalyzerTest, ReportsErrorsInArrayDisplay) {
  analyze("[0,1,2,3,select 4]", arity_rule);
  EXPECT_EQ(1, errorCount());
  EXPECT_EQ("Incorrect arity, expected binary use: select", errorMessage(0));
}
