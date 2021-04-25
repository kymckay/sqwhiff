#pragma once
#include "node_visitor.h"
#include "parser.h"

class Interpreter : NodeVisitor
{
    Parser &parser_;

public:
    Interpreter(Parser &);
    void visit(BinaryOp &) override;
    void visit(Number &) override;
};