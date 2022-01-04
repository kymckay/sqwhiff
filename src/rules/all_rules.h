#pragma once
// Convenience file to include all the analysis strategies
#include "./arity_rule.h"
#include <vector>

rule_set all_rules {
    std::make_shared<ArityRule>()
};
