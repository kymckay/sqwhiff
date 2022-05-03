#pragma once
#include <memory>

#include "../lexer/token.h"
#include "./ast.h"
#include "./node_visitor.h"

class UnaryOp : public AST {
 public:
  Token op;
  std::unique_ptr<AST> expr;

  UnaryOp(const Token &op, std::unique_ptr<AST> expr)
      : op(op), expr(std::move(expr)){};
  void accept(NodeVisitor &visitor) override { visitor.visit(*this); };
};
