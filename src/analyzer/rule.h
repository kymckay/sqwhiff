#pragma once
#include "./semantic_error.h"
#include "../ast/node_visitor.h"
#include "../ast/ast.h"
#include <vector>
#include <memory>

class Rule : protected NodeVisitor
{
protected:
    std::vector<SemanticError> errors_;

public:
    std::vector<SemanticError> getErrors(AST &);
};

using rule_ptr = std::shared_ptr<Rule>;
using rule_set = std::vector<rule_ptr>;
