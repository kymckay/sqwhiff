#pragma once
#include "src/parser/parser.h"
#include "src/ast/node_visitor.h"
#include "src/ast/all_nodes.h"
#include <memory>
#include <string>

// The tester converts an AST structure into a flat string for easy validation
class Tester : NodeVisitor
{
    Parser &parser_;
    std::string code_;

public:
    Tester(Parser &);
    std::string test();

    // Visitor interface implementation

    void visit(Compound &) override;
    void visit(NoOp &) override;
    void visit(NullaryOp &) override;
    void visit(UnaryOp &) override;
    void visit(BinaryOp &) override;
    void visit(Assign &) override;
    void visit(Variable &) override;
    void visit(Array &) override;
    void visit(Code &) override;
    void visit(Number &) override;
    void visit(StringLiteral &) override;
};

Tester::Tester(Parser &p) : parser_(p){};

std::string Tester::test()
{
    std::unique_ptr<AST> tree = parser_.parse();
    tree->accept(*this);

    return code_;
};

void Tester::visit(Compound &c)
{
    if (c.children.empty())
        return;

    for (auto &&child : c.children)
    {
        child->accept(*this);
        code_.push_back(',');
    }
    code_.pop_back(); // Prevent final delimiter
};

void Tester::visit(NoOp &)
{
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

void Tester::visit(Array &node)
{
    code_.append("[");
    node.expressions->accept(*this);
    code_.append("]");
};

void Tester::visit(Code &node)
{
    code_.append("{");
    node.children->accept(*this);
    code_.append("}");
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