#pragma once
#include <memory>
#include <vector>

#include "./ast.h"
#include "./node_visitor.h"

// An array display contains a compound node (or NoOp node) where each child is
// an expression (returns something)
class Array : public AST {
 public:
  // A compound AST node to capture an ordered list of statements (in this case
  // expressions)
  ast_ptr expressions;

  Array(ast_ptr expressions) : expressions(std::move(expressions)){};
  void accept(NodeVisitor &visitor) override { visitor.visit(*this); };
};
