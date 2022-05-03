#pragma once

// Visitor type must be forward declared
class NodeVisitor;

class AST {
 public:
  // Virtual destructor allows deletion of subclasses via base class pointer
  virtual ~AST(){};

  virtual void accept(NodeVisitor &) = 0;
};
