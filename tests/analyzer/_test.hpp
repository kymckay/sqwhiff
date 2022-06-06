#include <gtest/gtest.h>

#include <sstream>
#include <string>

#include "sqwhiff/analyzer/analyzer.hpp"

class AnalyzerTest : public ::testing::Test {
 protected:
  error_storage errors_;

  void analyze(std::string s) { analyze(s, rule_set()); }

  void analyze(std::string s, rule_ptr r) {
    rule_set singular_rule = rule_set{{0, r}};
    analyze(s, singular_rule);
  }

  void analyze(std::string s, rule_set rules) {
    std::stringstream input(s);
    Preprocessor pp(input);
    Lexer l(pp);
    Parser p(l);
    Analyzer a(p);
    errors_ = a.analyze(rules);
  }

  inline int errorCount() { return errors_.size(); }

  inline std::string errorMessage(int index) {
    auto it = errors_.begin();
    std::advance(it, index);
    return it->second->what();
  }

  inline std::string errorType(int index) {
    auto it = errors_.begin();
    std::advance(it, index);
    return it->second->type();
  }

  inline int errorLine(int index) {
    auto it = errors_.begin();
    std::advance(it, index);
    return it->second->line;
  }

  inline int errorColumn(int index) {
    auto it = errors_.begin();
    std::advance(it, index);
    return it->second->col;
  }
};
