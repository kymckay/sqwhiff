#include "sqwhiff/rules/type_rule.hpp"

#include "sqwhiff/sqf/all_keywords.hpp"

void TypeRule::visit(Compound &c) {
  for (auto &&child : c.children) {
    child->accept(*this);
  }
};

void TypeRule::visit(NoOp &){};

void TypeRule::visit(NullaryOp &op) {
  // Nullary guaranteed to be unique and present
  last_type_ = SQF::Nullary_Keywords.at(op.op.raw);
};

void TypeRule::visit(UnaryOp &op) {
  op.expr->accept(*this);

  auto range = SQF::Unary_Keywords.equal_range(op.op.raw);

  // If input is unknown type and command has differing return types then
  // the return type must be unknown also
  if (last_type_ == SQF::Type::any) {
    const SQF::Type target_type = range.first->second.value;

    for (auto it = range.first; it != range.second; ++it) {
      if (it->second.value != target_type) {
        last_type_ = SQF::Type::any;
        return;
      }
    }
  }

  for (auto it = range.first; it != range.second; ++it) {
    SQF::UnaryCommand command = it->second;

    if (command.right == last_type_ || command.right == SQF::Type::any ||
        last_type_ == SQF::Type::any) {
      last_type_ = command.value;
      return;
    }
  }

  // Error when no version of the command matched the expected type
  addError(op.op, "Incorrect type used with: " + op.op.raw);

  // When command was not found, consider as any type to avoid cascading errors
  last_type_ = SQF::Type::any;
};

void TypeRule::visit(BinaryOp &op) {
  op.left->accept(*this);
  SQF::Type left_type = last_type_;

  op.right->accept(*this);
  SQF::Type right_type = last_type_;

  auto range = SQF::Binary_Keywords.equal_range(op.op.raw);

  // If either input is unknown type and command has differing return types then
  // the return type must be unknown also
  if (left_type == SQF::Type::any || right_type == SQF::Type::any) {
    const SQF::Type target_type = range.first->second.value;

    for (auto it = range.first; it != range.second; ++it) {
      if (it->second.value != target_type) {
        last_type_ = SQF::Type::any;
        return;
      }
    }
  }

  for (auto it = range.first; it != range.second; ++it) {
    SQF::BinaryCommand command = it->second;

    if ((left_type == SQF::Type::any || command.left == SQF::Type::any ||
         left_type == command.left) &&
        (right_type == SQF::Type::any || command.right == SQF::Type::any ||
         right_type == command.right)) {
      last_type_ = command.value;
      return;
    }
  }

  // Error when no version of the command matched the expected type
  addError(op.op, "Incorrect type used with: " + op.op.raw);

  // When command was not found, consider as any type to avoid cascading errors
  last_type_ = SQF::Type::any;
};

void TypeRule::visit(Assign &op) { op.right->accept(*this); };

void TypeRule::visit(Variable &var) { last_type_ = SQF::Type::any; };

void TypeRule::visit(Array &node) {
  node.expressions->accept(*this);

  last_type_ = SQF::Type::array;
};

void TypeRule::visit(Code &node) {
  node.children->accept(*this);

  last_type_ = SQF::Type::code;
};

void TypeRule::visit(Number &num) { last_type_ = SQF::Type::scalar; };

void TypeRule::visit(StringLiteral &s) { last_type_ = SQF::Type::string; };
