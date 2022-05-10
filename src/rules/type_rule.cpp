#include "./type_rule.h"

void TypeRule::visit(Compound &c) {
  for (auto &&child : c.children) {
    child->accept(*this);
  }
};

void TypeRule::visit(NoOp &){};

void TypeRule::visit(NullaryOp &op) {
  // Nullary guaranteed to be unique and present
  type_stack_.push(SQF_Nullary_Keywords.at(op.op.raw));
};

void TypeRule::visit(UnaryOp &op) {
  op.expr->accept(*this);

  SQFType last_type = type_stack_.top();
  type_stack_.pop();

  auto range = SQF_Unary_Keywords.equal_range(op.op.raw);
  for (auto it = range.first; it != range.second; ++it) {
    SQFUnaryCommand command = it->second;

    if (command.right == last_type || last_type == SQFType::any) {
      type_stack_.push(command.value);
      return;
    }
  }

  // Error when no version of the command matched the expected type
  errors_.push_back(SemanticError(op.op.line, op.op.column,
                                  "Incorrect type used with: " + op.op.raw));

  // When command was not found, consider as any type to avoid cascading errors
  type_stack_.push(SQFType::any);
};

void TypeRule::visit(BinaryOp &op) {
  op.left->accept(*this);
  op.right->accept(*this);

  SQFType right_type = type_stack_.top();
  type_stack_.pop();
  SQFType left_type = type_stack_.top();
  type_stack_.pop();

  auto range = SQF_Binary_Keywords.equal_range(op.op.raw);
  for (auto it = range.first; it != range.second; ++it) {
    SQFBinaryCommand command = it->second;

    if ((left_type == SQFType::any || left_type == command.left) &&
        (right_type == SQFType::any || right_type == command.right)) {
      type_stack_.push(command.value);
      return;
    }
  }

  // Error when no version of the command matched the expected type
  errors_.push_back(SemanticError(op.op.line, op.op.column,
                                  "Incorrect type used with: " + op.op.raw));

  // When command was not found, consider as any type to avoid cascading errors
  type_stack_.push(SQFType::any);
};

void TypeRule::visit(Assign &op) { op.right->accept(*this); };

void TypeRule::visit(Variable &var) { type_stack_.push(SQFType::any); };

void TypeRule::visit(Array &node) {
  node.expressions->accept(*this);

  type_stack_.push(SQFType::array);
};

void TypeRule::visit(Code &node) {
  node.children->accept(*this);

  type_stack_.push(SQFType::code);
};

void TypeRule::visit(Number &num) { type_stack_.push(SQFType::scalar); };

void TypeRule::visit(StringLiteral &s) { type_stack_.push(SQFType::string); };
