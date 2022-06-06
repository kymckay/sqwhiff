#pragma once
#include "sqwhiff/ast/ast.hpp"
#include "sqwhiff/ast/node_visitor.hpp"
#include "sqwhiff/tokens/token.hpp"

class NullaryOp : public AST {
 public:
  Token op;

  NullaryOp(const Token &op) : op(op){};
  void accept(NodeVisitor &visitor) override { visitor.visit(*this); };
};
