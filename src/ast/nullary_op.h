#pragma once
#include "../lexer/token.h"
#include "./ast.h"
#include "./node_visitor.h"

class NullaryOp : public AST {
 public:
  Token op;

  NullaryOp(const Token &op) : op(op){};
  void accept(NodeVisitor &visitor) override { visitor.visit(*this); };
};
