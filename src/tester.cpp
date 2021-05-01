#include "tester.h"
#include "ast.h"
#include "token_type.h"
#include <memory>
#include <iostream>

Tester::Tester(Parser &p) : parser_(p){};

void Tester::test()
{
    std::unique_ptr<AST> tree = parser_.parse();
    tree->accept(*this);

    std::cout << code_;
};

void Tester::visit(Compound &c)
{
    for (auto &&child : c.children)
    {
        child->accept(*this);
        code_.push_back('\n');
    }
    code_.pop_back(); // Prevent final newline
};

void Tester::visit(NoOp &){
    code_.append("<NoOp>");
};

void Tester::visit(NullaryOp &op)
{
    code_.append("(" + op.op.raw + ")");
};

void Tester::visit(UnaryOp &op)
{
    code_.append("(" + op.op.raw + " ");
    op.expr->accept(*this);
    code_.append(")");
};

void Tester::visit(BinaryOp &op)
{
    code_.append("(");
    op.left->accept(*this);
    code_.append(" " + op.op.raw + " ");
    op.right->accept(*this);
    code_.append(")");
};

void Tester::visit(Assign &op)
{
    if (op.isPrivate)
        code_.append("<p=>");
    op.left->accept(*this);
    code_.append(" = (");
    op.right->accept(*this);
    code_.append(")");
};

void Tester::visit(Variable &var)
{
    code_.append("<Var:");
    code_.append(var.token.raw);
    code_.append(">");
};

void Tester::visit(Code &node)
{
    code_.append("<Code:(");
    node.children->accept(*this);
    code_.append(")>");
};

void Tester::visit(Number &num)
{
    if (num.token.type == TokenType::dec_literal)
    {
        code_.append("<Dec:");
    }
    else
    {
        code_.append("<Hex:");
    }
    code_.append(num.token.raw);
    code_.append(">");
};

void Tester::visit(StringLiteral &s)
{
    code_.append("<Str:");
    code_.append(s.token.raw);
    code_.append(">");
};