#include <gtest/gtest.h>

#include <sstream>
#include <string>

#include "src/analyzer/analyzer.h"

class AnalyzerTest : public ::testing::Test {
 protected:
  std::string output_;
  int errorc_ = -1;

  void analyze(std::string s) { analyze(s, rule_set()); }

  void analyze(std::string s, rule_ptr r) {
    rule_set singular_rule = rule_set{{0, r}};
    analyze(s, singular_rule);
  }

  void analyze(std::string s, rule_set rules) {
    std::stringstream input(s);
    std::stringstream output("");
    Preprocessor pp(input);
    Lexer l(pp);
    Parser p(l);
    Analyzer a(p);
    errorc_ = a.analyze(output, rules);

    output_ = output.str();
  }
};
