#pragma once
#include "../parser/parser.h"
#include "../ast/node_visitor.h"
#include "../ast/all_nodes.h"
#include <ostream>
#include <string>

class Analyzer : NodeVisitor
{
    Parser &parser_;
    std::ostream &out_;
    int errorc_ = 0;

    void error(Token, std::string);

public:
    Analyzer(Parser &, std::ostream &);
    int analyze();

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