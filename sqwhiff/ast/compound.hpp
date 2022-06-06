#pragma once
#include <memory>
#include <vector>

#include "sqwhiff/ast/ast.hpp"
#include "sqwhiff/ast/node_visitor.hpp"

// A compound node used to store a sequence of statements (whole SQF file or
// statements within a code display)
class Compound : public AST {
 public:
  // A compound AST node to capture an ordered list of statements
  std::vector<ast_ptr> children;

  Compound(std::vector<ast_ptr> children) : children(std::move(children)){};
  void accept(NodeVisitor &visitor) override { visitor.visit(*this); };
};
