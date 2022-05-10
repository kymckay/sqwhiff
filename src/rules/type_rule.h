#pragma once
#include <stack>

#include "../analyzer/rule.h"
#include "../ast/all_nodes.h"
#include "../sqf/all_keywords.h"

class TypeRule : public Rule {
  std::stack<SQFType> type_stack_;

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
