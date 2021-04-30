#include "interpreter.h"
#include "ast.h"
#include <memory>
#include <iostream>

Interpreter::Interpreter(Parser &p) : parser_(p) {};

void Interpreter::interpret(){
    std::unique_ptr<AST> tree = parser_.parse();
    tree->accept(*this);
};

void Interpreter::visit(Compound &c)
{
    for (auto &&child : c.children)
    {
        child->accept(*this);
    }
};

void Interpreter::visit(NoOp &) {};

void Interpreter::visit(NullaryOp &op)
{
    std::cout << op.op.raw;
};

void Interpreter::visit(UnaryOp &op)
{
    std::cout << op.op.raw;
    op.expr->accept(*this);
};

void Interpreter::visit(BinaryOp &op)
{
    op.left->accept(*this);
    std::cout << op.op.raw;
    op.right->accept(*this);
};

void Interpreter::visit(Assign &op)
{
    op.left->accept(*this);
    std::cout << op.op.raw;
    op.right->accept(*this);
};

void Interpreter::visit(Variable &var)
{
    std::cout << var.token.raw;
};

void Interpreter::visit(Number &num)
{
    std::cout << num.token.raw;
};

void Interpreter::visit(StringLiteral &s)
{
    std::cout << s.token.raw;
};