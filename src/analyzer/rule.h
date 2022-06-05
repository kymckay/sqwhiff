#pragma once
#include <map>
#include <memory>
#include <vector>

#include "../ast/ast.h"
#include "../ast/node_visitor.h"
#include "../errors/semantic_error.h"

class Rule : protected NodeVisitor {
 protected:
  std::vector<SemanticError> errors_;

 public:
  std::vector<SemanticError> getErrors(AST &);
};

using rule_ptr = std::shared_ptr<Rule>;
// A map is used to ensure rules are applied in consistent order and
// enable removal of rules in future while preserving indices
using rule_set = std::map<int, rule_ptr>;
