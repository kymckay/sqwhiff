#pragma once
#include "./rule.h"
#include "../parser/parser.h"
#include <ostream>
#include <vector>
#include <memory>

class Analyzer
{
    Parser &parser_;

public:
    Analyzer(Parser &);
    int analyze(std::ostream &, rule_set &);
};
