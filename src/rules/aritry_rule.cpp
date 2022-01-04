#include "./arity_rule.h"
#include "../sqf/all_keywords.h"

void ArityRule::visit(Compound &c)
{
    for (auto &&child : c.children)
    {
        child->accept(*this);
    }
};

void ArityRule::visit(NoOp &){};

void ArityRule::visit(NullaryOp &op){};

void ArityRule::visit(UnaryOp &op)
{
    if (SQF_Unary_Keywords.find(op.op.raw) == SQF_Unary_Keywords.end())
    {
        errors_.push_back(SemanticError(op.op.line, op.op.column, "Incorrect arity, expected binary use"));
    }

    op.expr->accept(*this);
};

void ArityRule::visit(BinaryOp &op)
{
    if (SQF_Binary_Keywords.find(op.op.raw) == SQF_Binary_Keywords.end())
    {
        errors_.push_back(SemanticError(op.op.line, op.op.column, "Incorrect arity, expected unary use"));
    }

    op.left->accept(*this);
    op.right->accept(*this);
};

void ArityRule::visit(Assign &op)
{
    op.right->accept(*this);
};

void ArityRule::visit(Variable &var){};

void ArityRule::visit(Array &node){};

void ArityRule::visit(Code &node){};

void ArityRule::visit(Number &num){};

void ArityRule::visit(StringLiteral &s){};
