#pragma once
#include <memory>

#include "sqwhiff/ast/ast.hpp"
#include "sqwhiff/ast/node_visitor.hpp"
#include "sqwhiff/tokens/token.hpp"

class UnaryOp : public AST {
 public:
  Token op;
  ast_ptr expr;

  UnaryOp(const Token &op, ast_ptr expr) : op(op), expr(std::move(expr)){};
  void accept(NodeVisitor &visitor) override { visitor.visit(*this); };
};
