#include "./type_rule.h"

void TypeRule::visit(Compound &c) {
  for (auto &&child : c.children) {
    child->accept(*this);
  }
};

void TypeRule::visit(NoOp &){};

void TypeRule::visit(NullaryOp &op) {
  // Nullary guaranteed to be unique and present
  last_type_ = SQF_Nullary_Keywords.at(op.op.raw);
};

void TypeRule::visit(UnaryOp &op) {
  op.expr->accept(*this);

  auto range = SQF_Unary_Keywords.equal_range(op.op.raw);
  for (auto it = range.first; it != range.second; ++it) {
    SQFUnaryCommand command = it->second;

    if (command.right == last_type_ || last_type_ == SQFType::any) {
      last_type_ = command.value;
      return;
    }
  }

  // Error when no version of the command matched the expected type
  errors_.push_back(SemanticError(op.op.line, op.op.column,
                                  "Incorrect type used with: " + op.op.raw));

  // When command was not found, consider as any type to avoid cascading errors
  last_type_ = SQFType::any;
};

void TypeRule::visit(BinaryOp &op) {
  op.left->accept(*this);
  SQFType left_type = last_type_;

  op.right->accept(*this);
  SQFType right_type = last_type_;

  auto range = SQF_Binary_Keywords.equal_range(op.op.raw);
  for (auto it = range.first; it != range.second; ++it) {
    SQFBinaryCommand command = it->second;

    if ((left_type == SQFType::any || left_type == command.left) &&
        (right_type == SQFType::any || right_type == command.right)) {
      last_type_ = command.value;
      return;
    }
  }

  // Error when no version of the command matched the expected type
  errors_.push_back(SemanticError(op.op.line, op.op.column,
                                  "Incorrect type used with: " + op.op.raw));

  // When command was not found, consider as any type to avoid cascading errors
  last_type_ = SQFType::any;
};

void TypeRule::visit(Assign &op) { op.right->accept(*this); };

void TypeRule::visit(Variable &var) { last_type_ = SQFType::any; };

void TypeRule::visit(Array &node) {
  node.expressions->accept(*this);

  last_type_ = SQFType::array;
};

void TypeRule::visit(Code &node) {
  node.children->accept(*this);

  last_type_ = SQFType::code;
};

void TypeRule::visit(Number &num) { last_type_ = SQFType::scalar; };

void TypeRule::visit(StringLiteral &s) { last_type_ = SQFType::string; };
