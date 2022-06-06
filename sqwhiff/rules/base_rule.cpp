#include "sqwhiff/rules/base_rule.hpp"

#include "sqwhiff/errors/semantic_error.hpp"

error_storage BaseRule::getErrors(AST& tree) {
  // New set of errors each time rule is called
  errors_ = error_storage();
  tree.accept(*this);
  return std::move(errors_);
}

void BaseRule::addError(const Token& op, const std::string& msg) {
  auto err = std::make_unique<sqwhiff::SemanticError>(op.line, op.column,
                                                      op.file, msg);
  errors_.emplace(err->uid(), std::move(err));
}
