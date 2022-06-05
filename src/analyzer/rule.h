#pragma once
#include <map>
#include <memory>

#include "../ast/ast.h"
#include "../ast/node_visitor.h"
#include "../errors/semantic_error.h"
#include "../lexer/token.h"

// Using a map so that errors are stored in sorted order by uid
using error_storage =
    std::map<std::string, std::unique_ptr<sqwhiff::BaseError>>;

class Rule : protected NodeVisitor {
 protected:
  error_storage errors_;

  void addError(const Token&, const std::string&);

 public:
  error_storage getErrors(AST&);
};

using rule_ptr = std::shared_ptr<Rule>;
// A map is used to ensure rules are applied in consistent order and
// enable removal of rules in future while preserving indices
using rule_set = std::map<int, rule_ptr>;
