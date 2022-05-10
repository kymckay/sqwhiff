#pragma once
#include "../analyzer/rule.h"
#include "../ast/all_nodes.h"
#include "../sqf/all_keywords.h"

class TypeRule : public Rule {
  SQFType last_type_;

  // Visitor interface implementation

  void visit(Compound&) override;
  void visit(NoOp&) override;
  void visit(NullaryOp&) override;
  void visit(UnaryOp&) override;
  void visit(BinaryOp&) override;
  void visit(Assign&) override;
  void visit(Variable&) override;
  void visit(Array&) override;
  void visit(Code&) override;
  void visit(Number&) override;
  void visit(StringLiteral&) override;
};
