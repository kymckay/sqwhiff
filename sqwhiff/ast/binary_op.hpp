#pragma once
#include <memory>

#include "sqwhiff/ast/ast.hpp"
#include "sqwhiff/ast/node_visitor.hpp"
#include "sqwhiff/tokens/token.hpp"

class BinaryOp : public AST {
 public:
  ast_ptr left;
  Token op;
  ast_ptr right;

  BinaryOp(ast_ptr left, const Token &op, ast_ptr right)
      : left(std::move(left)), op(op), right(std::move(right)){};
  void accept(NodeVisitor &visitor) override { visitor.visit(*this); };
};
