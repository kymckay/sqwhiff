#include "./rule.h"

std::vector<sqwhiff::SemanticError> Rule::getErrors(AST &tree) {
  // New set of errors each time rule is called
  errors_ = std::vector<sqwhiff::SemanticError>();
  tree.accept(*this);
  return errors_;
}
