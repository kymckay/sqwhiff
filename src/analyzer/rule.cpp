#include "./rule.h"

std::vector<SemanticError> Rule::getErrors(AST &tree)
{
    // New set of errors each time rule is called
    errors_ = std::vector<SemanticError>();
    tree.accept(*this);
    return errors_;
}
