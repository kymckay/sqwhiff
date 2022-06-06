#pragma once
#include "sqwhiff/parser/parser.hpp"
#include "sqwhiff/rules/base_rule.hpp"

class Analyzer {
  Parser &parser_;

 public:
  Analyzer(Parser &);
  error_storage analyze(rule_set &);
};
