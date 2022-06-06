// Convenience file to include all the analysis strategies
#pragma once
#include <vector>

#include "sqwhiff/rules/arity_rule.hpp"
#include "sqwhiff/rules/type_rule.hpp"

rule_set all_rules{{-1, std::make_shared<ArityRule>()},
                   {0, std::make_shared<TypeRule>()}};
