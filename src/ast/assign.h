#pragma once
#include <memory>

#include "../lexer/token.h"
#include "./ast.h"
#include "./node_visitor.h"

class Assign : public AST {
 public:
  bool isPrivate;
  ast_ptr left;
  Token op;
  ast_ptr right;

  Assign(bool isPrivate, ast_ptr left, const Token &op, ast_ptr right)
      : isPrivate(isPrivate),
        left(std::move(left)),
        op(op),
        right(std::move(right)){};
  void accept(NodeVisitor &visitor) override { visitor.visit(*this); };
};
