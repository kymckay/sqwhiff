#pragma once
// Convenience file to include all the analysis strategies
#include <vector>

#include "./arity_rule.h"

rule_set all_rules{{-1, std::make_shared<ArityRule>()}};
