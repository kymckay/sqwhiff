#pragma once
#include "sqwhiff/analyzer/rule.hpp"
#include "sqwhiff/parser/parser.hpp"

class Analyzer {
  Parser &parser_;

 public:
  Analyzer(Parser &);
  error_storage analyze(rule_set &);
};
