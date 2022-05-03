#pragma once
#include <memory>
#include <vector>

#include "./ast.h"
#include "./node_visitor.h"

// A compound node used to store a sequence of statements (whole SQF file or
// statements within a code display)
class Compound : public AST {
 public:
  // A compound AST node to capture an ordered list of statements
  std::vector<std::unique_ptr<AST>> children;

  Compound(std::vector<std::unique_ptr<AST>> children)
      : children(std::move(children)){};
  void accept(NodeVisitor &visitor) override { visitor.visit(*this); };
};
