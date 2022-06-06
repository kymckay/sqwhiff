#include "sqwhiff/rules/all_rules.hpp"
#include "tests/analyzer/_test.hpp"

rule_ptr type_rule = std::make_shared<TypeRule>();

TEST_F(AnalyzerTest, ReportsIncorrectUnaryType) {
  analyze("round \"\"", type_rule);

  EXPECT_EQ(1, errorCount());
  EXPECT_EQ("Incorrect type used with: round", errorMessage(0));
}

TEST_F(AnalyzerTest, ReportsIncorrectBinaryLeftType) {
  analyze("\"\" atan2 5", type_rule);

  EXPECT_EQ(1, errorCount());
  EXPECT_EQ("Incorrect type used with: atan2", errorMessage(0));
}

TEST_F(AnalyzerTest, ReportsIncorrectBinaryRightType) {
  analyze("2 atan2 \"\"", type_rule);

  EXPECT_EQ(1, errorCount());
  EXPECT_EQ("Incorrect type used with: atan2", errorMessage(0));
}

TEST_F(AnalyzerTest, ReportsIncorrectTypeFromNullary) {
  analyze("asin objnull", type_rule);

  EXPECT_EQ(1, errorCount());
  EXPECT_EQ("Incorrect type used with: asin", errorMessage(0));
}

TEST_F(AnalyzerTest, ReportsIncorrectTypeInCode) {
  analyze("{comment \"\";round \"\"}", type_rule);

  EXPECT_EQ(1, errorCount());
  EXPECT_EQ("Incorrect type used with: round", errorMessage(0));
}

TEST_F(AnalyzerTest, ReportsIncorrectTypeInArray) {
  analyze("[0,round \"\"]", type_rule);

  EXPECT_EQ(1, errorCount());
  EXPECT_EQ("Incorrect type used with: round", errorMessage(0));
}

TEST_F(AnalyzerTest, HandlesArrays) {
  analyze("[21,5,3] select [0,1];", type_rule);

  EXPECT_EQ(0, errorCount()) << "The expressions inside the arrays should not "
                                "confuse analysis of `select`.";
}

TEST_F(AnalyzerTest, HandlesDifferingParameters) {
  analyze("[] select []; \"\" select []", type_rule);

  EXPECT_EQ(0, errorCount()) << "There are multiple ways to use `select`, any "
                                "valid permutation should work.";
}

TEST_F(AnalyzerTest, ReportsIncorrectDifferingReturn) {
  analyze("comment (\"\" select [0,2]); comment ([] select [0,2])", type_rule);

  EXPECT_EQ(1, errorCount());
  EXPECT_EQ("Incorrect type used with: comment", errorMessage(0))
      << "The return type of `select` should differ by usage";
}

TEST_F(AnalyzerTest, DISABLED_ReportsIncorrectTypeFromVariable) {
  analyze("_test = 1; allturrets _test", type_rule);

  EXPECT_EQ(1, errorCount());
  EXPECT_EQ("Incorrect type used with: allturrets", errorMessage(0));
}

// TODO: Should be more explicitly testing that the rule gets the correct type
TEST_F(AnalyzerTest, HandlesUnknownReturnType) {
  analyze("_a / _b * _c", type_rule);

  EXPECT_EQ(0, errorCount())
      << "When an unknown type is used with a command which may return "
         "multiple types, the return type must also be unknown";

  analyze("+_a get []; +_a * 2; +_a select 1", type_rule);

  EXPECT_EQ(0, errorCount())
      << "When an unknown type is used with a command which may return "
         "multiple types, the return type must also be unknown";
}

TEST_F(AnalyzerTest, HandlesParameterTypeAny) {
  analyze("[] call somefunc; 0 call somefunc;", type_rule);

  EXPECT_EQ(0, errorCount())
      << "If a command (e.g. call) allows any type to be used with it then "
         "that should not be compared against the actual type given";
}
