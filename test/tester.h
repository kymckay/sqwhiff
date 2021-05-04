#pragma once
#include "src/parser/parser.h"
#include "src/ast/node_visitor.h"
#include "src/ast/all_nodes.h"
#include <string>

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