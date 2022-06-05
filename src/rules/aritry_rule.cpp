#include "../sqf/all_keywords.h"
#include "./arity_rule.h"

void ArityRule::visit(Compound &c) {
  for (auto &&child : c.children) {
    child->accept(*this);
  }
};

void ArityRule::visit(NoOp &){};

void ArityRule::visit(NullaryOp &op){};

void ArityRule::visit(UnaryOp &op) {
  if (SQF::Unary_Keywords.find(op.op.raw) == SQF::Unary_Keywords.end()) {
    addError(op.op, "Incorrect arity, expected binary use: " + op.op.raw);
  }

  op.expr->accept(*this);
};

void ArityRule::visit(BinaryOp &op) {
  if (SQF::Binary_Keywords.find(op.op.raw) == SQF::Binary_Keywords.end()) {
    addError(op.op, "Incorrect arity, expected unary use: " + op.op.raw);
  }

  op.left->accept(*this);
  op.right->accept(*this);
};

void ArityRule::visit(Assign &op) { op.right->accept(*this); };

void ArityRule::visit(Variable &var){};

void ArityRule::visit(Array &node) { node.expressions->accept(*this); };

void ArityRule::visit(Code &node) { node.children->accept(*this); };

void ArityRule::visit(Number &num){};

void ArityRule::visit(StringLiteral &s){};
