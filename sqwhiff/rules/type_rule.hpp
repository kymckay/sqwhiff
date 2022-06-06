#pragma once
#include "sqwhiff/ast/all_nodes.hpp"
#include "sqwhiff/rules/base_rule.hpp"
#include "sqwhiff/sqf/type.hpp"

class TypeRule : public BaseRule {
  SQF::Type last_type_;

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
