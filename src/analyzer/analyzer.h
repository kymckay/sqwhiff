#pragma once
#include <memory>
#include <ostream>
#include <vector>

#include "../parser/parser.h"
#include "./rule.h"

class Analyzer {
  Parser &parser_;

 public:
  Analyzer(Parser &);
  int analyze(std::ostream &, rule_set &);
};
