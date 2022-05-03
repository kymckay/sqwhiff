#pragma once
#include "../lexer/token.h"
#include "./ast.h"
#include "./node_visitor.h"

class Variable : public AST {
 public:
  Token token;

  Variable(const Token &t) : token(t){};
  void accept(NodeVisitor &visitor) override { visitor.visit(*this); };
};
