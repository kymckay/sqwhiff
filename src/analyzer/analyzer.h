#pragma once
#include "../parser/parser.h"
#include "./rule.h"

class Analyzer {
  Parser &parser_;

 public:
  Analyzer(Parser &);
  error_storage analyze(rule_set &);
};
