#pragma once
#include "sqwhiff/ast/ast.hpp"
#include "sqwhiff/ast/node_visitor.hpp"

class NoOp : public AST {
 public:
  void accept(NodeVisitor &visitor) override { visitor.visit(*this); };
};
