#pragma once
#include <memory>
#include <vector>

#include "./ast.h"
#include "./node_visitor.h"

// A code display contains a compound node (or NoOp node) that doesn't execute
// unless dynamically called
class Code : public AST {
 public:
  // A compound AST node to capture an ordered list of statements
  ast_ptr children;

  Code(ast_ptr children) : children(std::move(children)){};
  void accept(NodeVisitor &visitor) override { visitor.visit(*this); };
};
