#include "./rule.h"

std::vector<SemanticError> Rule::getErrors(AST &tree)
{
    tree.accept(*this);
    return errors_;
}
