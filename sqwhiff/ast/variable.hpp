#pragma once
#include "sqwhiff/ast/ast.hpp"
#include "sqwhiff/ast/node_visitor.hpp"
#include "sqwhiff/tokens/token.hpp"

class Variable : public AST {
 public:
  Token token;

  Variable(const Token &t) : token(t){};
  void accept(NodeVisitor &visitor) override { visitor.visit(*this); };
};
