#pragma once
#include "src/parser/parser.h"
#include "src/ast/node_visitor.h"
#include "src/ast/all_nodes.h"

class Analyzer : NodeVisitor
{
    Parser &parser_;

public:
    Analyzer(Parser &);
    void analyze();

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